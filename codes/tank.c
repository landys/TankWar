
/********************************************************************/
/********************* Play Tank War From Here **********************/
/********************************************************************/

#include "define.c"
#include "graphic.c"
#include "key.c"
#include "word.c"
#include "initial.c"
#include "sprite.c"


int main(void)
{
	Savemode = get_video_mode();     /* get the current video mode */
	set_video_mode(0x101);		/* set video mode */

	choose_to_begin();    /* choose to begin game or see help or exit */

	do
	{
		new_game();		/* initial a new game */
		sprite_main();     /* the main function when playing the Tank War */
		if (Gameresult == FAIL)
		{
			game_over();     /* a game over */
			choose_to_begin();     /* choose something to begin the game */
		}
	}
	while (Gameresult != GIVEUP);       /* find what to do after a game */
	
	end_game();    /* end the game */

	set_video_mode(Savemode);		/* recover the video mode */ 
	return 0;
}



/********************************************************************/
/*************************** End of Program *************************/
/***************************** Thank you ****************************/
/********************************************************************/