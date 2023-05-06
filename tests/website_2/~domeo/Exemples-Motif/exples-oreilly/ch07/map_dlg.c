/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* map_dlg.c -- Use the XmNmapCallback to automatically position
 * a dialog on the screen.  Each time the dialog is displayed, it
 * is mapped down and to the right by 200 pixels in each direction.
 */
#include <Xm/MessageB.h>
#include <Xm/PushB.h>

/* main() --create a pushbutton whose callback pops up a dialog box */
main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    XtAppContext app;
    void pushed();

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    button = XtVaCreateManagedWidget ("Push Me", 
        xmPushButtonWidgetClass, toplevel, 
        NULL);

    XtAddCallback (button, XmNactivateCallback, pushed, "Hello World");

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* callback function for XmNmapCallback.  Position dialog in 200 pixel
 * "steps".  When the edge of the screen is hit, start over.
 */
static void
map_dialog(dialog, client_data, call_data)
Widget dialog;
XtPointer client_data;
XtPointer call_data;
{
    static Position x, y;
    Dimension w, h;

    XtVaGetValues(dialog, 
        XmNwidth, &w, 
        XmNheight, &h, 
        NULL);

    if ((x + w) >= WidthOfScreen (XtScreen (dialog)))
        x = 0;
    if ((y + h) >= HeightOfScreen (XtScreen (dialog)))
        y = 0;
    XtVaSetValues (dialog, 
        XmNx, x, 
        XmNy, y, 
        NULL);
    
    x += 200;
    y += 200;
}

/* pushed() --the callback routine for the main app's pushbutton.
 * Create and popup a dialog box that has callback functions for
 * the Ok, Cancel and Help buttons.
 */
void
pushed(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    extern void response();
    Widget dialog;
    Arg arg[5];
    int n = 0;
    char *message = (char *) client_data; 
    XmString t = XmStringCreateLocalized (message);

    XtSetArg (arg[n], XmNmessageString, t); n++;
    XtSetArg (arg[n], XmNdefaultPosition, False); n++;
    dialog = XmCreateMessageDialog (w, "notice", arg, n);
    XmStringFree (t);

    XtAddCallback (dialog, XmNmapCallback, map_dialog, NULL);

    XtManageChild (dialog);
    XtPopup (XtParent (dialog), XtGrabNone);
}
