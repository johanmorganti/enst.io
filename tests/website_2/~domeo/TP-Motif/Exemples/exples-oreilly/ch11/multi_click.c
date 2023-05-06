/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* multi_click.c -- demonstrate handling multiple PushButton clicks.
 * First, obtain the time interval of what constitutes a multiple
 * button click from the display and pass this as the client_data
 * for the button_click() callback function.  In the callback, single
 * button clicks set a timer to expire on that interval and call the
 * function process_clicks().  Double clicks remove the timer and
 * just call process_clicks() directly.
 */
#include <Xm/PushB.h>

XtAppContext app;

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, button;
    void button_click();
    XmString btn_text;
    int interval;

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    /* get how long for a double click */
    interval = XtGetMultiClickTime (XtDisplay (toplevel));
    printf ("Interval = %d\n", interval);

    btn_text = XmStringCreateLocalized ("Push Here");
    button = XtVaCreateManagedWidget ("button",
        xmPushButtonWidgetClass, toplevel,
        XmNlabelString, btn_text,
        NULL);
    XmStringFree (btn_text);
    XtAddCallback (button, XmNactivateCallback, button_click, interval);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* Process button clicks.  Single clicks set a timer, double clicks
 * remove the timer, and extended clicks are ignored.
 */
void
button_click(w, client_data, call_data)
Widget w;
XtPointer client_data;
XtPointer call_data;
{
    static XtIntervalId id;
    void process_clicks();
    int interval = (int) client_data;
    XmPushButtonCallbackStruct *cbs = 
        (XmPushButtonCallbackStruct *) call_data;

    if (cbs->click_count == 1)
        id = XtAppAddTimeOut (app, interval, process_clicks, False);
    else if (cbs->click_count == 2) {
        XtRemoveTimeOut (id);
        process_clicks (True);
    }
}

/* This function won't be called until we've established whether
 * or not a single or a double click has occured.
 */
void
process_clicks(client_data, id)
XtPointer client_data;
XtIntervalId id;
{
    int double_click = (int) client_data;

    if (double_click)
        puts ("Double click");
    else
        puts ("Single click");
}
