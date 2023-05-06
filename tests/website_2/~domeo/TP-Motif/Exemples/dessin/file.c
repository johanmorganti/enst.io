
/* ********************************************************************* *
 *
 *  file.c
 *  lecture / ecriture de rasterfiles / afficahe d'images
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <rasterfile.h>
#include <Xm/Xm.h>

/* ===================================================================== */
/* variable globale definie dans show.c*/

extern Display *display;


/* ********************************************************************* */

char *ReadRasterFile(filename, width, height, depth)
     char *filename;
     unsigned int *width, *height, *depth;
{
  int fd;
  char *colormap = NULL;
  char *pixmap = NULL;

  if (! filename || ! filename[0] 
      || (fd = open(filename,O_RDONLY)) == -1)
    fprintf(stderr, "! ReadRasterFile: impossible d'ouvrir le fichier: %s\n",
	    filename);
  else {

    struct rasterfile header;

    read(fd, &header, sizeof(header));

    if (header.ras_magic != RAS_MAGIC)
      fprintf(stderr, "! ReadRasterFile: %s n'est pas un rasterfile\n",
	      filename);

    else if (header.ras_depth != 1 && header.ras_depth != 8)
      fprintf(stderr, "! ReadRasterFile: invalid depth (file: %s, depth: %d) \n",
	      filename, header.ras_depth);

    else {

      /* taille du pixmap (directement utilisable dans une XImage) :
       *
       * - pixmap_height = hauteur logique image
       * - pixmap_bytewidth  = taille en bytes d'une ligne du pixmap
       *     * toujours egale a la largeur logique image si depth = 8
       *     * arrondie pour que la ligne soit un multiple de 8 bits si depth = 1
       *   ==> la taille d'une ligne du pixmap est toujours un multiple de 8 BITS.
       *
       * ! attention: la XImage devra etre cree avec bitmap_pad = 8 
       *   (bitmap_pad est le quantum d'une scanline, ie. deux lignes successives
       *   sont separee par un mutiple de ce nombre de bits)
       * - de meme bytes_per_lines devra valoir 0 car les lignes sont contigues
       */
      register char *p;
      register int
	h,
	pixmap_height = header.ras_height,
	pixmap_bytewidth = (header.ras_depth == 8) ? 
	  header.ras_width:
	  header.ras_width / 8 + ((header.ras_width % 8) != 0);

      /* taille du data du rasterfile :
       *
       * -1- pour ras_type != RT_OLD, ras_length indique la taille totale
       *      de la partie data du fichier (si celle-ci n'est pas encodee !!).
       * -2- les lignes des rasterfiles sont obligatoirement des multiples de
       *      16 BITS (alors que celles des pixmaps sont des mutiples de 8 bits)
       *      ==> si pixmap_bytewidth n'est pas divisible par 2 alors la taille
       *      en bytes d'une ligne du rasterfile vaut :
       *          ras_bytewidth = pixmap_bytewidth + 1  
       *         (sinon ras_bytewidth == pixmap_bytewidth)
       */
      int ras_bytewidth = pixmap_bytewidth + ((pixmap_bytewidth % 2) != 0);

      if ((pixmap = XtMalloc(pixmap_height * pixmap_bytewidth)) == NULL
	       || 
	       (header.ras_maptype != RMT_NONE && header.ras_maplength != 0
		&& (colormap = XtMalloc(header.ras_maplength)) == NULL)
	       )
	fprintf(stderr,
		"! ReadRasterFile: memoire insuffisante pour creer%s\n",
		filename);
      else {

	*width  = header.ras_width;
	*height = header.ras_height;
	*depth  = header.ras_depth;

	printf("> ReadRasterFile: file: %s\n", filename);
	printf("> Size: %d x %d, depth: %d \n", *width, *height, *depth);

	/* !!! PAS SAUVEGARDEE !! lire la colormap si elle existe */

	if (header.ras_maptype != RMT_NONE && header.ras_maplength != 0) {
	  read(fd, colormap, header.ras_maplength);
	}

	/* si les lignes du pixmap et du fichier ont la meme taille en bytes  */

	if (pixmap_bytewidth == ras_bytewidth) {
	  for (p = pixmap, h = 0; h < pixmap_height; h++, p += pixmap_bytewidth)
	    read(fd, p, pixmap_bytewidth);
	}
	else {
	  /* dans ce cas il faut lire le pad mais l'omettre du pixmap */
	  char pad;

	  for (p = pixmap, h = 0; h < pixmap_height; h++, p += pixmap_bytewidth) {
	    read(fd, p, pixmap_bytewidth);
	    read(fd, &pad, 1);       /* pad => garbage */
	  }
	}
      }
    }   
    close(fd);
    if (colormap) free(colormap);
  }

  return(pixmap);		/* vaut NULL si une des etapes a echoue */
}

