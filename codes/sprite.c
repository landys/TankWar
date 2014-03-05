/***** the most important part of the program *****/

typedef struct direct_type     /* mark the direction of the sprite or the bullet */
{
	int x;
	int y;
} direct;

typedef struct bullet_type
{
	int x, y;		/* position of bullet */
	int x_old, y_old;      /*present position of bullet */
	int ismove;    /* mark whether the bullet is moving on */
	int howmove;	  /* mark how the bullet moving */ 
	int mark;		/* to differ the bullet of enemy's and player's */
	int flag;		/* to judge whether a bullet has been shot */ 
	char background[BULLET_WIDTH*BULLET_HEIGHT];
	direct direction;		/* the direction of bullet */
} bullet, *bullet_ptr;

typedef struct sprite_type
{
	direct direct;
	int num;        /* the num of tanks */
	int mark;				/* mark the tank is an enemy or a player */
	int x, y;				/* the current position of the sprite */
	int x_old, y_old;    /* the present position of the sprite */
	int isexplode;     /* mark whether the tank is exploded */
	int ismove;     /* mark whether the tank is on moving */
	int howmove;    /* mark how the tank is moving */
	char *frames[MAX_SPRITE_FRAMES];       /* store the frames of sprite */
	int cur_frame;     /* the number of current frame of sprite */
	int sum_frames;    /* the sum of frames of sprite */
	char background[SPRITE_WIDTH*SPRITE_HEIGHT];     /* store the background of a sprite */
	struct sprite_type *next;    /* point to the next sprite */
	void (*spr)(struct sprite_type *spr);       /* the function of the sprite */
	int temp1, temp2;      /* store something if neccessary */
	bullet bullet;      /* the bullet of the sprite */
} sprite, *sprite_ptr;    

sprite_ptr Head = NULL, Now = NULL, Pre = NULL;     /* define some points to point sprite */


/***** save the background of bullet *****/

void behind_bullet(sprite_ptr sprite)
{
	int x, y;

	x = sprite->bullet.x + STARTX;
	y = sprite->bullet.y + STARTY;
	save_bmp_video(x, y, BULLET_WIDTH, BULLET_HEIGHT, sprite->bullet.background);
}


/****** initial data of the bullet *****/

void init_bullet(sprite_ptr sprite)
{
	sprite->bullet.direction.x = sprite->direct.x;
	sprite->bullet.direction.y = sprite->direct.y;
	if (sprite->direct.y == -1)            /* the direction of tank is upward */
	{
		sprite->bullet.x = sprite->x + (SPRITE_WIDTH - BULLET_WIDTH) / 2;
		sprite->bullet.y = sprite->y - BULLET_HEIGHT - 4;
	}
	else if (sprite->direct.y == 1)							/* down */
	{
		sprite->bullet.x = sprite->x + (SPRITE_WIDTH - BULLET_WIDTH) / 2;
		sprite->bullet.y = sprite->y + SPRITE_HEIGHT + 4;
	}
	else if (sprite->direct.x == -1)            /* left */
	{
		sprite->bullet.x = sprite->x - BULLET_WIDTH - 4;
		sprite->bullet.y = sprite->y + (SPRITE_HEIGHT - BULLET_HEIGHT) / 2;
	}
	else if (sprite->direct.x == 1)             /* right */
	{
		sprite->bullet.x = sprite->x + SPRITE_WIDTH + 4;
		sprite->bullet.y = sprite->y + (SPRITE_HEIGHT - BULLET_HEIGHT) / 2;
	}
	sprite->bullet.x_old = sprite->bullet.x;
	sprite->bullet.y_old = sprite->bullet.y;
	sprite->bullet.flag = 0;
	sprite->bullet.ismove = 0;
	sprite->bullet.howmove = 0;
	behind_bullet(sprite);
}


/***** this function initializes a sprite with the sent data *****/

void sprite_init(sprite_ptr sprite, int i, int j, int num, int x, int y, int cur_frame, int sum_frames, int ismove, void (*spr)(sprite_ptr sprite))
{
	int index;

	sprite->direct.x = i;
	sprite->direct.y = j;
	sprite->num = num;
	sprite->x = x;
	sprite->y = y;
	sprite->x_old = x;
	sprite->y_old = y;
	init_bullet(sprite);
	sprite->cur_frame = cur_frame;
	sprite->sum_frames = sum_frames;
	sprite->isexplode = 0;
	sprite->ismove = ismove;
	sprite->howmove = 0;
	sprite->spr = spr;
} 


/***** this function deletes all the memory associated with a sprire *****/

void sprite_delete(sprite_ptr sprite)
{
	int i;

	free(sprite->background);

	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
			free(sprite->frames[i]);
	}
	free(sprite);
} 


/***** load the *.bmp file to frames[] *****/

void bmp_load_frames(sprite_ptr sprite, char *bmpname[])
{
	int i;

	for (i=0; i<sprite->sum_frames; i++)
	{
		bmp_load_buffer(bmpname[i], sprite->frames[i]);
	}
}


/***** draw sprite *****/

