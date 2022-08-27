/*
 *	ThreeD.c
 *
 *	Creation : 	G.Dauphin	11/05/91
 */

#include <stdio.h>
#include <X11/StringDefs.h>
#include <X11/IntrinsicP.h>
#include <X11/Intrinsic.h>
#include <X11/Shell.h>

#include "ThreeDP.h"

/*
 *	DEFINITIONS
 */

#define DEFAULT_WIDTH	(256)
#define DEFAULT_HEIGHT	(256)

#define FieldOffset(f) XtOffset(ThreeDWidget,threeD.f)
#define CoreOffset(f)  XtOffset(ThreeDWidget,core.f)

#define SUPERCLASS	(&coreClassRec)

/*
 *	ROUTINES
 */

static void		Initialize();
static void		Realize();
static void		Destroy();
static void		Resize();
static void		Redisplay();
static Boolean		SetValues();

/*
 *	DECLARATIONS
 */


static float maxVal = 50.0;
static float minVal = -50.0;
static float rotxVal = 63.7;
static float rotzVal = 31.8;
static float eyedVal = 8.0;
static float viewdVal = 16.0;

/*
 *	RESOURCES
 */

static XtResource resources[] = {
	{ XtNwidth,XtCWidth,XtRDimension,sizeof(Dimension),
	  CoreOffset(width), XtRImmediate,(caddr_t)DEFAULT_WIDTH},
	{ XtNheight,XtCHeight,XtRDimension,sizeof(Dimension),
	  CoreOffset(height), XtRImmediate,(caddr_t)DEFAULT_HEIGHT},
	{ XtNforeground, XtCForeground, XtRPixel, sizeof(Pixel),
	  FieldOffset(foreground), XtRString, "white" },
	{ XtNbackground, XtCBackground, XtRPixel, sizeof(Pixel),
	  CoreOffset(background_pixel), XtRString, "black"},
	{ TksNminimumValue,TksCMinimumValue,XtRFloat,sizeof(float),
	  FieldOffset(minval),XtRFloat,(caddr_t)&minVal},
	{ TksNmaximumValue,TksCMaximumValue,XtRFloat,sizeof(float),
	  FieldOffset(maxval),XtRFloat,(caddr_t)&maxVal},
	{ TksNrotateX,TksCRotateX,XtRFloat,sizeof(float),
	  FieldOffset(rotx),XtRFloat,(caddr_t)&rotxVal},
	{ TksNrotateZ,TksCRotateZ,XtRFloat,sizeof(float),
	  FieldOffset(rotz),XtRFloat,(caddr_t)&rotzVal},
	{ TksNeyeDistance,TksCEyeDistance,XtRFloat,sizeof(float),
	  FieldOffset(eyed),XtRFloat,(caddr_t)&eyedVal},
	{ TksNviewDistance,TksCViewDistance,XtRFloat,sizeof(float),
	  FieldOffset(viewd),XtRFloat,(caddr_t)&viewdVal},
	{ TksNperspective,TksCPerspective,XtRBoolean,sizeof(Boolean),
	  FieldOffset(type_projection),XtRImmediate,(caddr_t) True},
};
#undef FieldOffset
#undef CoreOffset

/*
 *	ALLOCATION DE CLASSE
 */

static ThreeDClassRec threeDClassRec = {
	{
		/* core_class field */
	/* superclass           */      (WidgetClass)SUPERCLASS,
	/* class_name           */      "ThreeD",
	/* size                 */      sizeof(ThreeDRec),
	/* class_initialize     */      NULL,
	/* class_part_initialize*/      NULL,
	/* class_inited         */      FALSE,
	/* initialize           */      Initialize,
	/* initialize_hook      */      NULL,
	/* realize              */      Realize,
	/* actions              */      NULL,
	/* num_actions          */      0,
	/* resources            */      resources,
	/* num_resources        */      XtNumber(resources),
	/* xrm_class            */      NULLQUARK,
	/* compress_motion      */      TRUE,
	/* compress_exposure    */      TRUE,
	/* compress_enterleave  */      TRUE,
	/* visible_interest     */      FALSE,
	/* destroy              */      Destroy,
	/* resize               */      Resize,
	/* expose               */      Redisplay,
	/* set_values           */      SetValues,
	/* set_values_hook      */      NULL,
	/* set_values_almost    */      XtInheritSetValuesAlmost,
	/* get_values_hook      */      NULL,
	/* accept_focus         */      NULL,
	/* version              */      XtVersion,
	/* callback_private     */      NULL,
	/* tm_table             */      NULL,
	/* query_geometry       */      XtInheritQueryGeometry,
	/* display_accelerator	*/	XtInheritDisplayAccelerator,
	/* extension		*/	NULL
	}, /* CorePart */
        {
	/* dummy field */	 	0
	}  /* ThreeD class part */
};

