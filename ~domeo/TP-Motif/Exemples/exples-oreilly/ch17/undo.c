/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* undo.c -- demonstrate undoing a clipboard copy */

#include <Xm/Xm.h>
#include <Xm/CutPaste.h>
#include <Xm/RowColumn.h>
#include <Xm/PushBG.h>

void
cut_to_clipboard(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    unsigned long item_id = 0;	/* clipboard item id */
    int data_id = 0;	/* clipboard data id */
    int status = 0;	/* clipboard status  */
    XmString clip_label;
    char buf[32];
    static int cnt;
    Display *dpy = XtDisplayOfObject (widget);
    Window window = XtWindowOfObject (widget);
    char *data = (char *) client_data;

    sprintf (buf, "%s %d", data, ++cnt);
    printf ("Putting \"%s\" on clipboard\n", buf);

    clip_label = XmStringCreateLocalized ("cut_to_clipboard");
    /*
     * start copy to clipboard, and continue till a sucessful start copy
     * is made
     */

    do
	status = XmClipboardStartCopy (dpy, window,
	    clip_label, CurrentTime, NULL, NULL, &item_id);
    while (status != ClipboardSuccess);
    /*
     * move the data to the clipboard, and continue till a sucessful copy
     * is made
     */
    do
        status = XmClipboardCopy (dpy, window,
	    item_id, "STRING", buf, (long) strlen (buf) + 1, 0, &data_id);
    while (status != ClipboardSuccess);

    /*
     * end the copy to the clipboard and continue till a sucessful end
     * copy is made
     */
    do
        status = XmClipboardEndCopy (dpy, window, item_id);
    while (status != ClipboardSuccess);
}

void
undo(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    XmClipboardUndoCopy (XtDisplayOfObject (widget), 
        XtWindowOfObject (widget));
}

void
retrieve_from_clipboard(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int status;
    char buf[32];
    Display *dpy = XtDisplayOfObject (widget);
    Window window = XtWindowOfObject (widget);

    XmClipboardStartRetrieve (dpy, window, CurrentTime);
    do {
	status = XmClipboardRetrieve (dpy, window,
	    "STRING", buf, sizeof buf, NULL, NULL);
	printf ("Status = %s\n",
	    (status == ClipboardSuccess)? "success" :
	    (status == ClipboardLocked)? "locked" :
	    (status == ClipboardNoData)? "no data" :
	    (status == ClipboardTruncate)? "data truncated" :
	    (status == ClipboardFail)? "Failed" : "Bad Format");
	if (status == ClipboardSuccess)
	    puts (buf);
    } while (status != ClipboardSuccess);

    XmClipboardEndRetrieve (dpy, window);
}

main(argc, argv)
char *argv[];
{
    Widget toplevel, rowcol, button;
    XtAppContext app;

  XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize(&app, "Demos", NULL, 0,
	&argc, argv, NULL, NULL);

    rowcol = XtVaCreateManagedWidget("rowcol",
	xmRowColumnWidgetClass, toplevel,
	NULL);
    button = XtVaCreateManagedWidget("button1",
	xmPushButtonGadgetClass, rowcol,
	XtVaTypedArg, XmNlabelString, XmRString,
	    "Cut To Clipboard", sizeof (char *),
	NULL);
    XtAddCallback(button, XmNactivateCallback, cut_to_clipboard, "data");

    button = XtVaCreateManagedWidget("button2",
	xmPushButtonGadgetClass, rowcol,
	XtVaTypedArg, XmNlabelString, XmRString,
	   "Undo Cut", sizeof (char *),
	NULL);
    XtAddCallback(button, XmNactivateCallback, undo, NULL);

    button = XtVaCreateManagedWidget("retrieve",
	xmPushButtonGadgetClass, rowcol,
	XtVaTypedArg, XmNlabelString, XmRString,
	    "Retrieve From Clipboard", sizeof (char *),
	NULL);
    XtAddCallback(button, XmNactivateCallback, retrieve_from_clipboard, NULL);

    XtRealizeWidget(toplevel);
    XtAppMainLoop(app);
}