void draw_sprite(sprite_ptr sprite)
{
	int x, y;

	x = sprite->x + STARTX;
	y = sprite->y + STARTY;
	nobg_load_video(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, sprite->frames[sprite->cur_frame], WHITE);
}


/***** save the background of sprite *****/

void behind_sprite(sprite_ptr sprite)
{
	int x, y;

	x = sprite->x + STARTX;
	y = sprite->y + STARTY;
	save_bmp_video(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, sprite->background);
}


/***** recover the background where sprite goes over *****/	

void erase_sprite(sprite_ptr sprite)
{
	int x, y;

	x = sprite->x_old + STARTX;
	y = sprite->y_old + STARTY;
	bmp_load_video(x, y, SPRITE_WIDTH, SPRITE_HEIGHT, sprite->background);
}


/***** draw bullet *****/

void draw_bullet(sprite_ptr sprite)
{
	int x, y;
	
	x = STARTX + sprite->bullet.x;
	y = STARTY + sprite->bullet.y;
	put_pixel(x+1, y+1, YELLOW);
	put_pixel(x+2, y+1, YELLOW);
	put_pixel(x+1, y+2, YELLOW);
	put_pixel(x+2, y+2, YELLOW);
	
	put_pixel(x+1, y, RED);
	put_pixel(x+2, y, RED);
	put_pixel(x, y+1, RED);
	put_pixel(x, y+2, RED);
	put_pixel(x+1, y+3, RED);
	put_pixel(x+2, y+3, RED);
	put_pixel(x+3, y+1, RED);
	put_pixel(x+3, y+2, RED);
}
	
	
/***** recover the background where bullet goes over *****/	

void erase_bullet(sprite_ptr sprite)
{
	int x, y;

	x = sprite->bullet.x_old + STARTX;
	y = sprite->bullet.y_old + STARTY;
	bmp_load_video(x, y, BULLET_WIDTH, BULLET_HEIGHT, sprite->bullet.background);
}

	
/***** build every sprite *****/

void sprite_build(void)

{
	void player1(sprite_ptr sprite);
	void player2(sprite_ptr sprite);
	void enemy(sprite_ptr sprite);
	
	int i;
	
	if ((Now = Pre = (struct sprite_type *)malloc(sizeof(struct sprite_type))) == NULL)    
	{
		printf("Cannot allocate enough buffer\n");
		getch();
		exit(1);
	}
	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
		if ((Now->frames[i] = (char *)malloc(SPRITE_WIDTH*SPRITE_HEIGHT)) == NULL)
		{
			printf("Cannot allocate enough buffer\n");
			getch();
			exit(1);
		}
	}
	Head = Now;
	sprite_init(Now, 0, -1, Num1, (COL-5)/2*SPRITE_WIDTH, (ROW-1)*SPRITE_HEIGHT, 0, 4, STEP_PLAYER, player1);
	Now->mark = PLAYER1;
	Now->bullet.mark = BULLET1;
	bmp_load_frames(Now, Player1);
	
	if (Numplayer == TWOPLAYER)           /* if there are two players, do this */
	{
		if ((Now = (struct sprite_type *)malloc(sizeof(struct sprite_type))) == NULL)
		{
			printf("Cannot allocate enough buffer\n");
			getch();
			exit(1);
		}
		for (i=0; i<MAX_SPRITE_FRAMES; i++)
		{
			if ((Now->frames[i] = (char *)malloc(SPRITE_WIDTH*SPRITE_HEIGHT)) == NULL)
			{
				printf("Cannot allocate enough buffer\n");
				getch();
				exit(1);
			}
		}
		Pre->next = Now;
		Pre = Now;
		sprite_init(Now, 0, -1, Num2, (COL+3)/2*SPRITE_WIDTH, (ROW-1)*SPRITE_HEIGHT, 0, 4, STEP_PLAYER, player2);
		Now->mark = PLAYER2;
		Now->bullet.mark = BULLET1;
		bmp_load_frames(Now, Player2);
	}
	
	if ((Now = (struct sprite_type *)malloc(sizeof(struct sprite_type))) == NULL)
	{
		printf("Cannot allocate enough buffer\n");
		getch();
		exit(1);
	}
	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
		if ((Now->frames[i] = (char *)malloc(SPRITE_WIDTH*SPRITE_HEIGHT)) == NULL)
		{
			printf("Cannot allocate enough buffer\n");
			getch();
			exit(1);
		}
	}
	Pre->next = Now;
	Pre = Now;
	sprite_init(Now, 0, 1, NUMENEMY, (COL-1)/2*SPRITE_WIDTH, 0, 1, 4, STEP_ENEMY, enemy);
	Now->mark = ENEMY1;
	Now->bullet.mark = BULLET2;
	bmp_load_frames(Now, Enemy);
	
	if ((Now = (struct sprite_type *)malloc(sizeof(struct sprite_type))) == NULL)
	{
		printf("Cannot allocate enough buffer\n");
		getch();
		exit(1);
	}
	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
		if ((Now->frames[i] = (char *)malloc(SPRITE_WIDTH*SPRITE_HEIGHT)) == NULL)
		{
			printf("Cannot allocate enough buffer\n");
			getch();
			exit(1);
		}
	}
	Pre->next = Now;
	Pre = Now;
	sprite_init(Now, 0, 1, NUMENEMY, 0, 0, 1, 4, STEP_ENEMY, enemy);
	Now->mark = ENEMY2;
	Now->bullet.mark = BULLET2;
	bmp_load_frames(Now, Enemy1);
	
	if ((Now = (struct sprite_type *)malloc(sizeof(struct sprite_type))) == NULL)
	{
		printf("Cannot allocate enough buffer\n");
		getch();
		exit(1);
	}
	for (i=0; i<MAX_SPRITE_FRAMES; i++)
	{
		if ((Now->frames[i] = (char *)malloc(SPRITE_WIDTH*SPRITE_HEIGHT)) == NULL)
		{
			printf("Cannot allocate enough buffer\n");
			getch();
			exit(1);
		}
	}
	Pre->next = Now;
	Pre = Now;
	sprite_init(Now, 0, 1, NUMENEMY, (COL-1)*SPRITE_WIDTH, 0, 1, 4, STEP_ENEMY, enemy);
	Now->mark = ENEMY3;
	Now->bullet.mark = BULLET2;
	bmp_load_frames(Now, Enemy2);
	Now->next = NULL;
}


