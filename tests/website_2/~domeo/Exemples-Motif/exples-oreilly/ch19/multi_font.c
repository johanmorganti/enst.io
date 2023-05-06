/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* multi_font.c -- create three compound strings using 12, 14 and 18 
 * point fonts.  The user can specify resources so that each of the strings
 * use different fonts by setting resources similar to that shown
 * by the fallback resources.
 */
#include <Xm/Label.h>

String fallbacks[] = {
    "multi_font*fontList:\
-*-courier-*-r-*--12-*=TAG1,\
-*-courier-bold-o-*--14-*=TAG2,\
-*-courier-medium-r-*--18-*=TAG3",
    NULL
};

main(argc, argv)
int argc;
char *argv[];
{
    Widget        toplevel;
    XtAppContext  app;
    XmString      s1, s2, s3, text, tmp;
    String        string1 = "This is a string ",
                  string2 = "that contains three ",
                  string3 = "separate fonts.";

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "String", NULL, 0,
        &argc, argv, fallbacks, NULL);

    s1 = XmStringCreate (string1, "TAG1");
    s2 = XmStringCreate (string2, "TAG2");
    s3 = XmStringCreate (string3, "TAG3");

    /* concatenate the 3 strings on top of each other, but we can only
     * do two at a time.  So do s1 and s2 onto tmp and then do s3.
     */
    tmp = XmStringConcat (s1, s2);
    text = XmStringConcat (tmp, s3);

    XtVaCreateManagedWidget ("widget_name",
        xmLabelWidgetClass, toplevel,
        XmNlabelString,     text,
        NULL);

    XmStringFree (s1);
    XmStringFree (s2);
    XmStringFree (s3);
    XmStringFree (tmp);
    XmStringFree (text);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
