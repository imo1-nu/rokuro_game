#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <math.h>
#include "winlib_db.h"
#include "kbhit.h"

#define R   30 //球の半径
#define Z   1 //stlファイル書き出し時に、回転体を縦に分割する際の幅。1だと300個に分割される
#define NumOfAngles   360 //stlファイル書き出し時に、回転体を上から見て何角形にするか

//Zを大きくする(300の約数にしてください)
//NumOfAnglesを小さくする(360の約数にしてください)
//などしてから外部ソフトで読み込むと、stlの書き出し方のイメージがつかめると思います

int t, t2; //時間管理
char t_str[20] = "Time      "; //残り時間表示用
int score[2] = {}; //スコアを格納
char highscore[2][4] = {}; //ハイスコア記録用
char score_str[20] = "Score     "; //スコア表示用
int speed = 4; //球の横方向の移動スピード
int speed_z = 20; //球の縦方向の移動スピード
int input; //球が進むかどうかのフラグ
int x, z; //球の位置、xが横、zが縦
int mid; //ろくろの中央のx座標
int x0; //ろくろの最初の幅
int model_x[3][301] = {}; //お題の形を後で格納
int work_x[301]; //作ってるものの形を格納
int Rcos[2 * R + 1]; //球の描画に利用
int mode; //ゲームモード
int end; //終了判定のフラグ
FILE *file; //ファイルを開くために使用

void menu(); //メニュー画面の描画
void countdown(); //カウントダウンの描画
void draw(); //1,2番の描画
void draw_create(); //3番の描画
int calculate(); //ろくろ回しのスコア計算
void *logic(); //ろくろ回しの諸々の計算
void game(); //ゲーム開始時の初期設定
void rokuro(); //ろくろ回し中のキーボードの入力
int openfile(char *filename); //ファイルが開けるかの確認
void print_g_model(); //モデルを描画する関数の書き出し
void print_stl(); //モデルのstlファイルの書き出し

int main()
{
  srand((unsigned int)time(NULL));
  initwin();

  int i; //初期設定
  for (i = 0; i <= 2 * R; i++) {
    Rcos[i] = (int)sqrt(R * R - (i - R) * (i - R));
  }

  for (i = 0; i <= 300; i++) {
    model_x[1][i] = vase(i);
  }
  for (i = 0; i <= 300; i++) {
    model_x[2][i] = wine(i);
  }
  menu();

  return 0;
}

void menu()
{
  g_clear();
  sprintf(highscore[0], "%d", score[0]);
  sprintf(highscore[1], "%d", score[1]);

  int i;
  for (i = 0; i < 65536; i += 60) { //タイトル画面、だんだんと図形がくっきり見える
    if (kbhit()) break; //入力を受け付けるとメニューへ
    g_rgb (65535 - i, 65535 - i, 65535 - i);
    g_draw(1, 125, 250, M_PI / 4, 100); //ろくろで作ったものを描画する関数をもとに作った関数。x11winlib_db.cに入れた。
    g_draw(2, 250, 375, - M_PI / 8, 100);
    g_draw(3, 375, 125, M_PI / 8, 100);
    display_update();
  }

  g_rgb(0, 0, 0);
  mode = 1;
  char c = '\0';
  text(220, 240, "Enter: start");
  display_update();

  while (c != '\n') { //エンターを押すとゲームへ
    if (kbhit()) {
      if (c != '\0') c = getchar();
      else c = getchar() + 1; //タイトル画面でエンターを押してもすぐゲームに移行しないように、最初はc == '\0'にしてある。

      if (c == ' ') mode = mode % 3 + 1; //スペースを押すとモード切替
      g_clear();
      text(230, 100, "Select game mode");
      text(20, 40, "Spase: change game mode");
      text(230, 200, "1. Vase");
      text(340, 200, highscore[0]);
      text(230, 300, "2. Wine grass");
      text(340, 300, highscore[1]);
      text(230, 400, "3. Create");
      g_draw(mode, 205, 100 * (mode + 1) - 5, 0, 20);
      display_update();
    }
  }

  game();
}

