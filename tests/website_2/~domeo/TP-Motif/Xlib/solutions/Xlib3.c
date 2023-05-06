#include <stdio.h>
#include <X11/Xlib.h>
 
#define LARGE 400
#define HAUT 200
 
char * display_name = NULL;
unsigned long white_pixel, black_pixel;
Display * dpy; 
Window rootw, win;
Window winbar,winscroll;
int screen;   
#define NPOINTS LARGE*2
static XPoint figure[NPOINTS];
static short tabsin[NPOINTS];
GC gc_fg;
 
getfigure()
{
	int i;
	double sin();

	for (i=0; i< NPOINTS; i++)
		tabsin[i] = 51 - 50 * sin(2.0 * 3.1416 * i / 360.0) ;
}
main(argc,argv)  
        int argc;
        char * argv[];
{
	XSetWindowAttributes xswa;
	unsigned long vmask;
	unsigned long gcmask;
	XGCValues gcvalues;

        if (argc == 3 && !strcmp(argv[1],"-display"))
                display_name = argv[2];
        dpy = XOpenDisplay(display_name);
	getfigure();
        if (dpy == NULL){
                fprintf(stderr,"impossible d'ouvrir le display!\n");
                exit(1);
        }
        screen = DefaultScreen(dpy);
        rootw = RootWindow(dpy,screen);
        white_pixel = WhitePixel(dpy,screen);
        black_pixel = BlackPixel(dpy,screen);

	xswa.background_pixel = white_pixel;
	xswa.border_pixel = black_pixel;
	xswa.event_mask = ButtonPressMask | ButtonReleaseMask |
			  ExposureMask;

	vmask = CWBackPixel | CWBorderPixel | CWEventMask ;
        win = XCreateWindow(dpy,	/* Le Display */
			rootw,		/* Fenetre mere */
			100,100,	/* x,y position */
			LARGE, HAUT,	/* width , height */
			2,		/* border_width */
			CopyFromParent, /* depth */
			InputOutput,	/* Class */
			CopyFromParent,	/* Visual */
			vmask,		/* valuemask*/
			&xswa);		/* attributes */
	gcmask = GCFunction | GCForeground;
	gcvalues.function = GXcopy;
	gcvalues.foreground = black_pixel;
	gc_fg = XCreateGC(dpy,win,gcmask,&gcvalues);

	xswa.background_pixel = white_pixel;
	xswa.border_pixel = black_pixel;
	xswa.event_mask = ButtonPressMask | ButtonReleaseMask |
			 Button1MotionMask | ExposureMask;

	vmask = CWBackPixel | CWBorderPixel | CWEventMask ;
	winbar = XCreateWindow(dpy,win, -1, HAUT - 20, LARGE, 20, 1,
			CopyFromParent, InputOutput,CopyFromParent,
			vmask,&xswa);
	winscroll = XCreateSimpleWindow(dpy,winbar,0,0,19,19,1,black_pixel,black_pixel);
	XMapSubwindows(dpy,winbar);
	XMapSubwindows(dpy,win);
        XMapWindow(dpy,win);

	while(1){
		XEvent ev;

		XNextEvent(dpy,&ev);
		switch(ev.type){
        	case ButtonPress:
			printf("recoit l'evenement ButtonPress\n");
			printf("   window = %d, subwindow = %d\n",
				ev.xbutton.window,ev.xbutton.subwindow);
			if ( ev.xbutton.button == Button3) exit(0);
			break;
        	case ButtonRelease:
			printf("recoit l'evenement ButtonRelease\n");
			printf("   window = %d, subwindow = %d\n",
				ev.xbutton.window,ev.xbutton.subwindow);
			break;
        	case MotionNotify:
			printf("recoit l'evenement MotionNotify\n");
			printf("   window = %d, subwindow = %d\n",
				ev.xmotion.window,ev.xmotion.subwindow);
			if (ev.xmotion.window != winbar) break;
/*	calculer a partir de tabsin le tableau figure */
/*	copier ce qui vient d'etre decaler par XCopyArea */
/*	redessiner la partie manquante */
/*	bouger la fenetre winscroll */
			XMoveWindow(dpy,winscroll,ev.xmotion.x,0);
			break;
        	case KeyPress:
			printf("recoit l'evenement KeyPress\n");
			break;
        	case KeyRelease:
			printf("recoit l'evenement KeyRelease\n");
			break;
        	case EnterNotify:
			printf("recoit l'evenement EnterNotify\n");
			break;
        	case LeaveNotify:
			printf("recoit l'evenement LeaveNotify\n");
			break;
        	case FocusIn:
			printf("recoit l'evenement FocusIn\n");
			break;
        	case FocusOut:
			printf("recoit l'evenement FocusOut\n");
			break;
        	case KeymapNotify:
			printf("recoit l'evenement KeymapNotify\n");
			break;
        	case Expose: 
			printf("recoit l'evenement Expose\n");
			if (ev.xexpose.count) break; /* Si vous avez une question */
						    /* C'est le momment! */
			{ int i;
				for (i=0; i<NPOINTS;i++){
					figure[i].x = i;
					figure[i].y = tabsin[i];
				}
			}
			XDrawLines(dpy,win,gc_fg,
				figure,NPOINTS,CoordModeOrigin);
			break;
       		default:
			printf("recoit un evenement INCONU %d \n",ev.type);
			break;
		} /* switch */
	} /* while */
}/* main */
