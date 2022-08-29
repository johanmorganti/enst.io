
/* ********************************************************************* *
 *
 *  xmutil.h
 *  header de xmutil.c
 *  [Eric Lecolinet - 1994].
 *
 * ********************************************************************* */


extern XmString StringCreate(String text);

extern String StringGet(XmString xmstring);

extern void 
  OpenDialog(Widget w, XtPointer dialog, XtPointer xxx),
  CloseDialog(Widget w, XtPointer dialog, XtPointer xxx);

extern Widget
  CreateMenu(Widget parent, String name, ...),
  CreateDialog(Widget parent, String name, Widget (*create_function)(),
	       XtCallbackProc ok_callback, XtPointer data);

extern Cardinal SetArgs(ArgList arglist, Cardinal maxlength, ...);


extern XImage
  *ReadImage(char *filename),
  *WriteImage(char *filename, XImage *ximage),
  *CreateImage(char *pixmap, int width, int height, int depth);

extern void DisplayImage(Widget drawing, XImage *ximage);
