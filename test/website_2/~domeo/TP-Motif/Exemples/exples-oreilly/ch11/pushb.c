/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* pushb.c -- demonstrate the pushbutton widget.  Display one
 * PushButton with a single callback routine.  Print the name
 * of the widget and the number of "multiple clicks".  This
 * value is maintained by the toolkit.
 */
#include <Xm/PushB.h>

main(argc, argv)
int argc;
char *argv[];
{
    XtAppContext app;
    Widget toplevel, button;
    void my_callback();
    XmString btn_text;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    btn_text = XmStringCreateLocalized ("Push Here");
    button = XtVaCreateManagedWidget ("button",
        xmPushButtonWidgetClass, toplevel,
        XmNlabelString, btn_text,
        NULL);
    XmStringFree (btn_text);
    XtAddCallback (button, XmNarmCallback, my_callback, NULL);
    XtAddCallback (button, XmNactivateCallback, my_callback, NULL);
    XtAddCallback (button, XmNdisarmCallback, my_callback, NULL);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

void
my_callback(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    XmPushButtonCallbackStruct *cbs = 
        (XmPushButtonCallbackStruct *) call_data;

    if (cbs->reason == XmCR_ARM)
        printf ("%s: armed\n", XtName (w));
    else if (cbs->reason == XmCR_DISARM)
        printf ("%s: disarmed\n", XtName (w));
    else
        printf ("%s: pushed %d times\n", XtName (w), cbs->click_count);
}