/***** the animation of tank explored *****/

int explode(sprite_ptr sprite)
{
	void player1(sprite_ptr spr);
	void player2(sprite_ptr spr);
	void enemy(sprite_ptr spr);

	static int k = 0;
	int i;
	
	k++;
	if(k % 8 == 0)
	{
		k = 0;
		sprite->isexplode = 0;
		Isexplode = 0;
		
		erase_sprite(sprite);
		if (sprite->bullet.mark==BULLET2)
		{
			Numalive--;
			draw_bmp(STARTX+COL*SIZEX+1, STARTY+76, Num[Numalive/10]);
			draw_bmp(STARTX+COL*SIZEX+15, STARTY+76, Num[Numalive%10]);
			if (Numalive-3 >= 0)
			{
				switch (sprite->mark)
				{
					case ENEMY1:
						sprite_init(sprite, 0, 1, sprite->num, (COL-1)/2*SPRITE_WIDTH, 0, 1, 4, STEP_ENEMY, enemy);
						bmp_load_video(STARTX+(COL-1)/2*SIZEX, STARTY, SIZEX, SIZEY, Blank);
						draw_sprite(sprite);
						return 1;
					case ENEMY2:
						sprite_init(sprite, 0, 1, sprite->num, 0, 0, 1, 4, STEP_ENEMY, enemy);
						bmp_load_video(STARTX, STARTY, SIZEX, SIZEY, Blank);
						draw_sprite(sprite);
						return 1;
					case ENEMY3:
						sprite_init(sprite, 0, 1, sprite->num, (COL-1)*SPRITE_WIDTH, 0, 1, 4, STEP_ENEMY, enemy);
						bmp_load_video(STARTX+(COL-1)*SIZEX, STARTY, SIZEX, SIZEY, Blank);
						draw_sprite(sprite);
						return 1;
				}
			}
			else
			{
				Pre->next = sprite->next;
				Now = Pre->next;
				sprite_delete(sprite);
				return 1;
			}	
		}
		else if (sprite->mark == PLAYER1)
		{
			Num1 = sprite->num;	
			draw_bmp(STARTX+COL*SIZEX+1, STARTY+234, Num[Num1/10]);
			draw_bmp(STARTX+COL*SIZEX+15, STARTY+234, Num[Num1%10]);
			
			if (Num1 >= 0)
			{				 
				sprite_init(sprite, 0, -1, sprite->num, (COL-5)/2*SPRITE_WIDTH, (ROW-1)*SPRITE_HEIGHT, 0, 4, STEP_PLAYER, player1);
				bmp_load_video(STARTX+(COL-5)/2*SIZEX, STARTY+(ROW-1)*SIZEY, SIZEX, SIZEY, Blank);
				draw_sprite(sprite);
				return 1;
			}
			else
			{
				Now = Head = Head->next;
				sprite_delete(sprite);
				return 1;
			}
		}	
		else if (sprite->mark == PLAYER2)
		{
			Num2 = sprite->num;
			draw_bmp(STARTX+COL*SIZEX+1, STARTY+326, Num[Num2/10]);
			draw_bmp(STARTX+COL*SIZEX+15, STARTY+326, Num[Num2%10]);
			
			if (Num2 >= 0)
			{	
				sprite_init(sprite, 0, -1, sprite->num, (COL+3)/2*SPRITE_WIDTH, (ROW-1)*SPRITE_HEIGHT, 0, 4, STEP_PLAYER, player2);
				bmp_load_video(STARTX+(COL+3)/2*SIZEX, STARTY+(ROW-1)*SIZEY, SIZEX, SIZEY, Blank);
				draw_sprite(sprite);
				return 1;
			}
			else
			{
				Pre->next = sprite->next;
				Now = Pre->next;
				sprite_delete(sprite);
				return 1;
			}
		}
	}
	nobg_draw_bmp(STARTX+sprite->x, STARTY+sprite->y, Explode[k-1], BLACK);
	return 0;
} 
	
	
/***** judge whether bullet collides with something *****/

