
/****** define the initial data ******/

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <math.h>

#define YELLOW -5        /* define some color game used */
#define RED -62
#define BLUE -6
#define WHITE -1
#define BLACK 0

#define WORD_STARTX 350   /* define the start x of words */
#define WORD_STARTY 5   /* define the start y of words */
#define WORD_HEIGHT 24    /* define the height of a word */
#define HELP_STARTX 50    /* define the start x of help.bmp */
#define HELP_STARTY 18     /* define the start y of help.bmp */

#define FAIL 1       /* fail a game */ 
#define WIN 2		  /* win a game */
#define GIVEUP 3       /* give up a game */

#define ONEPLAYER 1     /* define option of game beginning */
#define TWOPLAYER 2
#define HELP 3
#define EXIT 4

#define VIDEOROW 480     /* height and width of video mode */
#define VIDEOCOL 640
#define LEVEL 50       /* number of levels */
#define ROW 15          /* row and col of blocks of the map */
#define COL 17
#define SIZEX 32      /* size of a block */
#define SIZEY 32
#define STARTX 19		/* the start of the screen */
#define STARTY 0

#define NUMENEMY 15 	/* the total enemy */
#define NUMPLAYER 4     /* the total tanks of player */

#define BLANK 0          /* define block tank and such things */
#define BRICK 1
#define STONE 2
#define MASK 3
#define RIVER 4
#define CASTLE 5

#define BULLET1 1        /* the bullet of player's */
#define BULLET2 2         /* the bullet of enemy's */
#define PLAYER1 1        /* define marks of tank to differ tanks */
#define PLAYER2 2
#define ENEMY1 3
#define ENEMY2 4
#define ENEMY3 5

#define SPRITE_WIDTH 32     /* the width and height of sprite and bullet */ 
#define SPRITE_HEIGHT 32
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 4

#define PLAYER_SPEED 8     /* define the speed of player, enemy and bullet */
#define ENEMY_SPEED 4
#define BULLET_SPEED 8
#define DELAY 54       
#define STEP_PLAYER (SIZEX / PLAYER_SPEED / 2)
#define STEP_ENEMY (SIZEX / PLAYER_SPEED / 2)
#define MAX_SPRITE_FRAMES 4     /* define the number of frames of the sprite */

#define WORD0 "��ã�"      /* define the about words */  
#define WORD1 "��������˳��˰������档" 
#define WORD2 "��ӭ�Ķ���Ϸ��顣"
#define WORD3 "����Ϸģ�½ֻ�̹�˴�ս��"
#define WORD4 "�����˾Ÿ��ؿ���"
#define WORD5 "���̹�˱��뱣��ӥ�α��ݡ�"
#define WORD6 "һ�����ڵ����м���ʧ�ܡ�"
#define WORD7 "���ÿ�����Ĵ�������"
#define WORD8 "ÿ��һ��������һ��"
#define WORD9 "���з�ȫ������Ҳ��ʧ�ܡ�"
#define WORD10 "�з���ʮ����̹�ˣ�"
#define WORD11 "����з�����̹�˼���ʤ��"
#define WORD12 "����Ϸ���������֡�"
#define WORD13 "��Ϸ��ͼƬԴ�����ϲ����޸ġ�"
#define WORD14 "һ��Դ���뿪�š�"
#define WORD15 "��л��"

