#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "winlib_db.h"

Display *disp;
int scrn;
Window win;
Colormap cmap;
GC    gc;
XImage *image;
XColor color;
Pixmap pixmap; //for double buffering
FILE *fp;

int width= DEFAULT_WIDTH;
int height= DEFAULT_HEIGHT;


void FatalError(char *str)
{
  if (fp != NULL) {
    fclose(fp);
  }
  fprintf(stderr,"Error:%s\n",str);
  
  fflush(stderr);
  exit(1);
}



void initwin(){
  XSetWindowAttributes wa;
  XVisualInfo vi;
  XSizeHints hints;
  XEvent event;
  if((disp =XOpenDisplay(NULL))==NULL){
    FatalError("Can't open display.");
  }
  scrn = DefaultScreen(disp);

  /*  if(! XMatchVisualInfo(disp,scrn,DISPLAY_DEPTH,DirectColor, &vi)){
    FatalError("Can't open color display.");
    }
  wa.background_pixel = 0;
  wa.border_pixel =0;
  wa.colormap =XCreateColormap(disp,RootWindow(disp,scrn),
			      vi.visual,AllocNone);
  wa.event_mask = KeyPressMask |StructureNotifyMask |ExposureMask
    |ButtonPressMask;
  */
  win =XCreateSimpleWindow(disp, RootWindow(disp,scrn),0,0,width,height,2,
			   2, WhitePixel(disp,scrn) );
  pixmap = XCreatePixmap(disp, win, width, height, DefaultDepth(disp, 0));
  
  /*  image =XCreateImage(disp, qvi.visual, vi.depth,
		      ZPixmap, 0, 0, width, height, 32, 0);
  if ((image->data = (char *)malloc(image->bytes_per_line*height)) ==NULL){
    FatalError("No memory..");
  }
  memset(image->data,0, image->bytes_per_line*height);
  */
  gc= XCreateGC(disp, win, 0,0);
  XSetForeground(disp, gc, BlackPixel(disp,scrn));
  XSetBackground(disp, gc, WhitePixel(disp,scrn));
  XSetFunction(disp, gc, GXcopy);
  XMapRaised(disp,win);
  g_clear();
  XFlush(disp);
}

void dot(int x, int y){
  XDrawLine(disp,pixmap,gc,x,y,x,y);
}

void text(int x, int y,char *str){
  XDrawString(disp,pixmap,gc,x,y,str,strlen(str));
}

void g_line(int x0, int y0, int x1, int y1){
  XDrawLine(disp,pixmap,gc,x0,y0,x1,y1);
}

void g_rgb(unsigned short r, unsigned short g, unsigned short b){
  XColor xc;
  xc.red = r; xc.green = g; xc.blue = b;
  XAllocColor(disp,DefaultColormap(disp,scrn),&xc);
  XSetForeground(disp,gc,xc.pixel);
}

void g_fill(int x0,int y0, int wid, int hei){
  XFillRectangle(disp,pixmap,gc,x0,y0,wid, hei);
}

void g_box(int x0, int y0, int wid, int hei){
  XDrawRectangle(disp,pixmap,gc,x0,y0,wid, hei);
}

void g_circle(int x0, int y0, int r){
  double i;
  for (i = 0; i < 180; i += 1) {
    g_line(x0 + r * cos(2 * M_PI * i / 360), y0 + r * sin(2 * M_PI * i / 360), x0 + r * cos(2 * M_PI * i / 360), y0 - r * sin(2 * M_PI * i / 360));
  }
}

void g_clear(){
  g_rgb(65535,65535,65535);
  XFillRectangle(disp,pixmap,gc,0,0,width, height);  
  g_rgb(0,0,0);
}

void display_update(){ //for double buffering
  XCopyArea(disp,pixmap,win,gc,0,0,width,height,0,0);
  XFlush(disp);
}