int bullet_collide(sprite_ptr sprite)
{
	sprite_ptr spr1;
	
	spr1 = Head;
	
	while (spr1 != NULL)
	{
		if (spr1 == sprite)
		{	
			spr1 = spr1->next;
		}
		
		if ((sprite->bullet.x>spr1->x-(BULLET_WIDTH-1) && (sprite->bullet.x<spr1->x+(SPRITE_WIDTH+BULLET_WIDTH-1)))
			 && (sprite->bullet.y>spr1->y-(BULLET_HEIGHT-1) && (sprite->bullet.y<spr1->y+(SPRITE_HEIGHT+BULLET_WIDTH-1))))
		{ 
			if (sprite->bullet.mark != spr1->bullet.mark)
			{
				spr1->bullet.x_old = spr1->bullet.x;
				spr1->bullet.y_old = spr1->bullet.y;
				erase_bullet(spr1);
				spr1->num -= 1;
				spr1->x_old = spr1->x;
				spr1->y_old = spr1->y;
				erase_sprite(spr1);
				spr1->isexplode = 1;
				Isexplode = 1;	
			}
			return 1;
		}
		else
		{
			if(abs(sprite->bullet.x - spr1->bullet.x)<BULLET_WIDTH+2 && abs(sprite->bullet.y - spr1->bullet.y)<BULLET_HEIGHT+2)
			{
				if (sprite->bullet.mark != spr1->bullet.mark)
				{
					spr1->bullet.x_old = spr1->bullet.x;
					spr1->bullet.y_old = spr1->bullet.y;
					sprite->bullet.x_old = sprite->bullet.x;
					sprite->bullet.y_old = sprite->bullet.y;
					erase_bullet(spr1);
					erase_bullet(sprite);
					init_bullet(spr1);
					init_bullet(sprite);
					return 1;
				}
			}
		}
		spr1 = spr1->next;	
	}
	return 0;
}


/***** the animation that castle has been explored *****/

void castle_explode(void)
{
	int i;
	
	for (i=0; i<8; i++)
	{
		nobg_draw_bmp(STARTX+(COL-1)/2*SIZEX, STARTY+(ROW-1)*SIZEY, Explode[i], BLACK);
		delay(80);
	}
}
	
	
/***** bullet move *****/

int bullet_move(sprite_ptr sprite)
{
	int i, j, ii, jj, t;
	
	sprite->bullet.x_old = sprite->bullet.x;
	sprite->bullet.y_old = sprite->bullet.y;
	sprite->bullet.x += sprite->bullet.direction.x * BULLET_SPEED;
	sprite->bullet.y += sprite->bullet.direction.y * BULLET_SPEED;
	erase_bullet(sprite);
	
	if (sprite->bullet.x > COL* SIZEX - BULLET_WIDTH || sprite->bullet.y > ROW * SIZEY - BULLET_HEIGHT || sprite->bullet.x < 0 || sprite->bullet.y < 0)
	{
		init_bullet(sprite);
		return 0;
	}
	if (bullet_collide(sprite))
	{
		init_bullet(sprite);
		return 0;
	}
	
	if (sprite->bullet.ismove == 1)
	{
		switch(sprite->bullet.howmove)
		{
			case 1:
				behind_bullet(sprite);
				draw_bullet(sprite);
				break;
			case 2:
				behind_bullet(sprite);
				break;
		}
		sprite->bullet.ismove = 0;
		return 0;
	}
	
	if (sprite->bullet.direction.y == -1)
	{
		if (sprite->bullet.y_old % (SIZEY / 2) == 0)
		{
			sprite->bullet.ismove = 1;
		}
		i = (sprite->bullet.y_old-1) / (SIZEY / 2);
		j = sprite->bullet.x_old / (SIZEX / 2);
		ii = i;
		if (j % 2 ==0)
		{
			jj = j + 1;
		}
		else
		{
			jj = j - 1;
		}
	}
	else if (sprite->bullet.direction.y == 1)
	{
		if ((sprite->bullet.y_old + BULLET_HEIGHT) % (SIZEY / 2) == 0)
		{
			sprite->bullet.ismove = 1;
		}
		i = (sprite->bullet.y_old + 5) / (SIZEY / 2);
		j = sprite->bullet.x_old / (SIZEX / 2);
		ii = i;
		if (j % 2 ==0)
		{
			jj = j + 1;
		}
		else
		{
			jj = j - 1;
		}
	}
	else if (sprite->bullet.direction.x == -1)
	{
		if (sprite->bullet.x_old % (SIZEX / 2) == 0)
		{
			sprite->bullet.ismove = 1;
		}
		i = sprite->bullet.y_old / (SIZEY / 2);
		j = (sprite->bullet.x_old - 1 ) / (SIZEX / 2);
		jj = j;
		if (i % 2 == 0)
		{
			ii = i + 1;
		}
		else
		{
			ii = i - 1;
		}
	}
	else if (sprite->bullet.direction.x == 1)
	{
		if ((sprite->bullet.x_old + BULLET_WIDTH) % (SIZEY / 2) == 0)
		{
			sprite->bullet.ismove = 1;
		}
		i = sprite->bullet.y_old / (SIZEY / 2);
		j = (sprite->bullet.x_old + 5)/ (SIZEX / 2);
		jj = j;
		if (i % 2 == 0)
		{
			ii = i + 1;
		}
		else
		{
			ii = i - 1;
		}
	}
	
	if (i > ii)
	{
		t = i;
		i = ii;
		ii = t;
	}
	if (j > jj)
	{ 
		t = j;
		j = jj;
		jj = t;
	}
	
	switch(Table[i][j])
	{
		case BLANK:
		case RIVER:
			if (Table[ii][jj] == BRICK)
			{
				init_bullet(sprite);
				Table[ii][jj] = BLANK;
				bmp_load_video(STARTX+SIZEX*jj/2, STARTY+SIZEY*ii/2, SIZEX/2, SIZEY/2, Blank);
			}
			else
			{
				behind_bullet(sprite);
				draw_bullet(sprite);
				sprite->bullet.howmove = 1;
			}
			return 0;
		case MASK:
			behind_bullet(sprite);
			sprite->bullet.howmove = 2;
			return 0;
		case STONE:
			init_bullet(sprite);
			return 0;
		case CASTLE:
			init_bullet(sprite);
			castle_explode();
			draw_bmp(STARTX+(COL-1)/2*SIZEX, STARTY+(ROW-1)*SIZEY, "graphics\\expcas.bmp");
			Expcastle = 1;
			return 0;
		case BRICK:
			init_bullet(sprite);
			Table[i][j] = BLANK;
			Table[ii][jj] = BLANK;
			bmp_load_video(STARTX+SIZEX*j/2, STARTY+SIZEY*i/2, SIZEX/2, SIZEY/2, Blank); 
			bmp_load_video(STARTX+SIZEX*jj/2, STARTY+SIZEY*ii/2, SIZEX/2, SIZEY/2, Blank); 
			return 0;
	}
	return 0;
}

			
/***** tank takes turns *****/			

