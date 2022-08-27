/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* icon_window.c -- demonstrate setting the iconWindow resource
 */
#include <Xm/Xm.h>
#include <X11/bitmaps/mailfull>

void
SetIconWindow(shell, image)
Widget shell;
Pixmap image;
{
    Window window, root;
    unsigned int width, height, border_width, depth;
    int x, y;
    Display *dpy = XtDisplay (shell);

    /* Get the current icon window associated with the shell */
    XtVaGetValues (shell, XmNiconWindow, &window, NULL);

    if (!window) {
        /* If there is no window associated with the shell, create one.  
         * Make it at least as big as the pixmap we're
         * going to use.  The icon window only needs to be a simple window.
         */
        if (!XGetGeometry (dpy, image, &root, &x, &y,
                &width, &height, &border_width, &depth) ||
            !(window = XCreateSimpleWindow (dpy, root, 0, 0, width, height,
                (unsigned)0, CopyFromParent, CopyFromParent))) {
	    XtVaSetValues (shell, XmNiconPixmap, image, NULL);
            return;
        }
        /* Now that the window is created, set it ... */
        XtVaSetValues (shell, XmNiconWindow, window, NULL);
    }
    /* Set the window's background pixmap to be the image. */
    XSetWindowBackgroundPixmap (dpy, window, image);
    /* cause a redisplay of this window, if exposed */
    XClearWindow (dpy, window);
}

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

    SetIconWindow (toplevel, bitmap);
    
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
