
int Numplayer = 0;       /* the number of the players */
int Table[ROW*2][COL*2];     /* store the kind of block */
char Blank[SIZEX*SIZEY];    /* store the blank.bmp */
char Mask[SIZEX*SIZEY];		/* store the mask.bmp */
char Tempbuffer[SIZEX*SIZEY];  /* store some graphic for something */
int Numalive = NUMENEMY;        /* the number of alive enemy */
int Num1 = NUMPLAYER;            /* the number of left tank of player1 */
int Num2 = NUMPLAYER;            /* .......................... player2 */
int Gameresult = 0;      /* mark the result of the game */
int Expcastle = 0;       /* mark whether castle be destroyed */
int Savemode = 0;
int Isexplode = 0;      /* mark whether the tank is being exploded */

int Level = 0;          /* the level of tank */
char *Map[LEVEL] = {"map\\1.txt", "map\\2.txt", "map\\3.txt",     /* mark the map text */
					          "map\\4.txt", "map\\5.txt", "map\\6.txt",
							      "map\\7.txt", "map\\8.txt", "map\\9.txt"};

char *Player1[4] = {"graphics\\player10.bmp", "graphics\\player11.bmp",
					          "graphics\\player12.bmp", "graphics\\player13.bmp"};
char *Player2[4] = {"graphics\\player20.bmp", "graphics\\player21.bmp",
					          "graphics\\player22.bmp", "graphics\\player23.bmp"};
char *Enemy[4] = {"graphics\\enemy0.bmp", "graphics\\enemy1.bmp",
				          "graphics\\enemy2.bmp", "graphics\\enemy3.bmp"};
char *Enemy1[4] = {"graphics\\enemy10.bmp", "graphics\\enemy11.bmp",
				           "graphics\\enemy12.bmp", "graphics\\enemy13.bmp"};
char *Enemy2[4] = {"graphics\\enemy20.bmp", "graphics\\enemy21.bmp",
				           "graphics\\enemy22.bmp", "graphics\\enemy23.bmp"};
char *Explode[8] = {"graphics\\explode0.bmp", "graphics\\explode1.bmp",
					          "graphics\\explode2.bmp", "graphics\\explode3.bmp",
					          "graphics\\explode4.bmp", "graphics\\explode5.bmp",
					          "graphics\\explode6.bmp", "graphics\\explode7.bmp"};
char *Bore[4] = {"graphics\\bore0.bmp", "graphics\\bore1.bmp",
				         "graphics\\bore2.bmp", "graphics\\bore3.bmp"};
char *Num[10] = {"graphics\\num0.bmp", "graphics\\num1.bmp",
                 "graphics\\num2.bmp", "graphics\\num3.bmp",
                 "graphics\\num4.bmp", "graphics\\num5.bmp",
                 "graphics\\num6.bmp", "graphics\\num7.bmp", 
                 "graphics\\num8.bmp", "graphics\\num9.bmp"};


/***** end game with a end graphic *****/

void end_game(void)
{
	clear_video_palette();
	draw_bmp(0, 0, "graphics\\end.bmp");
	set_video_palette(R, G, B);
	getch();
	set_video_mode(Savemode);
	exit(0);
}


/***** initial some data to begin game *****/

void first_init(void)
{
	int i, j;
	
	for (i=0; i<ROW*2; i++)
	{
		for (j=0; j<COL*2; j++)
		{
			Table[i][j] = BLANK;
		}
	}
	if (Gameresult == WIN)
	{
		Num1++;
		Num2++;
	}
	if (Gameresult == FAIL)
	{
		Num1 = Num2 = NUMPLAYER;
	}
	Numalive = NUMENEMY;
	Expcastle = 0;
	Gameresult = 0;
	bmp_load_buffer("graphics\\blank.bmp", Blank);
	bmp_load_buffer("graphics\\mask.bmp", Mask);
}
	

/***** choose something to begin to play the game */

