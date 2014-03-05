
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

#define WORD0 "你好！"      /* define the about words */  
#define WORD1 "按任意键退出此帮助界面。" 
#define WORD2 "欢迎阅读游戏简介。"
#define WORD3 "本游戏模仿街机坦克大战，"
#define WORD4 "共设了九个关卡。"
#define WORD5 "玩家坦克必须保护鹰形堡垒。"
#define WORD6 "一旦被炮弹击中即告失败。"
#define WORD7 "玩家每人有四次生命，"
#define WORD8 "每过一关生命加一，"
#define WORD9 "被敌方全部消灭也告失败。"
#define WORD10 "敌方有十五辆坦克，"
#define WORD11 "消灭敌方所有坦克即获胜。"
#define WORD12 "本游戏仅用以娱乐。"
#define WORD13 "游戏中图片源自网上并经修改。"
#define WORD14 "一切源代码开放。"
#define WORD15 "多谢。"


