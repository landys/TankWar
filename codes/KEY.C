
/***** get the pressed key *****/

#define ENTER 0x1c      /* define some key scan code used */
#define SPACE 0x39
#define ESC 0x01
#define UP 0x48
#define DOWN 0x50
#define LEFT 0x4b
#define RIGHT 0x4d
#define FIGHT 0x52
#define W 0x11
#define S 0x1f
#define A 0x1e
#define D 0x20
#define H 0x23


static char keystate[128], keypressed[128];	/* save the state of keyboard */
static void interrupt far (*old_int9_handler)(void);	/* save the present system keyboard interupt */


/***** delay exact time *****/

void exact_delay(int clicks)
{
	unsigned int far *clock = (unsigned int far *)0x0000046CL;
	unsigned int now;
	
	now = *clock;
	while(abs(*clock-now) < clicks);
}


/***** new keyboard interupt *****/

void far interrupt new_int9(void)			
{
	unsigned char scancode, temp;

	scancode = inportb(0x60);
	temp = inportb(0x61);
	outportb(0x61, temp|0x80);
	outportb(0x61, temp&0x7f);

	if (scancode & 0x80)
	{
		scancode &= 0x7f;
		keystate[scancode] = 0;
	}
	else
	{
		keystate[scancode] = 1;
		keypressed[scancode] = 1;
	}
	outportb(0x20, 0x20);
}


/***** install keyboard interupt *****/

void install_keyboard(void)	
{			
	int i;

	disable();
	for (i=0; i<128; i++)
	{
		keystate[i] = keypressed[i]= 0;
	}
	old_int9_handler = getvect(9);
	setvect(9, new_int9);
	enable();
}


/***** recover the present keyboard interupt *****/

void shutdown_keyboard(void)			
{
	disable();
	setvect(9, old_int9_handler);
	enable();
}


/***** judge the key pressed *****/

int judge_key(int scancode)			
{
	int res;

	res = keystate[scancode] | keypressed[scancode];
	keypressed[scancode] = 0;
	return res;
}


/***** read scan code from keyboard buffer *****/

int scan_key(void)
{
  int  start, end;
	int key = 0;
  
  start = peek(0, 0x41a);
  end = peek(0, 0x41c);
  if (start == end) 
  {
  	return(0);
  }
  else
  {
    key =  peek(0x40, start);
    start += 2;
    if (start == 0x3e) 
    {
    	start = 0x1e;
    }
    poke(0x40, 0x1a, start);
    return(key/256);
  }
}