void countdown()
{
  int t3;
  if (t < 0) { //開始時のカウントダウン
    g_rgb(0, 0, 0);
    t3 = t;
  }
  else { //残り3秒からのカウントダウン
    g_rgb(65535, 0, 0);
    t3 = t - 300;
  }

  if (t3 >= - 10) { //1の描画
    g_fill(240, 200, 20, 100);
  }
  else if (t3 >= - 20) { //2の描画
    g_fill(225, 200, 50, 20);
    g_fill(255, 200, 20, 50);
    g_fill(225, 240, 50, 20);
    g_fill(225, 250, 20, 50);
    g_fill(225, 280, 50, 20);
  }
  else { //3の描画
    g_fill(225, 200, 50, 20);
    g_fill(255, 200, 20, 50);
    g_fill(225, 240, 50, 20);
    g_fill(255, 250, 20, 50);
    g_fill(225, 280, 50, 20);
  }

  int a = - t3 % 10;
  if (a == 0) a = 10;
  int i;
  for (i = 0; i < a; i++) { //数字の周りの描画
    g_line(250 + 75 * cos(M_PI * (- 5 - 2 * i) / 10), 250 + 75 * sin(M_PI * (- 5 - 2 * i) / 10), 250 + 75 * cos(M_PI * (- 5 - 2 * (i + 1)) / 10), 250 + 75 * sin(M_PI * (- 5 - 2 * (i + 1)) / 10));
  }
}

