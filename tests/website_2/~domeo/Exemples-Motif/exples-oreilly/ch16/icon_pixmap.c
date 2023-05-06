/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* icon_pixmap.c -- demonstrate setting the iconPixmap resource
 */
#include <Xm/Xm.h>
#include <X11/bitmaps/mailfull>

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel;
    XtAppContext app;
    Pixmap bitmap;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL,
        XmNwidth, 100, /* size is irrelevant -- toplevel is iconified */
        XmNheight, 100, /* it just can't be 0, or Xt complains */
        XmNiconic,     True,
        NULL);

    bitmap = XCreatePixmapFromBitmapData (XtDisplay (toplevel),
        RootWindowOfScreen (XtScreen (toplevel)),
        mailfull_bits, mailfull_width, mailfull_height, 1, 0, 1);

    XtVaSetValues (toplevel,
        XmNiconPixmap, bitmap,
        NULL);
    
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