WidgetClass threeDWidgetClass = (WidgetClass) &threeDClassRec;

/*
 *	INITIALIZE
 */

static void Initialize(request, new)
	ThreeDWidget request , new;
{
	XGCValues values;
	XtGCMask mask = GCForeground ;
	int ncoul;
	int i;
	Pixel * tabpix;


	new->threeD.data = NULL;
	new->threeD.col = 0;
	new->threeD.row = 0;

        if (Width(request) <= 0)
		Width(new) = DEFAULT_WIDTH;
        if (Height(request) <= 0)
		Height(new) = DEFAULT_HEIGHT;

	mask = GCForeground |GCFunction | GCPlaneMask;
	values.function = GXcopy;
	values.plane_mask = AllPlanes;
	values.foreground = TFForeground(new);
	TFDrawGC(new) = XtGetGC(new,mask,&values);
	values.foreground = TFBackground(new);
	TFFillGC(new) = XtGetGC(new,mask,&values);
}

/*
 *	Realize
 */

static void Realize(w,valueMask,attrs)
	ThreeDWidget w;
	XtValueMask *valueMask;
	XSetWindowAttributes *attrs;
{
	XSetWindowAttributes xswa;

	/*Preserve content of attrs */
	xswa = *attrs;
	xswa.backing_store = WhenMapped;
        XtCreateWindow(w,InputOutput,(Visual *)CopyFromParent,
                *valueMask|CWBackingStore,&xswa);

} /* End Realize */


static void Redisplay(w, event)
	ThreeDWidget w;
	XExposeEvent *event;
{
	if(!XtIsRealized(w)) return;
	XClearArea(XtDisplay(w),XtWindow(w),0,0,0,0,False);
	TksX3DMesh(w,TFDrawGC(w),TFFillGC(w),w->threeD.data,
		w->threeD.col,w->threeD.row,
		w->threeD.minval,w->threeD.maxval,
		w->threeD.rotx,w->threeD.rotz,
		w->threeD.eyed,w->threeD.viewd,
		w->threeD.type_projection);
}

/*
 * SETVALUES
 */

static Boolean SetValues(current,request,new)
	Widget current , request, new;
{
	ThreeDWidget curw = (ThreeDWidget) current;
	ThreeDWidget newcw = (ThreeDWidget) new;
	Boolean do_redisplay = False;

	return (do_redisplay);
} /* End SetValues */

/*
 *	Resize
 */
static void Resize(w)
	ThreeDWidget w;
{
}


/*
 *	DESTROY
 */

static void Destroy(w)
	ThreeDWidget w;
{
	XtFree(w->threeD.data);
	XtReleaseGC(w,TFDrawGC(w));
	XtReleaseGC(w,TFFillGC(w));
}


void TksThreeDMettreThreeD(w,data,col,row)
	ThreeDWidget w;
	float * data;	/* tableau 2D de float data[col][row] */
	int col,row;	/* dimension data[col][row] */
	
{
	int i,j,k,ngc;
	double val,minp;
	float *front,top,inter;
	unsigned int ind;


	if(XtIsRealized(w))
		XClearArea(XtDisplay(w),XtWindow(w),0,0,0,0,False);
	if(w->threeD.data){	/* liberer les memoires*/
		for(i=0;i<w->threeD.col;i++){
			XtFree(w->threeD.data[i]);
		}
		XtFree(w->threeD.data);
		w->threeD.data = NULL;
		w->threeD.col = w->threeD.row = 0;
	}
	if(data == NULL) return;

	w->threeD.col = col;
	w->threeD.row = row;
	w->threeD.data =(float**)XtMalloc(col*sizeof(float*));
	for(i=0;i<col;i++){
		w->threeD.data[i] = (float *) XtMalloc(row * sizeof(float));
	}
	for(i=0;i<col;i++)
		for(j=0;j<row;j++)
			w->threeD.data[i][j] = data[i*row+j];
	if(XtIsRealized(w)){
		XClearArea(XtDisplay(w),XtWindow(w),0,0,0,0,False);
		TksX3DMesh(w,TFDrawGC(w),TFFillGC(w),w->threeD.data,
			w->threeD.col,w->threeD.row,
			-50.0,50.0,63.7,31.8,8.,16.0);
	}
}

