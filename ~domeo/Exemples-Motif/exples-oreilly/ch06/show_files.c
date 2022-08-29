/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* show_files.c -- introduce FileSelectionDialog; print the file
 * selected by the user.
 */
#include <Xm/FileSB.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget        toplevel, text_w, dialog;
    XtAppContext  app;
    extern void   exit(), echo_file();

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos",
        NULL, 0, &argc, argv, NULL, NULL);

    /* Create a simple FileSelectionDialog -- no frills */
    dialog = XmCreateFileSelectionDialog (toplevel, "filesb", NULL, 0);
    XtAddCallback (dialog, XmNcancelCallback, exit, NULL);
    XtAddCallback (dialog, XmNokCallback, echo_file, NULL);
    XtManageChild (dialog);

    XtAppMainLoop (app);
}

/* callback routine when the user selects OK in the FileSelection
 * Dialog.  Just print the file name selected.
 */
void
echo_file(widget, client_data, call_data)
Widget widget;  /* file selection box */
XtPointer client_data;
XtPointer call_data;
{
    char *filename;
    XmFileSelectionBoxCallbackStruct *cbs = 
        (XmFileSelectionBoxCallbackStruct *) call_data;

    if (!XmStringGetLtoR (cbs->value, XmFONTLIST_DEFAULT_TAG, &filename))
        return; /* must have been an internal error */

    if (!*filename) { /* nothing typed? */
        puts ("No file selected.");
        XtFree( filename); /* even "" is an allocated byte */
        return;
    }

    printf ("Filename given: \"%s\"\n", filename);
    XtFree (filename);
}
