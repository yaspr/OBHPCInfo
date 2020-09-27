/*
  Fourrier Transform visualization
  
 */

#include <math.h>
#include <stdio.h>
#include <unistd.h>

#include "flame.h"

#include "pi.h"

#define MAX 100

#define COLOR_WHITE 0xFF
#define COLOR_BLACK 0x00

//
typedef unsigned char byte;
typedef unsigned long long u64;

//
void draw_circle(flame_obj_t *fo, float x, float y, float r, byte col_r, byte col_g, byte col_b)
{
  flame_set_color(fo, col_r, col_g, col_b);
  
  XDrawArc(fo->display, fo->window, fo->gc, x - (r / 2), y - (r / 2), r, r, 0, 360 * 64);
}

//
int main(int argc, char **argv)
{
  char c;

  //Event for keyboard & mouse interruptions
  XEvent event;

  //Set display size
  int x_min = 0, x_max = 1920;
  int y_min = 0, y_max = 1080;

  //Initialize display
  flame_obj_t *fo = flame_open("Butterfly", x_max, y_max);

  //
  int scale = 1;
  int bx = 0, by = y_max - 200; //Start in the middle of the screen

  //
  u64 n = 10;             
  float x, y, px, py, rounds = 5, f[MAX], r[MAX];
  
  //
  for (u64 i = 0; i < n; i++)
    {
      f[i] = 2 * (i + 1) - 1; // i + 1
      r[i] = 120 / f[i];      // 80 / f[i];
    }
  
 lbl_start:
  
  //
  while (1)
    {
      //Handle input
      if (XPending(fo->display) > 0)
	{
	  XNextEvent(fo->display, &event);

	  //Keyboard input
	  if (event.type == KeyPress)
	    {
	      c = XLookupKeysym(&event.xkey, 0);
	      
	      if (c == 'q')
		break;
	      else
		if (c == 'r')
		  {
		    flame_clear_display(fo);
		    goto lbl_start;
		  }
		else
		  {
		    ;
		  }
	    }
	  else //Mouse input
	    if (event.type == ButtonPress)
	      {
	      }
	}
      else
	{
	  for (float a = 0.0; a < rounds * 2 * PI; a += 0.001)
	    {
	      x = y = 0.0;
	      px = py = 0.0;
	      
	      //Leave a trail
	      for (u64 i = 0; i < n; i++)
	  	{
		  draw_circle(fo, bx + px + 400, by - (py + 400), r[i], 0, 0, 0);
		  
		  draw_circle(fo, bx + x + 400, by - (y + 400), r[i], COLOR_WHITE - (r[i]), COLOR_WHITE - (r[i] * 5), COLOR_WHITE - (r[i] * 3));
		  
		  px = x;
		  py = y;
		  
		  x += r[i] * cos(f[i] * a);
	  	  y += r[i] * sin(f[i] * a);
		  
		  usleep(10);
		}

	      flame_set_color(fo, 0, 255, 0);
	      flame_draw_point(fo, bx + a * 20 + 900, by - (y + 400));

	      usleep(1000);
	    }

	  flame_clear_display(fo);
	}
    }
  
  flame_close(fo);
  
  return 0;
}