void draw()
{
  if (t <= 300 && end == 0) {
    g_clear();
    if (t >= 0) text(20,20,t_str);

    text(20,40,"P: push, W: up, S: down, F: finish"); //操作方法

    g_rgb(30000, 30000, 30000); //球の描画
    g_circle(x, z, R); //自作の円状に塗りつぶす関数。x11winlib_db.cに入れた。
    g_circle(250 - x, z, R);
    g_rgb(0, 0, 0);

    int i;
    g_rgb(52000, 37600, 23600); //薄い茶色で対象の形に塗りつぶし
    for (i = 0; i <= 300; i++) {
      g_line(125 - work_x[i], i + 100, 125 + work_x[i], i + 100);
      g_line(375 - model_x[mode][i], i + 100, 375 + model_x[mode][i], i + 100);
    }

    g_rgb(20000, 20000, 20000); //灰色で土台の形に塗りつぶし
    for (i = 0; i <= R; i++) {
      g_line(125 - 5 - R + Rcos[30 + i], i + 400, 125 + 5 + R - Rcos[30 + i], i + 400);
      g_line(375 - 5 - R + Rcos[30 + i], i + 400, 375 + 5 + R - Rcos[30 + i], i + 400);
    }

    for (i = 0; i <= 50; i++) {
      g_line(50, i + 400 + R, 200, i + 400 + R);
      g_line(300, i + 400 + R, 450, i + 400 + R);
    }

    g_rgb(41000, 26600, 12600); //濃い茶色で隙間を残しつつ対象を描画
    for (i = 0; i <= 300; i += (rand() % 20) / 19 + 1) { //5%の確率で隙間が生じる
      g_line(125 - work_x[i], i + 100, 125 + work_x[i], i + 100);
      g_line(375 - model_x[mode][i], i + 100, 375 + model_x[mode][i], i + 100);
    }

    g_rgb(0, 0, 0); //黒で隙間を残しつつ土台を描画
    for (i = 0; i <= R; i += (rand() % 20) / 19 + 1) {  //5%の確率で隙間が生じる
      g_line(125 - 5 - R + Rcos[30 + i], i + 400, 125 + 5 + R - Rcos[30 + i], i + 400);
      g_line(375 - 5 - R + Rcos[30 + i], i + 400, 375 + 5 + R - Rcos[30 + i], i + 400);
    }

    for (i = 0; i <= 50; i += (rand() % 20) / 19 + 1) {
      g_line(50, i + 400 + R, 200, i + 400 + R);
      g_line(300, i + 400 + R, 450, i + 400 + R);
    }

    double co = cos((double)t / 4); //以下薄い縦線の描画
    if (sin((double)t / 4) >= 0) { //表側に来た時
      g_rgb(52000, 37600, 23600);
      for (i = 0; i <= 300; i++) { //横幅 * cosの位置を薄く塗る
        g_line(125 - work_x[i] * co, i + 100, 125 - work_x[i] * co + 1, i + 100);
        g_line(375 - model_x[mode][i] * co, i + 100, 375 - model_x[mode][i] * co + 1, i + 100);
      }

      g_rgb(20000, 20000, 20000);
      for (i = 0; i <= R; i++) {
        g_line(125 - (5 + R -Rcos[30 + i]) * co, i + 400, 125 - (5 + R -Rcos[30 + i]) * co + 1, i + 400);
        g_line(375 - (5 + R - Rcos[30 + i]) * co, i + 400, 375 - (5 + R - Rcos[30 + i]) * co + 1, i + 400);
      }

      for (i = 0; i <= 50; i++) {
        g_line(125 - 75 * co, i + 400 + R, 125 - 75 * co + 1, i + 400 + R);
        g_line(375 - 75 * co, i + 400 + R, 375 - 75 * co + 1, i + 400 + R);
      }
    }

    if (t < 0 || t >= 270 && t < 300) countdown();

    display_update();
  }
  else { //終了時
    end = 1;
    int a = calculate();
    g_rgb(0, 0, 0);
    g_fill(0, 230, 500, 40);
    g_rgb(65535, 65535, 65535);
    text(230, 250, "Finish!");

    display_update();
    usleep(2000000); //2秒待つ
    g_clear();

    int i, j; //だんだん中央に近づく演出
    for (j = 0; j <= 125; j++) {
      g_clear();
      g_rgb(41000, 26600, 12600);
      for (i = 0; i <= 300; i++) {
        g_line(125 + j - work_x[i], i + 100, 125 + j + work_x[i], i + 100);
      }

      g_rgb(0, 0, 0);
      for (i = 0; i <= 300; i++) {
        dot(375 - j - model_x[mode][i], i + 100);
        dot(375 - j + model_x[mode][i], i + 100);
      }
      display_update();
      usleep(20000);
    }

    usleep(1000000);

    for (i = 0; i < 200; i++) {
      sprintf(score_str + 8, "%d", rand() % 101); //デデデーーー、ドン！感の演出
      text(370, 250, score_str);
      display_update();
      usleep(300);
      g_rgb(65535, 65535, 65535);
      g_fill(360, 230, 150, 40);
      g_rgb(0, 0, 0);
    }
    
    if (score[mode - 1] < a) {
      score[mode - 1] = a;
      g_rgb(65535, 0, 0);
      text(370, 230, "High score!");
      g_rgb(0, 0, 0);
    }
    sprintf(score_str + 8, "%d", a);
    text(370, 250, score_str);
    text(370, 270, "Enter: back menu");
    display_update();
  }
}

