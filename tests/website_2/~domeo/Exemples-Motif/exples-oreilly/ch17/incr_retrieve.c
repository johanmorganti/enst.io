/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* incr_retrieve.c -- simple copy and retrieve program.  Two
 * pushbuttons: the first places text in the clipboard, the other
 * receives text incrementally from the clipboard.  This just 
 * demonstrates the API involved.
 */
#include <Xm/CutPaste.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>

static void to_clipbd(), from_clipbd_incr();

main(argc, argv)
int argc;
char *argv[];
{
    Widget toplevel, rowcol, button;
    XtAppContext app;

    XtSetLanguageProc (NULL, NULL, NULL);

    /* Initialize toolkit, application context and toplevel shell */
    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    /* manage two buttons in a RowColumn widget */
    rowcol = XtVaCreateWidget ("rowcol", xmRowColumnWidgetClass, 
        toplevel, NULL);

    /* button1 copies to the clipboard */
    button = XtVaCreateManagedWidget ("button1",
        xmPushButtonWidgetClass, rowcol,
        XtVaTypedArg, XmNlabelString, XmRString,
            "Copy To Clipboard", 18, /* strlen() + 1 */
        NULL);
    XtAddCallback (button, XmNactivateCallback, to_clipbd, 
        "This is a long string that will require incremental retreival");

    /* button2 retrieves text stored in the clipboard */
    button = XtVaCreateManagedWidget ("button2",
        xmPushButtonWidgetClass, rowcol,
        XtVaTypedArg, XmNlabelString, XmRString,
            "Retrieve From Clipboard", 24, /* strlen() + 1 */
        NULL);
    XtAddCallback (button, XmNactivateCallback, from_clipbd_incr, NULL);

    /* manage RowColumn, realize toplevel shell and start main loop */
    XtManageChild (rowcol);
    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}

/* copy data to clipboard. */
static void
to_clipbd(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    unsigned long item_id = 0;  /* clipboard item id */
    int           status;
    XmString      clip_label;
    static int    cnt;
    Display      *dpy = XtDisplayOfObject (widget);
    Window        window = XtWindowOfObject (widget);
    char         *data = (char *) client_data;

    clip_label = XmStringCreateLocalized ("to_clipbd");

    /* start a copy -- retry till unlocked */
    do
        status = XmClipboardStartCopy (dpy, window,
            clip_label, CurrentTime, NULL, NULL, &item_id);
    while (status == ClipboardLocked);

    XmStringFree (clip_label);

    /* copy the data -- pass cnt as private id for kicks */
    do
        status = XmClipboardCopy (dpy, window, item_id, "STRING",
            data, (long) strlen (data)+1, cnt, NULL);
    while (status == ClipboardLocked);

    /* end the copy */
    do
        status = XmClipboardEndCopy (dpy, window, item_id);
    while (status == ClipboardLocked);

    printf ("Copied \"%s\" to clipboard.\n", data);
}

static void
from_clipbd_incr(widget, client_data, call_data)
Widget widget;
XtPointer client_data;
XtPointer call_data;
{
    int           status;
    unsigned      total_bytes;
    unsigned long received;
    char         *data = NULL, buf[32];
    Display      *dpy = XtDisplayOfObject (widget);
    Window        window = XtWindowOfObject (widget);

    do
        status = XmClipboardStartRetrieve (dpy, window, CurrentTime);
    while (status == ClipboardLocked);

    /* initialize data to contain at least one byte. */
    data = XtMalloc (1);
    total_bytes = 1;
    do  {
        /* retrieve data from clipboard -- if locked, try again */
        status = XmClipboardRetrieve (dpy, window, "STRING",
            buf, sizeof (buf), &received, NULL);

	/* reallocate data to contain enough space for everything */
	if (!(data = XtRealloc (data, total_bytes + received))) {
	    XtError ("Can't allocate space for data");
	    break; /* XtError may or may not return */
	}

	/* copy buf into data.  strncpy() does not NULL terminate */
	strncpy (&data[total_bytes-1], buf, received);
	total_bytes += received;
    } while (status == ClipboardTruncate);

    if (data)
        data[total_bytes] = 0; /* NULL terminate */

    if (status == ClipboardSuccess)
        printf ("Retrieved \"%s\" from clipboard.\n", data);

    status = XmClipboardEndRetrieve (dpy, window);
}
