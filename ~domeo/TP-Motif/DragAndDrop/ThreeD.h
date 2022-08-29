/*
 *	ThreeD.h
 *
 *	Definition Public pour la Wigdet ThreeD
 *
 *	Creation:	G.Dauphin	13/05/91
 */

#ifndef _TKSThreeD_h
#define _TKSThreeD_h

/*
 * Pas necessaire si le fichier en .c inclue IntrinsicP.h
 * avant ce fichier. Ca n'a pas d'importance
 */
#include <X11/Core.h>

/* Resources:

 Name		     Class		RepType		Default Value
 ----		     -----		-------		-------------
 x		     Position		Position	0
 y		     Position		Position	0
 width		     Width		Dimension	0
 height		     Height		Dimension	0
 borderWidth	     BorderWidth	Dimension	1
 sensitive	     Sensitive		Boolean		True
 ancestorSensitive   AncestoreSensitive Boolean
 borderColor	     BorderColor	Pixel		XtDefaultForeground
 screen		     Screen		Pointer		XtCopyScreen
 colormap	     Colormap		Pointer		XtCopyFromParent
 background	     Background		Pixel		XtDefaultBackground
 destroyCallback     Callback		Pointer		NULL
 mappedWhenManaged   MappedWhenManaged	Boolean		True

(from ThreeD)
 foreground	     Foreground		Pixel		XtDefaultForeground
*/



typedef struct _ThreeDClassRec	*ThreeDWidgetClass;
typedef struct _ThreeDRec	*ThreeDWidget;

/* declare the class constant */

extern WidgetClass threeDWidgetClass;

extern void TksThreeDMettreThreeD();

#define TksNminimumValue "minimumValue"
#define TksCMinimumValue "MinimumValue"
#define TksNmaximumValue	"maximumValue"
#define TksCMaximumValue	"MaximumValue"
#define TksNrotateX	"rotateX"
#define TksCRotateX	"RotateX"
#define TksNrotateZ	"rotateZ"
#define TksCRotateZ	"RotateZ"
#define TksNviewDistance	"viewDistance"
#define TksCViewDistance	"ViewDistance"
#define TksNeyeDistance	"eyeDistance"
#define TksCEyeDistance		"EyeDistance"
#define TksNperspective		"perspective"
#define TksCPerspective		"Perspective"

#endif /* _TKSThreeD_h */