int vase_x[301] = {
   39,
   39,
   39,
   39,
   38,
   38,
   38,
   38,
   37,
   38,
   37,
   38,
   37,
   37,
   37,
   37,
   36,
   36,
   36,
   36,
   35,
   35,
   35,
   35,
   34,
   35,
   34,
   33,
   33,
   32,
   32,
   31,
   31,
   31,
   30,
   30,
   30,
   30,
   29,
   29,
   29,
   29,
   28,
   29,
   28,
   29,
   28,
   28,
   28,
   27,
   27,
   26,
   26,
   26,
   25,
   26,
   25,
   25,
   25,
   25,
   24,
   25,
   24,
   24,
   24,
   24,
   23,
   23,
   23,
   23,
   22,
   23,
   23,
   23,
   24,
   25,
   24,
   25,
   25,
   25,
   26,
   26,
   26,
   27,
   27,
   27,
   27,
   28,
   27,
   28,
   28,
   28,
   28,
   29,
   29,
   29,
   30,
   31,
   31,
   31,
   32,
   32,
   33,
   33,
   34,
   34,
   35,
   36,
   35,
   36,
   36,
   36,
   37,
   37,
   38,
   38,
   39,
   39,
   39,
   40,
   39,
   40,
   40,
   40,
   40,
   41,
   41,
   41,
   41,
   42,
   41,
   42,
   42,
   42,
   42,
   43,
   42,
   43,
   43,
   43,
   43,
   44,
   43,
   44,
   43,
   44,
   43,
   44,
   44,
   44,
   44,
   45,
   44,
   45,
   45,
   45,
   45,
   46,
   45,
   46,
   45,
   46,
   45,
   46,
   46,
   46,
   46,
   47,
   46,
   47,
   46,
   47,
   47,
   47,
   47,
   48,
   47,
   48,
   47,
   48,
   47,
   48,
   48,
   48,
   48,
   49,
   48,
   49,
   48,
   49,
   48,
   49,
   48,
   49,
   49,
   49,
   49,
   50,
   50,
   50,
   50,
   51,
   50,
   51,
   50,
   51,
   50,
   51,
   50,
   51,
   50,
   51,
   50,
   51,
   50,
   51,
   51,
   51,
   52,
   53,
   52,
   53,
   52,
   53,
   52,
   53,
   52,
   53,
   53,
   53,
   53,
   54,
   53,
   54,
   53,
   54,
   53,
   54,
   53,
   54,
   54,
   54,
   54,
   55,
   54,
   55,
   54,
   55,
   54,
   54,
   54,
   54,
   53,
   54,
   53,
   54,
   53,
   54,
   53,
   53,
   53,
   53,
   52,
   52,
   52,
   52,
   51,
   52,
   51,
   51,
   51,
   51,
   50,
   51,
   50,
   51,
   50,
   50,
   50,
   49,
   49,
   49,
   47,
   46,
   46,
   46,
   45,
   45,
   45,
   45,
   44,
   43,
   43,
   42,
   42,
   41,
   40,
   39,
   39,
   39,
   38
};

int vase(int z)
{
  return vase_x[z];
}

int wine_x[301] = {
   38,
   39,
   39,
   39,
   39,
   40,
   40,
   40,
   40,
   41,
   41,
   41,
   41,
   42,
   42,
   42,
   42,
   43,
   43,
   43,
   43,
   44,
   44,
   44,
   44,
   45,
   45,
   45,
   45,
   46,
   46,
   46,
   46,
   47,
   47,
   47,
   47,
   48,
   48,
   48,
   48,
   49,
   49,
   49,
   49,
   50,
   49,
   50,
   50,
   50,
   50,
   51,
   50,
   51,
   51,
   51,
   51,
   52,
   51,
   52,
   52,
   52,
   52,
   53,
   52,
   53,
   53,
   53,
   53,
   54,
   53,
   54,
   54,
   54,
   54,
   55,
   55,
   55,
   55,
   56,
   56,
   56,
   56,
   57,
   56,
   57,
   57,
   57,
   57,
   58,
   57,
   58,
   58,
   58,
   58,
   59,
   58,
   59,
   59,
   59,
   59,
   60,
   59,
   60,
   60,
   60,
   60,
   61,
   60,
   61,
   61,
   61,
   61,
   62,
   61,
   61,
   61,
   61,
   60,
   60,
   60,
   59,
   59,
   58,
   57,
   55,
   52,
   51,
   49,
   46,
   43,
   42,
   39,
   36,
   33,
   32,
   30,
   27,
   24,
   21,
   18,
   17,
   16,
   15,
   14,
   13,
   13,
   12,
   12,
   12,
   11,
   11,
   11,
   10,
   10,
   10,
   9,
   8,
   8,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   7,
   8,
   8,
   8,
   9,
   12,
   15,
   18,
   21,
   24,
   26,
   29,
   30,
   33,
   34,
   37,
   39,
   42,
   44,
   47,
   49,
   52,
   51
};

int wine(int z)
{
  return wine_x[z];
}

