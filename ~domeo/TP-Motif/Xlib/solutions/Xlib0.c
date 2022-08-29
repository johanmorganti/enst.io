#include <stdio.h>
#include <X11/Xlib.h>
 
#define LARGE 200
#define HAUT 100
 
char * display_name = NULL;
unsigned long white_pixel, black_pixel;
Display * dpy; 
Window rootw, win;
Window winf1 , winf2,winf3;
int screen;   
 
main(argc,argv)  
        int argc;
        char * argv[];
{
        if (argc == 3 && !strcmp(argv[1],"-display"))
                display_name = argv[2];
        dpy = XOpenDisplay(display_name);
        if (dpy == NULL){
                fprintf(stderr,"impossible d'ouvrir le display!\n");
                exit(1);
        }
        screen = DefaultScreen(dpy);
        rootw = RootWindow(dpy,screen);
        white_pixel = WhitePixel(dpy,screen);
        black_pixel = BlackPixel(dpy,screen);

        win = XCreateSimpleWindow(dpy,rootw,100,100,
                LARGE, HAUT,2,white_pixel,black_pixel);
        winf1 = XCreateSimpleWindow(dpy,win,0,0,
                LARGE/3, HAUT/3,1,black_pixel,white_pixel);
        winf2 = XCreateSimpleWindow(dpy,win,30,30,
                LARGE/3, HAUT/3,2,black_pixel,white_pixel);
        winf3 = XCreateSimpleWindow(dpy,win,60,60,
                LARGE/3, HAUT/3,3,black_pixel,white_pixel);
	XMapSubwindows(dpy,win);
        XMapWindow(dpy,win);
        XFlush(dpy);
        getchar();
}