void turn(sprite_ptr sprite, int x, int y)
{
	sprite->direct.x = x;
	sprite->direct.y = y;
	
	if (y == -1)            /* the direction of tank is upward */
	{
		sprite->cur_frame = 0;
	}
	else if (y == 1)							/* down */
	{
		sprite->cur_frame = 1;
	}
	else if (x == -1)            /* left */
	{
		sprite->cur_frame = 2;
	}
	else if (x == 1)             /* right */
	{
		sprite->cur_frame = 3;
	}
	while (sprite->ismove != STEP_PLAYER)
	{
		sprite->x_old = sprite->x;
		sprite->y_old = sprite->y;
		sprite->x = sprite->x_old + sprite->direct.x * PLAYER_SPEED;
		sprite->y = sprite->y_old + sprite->direct.y * PLAYER_SPEED;
		switch(sprite->howmove)
		{
			case 1:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				sprite->ismove--;
				break;
			case 2:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
				sprite->ismove--;
				break;
		}
		if (sprite->ismove == 0)
		{
			sprite->ismove = STEP_PLAYER;
		}
	}
	sprite->x_old = sprite->x;
	sprite->y_old = sprite->y;
	erase_sprite(sprite);
	draw_sprite(sprite);
}	


/***** judge whether tank collide ******/

int tank_collide(sprite_ptr sprite)
{
	int dx, dy;
	int i, j;
	sprite_ptr spr;
	
	spr = Head;
	
	while (spr != NULL)
	{
		if (spr == sprite)
		{
			spr = spr->next;
		}

		dx = abs(sprite->x - spr->x);
		dy = abs(sprite->y - spr->y);
		if (dx < SPRITE_WIDTH && dy <SPRITE_HEIGHT)
		{
			sprite->x = sprite->x_old;
			sprite->y = sprite->y_old;
			return 1;
		} 
		spr = spr->next;
	}
	return 0;
}

/***** move the tank *****/

