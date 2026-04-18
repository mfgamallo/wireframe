main: main.c geo.c xlib.c state.c
	cc state.c geo.c xlib.c main.c -o main -I/usr/local/include -L/usr/local/lib -lX11 -lXext -lm
