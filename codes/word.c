/***** read and write Chinese words *****/ 

unsigned char Bit[8] = {128, 64, 32, 16, 8, 4, 2, 1};


/***** read a character from hzk16 *****/

void get_HzBit(char ch0, char ch1, char *bitdata)
{
	FILE *fp;
	long pos;
	
	pos = 32L * (((unsigned char)ch0 - 161) * 94 + ((unsigned char)ch1 - 161));
	if((fp = fopen("hzk16", "rb")) == NULL)
	{
		printf("Open hzk16 error!\n");
		exit(1);
	}
	fseek(fp, pos, SEEK_SET);
	fread(bitdata, 32, 1, fp);
	fclose(fp);
}


/***** write a character to screen *****/

void write_Hz(char ch0,char ch1,int x,int y,int color)
{
	register int i, j, k;
	unsigned vpos;
	char bitdata[32];
	
	get_HzBit(ch0, ch1, bitdata);
	for(i=0; i<16; i++)
	{
		for(j=0; j<8; j++)
		{
			if(bitdata[2*i] & Bit[j])
			{
				put_pixel(x+j, i+y, color);
			}
			if(bitdata[2*i+1] & Bit[j])
			{
				put_pixel(x+8+j, i+y, color);
			}
		}
	}
}


/***** write a string to screen *****/

void write_HzStr(char *str,int x,int y,int color)
{
	int num, i, xx;
	unsigned char s0, s1;
	
	num = strlen(str);
	xx = x;
	for(i=0; i<num; i+=2)
	{
		write_Hz(str[i], str[i+1], xx, y, color);
		xx += 16;
	}
}


/***** write characters to screen one by one *****/

void word_step(char *str, int x, int y, int color, int speed)
{
	int num, i, j, xx;
	unsigned char s0, s1;
	
	num = strlen(str);
	xx = x;
	for(i=0; i<num; i+=2)
	{
		if (!kbhit())
		{
			write_Hz(str[i], str[i+1], xx, y, color);
			delay(speed);
			xx += 16;
		}
	}
}