/* ==================================================== [Elc:93] ======= */
/* ==================================================== ======== ======= */

Boolean WriteRasterFile(filename, pixmap, width, height, depth)
     char *filename;
     char *pixmap;
     unsigned int width, height, depth;
{
  int fd;

  if (pixmap == NULL || (depth != 1 && depth != 8))
    fprintf(stderr, 
	    "! WriteRasterFile: null pixmap or invalid depth (file: %s, depth %d)\n",
	    filename, depth);

  else if (! filename || ! filename[0] 
	   || (fd = open(filename,  O_WRONLY|O_CREAT, 0666)) == -1)
    fprintf(stderr, "! WriteRasterFile: impossible d'ouvrir le fichier: %s\n",
	    filename);

  else {

    /* taille du pixmap:
     * - pixmap_height = hauteur logique image
     * - pixmap_bytewidth  = taille en bytes d'une ligne du pixmap
     *     * toujours egale a la largeur logique image si depth = 8
     *     * arrondie pour que la ligne soit un multiple de 8 bits si depth = 1
     *   ==> la taille d'une ligne du pixmap est toujours un multiple de 8 BITS.
     */
    register int
      h,
      pixmap_height = height,
      pixmap_bytewidth = (depth == 8) ? width: width / 8 + ((width % 8) != 0);
    register char *p;

    /* taille du data du rasterfile :
     *
     * -1- pour ras_type != RT_OLD, ras_length doit indiquer la taille totale
     *     de la partie data du fichier (si celle-ci n'est pas encodee !!).
     * -2- les lignes des rasterfiles sont obligatoirement des multiples de
     *     16 BITS (alors que celles des pixmaps sont des mutiples de 8 bits).
     *     ==> si pixmap_bytewidth n'est pas divisible par 2 alors la taille
     *     en bytes d'une ligne du rasterfile vaut :
     *          ras_bytewidth = pixmap_bytewidth + 1  
     *         (sinon ras_bytewidth == pixmap_bytewidth)
     */
    int ras_bytewidth = pixmap_bytewidth + ((pixmap_bytewidth % 2) != 0);
    struct rasterfile header;

    /* initialisation des champs du header */

    header.ras_magic  = RAS_MAGIC;
    header.ras_type   = RT_STANDARD;
    header.ras_width  = width;
    header.ras_height = height;
    header.ras_depth  = depth;
    header.ras_length = height * ras_bytewidth;
    
    /* colormaps pas traitees */

    header.ras_maptype = RMT_NONE;        /* pas de colormap */
    header.ras_maplength = 0;

    printf("> WriteRasterFile: file: %s\n", filename);
    printf("> Size: %d x %d, depth: %d \n", width, height, depth);

    write(fd, &header, sizeof(header));

    /* si les lignes du pixmap et du fichier ont la meme taille en bytes  */

    if (pixmap_bytewidth == ras_bytewidth) {
      for (p = pixmap, h = 0; h < pixmap_height; h++, p += pixmap_bytewidth)
	write(fd, p, pixmap_bytewidth);
    }
    else {
      /* dans ce cas il faut rajouter un pad */
      char pad = '\0';

      for (p = pixmap, h = 0; h < pixmap_height; h++, p += pixmap_bytewidth) {
	write(fd, p, pixmap_bytewidth);
	write(fd, &pad, 1);
      }
    }
    close(fd);
    return(True);			/* retourne True si succes */
  }

  return(False);			/* return False en cas d'echec */
}

/* ==================================================== [Elc:93] ======= */
/* ==================================================== ======== ======= */

/* Callback qui reaffiche la derniere image */

static void RefreshImage(Widget drawing, XtPointer pimage, XtPointer cdata)
{
  XImage *ximage = (XImage *) pimage;
		      /* call_data == NULL si fonction appelee directement */
  if (cdata == NULL
      || ((XmDrawingAreaCallbackStruct *)cdata)->event->xexpose.count == 0) {
    
    if (! ximage) XClearWindow(XtDisplay(drawing), XtWindow(drawing));
    else {
          /* nb1: 0,0,0,0 is the offset (can be used for partial redisplay) */
      XPutImage(XtDisplay(drawing), XtWindow(drawing), 
		XDefaultGCOfScreen(XtScreen(drawing)),
		ximage,
		(int)0, (int)0, (int)0, (int)0, 
		(u_int) ximage->width,
		(u_int) ximage->height);
    }
    XFlush(XtDisplay(drawing));
  }
}

/* ==================================================== [Elc:93] ======= */
/* ==================================================== ======== ======= */
/* Cree une XImage a partir d'un pixmap */
/* si pixmap == NULL, cree ce pixmap */