int choose_option(void)
{
	int key;
	int i;
	int option;
	int x, y;
	char buffer1[SIZEX*SIZEY], buffer2[SIZEX*SIZEY];
	
	x = 120 + 82;
	y = 79 + 161;
	option = ONEPLAYER;
	save_bmp_video(x, y, SIZEX, SIZEY, buffer1);
	bmp_load_buffer("graphics\\tank.bmp", buffer2);
	nobg_load_video(x, y, SIZEX, SIZEY, buffer2, WHITE);
	while (1)
	{
		key = scan_key();
		if (key == DOWN)
		{
			option++;
			bmp_load_video(x, y, SIZEX, SIZEY, buffer1);
			y += 30;
			if (y > 79 + 164 + 30 * 3)
			{
				y = 79 + 164;
				option = ONEPLAYER;
			}
			nobg_load_video(x, y, SIZEX, SIZEY, buffer2, WHITE);
		}
		else if (key == UP)
		{
			option--;
			bmp_load_video(x, y, SIZEX, SIZEY, buffer1);
			y -= 30;
			if (y < 79 + 164)
			{
				y = 79 + 164 + 30 * 3;
				option = EXIT;
			}
			nobg_load_video(x, y, SIZEX, SIZEY, buffer2, WHITE);
		}
		else if (key == ENTER)
		{
			if (option == ONEPLAYER || option == TWOPLAYER)
			{
				for (i=0; i<3; i++)
				{	
					bmp_load_video(x, y, SIZEX, SIZEY, buffer1);
					exact_delay(5);
					nobg_load_video(x, y, SIZEX, SIZEY, buffer2, WHITE);
					exact_delay(8);
				}
			}
			else 
			{
				bmp_load_video(x, y, SIZEX, SIZEY, buffer1);
				exact_delay(5);
				nobg_load_video(x, y, SIZEX, SIZEY, buffer2, WHITE);
				exact_delay(8);
			}
			free(buffer1);
			free(buffer2);
			return option;
		}	
	}
}

/***** show the help graphic *****/

void help(void)
{	
	clear_video_palette();
	clear_screen();
	draw_bmp(HELP_STARTX, HELP_STARTY, "graphics\\help.bmp");
	set_video_palette(R, G, B);
	word_step(WORD0, WORD_STARTX, WORD_STARTY+1*WORD_HEIGHT, 2, 150);
	word_step(WORD1, WORD_STARTX, WORD_STARTY+2*WORD_HEIGHT, 2, 150);
	word_step(WORD2, WORD_STARTX, WORD_STARTY+3*WORD_HEIGHT+10, 2, 150);
	word_step(WORD3, WORD_STARTX, WORD_STARTY+4*WORD_HEIGHT+10, 2, 150);
	word_step(WORD4, WORD_STARTX, WORD_STARTY+5*WORD_HEIGHT+10, 2, 150);
	word_step(WORD5, WORD_STARTX, WORD_STARTY+6*WORD_HEIGHT+10, 2, 150);
	word_step(WORD6, WORD_STARTX, WORD_STARTY+7*WORD_HEIGHT+10, 2, 150);
	word_step(WORD7, WORD_STARTX, WORD_STARTY+8*WORD_HEIGHT+10, 2, 150);
	word_step(WORD8, WORD_STARTX, WORD_STARTY+9*WORD_HEIGHT+10, 2, 150);
	word_step(WORD9, WORD_STARTX, WORD_STARTY+10*WORD_HEIGHT+10, 2, 150);
	word_step(WORD10, WORD_STARTX, WORD_STARTY+11*WORD_HEIGHT+10, 2, 150);
	word_step(WORD11, WORD_STARTX, WORD_STARTY+12*WORD_HEIGHT+10, 2, 150);
	word_step(WORD12, WORD_STARTX, WORD_STARTY+13*WORD_HEIGHT+20, 2, 150);
	word_step(WORD13, WORD_STARTX, WORD_STARTY+14*WORD_HEIGHT+20, 2, 150);
	word_step(WORD14, WORD_STARTX, WORD_STARTY+15*WORD_HEIGHT+20, 2, 150);
	word_step(WORD15, WORD_STARTX, WORD_STARTY+16*WORD_HEIGHT+20, 2, 150);
	getch();
}

/***** beginning the game with one or two players *****/

void choose_to_begin(void)
{
	int option;
	int flag;
	
	do
	{
		flag = 0;
		
		clear_video_palette();
		clear_screen();
		draw_bmp(120, 79, "graphics\\begin.bmp");
		set_video_palette(R, G, B);
		option = choose_option();
		switch (option)
		{
			case ONEPLAYER:
				Numplayer = ONEPLAYER;
				break;
			case TWOPLAYER:
				Numplayer = TWOPLAYER;
				break;
			case HELP:
				help();
				flag = 1;
				break;
			case EXIT:
				end_game();
				break;
		}
	}
	while (flag);
}