int moveon(sprite_ptr sprite)
{
	int i, j, ii, jj;

	sprite->x_old = sprite->x;
	sprite->y_old = sprite->y;
	sprite->x += sprite->direct.x * PLAYER_SPEED;
	sprite->y += sprite->direct.y * PLAYER_SPEED;
	if (sprite->x > (COL - 1) * SIZEX || sprite->y > (ROW - 1) * SIZEY || sprite->x < 0 || sprite->y < 0)
	{
		erase_sprite(sprite);
		sprite->x = sprite->x_old;
		sprite->y = sprite->y_old;
		bmp_load_video(sprite->x-sprite->direct.x * PLAYER_SPEED+STARTX, sprite->y-sprite->direct.y * PLAYER_SPEED+STARTY, SPRITE_WIDTH, SPRITE_HEIGHT, Blank);    /* to solve the picture frag */
		behind_sprite(sprite);
		draw_sprite(sprite);
		return 0;	
	}
	if (tank_collide(sprite))
	{
		return 0;
	}
	if (sprite->ismove == STEP_PLAYER)
	{
		if (sprite->direct.y == -1)            /* the direction of tank is upward */
		{
			i = sprite->y_old / (SIZEY / 2) - 1;
			j = sprite->x_old / (SIZEX / 2);
			ii = i;
			jj = j + 1;
		}
		else if (sprite->direct.y == 1)            /* down */
		{
			i = sprite->y_old / (SIZEX / 2) + 2;
			j = sprite->x_old / (SIZEX / 2);
			ii = i;
			jj = j + 1;
		}
		else if (sprite->direct.x == -1)            /* left */
		{
			i = sprite->y_old / (SIZEY / 2);
			j = sprite->x_old / (SIZEX / 2) - 1;
			ii = i + 1;
			jj = j;
		}
		else if (sprite->direct.x == 1)             /* right */
		{
			i = sprite->y_old / (SIZEY / 2);
			j = sprite->x_old / (SIZEX / 2) + 2;
			ii = i + 1;
			jj = j;
		}
	
		switch (Table[i][j])
		{
			case BLANK:
				if (Table[ii][jj] == BLANK || Table[ii][jj] == MASK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					sprite->howmove = 1;
					sprite->ismove--;
				}
				else
				{
					sprite->x = sprite->x_old;
					sprite->y = sprite->y_old;
				}
				return 0;
			case BRICK:
			case STONE:
			case RIVER:
			case CASTLE:
				sprite->x = sprite->x_old;
				sprite->y = sprite->y_old;
				return 0;
			case MASK:
				if(Table[ii][jj] == MASK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					if (i%2 == 1)
					{
						sprite->temp2 = i - 1;
					}
					else 
					{
						sprite->temp2 = i;
					}
					if (j%2 == 1)
					{
						sprite->temp1 = j - 1;
					}
					else 
					{
						sprite->temp1 = j;
					}
					nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
					sprite->howmove = 2;
					sprite->ismove--;
				}
				else if (Table[ii][jj] == BLANK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					sprite->howmove = 1;
					sprite->ismove--;
				}
				else
				{
					sprite->x = sprite->x_old;
					sprite->y = sprite->y_old;
				}		
				return 0;	
		}
	}
	else
	{
		switch(sprite->howmove)
		{
			case 1:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				sprite->ismove--;
				break;
			case 2:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
				sprite->ismove--;
				break;
		}
		if (sprite->ismove == 0)
		{
			sprite->ismove = STEP_PLAYER;
		}
	}
	return 0;
}	


/***** turn to a random direction *****/
					
void rand_turn(sprite_ptr sprite)
{
	int x, y;
	int r;
	static r0 = 0;
  
	randomize();
	r0 += 10;
	if (r0 == 200)
	{
		r0 = 0;
	}
	r = random(700+r0) % 6;     /* get a rand direction */
	
	switch(r)
	{
		case 0:
			x = -sprite->direct.x;
			y = -sprite->direct.y;
			break;
		case 1:          /* to decrese the posibility of turning down */
		case 2:
			x = 0;
			y = 1;
			break;
		case 3:
			x = 0;
			y = -1;
			break;
		case 4: 
			x = 1;
			y = 0;
			break;
		case 5:
			x = -1;
			y = 0;
			break;
	}
	if (!(x==sprite->direct.x && y==sprite->direct.y))
	{
		sprite->direct.x = x;
		sprite->direct.y = y;
	
		if (y == -1)            /* the direction of tank is upward */
		{
			sprite->cur_frame = 0;
		}
		else if (y == 1)							/* down */
		{
			sprite->cur_frame = 1;
		}
		else if (x == -1)            /* left */
		{
			sprite->cur_frame = 2;
		}
		else if (x == 1)             /* right */
		{
			sprite->cur_frame = 3;
		}
		while (sprite->ismove != STEP_ENEMY)
		{
			sprite->x_old = sprite->x;
			sprite->y_old = sprite->y;
			sprite->x = sprite->x_old + sprite->direct.x * PLAYER_SPEED;
			sprite->y = sprite->y_old + sprite->direct.y * PLAYER_SPEED;
			switch(sprite->howmove)
			{
				case 1:
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					sprite->ismove--;
					break;
				case 2:
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
					sprite->ismove--;
					break;
			}
			if (sprite->ismove == 0)
			{
				sprite->ismove = STEP_ENEMY;
			}
		}
		sprite->x_old = sprite->x;
		sprite->y_old = sprite->y;
		erase_sprite(sprite);
		draw_sprite(sprite);
	}
}


/***** move automatically *****/