void draw_create()
{

  if (end == 0) {
    g_clear();
    text(20,40,"P: push, W: up, S: down, F: finish"); //操作方法

    g_rgb(30000, 30000, 30000); //球の描画
    g_circle(x, z, R);
    g_circle(500 - x, z, R);
    g_rgb(0, 0, 0);

    int i; 
    g_rgb(52000, 37600, 23600); //薄い茶色で対象の形に塗りつぶし
    for (i = 0; i <= 300; i++) {
      g_line(250 - work_x[i], i + 100, 250 + work_x[i], i + 100);
    }

    g_rgb(20000, 20000, 20000); //灰色で土台の形に塗りつぶし
    for (i = 0; i <= R; i++) {
      g_line(250 - 5 - R + Rcos[30 + i], i + 400, 250 + 5 + R - Rcos[30 + i], i + 400);
    }

    for (i = 0; i <= 50; i++) {
      g_line(100, i + 400 + R, 400, i + 400 + R);
    }

    g_rgb(41000, 26600, 12600); //濃い茶色で隙間を残しつつ対象を描画
    for (i = 0; i <= 300; i += (rand() % 20) / 19 + 1) { //5%の確率で隙間が生じる
      g_line(250 - work_x[i], i + 100, 250 + work_x[i], i + 100);
    }

    g_rgb(0, 0, 0); //黒で隙間を残しつつ土台を描画
    for (i = 0; i <= R; i += (rand() % 20) / 19 + 1) { //5%の確率で隙間が生じる
      g_line(250 - 5 - R + Rcos[30 + i], i + 400, 250 + 5 + R - Rcos[30 + i], i + 400);
    }

    for (i = 0; i <= 50; i += (rand() % 20) / 19 + 1) {
      g_line(100, i + 400 + R, 400, i + 400 + R);
    }

    double co = cos((double)t / 4); //以下薄い縦線の描画
    if (sin((double)t / 4) >= 0) { //表側に来た時
      g_rgb(52000, 37600, 23600);
      for (i = 0; i <= 300; i++) { //横幅 * cosの位置を薄く塗る
        g_line(250 - work_x[i] * co, i + 100, 250 - work_x[i] * co + 1, i + 100);
      }

      g_rgb(20000, 20000, 20000);
      for (i = 0; i <= R; i++) {
        g_line(250 - (5 + R -Rcos[30 + i]) * co, i + 400, 250 - (5 + R -Rcos[30 + i]) * co + 1, i + 400);
      }

      for (i = 0; i <= 50; i++) {
        g_line(250 - 150 * co, i + 400 + R, 250 - 150 * co + 1, i + 400 + R);
      }
    }

    if (t < 0) countdown();

    display_update();
  }
  else {
    g_rgb(0, 0, 0);
    g_fill(0, 230, 500, 40);
    g_rgb(65535, 65535, 65535);
    text(230, 250, "Finish!");

    display_update();
    usleep(2000000); //2秒待つ

    g_rgb(0, 0, 0);
    text(190, 70, "Check g_model.c model.stl");
    text(190, 90, "Enter: back menu");
    display_update();
  }
}

int calculate()
{
  double sum;
  int i;
  for (i = 0; i <= 300; i++) { //スコアが0~100の間の値を取りつつ、体感の出来と齟齬が生まれないように計算式を考えた
    sum += cbrt((double)(work_x[i] - model_x[mode][i]) * (work_x[i] - model_x[mode][i]) / (75 * 75));
  }

  return 100 - sum / 3;
}

void *logic()
{
  int i, flag;
  while(end == 0){ 
    flag = 0;
    if (input == 1 && x < mid - 5 - R) x += speed; //Pが押されてから少しの間球が移動

    for (i = 0; i <= 300; i++) {
      if (i + 100 >= z - R && i + 100 <= z + R && x + Rcos[i + 100 - z + R] > mid - work_x[i]) {
        work_x[i] = mid - x -  Rcos[i + 100 - z + R];
        flag = 1;
      }
    }

    if (flag == 1 && t % 5 == 0) { //球が削っている間は少しずつ対象が膨らむ
      for (i = 0; i <= 300; i++) {
        if (work_x[i] < x0) work_x[i]++;
      }
    }

    if (t >= 0)sprintf(t_str + 6, "%d",30 - t / 10);

    if (mode == 1 || mode == 2) draw();
    else draw_create();
    usleep(100000); //0.1秒待つ
    t++;
    if (t - t2 == 3) input = 0; //Pを押してから少しすると球が止まる
  }
}

