#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
 
#define LARGE 400
#define HAUT 200
 
char * display_name = NULL;
unsigned long white_pixel, black_pixel;
Display * dpy; 
Window rootw, win;
int screen;   
GC gc_fg;
#define STARTTEXTE 10
int posx = STARTTEXTE;
int posy = HAUT/2;
int fonth;
 
main(argc,argv)  
        int argc;
        char * argv[];
{
	XSetWindowAttributes xswa;
	unsigned long vmask;
	unsigned long gcmask;
	XGCValues gcvalues;
	XFontStruct * font;

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
	xswa.event_mask = KeyPressMask | EnterWindowMask |
			 /*LeaveWindowMask|*/  ExposureMask;

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
	font = XLoadQueryFont(dpy,"fixed");
	fonth = font->max_bounds.ascent + font->max_bounds.descent;
	gcmask = GCFunction | GCForeground | GCFont;
	gcvalues.font = font->fid;
	gcvalues.function = GXcopy;
	gcvalues.foreground = black_pixel;
	gc_fg = XCreateGC(dpy,win,gcmask,&gcvalues);
        XMapWindow(dpy,win);

	while(1){
		XEvent ev;
		KeySym ks;
		char buf[10];
		char texte[130];
		int numc=0;
		char *p = texte;
		int i;

		XNextEvent(dpy,&ev);
		switch(ev.type){
        	case KeyPress:
			printf("recoit l'evenement KeyPress\n");
                        i = XLookupString(&ev,buf, sizeof(buf) - 1 ,
                                &ks,0);
                        buf[i] = '\0';
                        printf("longueur = %d, Keysym = %d, buf='%s'\n",
                                i, ks,buf);
                        if( ( ks == XK_Return) ||
			    ( strlen(texte) >= (sizeof(texte)-1) ) ) {
				printf("la chaine est %s\n",texte);
                                XClearWindow(dpy, win);
				p = texte;
                                *p = '\0';
				posx = STARTTEXTE;
				continue;
                        }
                        if ( ks == XK_Delete) {
                                if(texte[0]) {
                                        p--;
                                        posx -= XTextWidth(font, p, 1);
					*p = '\0';
                                }
                                /* detruire le caractere */
                                XClearArea(dpy,win,posx,
                                        posy-font->max_bounds.ascent,
                                        100, fonth, False);
                                break;
                        }
                        if (i == 1){
                                XDrawString(dpy,win,gc_fg,posx,posy, buf,1);
                                *p++ = buf[0]; *p = '\0';
                                posx += XTextWidth(font , buf, 1) ;
			}
			break;
        	case EnterNotify:
			printf("recoit l'evenement EnterNotify\n");
			XSetInputFocus(dpy,win,RevertToPointerRoot,CurrentTime);
			break;
        	case LeaveNotify:
			printf("recoit l'evenement LeaveNotify\n");
			XSetInputFocus(dpy, rootw, RevertToPointerRoot, CurrentTime);
			break;
        	case FocusIn:
			printf("recoit l'evenement FocusIn\n");
			break;
        	case FocusOut:
			printf("recoit l'evenement FocusOut\n");
			break;
        	case Expose: 
			printf("recoit l'evenement Expose\n");
			if (ev.xexpose.count) break;
			XDrawString(dpy,win,gc_fg,STARTTEXTE,posy,texte,
				strlen(texte));
			break;
		} /* switch */
	} /* while */
}/* main */