int auto_moveon(sprite_ptr sprite)
{
	int i, j, ii, jj;
	int r;
	static int r0 = 0;

	sprite->x_old = sprite->x;
	sprite->y_old = sprite->y;
	sprite->x += sprite->direct.x * ENEMY_SPEED;
	sprite->y += sprite->direct.y * ENEMY_SPEED;
	if (sprite->x > (COL - 1) * SIZEX || sprite->y > (ROW - 1) * SIZEY || sprite->x < 0 || sprite->y < 0)
	{
		erase_sprite(sprite);
		sprite->x = sprite->x_old;
		sprite->y = sprite->y_old;
		bmp_load_video(sprite->x-sprite->direct.x * PLAYER_SPEED+STARTX, sprite->y-sprite->direct.y * PLAYER_SPEED+STARTY, SPRITE_WIDTH, SPRITE_HEIGHT, Blank);   /* to solve the picture frag */
		behind_sprite(sprite);
		rand_turn(sprite);
		draw_sprite(sprite);
		return 0;	
	}
	if (tank_collide(sprite))
	{
		rand_turn(sprite);
		return 0;
	}
	if (sprite->ismove == STEP_ENEMY)
	{
		if (sprite->direct.y == -1)            /* the direction of tank is upward */
		{
			i = sprite->y_old / (SIZEY / 2) - 1;
			j = sprite->x_old / (SIZEX / 2);
			ii = i;
			jj = j + 1;
		}
		else if (sprite->direct.y == 1)            /* down */
		{
			i = sprite->y_old / (SIZEX / 2) + 2;
			j = sprite->x_old / (SIZEX / 2);
			ii = i;
			jj = j + 1;
		}
		else if (sprite->direct.x == -1)            /* left */
		{
			i = sprite->y_old / (SIZEY / 2);
			j = sprite->x_old / (SIZEX / 2) - 1;
			ii = i + 1;
			jj = j;
		}
		else if (sprite->direct.x == 1)             /* right */
		{
			i = sprite->y_old / (SIZEY / 2);
			j = sprite->x_old / (SIZEX / 2) + 2;
			ii = i + 1;
			jj = j;
		}
		
		switch (Table[i][j])
		{
			case BLANK:
				if (Table[ii][jj] == BLANK || Table[ii][jj] == MASK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					sprite->howmove = 1;
					sprite->ismove--;
				}
				else
				{
					sprite->x = sprite->x_old;
					sprite->y = sprite->y_old;
					rand_turn(sprite);
				}
				return 0;
			case BRICK:
			case STONE:
			case RIVER:
			case CASTLE:
				sprite->x = sprite->x_old;
				sprite->y = sprite->y_old;
				rand_turn(sprite);
				return 0;
			case MASK:
				if(Table[ii][jj] == MASK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					if (i%2 == 1)
					{
						sprite->temp2 = i - 1;
					}
					else 
					{
						sprite->temp2 = i;
					}
					if (j%2 == 1)
					{
						sprite->temp1 = j - 1;
					}
					else 
					{
						sprite->temp1 = j;
					}
					nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
					sprite->howmove = 2;
					sprite->ismove--;
				}
				else if (Table[ii][jj] == BLANK)
				{
					erase_sprite(sprite);
					behind_sprite(sprite);
					draw_sprite(sprite);
					sprite->howmove = 1;
					sprite->ismove--;
				}
				else
				{
					sprite->x = sprite->x_old;
					sprite->y = sprite->y_old;
					rand_turn(sprite);
				}		
				return 0;	
		}
	}
	else if (sprite->ismove != STEP_ENEMY) 
	{
		switch(sprite->howmove)
		{
			case 1:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				sprite->ismove--;
				break;
			case 2:
				erase_sprite(sprite);
				behind_sprite(sprite);
				draw_sprite(sprite);
				nobg_load_video(STARTX+SIZEX/2*sprite->temp1, STARTY+SIZEY/2*sprite->temp2, SIZEX, SIZEY, Mask, BLACK);
				sprite->ismove--;
				break;
		}
		if (sprite->ismove == 0)
		{
			sprite->ismove = STEP_ENEMY;
			r0 += 10;
			if (r0 == 200)
			{
				r0 = 0;
			}
			r = random(800+r0) % 15;
			if (r == 0)
			{
				rand_turn(sprite); 
			}
		}
	}
	return 0;
}			


/***** the function of player1 *****/

