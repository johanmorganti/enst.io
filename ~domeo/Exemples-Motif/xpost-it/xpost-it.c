/******************************************************************************/
/*                                                                            */
/*                                 xpost-it.c                                 */
/*                                                                            */
/*                       by Jean-Charles FERRERI (1995)                       */
/*                                                                            */
/******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/Protocols.h>
#include <Xm/AtomMgr.h>
#include <Xm/Text.h>
#include <Xm/RowColumn.h>
#include <Xm/PushB.h>
#include <Xm/Separator.h>

XtAppContext app;

Widget appshell;

typedef struct post_it
{
  Widget shell;
  struct post_it *previous, *next;
} post_it;
post_it *list = NULL, *reverselist = NULL;

static Widget GetText(const Widget shell)
{
  WidgetList children;
  XtVaGetValues(shell, XmNchildren, &children, NULL);
  return *children;
}
static FILE * OpenIniFile(const char *const mode)
{
  const char *const IniFile = ".xpost-it";
  const char *const home = getenv("HOME");
  char *const file = 
    (home != NULL) ?
      strcat(strcat(strcpy((char *)calloc(strlen(home) + 1 + strlen(IniFile) + 1, sizeof(char)), home), "/"), IniFile)
	: 
      strcpy((char *)calloc(strlen(IniFile) + 1, sizeof(char)), IniFile);
  FILE *const f = fopen(file, mode);

  free(file);
  return f;
}

static Save(void)
{
  FILE *const f = OpenIniFile("w");
  
  if (f != NULL)
  {
    XEvent *event=(XEvent *)malloc(sizeof(XEvent));

    if (event != NULL)
    {
      post_it *reflist = list;
  
      while (reflist != NULL)
      {
	XtUnmapWidget(reflist->shell);    
	XtVaSetValues(reflist->shell, XmNmwmDecorations, 0, XmNinitialState, NormalState, NULL);
	XtVaSetValues(GetText(reflist->shell), XmNsensitive, False, NULL);
	XtMapWidget(reflist->shell); 

	reflist = reflist->next;
      }  

      while (XtAppPending(app) != 0)
      {
	XtAppNextEvent(app, event);
	XtDispatchEvent(event);
      }

      sleep(1);

      while (XtAppPending(app) != 0)
      {
	XtAppNextEvent(app, event);
	XtDispatchEvent(event);
      }

      free(event);
    }

    while (reverselist != NULL)
    {
      Dimension height, width;
      Position x, y;
      const char *value;
      post_it *const oldelt = reverselist;
      
      XtVaGetValues(reverselist->shell, XmNheight, &height, XmNwidth, &width, XmNx, &x, XmNy, &y, NULL); 
      XtVaGetValues(GetText(reverselist->shell), XmNvalue, &value, NULL);
      
      fprintf(f, "height:%hu width:%hu x:%hd y:%hd length:%u\n%s\n", height, width, x, y, strlen(value), value);
 
      if (reverselist->previous != NULL)
	reverselist->previous->next = NULL;
      if (reverselist == list)
	list = NULL;
      reverselist = reverselist->previous;

      free(oldelt);
    }
    fclose(f);
  }

  exit(0);
}

static Widget Add(void);
static void DecorationsCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  Widget shell = (Widget)closure;
  int decor;
 
  XtUnmapWidget(shell);

  XtVaGetValues(shell, XmNmwmDecorations, &decor, NULL);

  if (decor == 0)
    XtVaSetValues(shell, XmNmwmDecorations, -1, NULL);
  else
    XtVaSetValues(shell, XmNmwmDecorations, 0, NULL);

  XtVaSetValues(shell, XmNinitialState, NormalState, NULL);
  XtMapWidget(shell);
}
static void NewCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  const Widget shell = Add();
  if (shell != NULL)
    XtPopup(shell, None);
}
static void DestroyCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  Widget shell = (Widget)closure;
  post_it *reflist;

  XtVaGetValues(GetText(shell), XmNuserData, &reflist, NULL);

  if  (reflist->previous == NULL && reflist->next == NULL)
    XtVaSetValues(GetText(reflist->shell), XmNvalue, "", NULL);
  else
  {
    if (reflist->previous != NULL)
      reflist->previous->next = reflist->next;
    if (reflist == list)
      list = reflist->next;
    if (reflist->next != NULL)
      reflist->next->previous = reflist->previous;
    if (reflist == reverselist)
      reverselist = reflist->previous;

    XtDestroyWidget(shell);
  }
}
static void MinimizeCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  post_it *reflist = list;
  
  while (reflist != NULL)
  {
    XtUnmapWidget(reflist->shell);    
    XtVaSetValues(reflist->shell, XmNinitialState, IconicState, NULL);
    XtMapWidget(reflist->shell);    

    reflist = reflist->next;
  }  
}
static void RaiseCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  Widget shell = (Widget)closure;
  post_it *reflist = list;
  
  while (reflist != NULL)
  {
    if (reflist->shell != shell)
    {
      XtUnmapWidget(reflist->shell);    
      XtVaSetValues(reflist->shell, XmNinitialState, NormalState, NULL);
      XtMapWidget(reflist->shell);   
    }
    
    reflist = reflist->next;
  }  

  XtUnmapWidget(shell);
  XtVaSetValues(shell, XmNinitialState, NormalState, NULL);
  XtMapWidget(shell);
}
static void ExitCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  Save();
}
static void MenuCallback(const Widget widget, const XtPointer closure, const XtPointer call_data)
{
  XButtonPressedEvent *const event = (XButtonPressedEvent *)((XmAnyCallbackStruct *)call_data)->event;

  const Widget shell = (Widget)closure;

  const Widget menu = XmCreatePopupMenu(GetText(shell), "Menu", NULL, 0);

  const Widget decorations = XmCreatePushButton(menu, "Decorations", NULL, 0);
  const Widget new = XmCreatePushButton(menu, "New", NULL, 0);
  const Widget destroy = XmCreatePushButton(menu, "Destroy", NULL, 0);
  const Widget separatorlocal = XmCreateSeparator(menu, "Separator", NULL, 0);
  const Widget minimize = XmCreatePushButton(menu, "Minimize", NULL, 0);
  const Widget raise = XmCreatePushButton(menu, "Raise", NULL, 0);
  const Widget separatorglobal = XmCreateSeparator(menu, "Separator", NULL, 0);
  const Widget exit = XmCreatePushButton(menu, "Exit", NULL, 0);

  XtAddCallback(decorations, XmNactivateCallback, DecorationsCallback, shell);
  XtManageChild(decorations);
  XtAddCallback(new, XmNactivateCallback, NewCallback, shell);
  XtManageChild(new);
  XtAddCallback(destroy, XmNactivateCallback, DestroyCallback, shell);
  XtManageChild(destroy);
  XtManageChild(separatorlocal);
  XtAddCallback(minimize, XmNactivateCallback, MinimizeCallback, shell);
  XtManageChild(minimize);
  XtAddCallback(raise, XmNactivateCallback, RaiseCallback, shell);
  XtManageChild(raise);
  XtManageChild(separatorglobal);
  XtAddCallback(exit, XmNactivateCallback, ExitCallback, shell);
  XtManageChild(exit);

  XmMenuPosition(menu, event);
  XtManageChild(menu);
}

static Widget Add(void)
{
  post_it *const newlist = (post_it *)malloc(sizeof(post_it));
  
  if (newlist != NULL)
  {
    Arg shellargs[] = { {XmNallowShellResize, True},
			{XmNdeleteResponse, XmDO_NOTHING},
			{XmNmwmDecorations, 0}
		      };
    const Widget shell = XtCreatePopupShell("xpost-it", topLevelShellWidgetClass, appshell, shellargs, XtNumber(shellargs));
 
    Arg textargs[] = { {XmNbottomAttachment, XmATTACH_FORM},
		       {XmNleftAttachment, XmATTACH_FORM},
		       {XmNrightAttachment, XmATTACH_FORM},
		       {XmNtopAttachment, XmATTACH_FORM},
		       {XmNeditMode, XmMULTI_LINE_EDIT},
		       {XmNresizeHeight, True},
		       {XmNwordWrap, True},
		       {XmNscrollHorizontal, False},
		       {XmNscrollVertical, False}
		     };
    const Widget text = XmCreateText(shell, "text", textargs, XtNumber(textargs));

    newlist->shell = shell;
    newlist->previous = reverselist;
    newlist->next = NULL;
    if (reverselist != NULL)
      reverselist->next = newlist;
    reverselist = newlist;
    if (list == NULL)
      list = newlist;
  
    XtVaSetValues(text, XmNuserData, (XtPointer)newlist, NULL);

    XmAddWMProtocolCallback(shell, XmInternAtom(XtDisplay(shell), "WM_DELETE_WINDOW", False), DestroyCallback, shell);
  
    XtAddCallback(text, XmNactivateCallback, MenuCallback, shell);
    XtAugmentTranslations(text, XtParseTranslationTable("<Btn3Down>:activate()"));

    XtManageChild(text);

    return shell;
  }
  else
    return NULL;
}

static void SignalHandler(int code)
{
  Save();
}

static void Load(void)
{
  FILE *const f = OpenIniFile("r");

  if (f != NULL)
  {
    while (!feof(f))
    {
      Dimension height, width;
      Position x, y;
      unsigned int length;

      while (!feof(f) && fscanf(f, "height:%hu width:%hu x:%hd y:%hd length:%u", &height, &width, &x, &y, &length) != 5)
	while (!feof(f) && fgetc(f) != '\n');
      if (!feof(f))
      {
	char *const value = (char *)calloc(length + 1, sizeof(char));
	char *ptrchar = value;
	const Widget shell = Add();

	fgetc(f);

	if (shell != NULL)
	  XtVaSetValues(shell, XmNheight, height, XmNwidth, width, XmNdefaultPosition, False, XmNx, x, XmNy, y, NULL);

	while (!feof(f) && ptrchar - value < length)
	{
	  *ptrchar = fgetc(f);
	  if (!feof(f))
	    ptrchar++;
	}
	*ptrchar = '\0';
	while (!feof(f) && ptrchar - value < length)
	{
	  fgetc(f);
	  if (!feof(f))
	    length--;
	}
	fgetc(f);

	if (shell != NULL)
	  XtVaSetValues(GetText(shell), XmNvalue, value, NULL);
	free(value);

	if (shell != NULL)
	  XtPopup(shell, None);
      }
    }
    fclose(f);
  }

  if (list == NULL)
  {
    const Widget shell = Add();
    if (shell != NULL)
      XtPopup(shell, None);
  }
}

int main(int argc, char **argv)
{
  String fallbacks[] = { "*.text.background : Yellow",
			 NULL
		       };
  appshell = XtAppInitialize(&app, "XPost-It", NULL, 0, &argc, argv, fallbacks, NULL, 0);  

  Load();

  signal(SIGHUP, SignalHandler);
  signal(SIGILL, SignalHandler);
  signal(SIGTRAP, SignalHandler);
  signal(SIGIOT, SignalHandler);
  signal(SIGEMT, SignalHandler);
  signal(SIGFPE, SignalHandler);
  signal(SIGBUS, SignalHandler);
  signal(SIGSEGV, SignalHandler);
  signal(SIGSYS, SignalHandler);
  signal(SIGPIPE, SignalHandler);
  signal(SIGTERM, SignalHandler);
  signal(SIGXCPU, SignalHandler);
  signal(SIGXFSZ, SignalHandler);

  XtAppMainLoop(app);

  return 0;
}

