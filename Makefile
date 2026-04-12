main: main.c geo.c xlib.c
	cc geo.c xlib.c main.c -o main -I/usr/local/include -L/usr/local/lib -lX11 -lXext -lm