void player1(sprite_ptr sprite)
{
	if (sprite->bullet.flag == 1)	
	{
		bullet_move(sprite);
	}
	
	if (judge_key(W))
	{
		if (!(sprite->direct.y == -1))
		{
			turn(sprite, 0, -1);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(S))
	{
		if (!(sprite->direct.y == 1))
		{
			turn(sprite, 0, 1);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(A))
	{
		if (!(sprite->direct.x == -1))
		{
			turn(sprite, -1, 0);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(D))
	{
		if (!(sprite->direct.x == 1))
		{
			turn(sprite, 1, 0);
		}
		else
		{
			moveon(sprite);
		}
	}
	if (judge_key(H) && sprite->bullet.flag == 0)
	{
		init_bullet(sprite);
		behind_bullet(sprite);
		sprite->bullet.flag = 1;
		sprite->bullet.ismove = sprite->ismove;
		bullet_move(sprite);
	}
}


/***** the function of player2 *****/

void player2(sprite_ptr sprite)
{
	if (sprite->bullet.flag == 1)	
	{
		bullet_move(sprite);
	}
	
	if (judge_key(UP))
	{
		if (!(sprite->direct.y == -1))
		{
			turn(sprite, 0, -1);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(DOWN))
	{
		if (!(sprite->direct.y == 1))
		{
			turn(sprite, 0, 1);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(LEFT))
	{
		if (!(sprite->direct.x == -1))
		{
			turn(sprite, -1, 0);
		}
		else
		{
			moveon(sprite);
		}
	}
	else if (judge_key(RIGHT))
	{
		if (!(sprite->direct.x == 1))
		{
			turn(sprite, 1, 0);
		}
		else
		{
			moveon(sprite);
		}
	}
	if (judge_key(FIGHT) && sprite->bullet.flag == 0)
	{
		init_bullet(sprite);
		behind_bullet(sprite);
		sprite->bullet.flag = 1;
		sprite->bullet.ismove = sprite->ismove;
		bullet_move(sprite);
	}
}


/***** the function of enemy *****/

void enemy(sprite_ptr sprite)
{
  int r;
  static int r0 = 0;
	
	if (sprite->bullet.flag == 1)	
	{
		bullet_move(sprite);
	}

	randomize();
	r0 += 10;
	if (r0 == 200)
	{
		r0 = 0;
	}
	r = random(600+r0) % 5;
	if (r == 0 && sprite->bullet.flag == 0 && sprite->ismove == STEP_ENEMY)
	{
		init_bullet(sprite);
		behind_bullet(sprite);
		sprite->bullet.flag = 1;
		bullet_move(sprite);
	}
	auto_moveon(sprite);
}
	
	
/***** take turns to judge each sprite *****/

void judge_sprite(void)
{
	Pre = Now = Head;
	do
	{  
		if (Now->isexplode)
		{
			explode(Now);
		}
		if (!(Now->isexplode))
		{   
			(*(Now->spr))(Now);
		}
		Pre = Now;
		if (Now != NULL)
		{
			Now = Now->next;
		}
	}
	while(Now != NULL);
	delay(DELAY);
}


/***** check whether players win the game *****/
		
void check_win(void)
{
	if ((Num1 == 0 && (Num2 == 0 || Numplayer == ONEPLAYER)) || Expcastle == 1) 
	{
		Gameresult = FAIL;
	}
	else if (Numalive == 0)
	{
		Gameresult = WIN;
	}
}	

/***** animation when tank is born *****/

void bore(void)
{
	int i;

	for(i=0; i<12; i++)
	{
		nobg_draw_bmp(STARTX+(COL-5)/2*SPRITE_WIDTH, STARTY+(ROW-1)*SPRITE_HEIGHT, Bore[i%4], BLACK);
		if (Numplayer == TWOPLAYER)
		{
			nobg_draw_bmp(STARTX+(COL+3)/2*SPRITE_WIDTH, STARTY+(ROW-1)*SPRITE_HEIGHT, Bore[i%4], BLACK);
		}
		delay(70);
	}
	bmp_load_video(STARTX+(COL+3)/2*SPRITE_WIDTH, STARTY+(ROW-1)*SPRITE_HEIGHT, SIZEX, SIZEY, Blank);
	bmp_load_video(STARTX+(COL-5)/2*SPRITE_WIDTH, STARTY+(ROW-1)*SPRITE_HEIGHT, SIZEX, SIZEY, Blank);
	Now = Head;
	do 
	{
		behind_sprite(Now);
		draw_sprite(Now);
		Now = Now->next;
	}
	while(Now != NULL);
}
	
/***** main function of sprite *****/

int sprite_main(void)
{
	char c;
	char far *buffer;
	
	install_keyboard();
	sprite_build();
	bore();
	
	
	while(1)            /* judge the result */
	{
		if (judge_key(SPACE))              /* stop when press SPACE */
		{
			shutdown_keyboard();
			getch();
			install_keyboard();
		}
		else if (judge_key(ESC))
		{
			if ((buffer = (char far *)malloc(123 * 79)) == NULL)
			{
				printf("Cannot allocate enough buffer\n");
				shutdown_keyboard();
				getch();
				exit(1);
			}
			save_bmp_video(257, 190, 123, 79, buffer);	
			nobg_draw_bmp(257, 190, "graphics\\quitgame.bmp", BLACK);
		
			while(1)
			{
				shutdown_keyboard();
				c = getch();
				install_keyboard();
				if (c=='Y' || c=='y')
				{
					free(buffer);
					Gameresult = GIVEUP;
					shutdown_keyboard();
					return Gameresult;
				}
				if (c=='N' || c=='n')
				{
					bmp_load_video(257, 190, 123, 79, buffer);
					free(buffer);
					break;
				}
			}
		}
		
		judge_sprite();
		
		if (!Isexplode)
		{
			check_win();
		}
		if (Gameresult)
		{
			Now = Head;
			do
			{
				sprite_delete(Now);
				Now = Now->next;
			}
			while (Now != NULL);
			shutdown_keyboard();
			return Gameresult;
		}
	}
}
	


