#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void initwin();
void dot(int x,int y);
void text(int x,int y,char *str);

void g_line(int x0,int y0, int x1, int y1);
void g_rgb(unsigned short r,unsigned short g, unsigned short b);
void g_fill(int x0,int y0, int wid, int hei);
void g_box(int x0,int y0, int wid, int hei);
void g_circle(int x0, int y0, int r);
void g_clear();
void g_model(int x, int y, double p, int size);
int vase(int z);
int wine(int z);
void g_draw(int mode, int x, int y, double p, int size);
void display_update();

#define DEFAULT_WIDTH    500
#define DEFAULT_HEIGHT   500
#define DISPLAY_DEPTH    8

