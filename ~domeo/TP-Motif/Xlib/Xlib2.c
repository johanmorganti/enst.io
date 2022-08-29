#include <stdio.h>
#include <X11/Xlib.h>
 
#define LARGE 400
#define HAUT 200
 
char * display_name = NULL;
unsigned long white_pixel, black_pixel;
Display * dpy; 
Window rootw, win;
int screen;   
 
main(argc,argv)  
        int argc;
        char * argv[];
{
	XSetWindowAttributes xswa;
	unsigned long vmask;
	Window winf;

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

	xswa.background_pixel = white_pixel;
	xswa.border_pixel = black_pixel;
	xswa.event_mask = ButtonPressMask | ButtonReleaseMask |
			 Button1MotionMask /*| OwnerGrabButtonMask*/;
/* A quoi peut bien servir OwnerGrabButtonMask ???*/

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
/* on creer la fille */
	xswa.background_pixel = black_pixel;
	xswa.border_pixel = white_pixel;
/*	xswa.event_mask = ButtonPressMask | ButtonReleaseMask;*/

	vmask = CWBackPixel | CWBorderPixel/*|CWEventMask*/ ;
	winf = XCreateWindow(dpy,win,0,0,20,20,1,CopyFromParent,
			InputOutput,CopyFromParent,vmask,&xswa);
	XMapSubwindows(dpy,win);
        XMapWindow(dpy,win);

	while(1){
		XEvent ev;
		int posx,posy;

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
			if (ev.xmotion.window != win) break;
			/* la fenetre fille fait 20x20 et un border de 1 */
			posx = ev.xmotion.x - 10 - 1;
			posy = ev.xmotion.y - 10 - 1;
			XMoveWindow(dpy,winf,posx,posy);
/* cette partie de code ne suffit pas. Si je clique et que je relache */
/* la fenetre ne bouge pas !!! */
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
			break;
        	case GraphicsExpose:
			printf("recoit l'evenement GraphicsExpose\n");
			break;
        	case NoExpose:
			printf("recoit l'evenement NoExpose\n");
			break;
        	case ConfigureNotify:
			printf("recoit l'evenement ConfigureNotify\n");
			break;
        	case CirculateNotify:
			printf("recoit l'evenement CirculateNotify\n");
			break;
        	case CreateNotify:
			printf("recoit l'evenement CreateNotify\n");
			break;
        	case DestroyNotify:
			printf("recoit l'evenement DestroyNotify\n");
			break;
        	case GravityNotify:
			printf("recoit l'evenement GravityNotify\n");
			break;
        	case MapNotify:
			printf("recoit l'evenement MapNotify\n");
			break;
        	case MappingNotify:
			printf("recoit l'evenement MappingNotify\n");
			break;
        	case ReparentNotify:
			printf("recoit l'evenement ReparentNotify\n");
			break;
        	case UnmapNotify:
			printf("recoit l'evenement UnmapNotify\n");
			break;
        	case VisibilityNotify:
			printf("recoit l'evenement VisibilityNotify\n");
			break;
        	case ClientMessage:
			printf("recoit l'evenement ClientMessage\n");
			break;
        	case PropertyNotify:
			printf("recoit l'evenement PropertyNotify\n");
			break;
        	case SelectionClear:
			printf("recoit l'evenement SelectionClear\n");
			break;
        	case SelectionNotify:
			printf("recoit l'evenement SelectionNotify\n");
			break;
        	case SelectionRequest:
			printf("recoit l'evenement SelectionRequest\n");
			break;
       		default:
			printf("recoit un evenement INCONU %d \n",ev.type);
			break;
		} /* switch */
	} /* while */
}/* main */