XImage *CreateImage(char *pixmap, int width, int height, int depth)
{
  XImage *xima = NULL;

  if (width <= 0 || height <= 0 || depth <= 0) {
    fprintf(stderr, "! CreateImage: invalid size (%d x %d) or depth (%d) \n",
	    width, height, depth);
    return NULL;
  }

  if (! pixmap) {		/* creer le pixmap s'il n'existe pas deja */
    int pixmap_bytewidth;
    switch (depth) {
    case 8: pixmap_bytewidth = width; break;
    case 1: pixmap_bytewidth = width / 8 + ((width % 8) != 0); break;
    default:
      fprintf(stderr, "! CreateImage: invalid depth (depth: %d) \n", depth);
      return NULL;
    }
    if (! (pixmap = XtCalloc(height * pixmap_bytewidth, sizeof(char))) ) {
      fprintf(stderr, "! CreateImage: not enough memory \n");
      return NULL;
    }
  }
  {
    int offset = 0,             /* scanline offset */
    bytes_per_line = 0,         /* 0 => contiguous scanlines */
    bitmap_pad = 8;             /* quantum of a scanline */
    /* !nb: rappel: donnee continue et base arrondie au byte superieur */   

    xima = XCreateImage(display,
		 XDefaultVisual(display, XDefaultScreen(display)),
		 (unsigned int) depth, 
				/* XYBitmap si depth == 1 */
		 (depth == 8) ? ZPixmap : XYBitmap,
		 offset, 
		 (char *) pixmap,
		 (u_int) width,
		 (u_int) height,
		 bitmap_pad, bytes_per_line);
  }

  if (xima) xima->obdata = NULL;
  else fprintf(stderr,"! CreateImage: X was unable to create an image\n");
  return xima;
}

/* ==================================================== ======== ======= */
/* cree une XImage a partir d'un rasterfile Sun */
/* retourne un pointeur de XImage ou NULL si la lecture a echoue */

XImage *ReadImage(char *filename)
{
  int width, height, depth;
  char *pixmap = ReadRasterFile (filename, &width, &height, &depth);
  return (pixmap) ? CreateImage (pixmap, width, height, depth) : NULL;
}

/* ==================================================== ======== ======= */
/* sauvegarde une XImage a partir d'un rasterfile Sun */
/* retourne un pointeur la XImage donnee en arg ou NULL si la lecture a echoue */

XImage *WriteImage(char *filename, XImage *ximage)
{
  return (WriteRasterFile(filename, ximage->data, 
			  ximage->width, ximage->height, ximage->depth)
	  ? ximage : NULL);
}

/* ==================================================== [Elc:93] ======= */
/* ==================================================== ======== ======= */
/* cree un nouveau widget si pas de widget associe */

void DisplayImage(Widget drawing, XImage *ximage)
{
  if (ximage) {

    XtVaSetValues(drawing,
		  XmNwidth,  (Dimension)ximage->width,
		  XmNheight, (Dimension)ximage->height,
		  NULL);
				/* eliminer anciens callbacks sur old xima */
    XtRemoveAllCallbacks(drawing, XmNexposeCallback);
				/* ajouter nouveau callback */
    XtAddCallback(drawing, XmNexposeCallback, RefreshImage, ximage);
				/* afficher l'image */
    RefreshImage(drawing, ximage, NULL);
  }
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
/* BackingStore : pour memoriser le graphique */

void SetBackingStore(Widget drawing)
{
  unsigned long valuemask = CWBackingStore | CWBackingPlanes | CWBackingPixel;
  XSetWindowAttributes watt;

  watt.backing_store  = Always; /* sauvegarde auto de l'image */
  watt.backing_planes = 0xff;   /* precise les plans a sauvegarder (tous) */
  watt.backing_pixel  = 0;
  XChangeWindowAttributes(XtDisplay(drawing), XtWindow(drawing), 
			  valuemask, &watt);
}

/* ==================================================== [Elc:94] ======= */
/* ==================================================== ======== ======= */
		      
void DisplayText(Widget drawing, int x, int y, int color, char *text)
{
  XGCValues gcval;
  static GC gc = NULL;
  if (! gc) gc = XCreateGC(display, XtWindow(drawing), 0, &gcval);

  gcval.function = GXcopy;
  gcval.foreground = (u_long) color;
  XChangeGC(display, gc, GCFunction | GCForeground, &gcval);

  XDrawString(display, XtWindow(drawing), gc, x, y, text, strlen(text));
  XFlush(display);
}

/* ==================================================== [TheEnd] ======= */
/* ==================================================== [Elc:93] ======= */