void game()
{
  if (mode == 1 || mode == 2) { //ゲームモードに応じて初期化
    x0 = 75;
    mid = 125;
  }
  else {
    x0 = 150;
    mid = 250;
  }
  t = - 30;
  t2 = 0;
  input = 0;
  x = mid - x0 - R;
  z = 100;
  end = 0;

  int i;
  for (i = 0; i <= 300; i++) {
    work_x[i] = x0;
  }

  pthread_t pthread;
  pthread_create(&pthread, NULL, &logic, NULL);

  rokuro();
  while (getchar() != '\n') {}
  menu();
}


void rokuro()
{
  char c;
  int i;
  while (end == 0)
  {
    if (kbhit())
    {
      c = getchar();
      if (t < 0) continue;
      if (c == 'p')
      {
        input = 1; //球が少しの間中央に進む
        t2 = t;
      }
      if (c == 'w' || c == 's')
      {
        if (c == 'w' && z > 100) z -= speed_z;
        if (c == 's' && z < 400) z += speed_z;

        int max = 250 - R;
        for (i = 0; i <= 300; i++) { //上下に移動する際は、球と制作物が接する位置に移動
          if (z + R >= i + 100 && z - R <= i + 100 && max > mid - work_x[i] -  Rcos[i + 100 - z + R]) {
            max = mid - work_x[i] -  Rcos[i + 100 - z + R];
          }
        }
        x = max;
      }
      if (c == 'f')
      {
        end = 1; //強制終了
        if (mode == 1 || mode == 2) draw();
        else draw_create();
        print_g_model();
        print_stl();
      }
    }
  }
}

int openFile(char filename[])
{
    if ((file = fopen(filename, "w")) == NULL)
    {
        printf("Cannot open file: %s\n", filename);
        return 0;
    }
    else return 1;
}

void print_g_model()
{
  if (openFile("g_model.c")) {
    int i;
    fprintf(file, "#include <stdio.h>\n");
    fprintf(file, "#include <X11/Xlib.h>\n");
    fprintf(file, "#include <X11/Xutil.h>\n");
    fprintf(file, "#include <stdarg.h>\n");
    fprintf(file, "#include <stdlib.h>\n");
    fprintf(file, "#include <string.h>\n");
    fprintf(file, "#include <math.h>\n");
    fprintf(file, "#include %cwinlib_db.h%c\n", '"', '"');
    fprintf(file, "\n");
    fprintf(file, "void g_model(int x, int y, double p, int size)\n");
    fprintf(file, "{\n");
    fprintf(file, " int model_x[301] = {\n");

    for (i = 0; i < 300; i++) {
      fprintf(file, "   %d,\n", work_x[i]);
    }

    fprintf(file, "   %d\n};\n", work_x[300]);
    fprintf(file, "\n");
    fprintf(file, " int i;\n");
    fprintf(file, " double a = (double)300 / size;\n");
    fprintf(file, " for (i = 0; i <= size; i++) {\n"); //回転させれるようにするため、少々式が長くなった
    fprintf(file, "   g_line((int)(cos(p) * - (double)size / 300 * model_x[(int)(i * a)] - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ( - (double)size / 300 * model_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y, (int)(cos(p) * ((double)size / 300 * model_x[(int)(i * a)]) - sin(p) * (- size / 2 + i)) + x, (int)(sin(p) * ((double)size / 300 * model_x[(int)(i * a)]) + cos(p) * (- size / 2 + i)) + y);\n");
    fprintf(file, " }\n");
    fprintf(file, "}\n");
    fclose(file);
  }
}

