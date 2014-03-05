

unsigned int Width, Height, Linebytes;    /* the width, height, and linebytes of picture */
char R[256], G[256], B[256];       /* each color of palette */
int Pagenew = 0, Pageold = 0;      /* pages of the screen */

typedef struct BMP_file        /* define head of *.bmp */

{
	int bfType;        /* always 'BM' */
	long bfSize;       /* file size */
	int bfReserved1;
	int bfReserved2;
	long bfOffset;      /* image data offset */
} bitmapfile;

typedef struct BMP_info
{
	long biSize;             /* size of BMP_file */
	long biWidth;            /* image width */
	long biHeight;           /* image height */
	int biPlanes;           /* must be 1 */
	int biBitCount;         /* bits per pixel, must be 1, 4, 8 or 24 */
	long biCompression;      /* compress method */
	long biSizeImage;        /* size of real image */
	long biXPelsPerMeter;    /* pixels per meter in horizontal direction */
	long biYPelsPerMeter;    /* pixels per meter in vertical direction */
	long biClrUsed;          /* color number used */
	long biClrImportant;     /* important colors */
} bitmapinfo;

typedef struct RGB_BMP_typ      /* define RGB struct */
{
	char blue;
	char green;
	char red;
	char reserved;
} BMP_RGB, *BMP_RGB_ptr;

typedef struct bmp_picture_typ      /* define bmp struct */
{
	bitmapfile file;
	bitmapinfo info;
	BMP_RGB palette[256];
	char *buffer;
} bmp_picture, *bmp_picture_ptr;


/***** select pages for SVGA mode *****/

void select_page(register char page)     
{
	union REGS r;

	r.x.ax = 0x4f05;
	r.x.bx = 0;
	r.x.dx = page;       /* select a page */
	int86(0x10, &r, &r);
}


/***** set video mode *****/

unsigned char set_video_mode(int mode)    
{
	union REGS r;

	r.x.ax = 0x4f02;
	r.x.bx = mode;
	int86(0x10, &r, &r);
	return(r.h.ah);
}


/***** get video mode *****/

unsigned int get_video_mode(void) 
{
	union REGS r;

	r.x.ax=0x4f03;
	int86(0x10,&r,&r);
	return(r.x.bx);
}


/***** set one palette *****/

void set_one_palette(int i, char r, char g, char b)
{
	outportb(0x03c8, i);
	outportb(0x03c9, r);
	outportb(0x03c9, g);
	outportb(0x03c9, b);
}  


/***** set all video palette *****/

void set_video_palette(char r[], char g[], char b[]) /* set palette */
{
	int  k;

 	for (k=0; k<256; k++) 
	{
		outportb(0x03c8, k);
		outportb(0x03c9, r[k]>>2);
		outportb(0x03c9, g[k]>>2);
		outportb(0x03c9, b[k]>>2);
	}
}


/***** get the video palette from a bmp *****/

void get_video_palette(char r[], char g[], char b[])
{
	int i;
	
	for (i=0; i<256; i++)
	{
		outportb(0x03c7, i);
		r[i] = inportb(0x03c9);
		g[i] = inportb(0x03c9);
		b[i] = inportb(0x03c9);
	}
} 


/***** set all palette to black *****/

void clear_video_palette(void)
{
	int k;
	
	for (k=0; k<256; k++)
	{
		outportb(0x03c8, k);
		outportb(0x03c9, 0);
		outportb(0x03c9, 0);
		outportb(0x03c9, 0);
	}
}


/***** check whether it is the bmp that the program can read *****/

void check_bmp(bmp_picture_ptr bmp_ptr)
{
	
	if (bmp_ptr->file.bfType != 0x4d42)   /* BM */
    {
        fprintf(stderr, "Not a BMP file!\n");
		getch();
        exit(1);
    }
	if (bmp_ptr->info.biBitCount > 8)
    {
       	fprintf(stderr, "Can not display ture color image !\n");
		getch();
		exit(1);
    }
	if (bmp_ptr->info.biCompression != 0)     /* cannot deal with compressed image */
    {
        fprintf(stderr, "Not non-compressed image !\n");
        getch();
		exit(1);
    }
}


/***** read the *.bmp and load to screen directly *****/

