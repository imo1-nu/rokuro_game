# 実行までの流れ
XQuartzをインストール済みのMacで以下を行う。

gcc -c -I/opt/X11/include -L/opt/X11/lib -lX11 x11winlib_db.c

gcc -c -I/opt/X11/include -L/opt/X11/lib -lX11 kbhit.c

gcc -I/opt/X11/include saisyu.c x11winlib_db.o kbhit.o -L/opt/X11/lib -lX11 -pthread

./a.out

# 作成したmodel.stlを表示できるwebサイト
私が作成したものではありません。

https://imagetostl.com/jp/view-stl-online
