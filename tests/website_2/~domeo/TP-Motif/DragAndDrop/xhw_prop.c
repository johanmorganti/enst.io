#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#define STRING	"Hello, world"
#define BORDER	1
#define FONT	"fixed"
#define	ARG_FONT "font"

char textwm[80]="";
int nexp = 0;

main(argc,argv)
    int argc;
    char **argv;
{
    Display    *dpy;			/* Connexion au serveur */
    Window      win;			/* ID de la fenetre */
    GC          gc;			/* GC pour ecrire */
    char       *fontName;		/* Nom de la police */
    XFontStruct *fontstruct;	/* Descripteur de la police */
    unsigned long ftw, fth, pad;/* Variables parametres de la font */
    unsigned long fg, bg, bd;	/* Valeurs des pixels */
    unsigned long bw;			/* Taille du bord de la fenetre */
    XGCValues   gcv;			/* Structure pour creation du GC */
    XEvent      event;			/* Evenements recus */
    XSetWindowAttributes xswa;	/* Structure pour creation de la fenetre */
    unsigned int wwidth,wheight;/* Dimension de la fenetre */
    int		wx,wy;		/* Position de la fenetre */

    /*
     * Ouvre le display a partir de la variable d'environnement DISPLAY
     */
    if ((dpy = XOpenDisplay(NULL)) == NULL) {
	fprintf(stderr, "%s: can't open %s\n", argv[0], XDisplayName(NULL));
	exit(1);
    }

    /*
     * Charge la police de caractere.
     */
    if ((fontName = XGetDefault(dpy, argv[0], ARG_FONT)) == NULL) 
	fontName = FONT;
    if ((fontstruct = XLoadQueryFont(dpy, fontName)) == NULL) {
	fprintf(stderr, "%s: display %s doesn't know font %s\n",
		argv[0], DisplayString(dpy), fontName);
	exit(1);
    }
    fth = fontstruct->max_bounds.ascent + fontstruct->max_bounds.descent;
    ftw = fontstruct->max_bounds.width;

    /*
     * Initialise les couleurs de la bordure, du foreground et background
     */
    bg = BlackPixel(dpy, DefaultScreen(dpy));
    bd = fg = WhitePixel(dpy, DefaultScreen(dpy));
    bw = 1;


    /*
     * Calcul de geometrie de la fenetre
     */
    pad = BORDER;
    wheight = fth + pad * 2;
    wwidth = XTextWidth(fontstruct, STRING, strlen(STRING)) + pad * 2;
    wx = (DisplayWidth(dpy, DefaultScreen(dpy)) - wwidth) / 2;
    wy = (DisplayHeight(dpy, DefaultScreen(dpy)) - wheight) / 2;


    /*
     * Creation de la fenetre
     */
    win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy),
                              wx, wy, wwidth, wheight,
                              bw, bd, bg);


    /*
     * Mettre le 'bit_gravity' pour reduire les evenements d'expose.
     */
    xswa.bit_gravity = CenterGravity;
    XChangeWindowAttributes(dpy, win, CWBitGravity, &xswa);


    /*
     * Selection des evenements qui nous interesse.
     */
    XSelectInput(dpy, win, ExposureMask);


    /*
     * Creation du GC pour ecrire.
     */
    gcv.font = fontstruct->fid;
    gcv.foreground = fg;
    gcv.background = bg;
    gc = XCreateGC(dpy, win, (GCFont | GCForeground | GCBackground), &gcv);


    /*
     * Rendre la fenetre visible.
     */
    XMapWindow(dpy, win);

    /*
     * Boucle infinie.
     */
    nexp = 0;
    while (1) {
	nexp++;
	sprintf(textwm,"test de WM_NAME %d",nexp);
	XChangeProperty(dpy,win,XA_WM_NAME,XA_STRING,8,
		PropModeReplace,textwm,strlen(textwm));
	sprintf(textwm,"test");
	XChangeProperty(dpy,win,XA_WM_ICON_NAME,XA_STRING,8,
		PropModeReplace,textwm,strlen(textwm));
	/*
	 * prendre un evenement dans la queue.
	 */
	XNextEvent(dpy, &event);

	/*
	 * Seul le dernier Expose redessine la fenetre.
	 */
	if (event.type == Expose && event.xexpose.count == 0) {
	    XWindowAttributes xwa;	/* Temp Get Window Attribute struct */
	    int         x, y;

	    /*
	     * Purge les evenements d'expose en attente dans la queue.
	     * Evite les ecritures multiples
	     */
	    while (XCheckTypedEvent(dpy, Expose, &event));

	    /*
	     * Trouver les dimensions de la fenetre pour centrer le texte.
	     */
	    if (XGetWindowAttributes(dpy, win, &xwa) == 0)
		break;
	    x = (xwa.width - XTextWidth(fontstruct,STRING,strlen(STRING))) / 2;
	    y = (xwa.height + fontstruct->max_bounds.ascent
		 - fontstruct->max_bounds.descent) / 2;

	    /*
	     * Effacer la fenetre et la redessiner.
	     */
	    XClearWindow(dpy, win);
	    XDrawString(dpy, win, gc, x, y, STRING, strlen(STRING));
	}
    }
    exit(1);
}