//回転体を正多角形を積み立てたものと考えている。縦は300に分割して考えている。
//stlファイルは、まず正多角形の一辺について縦に積み上げていき、それを角の数だけ繰り返すイメージ。
void print_stl() 
{
  if (openFile("model.stl")) {
    int i, j;
    double tan[300 / Z];
    for (i = 0; i < 300 / Z; i++) {
      tan[i] = (double)(work_x[Z * (i + 1)] - work_x[Z * i]) / Z;
    }

    fprintf(file, "solid cone\n");
    for (i = 0; i < NumOfAngles; i++) { //正多角形の一辺について考え、それを角の数だけ繰り返す
      fprintf(file, " facet normal 0 0 -1\n"); //底面の情報。法線ベクトル→3点の座標の順
      fprintf(file, "   outer loop\n");
      fprintf(file, "     vertex 0 0 0\n");
      fprintf(file, "     vertex %lf %lf 0\n", work_x[0] * cos(2 * M_PI * i / NumOfAngles), work_x[0] * sin(2 * M_PI * i / NumOfAngles));
      fprintf(file, "     vertex %lf %lf 0\n", work_x[0] * cos(2 * M_PI * (i + 1) / NumOfAngles), work_x[0] * sin(2 * M_PI * (i + 1) / NumOfAngles));
      fprintf(file, "   endloop\n");
      fprintf(file, " endfacet\n");
      for (j = 0; j < 300; j += Z) { //側面の情報。300個生まれる長方形を2つの直角三角形に分解して考えている。
        fprintf(file, " facet normal %lf %lf %lf\n", cos(M_PI * (1 + 2 * i) / NumOfAngles), sin(M_PI * (1 + 2 * i) / NumOfAngles), -tan[j / Z]);
        fprintf(file, "   outer loop\n");
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j] * cos(2 * M_PI * i / NumOfAngles), work_x[j] * sin(2 * M_PI * i / NumOfAngles), j);
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j] * cos(2 * M_PI * (i + 1) / NumOfAngles), work_x[j] * sin(2 * M_PI * (i + 1) / NumOfAngles), j);
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j + Z] * cos(2 * M_PI * (i + 1) / NumOfAngles), work_x[j + Z] * sin(2 * M_PI * (i + 1) / NumOfAngles), j + Z);
        fprintf(file, "   endloop\n");
        fprintf(file, " endfacet\n");
        fprintf(file, " facet normal %lf %lf %lf\n", cos(M_PI * (1 + 2 * i) / NumOfAngles), sin(M_PI * (1 + 2 * i) / NumOfAngles), -tan[j / Z]);
        fprintf(file, "   outer loop\n");
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j] * cos(2 * M_PI * i / NumOfAngles), work_x[j] * sin(2 * M_PI * i / NumOfAngles), j);
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j + Z] * cos(2 * M_PI * (i + 1) / NumOfAngles), work_x[j + Z] * sin(2 * M_PI * (i + 1) / NumOfAngles), j + Z);
        fprintf(file, "     vertex %lf %lf %d\n", work_x[j + Z] * cos(2 * M_PI * i / NumOfAngles), work_x[j + Z] * sin(2 * M_PI * i / NumOfAngles), j + Z);
        fprintf(file, "   endloop\n");
        fprintf(file, " endfacet\n");
      }
      fprintf(file, " facet normal 0 0 1\n"); //上面の情報
      fprintf(file, "   outer loop\n");
      fprintf(file, "     vertex 0 0 300\n");
      fprintf(file, "     vertex %lf %lf 300\n", work_x[300] * cos(2 * M_PI * i / NumOfAngles), work_x[300] * sin(2 * M_PI * i / NumOfAngles));
      fprintf(file, "     vertex %lf %lf 300\n", work_x[300] * cos(2 * M_PI * (i + 1) / NumOfAngles), work_x[300] * sin(2 * M_PI * (i + 1) / NumOfAngles));
      fprintf(file, "   endloop\n");
      fprintf(file, " endfacet\n");
    }
    fprintf(file, "endsolid cone\n");

    fclose(file);
  }
}