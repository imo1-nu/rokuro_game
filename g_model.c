#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "winlib_db.h"

void g_model(int x, int y, double p, int size)
{
 int model_x[301] = {
   86,
   87,
   87,
   87,
   87,
   87,
   87,
   87,
   88,
   88,
   88,
   89,
   89,
   89,
   90,
   91,
   91,
   91,
   91,
   91,
   90,
   90,
   89,
   89,
   88,
   88,
   87,
   86,
   86,
   86,
   85,
   85,
   85,
   84,
   84,
   84,
   84,
   84,
   83,
   82,
   81,
   80,
   79,
   79,
   78,
   78,
   77,
   76,
   76,
   76,
   75,
   75,
   75,
   74,
   74,
   73,
   71,
   70,
   69,
   68,
   67,
   66,
   65,
   65,
   64,
   64,
   63,
   62,
   62,
   62,
   61,
   61,
   61,
   60,
   60,
   60,
   60,
   60,
   60,
   60,
   59,
   60,
   60,
   60,
   60,
   60,
   59,
   58,
   58,
   58,
   57,
   57,
   57,
   56,
   56,
   56,
   56,
   55,
   54,
   53,
   52,
   51,
   50,
   50,
   49,
   49,
   48,
   47,
   47,
   47,
   46,
   46,
   46,
   45,
   45,
   43,
   41,
   40,
   39,
   38,
   37,
   36,
   35,
   35,
   34,
   34,
   33,
   32,
   32,
   32,
   31,
   31,
   31,
   30,
   30,
   30,
   30,
   30,
   30,
   30,
   29,
   30,
   30,
   30,
   30,
   30,
   30,
   30,
   31,
   31,
   31,
   32,
   32,
   32,
   33,
   34,
   34,
   35,
   35,
   36,
   37,
   38,
   39,
   39,
   39,
   39,
   39,
   39,
   40,
   40,
   40,
   41,
   41,
   41,
   42,
   43,
   43,
   44,
   44,
   45,
   46,
   47,
   48,
   49,
   50,
   52,
   54,
   55,
   58,
   61,
   68,
   74,
   74,
   74,
   75,
   76,
   76,
   77,
   77,
   78,
   79,
   80,
   81,
   82,
   83,
   85,
   87,
   88,
   91,
   94,
   95,
   96,
   96,
   96,
   97,
   98,
   98,
   99,
   99,
   100,
   101,
   102,
   103,
   104,
   105,
   107,
   109,
   110,
   113,
   116,
   119,
   120,
   120,
   120,
   121,
   122,
   122,
   123,
   123,
   124,
   125,
   126,
   127,
   128,
   128,
   128,
   128,
   128,
   129,
   129,
   129,
   130,
   130,
   130,
   131,
   132,
   132,
   133,
   133,
   134,
   135,
   136,
   137,
   138,
   139,
   141,
   141,
   141,
   142,
   142,
   142,
   143,
   143,
   143,
   144,
   145,
   145,
   146,
   146,
   147,
   148,
   149,
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