/****** load a map by read a text file ******/

void load_map(void)
{
	FILE *fp;
	char temp;
	int i, j;

	if ((fp = fopen(Map[Level], "rb")) == NULL)      /* open a *.txt to load a map */
	{
		printf("Cannot open file. \n");
		printf("Press any key to halt:\n");
		getch();
		exit(1);
	}

	for (i=0; i<ROW*2; i+=2)
	{
		for (j=0; j<COL*2; j+=2)
		{
			temp = fgetc(fp);
			Table[i][j] = temp - '0';
			Table[i][j+1] = temp - '0';
			Table[i+1][j] = temp - '0';
			Table[i+1][j+1] = temp - '0';
		}
		fgetc(fp);
		fgetc(fp);
	}
	fclose(fp);
}


/****** draw a block ******/

void draw_block(int i, int j)       
{
	int x, y;
	int k1, k2;

	x = STARTX + j * SIZEX / 2;
	y = STARTY + i * SIZEY / 2;
	switch(Table[i][j])
	{
		case BLANK:
			draw_bmp(x, y, "graphics\\blank.bmp");
			break;
		case BRICK:
			draw_bmp(x, y, "graphics\\brick.bmp");
			break;
		case STONE:
			draw_bmp(x, y, "graphics\\stone.bmp");
			break;
		case RIVER:
			draw_bmp(x, y, "graphics\\river.bmp");
			break;
		case MASK:
			draw_bmp(x, y, "graphics\\blank.bmp");
			nobg_draw_bmp(x, y, "graphics\\mask.bmp", BLACK);
			break;
		case CASTLE:
			draw_bmp(x, y, "graphics\\castle.bmp");
			break;
	}
}


/****** draw the map ******/

void draw_table(void)
{
	int i, j;

	load_map();
	clear_screen();
	for (i=0; i<ROW*2; i+=2)
	{
		for (j=0; j<COL*2; j+=2)
		{
			draw_block(i, j);
		}
	}
}

/***** show game_over graphic *****/

void game_over(void)
{
	nobg_draw_bmp(150, 159, "graphics\\gameover.bmp", BLACK);
	getch();
}


/***** show all over when players go through all games *****/

void all_over(void)
{
	nobg_draw_bmp(150, 140, "graphics\\allover.bmp", BLACK);
	end_game();
}


/***** draw the hint of the game *****/

void draw_hint(void)
{
	draw_bmp(STARTX+SIZEX*COL, STARTY, "graphics\\hint.bmp");
	draw_bmp(STARTX+COL*SIZEX+1, STARTY+76, Num[Numalive/10]);
	draw_bmp(STARTX+COL*SIZEX+15, STARTY+76, Num[Numalive%10]);
	draw_bmp(STARTX+COL*SIZEX+1, STARTY+234, Num[Num1/10]);
	draw_bmp(STARTX+COL*SIZEX+15, STARTY+234, Num[Num1%10]);
	if (Numplayer == ONEPLAYER)
	{
		Num2 = 0;
	}
	draw_bmp(STARTX+COL*SIZEX+1, STARTY+326, Num[Num2/10]);
	draw_bmp(STARTX+COL*SIZEX+15, STARTY+326, Num[Num2%10]);
	draw_bmp(STARTX+COL*SIZEX+1, STARTY+435, Num[(Level+1)/10]);
	draw_bmp(STARTX+COL*SIZEX+15, STARTY+435, Num[(Level+1)%10]);
}	


/***** begin a new game *****/

void new_game(void)
{
	int i;
	if (Gameresult == WIN)
	{
		Level += 1;
		if (Level == 9)
		{
			all_over();
		}
		nobg_draw_bmp(195, 159, "graphics\\godjob.bmp", BLACK);
		delay(1000);
		getch();
	}
	else if (Gameresult == FAIL)
	{
		Level = 0;
	}
	first_init();
	clear_video_palette();
	draw_table();
	draw_hint();
	set_video_palette(R, G, B);
	delay(500);
}