void g_draw(int mode, int x, int y, double p, int size)
{
  int i;
  double a = (double)300 / size;
  int box_x[301];
  for (i = 0; i <= 300; i++) {
    box_x[i] = 150;
  }

  if (mode == 1) {
    for (i = 0; i <= size; i++) {
      g_line((int)(cos(p) * - (double)size / 300 * vase_x[(int)(i * a)] - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ( - (double)size / 300 * vase_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y, (int)(cos(p) * ((double)size / 300 * vase_x[(int)(i * a)]) - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ((double)size / 300 * vase_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y);
    }
  }
  else if (mode == 2) {
    for (i = 0; i <= size; i++) {
      g_line((int)(cos(p) * - (double)size / 300 * wine_x[(int)(i * a)] - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ( - (double)size / 300 * wine_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y, (int)(cos(p) * ((double)size / 300 * wine_x[(int)(i * a)]) - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ((double)size / 300 * wine_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y);
    }
  }
  else {
    for (i = 0; i <= size; i++) {
      g_line((int)(cos(p) * - (double)size / 300 * box_x[(int)(i * a)] - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ( - (double)size / 300 * box_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y, (int)(cos(p) * ((double)size / 300 * box_x[(int)(i * a)]) - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ((double)size / 300 * box_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y);
    }
  }
}

void g_model(int x, int y, double p, int size)
{
 int model_x[301] = {
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   148,
   146,
   145,
   144,
   143,
   142,
   141,
   140,
   140,
   139,
   139,
   138,
   138,
   137,
   137,
   137,
   137,
   137,
   137,
   136,
   136,
   135,
   135,
   135,
   135,
   134,
   134,
   133,
   133,
   133,
   133,
   132,
   132,
   131,
   131,
   131,
   131,
   131,
   131,
   130,
   130,
   129,
   129,
   129,
   129,
   128,
   128,
   127,
   127,
   127,
   127,
   127,
   127,
   126,
   126,
   125,
   125,
   125,
   125,
   124,
   124,
   123,
   123,
   123,
   123,
   123,
   123,
   122,
   122,
   121,
   121,
   121,
   121,
   121,
   121,
   120,
   120,
   119,
   119,
   119,
   119,
   118,
   118,
   117,
   117,
   117,
   117,
   117,
   117,
   116,
   116,
   115,
   115,
   115,
   115,
   114,
   112,
   110,
   109,
   108,
   107,
   106,
   106,
   104,
   102,
   100,
   98,
   96,
   92,
   90,
   84,
   78,
   72,
   62,
   56,
   50,
   44,
   42,
   40,
   38,
   37,
   36,
   35,
   34,
   34,
   33,
   33,
   32,
   32,
   31,
   31,
   31,
   31,
   31,
   31,
   30,
   31,
   31,
   31,
   31,
   31,
   31,
   32,
   32,
   33,
   33,
   34,
   34,
   35,
   36,
   37,
   38,
   40,
   42,
   44,
   50,
   56,
   62,
   70,
   76,
   78,
   82,
   84,
   90,
   92,
   94,
   96,
   98,
   100,
   101,
   101,
   101,
   101,
   101,
   102,
   102,
   103,
   103,
   103,
   103,
   104,
   104,
   105,
   105,
   105,
   105,
   105,
   105,
   106,
   106,
   107,
   107,
   107,
   107,
   108,
   108,
   109,
   109,
   109,
   109,
   109,
   109,
   110,
   110,
   111,
   111,
   111,
   111,
   112,
   112,
   113,
   113,
   113,
   113,
   114,
   114,
   115,
   115,
   115,
   115,
   115,
   115,
   116,
   116,
   117,
   117,
   117,
   117,
   118,
   118,
   119,
   119,
   119,
   119,
   119,
   119,
   120,
   120,
   121,
   121,
   121,
   121,
   122,
   122,
   123,
   123,
   123,
   122,
   123,
   123,
   123,
   123,
   123,
   123,
   124,
   124,
   125,
   125,
   126,
   126,
   127,
   128,
   129,
   130,
   132,
   134,
   136,
   142,
   144,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150,
   150
};

 int i;
 double a = (double)300 / size;
 for (i = 0; i <= size; i++) {
   g_line((int)(cos(p) * - (double)size / 300 * model_x[(int)(i * a)] - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ( - (double)size / 300 * model_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y, (int)(cos(p) * ((double)size / 300 * model_x[(int)(i * a)]) - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ((double)size / 300 * model_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y);
 }
}