void draw_bmp(int x, int y, char *bmpname)        
{
	bmp_picture bmp;
	int i, j, n;
	char buffer[1024];
	
	long offset, position;
	FILE *fp;

	if ((fp = fopen(bmpname, "rb")) == NULL)
	{
		printf("Cannot open file: %s\n", bmpname);
		printf("Press any key to halt:\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.file, sizeof(bmp.file), 1, fp) != 1)
	{
		printf("Can't read file header!\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.info, sizeof(bmp.info), 1, fp) != 1)
	{
        printf("Can't read bmiHeader !\n");
		getch();
		exit(1);
	}
	if(fread((char *)&(bmp.palette[0]), 4, 256, fp) != 256)
	{
		printf("Can't get palette!\n");
		getch();
		exit(1);
	}

	check_bmp((bmp_picture_ptr)&bmp);

	for (i=0; i<256; i++)
	{
		R[i] = bmp.palette[i].red;
		G[i] = bmp.palette[i].green;
		B[i] = bmp.palette[i].blue;
	}
	
	Width = (unsigned int)bmp.info.biWidth;
	Height = (unsigned int)bmp.info.biHeight;
	Linebytes = ((Width * (long)bmp.info.biBitCount + 31) / 32) * 4;
	offset = (unsigned long)bmp.file.bfOffset;
	fseek(fp, offset, SEEK_SET);
	for(j=Height-1; j>=0; j--)
	{
		fread(buffer, Linebytes, 1, fp);
		for(i=0,n=0; i<Width; i++,n++)
		{
			position = (j + y) * (long)VIDEOCOL + i + x;
			Pagenew = position / 65536l;
			if(Pagenew != Pageold)
			{
				select_page(Pagenew);
				Pageold = Pagenew;
			}
			pokeb(0xa000, position%65536l, buffer[n]);
		}
	}
	fclose(fp);
	free(buffer);
}


/***** read the *.bmp and load to screen directly with no background of the *.bmp *****/

void nobg_draw_bmp(int x, int y, char *bmpname, char color)        
{
	bmp_picture bmp;
	int i, j, n;
	char buffer[1024];
	
	long offset, position;
	FILE *fp;

	if ((fp = fopen(bmpname, "rb")) == NULL)
	{
		printf("Cannot open file: %s\n", bmpname);
		printf("Press any key to halt:\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.file, sizeof(bmp.file), 1, fp) != 1)
	{
		printf("Can't read file header!\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.info, sizeof(bmp.info), 1, fp) != 1)
	{
        printf("Can't read bmiHeader !\n");
		getch();
		exit(1);
	}
	if(fread((char *)&(bmp.palette[0]), 4, 256, fp) != 256)
	{
		printf("Can't get palette!\n");
		getch();
		exit(1);
	}

	check_bmp((bmp_picture_ptr)&bmp);

	for (i=0; i<256; i++)
	{
		R[i] = bmp.palette[i].red;
		G[i] = bmp.palette[i].green;
		B[i] = bmp.palette[i].blue;
	}
	
	Width = (unsigned int)bmp.info.biWidth;
	Height = (unsigned int)bmp.info.biHeight;
	Linebytes = ((Width * (long)bmp.info.biBitCount + 31) / 32) * 4;
	offset = (unsigned long)bmp.file.bfOffset;
	fseek(fp, offset, SEEK_SET);
	for(j=Height-1; j>=0; j--)
	{
		fread(buffer, Linebytes, 1, fp);
		for(i=0,n=0; i<Width; i++,n++)
		{
			position = (j + y) * (long)VIDEOCOL + i + x;
			Pagenew = position / 65536l;
			if(Pagenew != Pageold)
			{
				select_page(Pagenew);
				Pageold = Pagenew;
			}
			if (buffer[n] != color)
			{
				pokeb(0xa000, position%65536l, buffer[n]);
			}
		}
	}
	fclose(fp);
	free(buffer);
}


/***** load the *.bmp to buffer and return buffer *****/

void bmp_load_buffer(char *bmpname, char *buffer)      
{
	bmp_picture bmp;
	int i, j;
	
	long offset;
	FILE *fp;

	if ((fp = fopen(bmpname, "rb")) == NULL)
	{
		printf("Cannot open file: %s\n", bmpname);
		printf("Press any key to halt:\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.file, sizeof(bmp.file), 1, fp) != 1)
	{
		printf("Can't read file header!\n");
		getch();
		exit(1);
	}
	if (fread((char *)&bmp.info, sizeof(bmp.info), 1, fp) != 1)
	{
		printf("Can't read bmiHeader !\n");
		getch();
		exit(1);
	}
	if(fread((char *)&(bmp.palette[0]), 4, 256, fp) != 256)
	{
		printf("Can't get palette!\n");
		getch();
		exit(1);
	}

	check_bmp((bmp_picture_ptr)&bmp);

	for (i=0; i<256; i++)        
	{
		R[i] = bmp.palette[i].red;
		G[i] = bmp.palette[i].green;
		B[i] = bmp.palette[i].blue;
	}
	
	Width = (unsigned int)bmp.info.biWidth;
	Height = (unsigned int)bmp.info.biHeight;
	Linebytes = ((Width * (long)bmp.info.biBitCount + 31) / 32) * 4;

	offset = (unsigned long)bmp.file.bfOffset;

	fseek(fp, offset, SEEK_SET);
	for(j=0; j<Height; j++)
	{
		fread(buffer+j*(int)Linebytes, Linebytes, 1, fp);
	}
	fclose(fp);
}


/***** load the graphic from buffer to screen *****/

void bmp_load_video(int x, int y, int sizex, int sizey, char *buffer)     
{
	long position;
	int i, j;

	for (i=sizey-1; i>=0; i--)
	{
		for (j=0; j<sizex; j++)
		{
			position = (i + y) * (long)VIDEOCOL + j + x;
			Pagenew = position / 65536l;
			if (Pagenew != Pageold)
			{
				select_page(Pagenew);
				Pageold = Pagenew;
			}
			pokeb(0xa000, position%65536l, buffer[j+(sizey-1-i)*sizex]);
		}
	}
}


/***** load the graphic from buffer to screen without certain color *****/

void nobg_load_video(int x, int y, int sizex, int sizey, char *buffer, char color)    
{
	long position;
	int i, j;

	for (i=sizey-1; i>=0; i--)
	{
		for (j=0; j<sizex; j++)
		{
			position = (i + y) * (long)VIDEOCOL + j + x;
			Pagenew = position / 65536l;
			if (Pagenew != Pageold)
			{
				select_page(Pagenew);
				Pageold = Pagenew;
			}
			if (buffer[j+(sizey-1-i)*sizex] != color)
			{
				pokeb(0xa000, position%65536l, buffer[j+(sizey-1-i)*sizex]);
			}
		}
	}
}


/***** save a picture from the screen to emery *****/

void save_bmp_video(int x, int y, int sizex, int sizey, char *buffer)     
{
	long position;
	int i, j;

	for (i=sizey-1; i>=0; i--)
	{
		for (j=0; j<sizex; j++)
		{
			position = (y + i) * (long)VIDEOCOL + x + j;
			Pagenew = position / 65536l;
			if (Pagenew != Pageold)
			{
				select_page(Pagenew);
				Pageold = Pagenew;
			}
			buffer[j+(sizey-1-i)*sizex] = peekb(0xa000, position%65536l);
		}
	}
}


/***** put a pixel to the screen *****/

void put_pixel(int x, int y, char color)
{
	long position;
	
	position = y * (long)VIDEOCOL + x;
	Pagenew = position / 65536l;
	if (Pagenew != Pageold)
	{
		select_page(Pagenew);
		Pageold = Pagenew;
	}
	pokeb(0xa000, position%65536l, color);
}


/***** clear screen *****/

void clear_screen(void)
{
	int i, j;
	for (i=0; i<VIDEOROW; i++)
	{
		for (j=0; j<VIDEOCOL; j++)
		{
			put_pixel(j, i, 0);
		}
	}
}


/***** clear certain place of the screen *****/
void clear_block(int x, int y, int xx, int yy, char color)
{
	int i, j;

	if (x <= xx && y <= yy)
	{
		for (i=x; i<=xx; i++)
		{
			for (j=y; j<=yy; j++)
			{
				put_pixel(i, j, color);
			}
		}
	}
}

