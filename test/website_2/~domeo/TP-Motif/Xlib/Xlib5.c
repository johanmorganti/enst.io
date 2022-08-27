#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xresource.h>
#include <X11/keysym.h>
 
#define LARGE 400
#define HAUT 200
#define NB_MENU_ITEM 4
 
char		*display_name = NULL;
unsigned long	white_pixel, black_pixel;
Display		* dpy; 
Window		rootw, win, menuwid;
int		screen;   
GC		gc_fg;
GC 		gc_tile;
GC		gc_invert;
int 		fonth;
int 		depth; 
Pixmap 		gray_pixmap;

struct Menu {
        char * texte;
        int data;
        int possible;
        Window wid;             /*fenetre choix */
} menu_item[NB_MENU_ITEM] = {
	{ " menu 1 ", 1, True, NULL },
	{ " menu 2 ", 2, True, NULL },
	{ " menu 3 ", 3, False, NULL},
	{ " menu 4 ", 4 ,True, NULL},
} ;

#define GRAYPIXW 32
#define GRAYPIXH 32

static char gray_pixmap_data[ GRAYPIXW * GRAYPIXH / 8] = {
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55,
        0xaa, 0xaa, 0xaa, 0xaa, 0x55, 0x55, 0x55, 0x55
};

 
main(argc,argv)  
        int argc;
        char * argv[];
{
	XSetWindowAttributes xswa;
	unsigned long vmask;
	unsigned long gcmask;
	XGCValues gcvalues;
	XFontStruct * font;
	int ltexte = 0;
	int i;

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
        depth = DefaultDepth(dpy,screen);

	xswa.background_pixel = white_pixel;
	xswa.border_pixel = black_pixel;
	xswa.event_mask = ButtonPressMask |OwnerGrabButtonMask;

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

        gray_pixmap = XCreatePixmapFromBitmapData(dpy, rootw, gray_pixmap_data,
                        GRAYPIXW, GRAYPIXH, black_pixel, white_pixel,depth);
        gcmask |= GCFillStyle;     gcvalues.fill_style = FillTiled;
        gcmask |= GCTile;          gcvalues.tile = gray_pixmap;
        gc_tile = (GC) XCreateGC(dpy, rootw, gcmask, &gcvalues);

        /* contexte pour inversion bg <-> fg */
        gcmask = 0;
        gcmask |= GCFunction;      gcvalues.function = GXinvert;
        gcmask |= GCPlaneMask;     gcvalues.plane_mask =/* AllPlanes*/ 1;
        gc_invert = (GC) XCreateGC(dpy, rootw, gcmask, &gcvalues);

	for (i = 0; i < NB_MENU_ITEM; i++){
                int w = XTextWidth(font, menu_item[i].texte,
                        strlen(menu_item[i].texte));
                if (w > ltexte) ltexte = w;
        }
        xswa.background_pixel = white_pixel;
        xswa.border_pixel = black_pixel;
        xswa.override_redirect = True; /*n'envoie rien au window manager*/
        xswa.event_mask = ExposureMask;
        xswa.save_under = True ;        /*sauve en dessous de la fenetre*/
/*
 * Creation de la fenetre qui contient le menu
 */
        menuwid = XCreateWindow(dpy, rootw,
                        0,      /* position en X */
                        0,      /* position en Y */
                        ltexte ,/*largeur*/
                        NB_MENU_ITEM *fonth , /*hauteur*/
                        1,                    /* border width */
                        CopyFromParent,         /* depth*/
                        InputOutput,            /* class */
                        CopyFromParent,         /* visual */
                        CWBorderPixel | CWSaveUnder| /*values mask*/
                        CWOverrideRedirect|CWEventMask|CWBackPixel,
                        &xswa);         /*attributes */
 
 
        xswa.event_mask = EnterWindowMask | LeaveWindowMask | ExposureMask|
                          ButtonPressMask | ButtonReleaseMask |
                          OwnerGrabButtonMask ;
        for(i = 0 ; i < NB_MENU_ITEM ; i++){
                menu_item[i].wid = XCreateWindow(dpy, menuwid, 0,
                        i*fonth,
                        ltexte , fonth, 1,
                        CopyFromParent, InputOutput,
                        CopyFromParent,
			CWEventMask|CWBorderPixel|CWBackPixel, &xswa);
        }
        /* On mappe les sous femetres de facon a faire le Popup
                avec XMapWindow(dpy,menu) par la suite */
        XMapSubwindows(dpy,menuwid);

        XMapWindow(dpy,win);

	while(1){
		XEvent ev;
		Window selwid;
		int x,y;
		GC gc;
		int select;

		XNextEvent(dpy,&ev);
		switch(ev.type){
        	case EnterNotify:
			printf("recoit l'evenement EnterNotify\n");


                        if( (i=findmenuwin(ev.xcrossing.window,menu_item))== -1)
                                break;
 
                        if(!menu_item[i].possible) break;
                        selwid = menu_item[i].wid;
			select = i;
 
                        XFillRectangle(dpy, selwid,
                                 gc_invert, 0, 0,
                                 ltexte, fonth);
                        break;
                case LeaveNotify:
			printf("recoit l'evenement LeaveNotify\n");
                        if(selwid)
                                XFillRectangle(dpy, selwid,
                                         gc_invert, 0, 0,
                                         ltexte, fonth);
                        selwid = NULL;
			select = -1;
                        break;
                case ButtonPress:
			if(ev.xbutton.window == win){
				XMoveWindow(dpy,menuwid,ev.xbutton.x_root,
					ev.xbutton.y_root);
				XMapRaised(dpy,menuwid);
				break;
			}
                        break;
                case ButtonRelease:
                        if (selwid)
				printf("select = %d\n",menu_item[select].data);
                        selwid =  NULL;
			select =-1;
			XUnmapWindow(dpy,menuwid);
                        break;


			break;
        	case Expose: 
			printf("recoit l'evenement Expose\n");


                        if(ev.xexpose.count )break;
                        if( (i=findmenuwin(ev.xexpose.window,menu_item))== -1)
                                break;
                        /* on trace le menu*/
                        y = font->max_bounds.ascent;
                        x = 0;
                        gc = menu_item[i].possible ? gc_fg :
                                        gc_tile;
 
                        XDrawString(dpy, menu_item[i].wid, gc,
                                x, y, menu_item[i].texte,
                                strlen(menu_item[i].texte));
                        if (selwid == menu_item[i].wid)
                                XFillRectangle(dpy, selwid,
                                         gc_invert, 0, 0,
                                         ltexte, fonth);
                        break;

		} /* switch */
	} /* while */
}/* main */

int findmenuwin(win, menu)
        Window win;
        struct Menu  * menu;
{
        int i;
        for(i = 0; i< NB_MENU_ITEM; i++){
                if(win == menu_item[i].wid) return i;
        }
        return -1;
}


