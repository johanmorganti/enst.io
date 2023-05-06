/* Written by Dan Heller and Paula Ferguson.  
 * Copyright 1994, O'Reilly & Associates, Inc.
 * Permission to use, copy, and modify this program without
 * restriction is hereby granted, as long as this copyright
 * notice appears in each copy of the program source code.
 * This program is freely distributable without licensing fees and
 * is provided without guarantee or warrantee expressed or implied.
 * This program is -not- in the public domain.
 */

/* fontlist.c -- demonstrate how to create, add to, and destroy
 * font lists.  The fonts and text displayed are hardcoded in
 * this program and cannot be overriden by user resources.
 */
#include <Xm/Label.h>

main(argc, argv)
int argc;
char *argv[];
{
    Widget        	toplevel;
    XtAppContext  	app;
    XmString      	s1, s2, s3, text, tmp;
    XmFontListEntry	entry1, entry2, entry3;
    XmFontList    	fontlist;
    String        	string1 = "This is a string ",
                  	string2 = "that contains three ",
                  	string3 = "separate fonts.";

    XtSetLanguageProc (NULL, NULL, NULL);

    toplevel = XtVaAppInitialize (&app, "Demos", NULL, 0,
        &argc, argv, NULL, NULL);

    entry1 = XmFontListEntryLoad (XtDisplay (toplevel), 
        "-*-courier-*-r-*--*-120-*", XmFONT_IS_FONT, "TAG1");
    entry2 = XmFontListEntryLoad (XtDisplay (toplevel),
        "-*-courier-bold-o-*--*-140-*", XmFONT_IS_FONT, "TAG2");
    entry3 = XmFontListEntryLoad (XtDisplay (toplevel),
        "-*-courier-medium-r-*--*-180-*", XmFONT_IS_FONT, "TAG3");
    fontlist = XmFontListAppendEntry (NULL, entry1);
    fontlist = XmFontListAppendEntry (fontlist, entry2);
    fontlist = XmFontListAppendEntry (fontlist, entry3);
    XmFontListEntryFree (&entry1);
    XmFontListEntryFree (&entry2);
    XmFontListEntryFree (&entry3);

    s1 = XmStringCreate (string1, "TAG1");
    s2 = XmStringCreate (string2, "TAG2");
    s3 = XmStringCreate (string3, "TAG3");

    /* concatenate the 3 strings on top of each other, but we can only
     * do two at a time.  So do s1 and s2 onto tmp and then do s3.
     */
    tmp = XmStringConcat (s1, s2);
    text = XmStringConcat (tmp, s3);

    XtVaCreateManagedWidget ("label", xmLabelWidgetClass, toplevel,
        XmNlabelString,     text,
        XmNfontList,        fontlist,
        NULL);

    XmStringFree (s1);
    XmStringFree (s2);
    XmStringFree (s3);
    XmStringFree (tmp);
    XmStringFree (text);
    XmFontListFree (fontlist);

    XtRealizeWidget (toplevel);
    XtAppMainLoop (app);
}
