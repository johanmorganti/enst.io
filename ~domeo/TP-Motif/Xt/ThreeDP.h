/*
 *	ThreeDP.h
 *
 *	Definition Prive pout la Wigdet ThreeD
 *
 *	Creation:	G.Dauphin	13/05/91
 */

#ifndef _TKSThreeDP_h
#define _TKSThreeDP_h

#include <X11/CoreP.h>
#include "ThreeD.h"


/***************************************************************************
 * Class Part & Class Record                                               *
 ***************************************************************************/

/* Nouveau champ pour ThreeD widget class record */

typedef struct {
	int	make_compiler_happy;	/* need dummy field */
} ThreeDClassPart;

/* Full class record declaration */

typedef struct _ThreeDClassRec {
	CoreClassPart		core_class;
	ThreeDClassPart threeD_class;
} ThreeDClassRec;

extern ThreeDClassRec	threeDClassRec;

/***************************************************************************
 * Instance Part & Instance Record                                         *
 ***************************************************************************/

/* nouveau champ pour ThreeD widget record */
typedef struct {
	/* resources */
	Pixel		foreground;	/* pour les popups */
	float		maxval;	/* valeur max de puissance affichable */
	float		minval;	
	float		rotx;	/* rotation autour de l'axe X*/
	float		rotz;	/* rotation autour de l'axe Z */
	float		eyed;	/* eye distance */
	float		viewd;	/*view distance */
	Boolean		type_projection; /* type de projection */

	/* private state */
	float **	data;	/* tableau de ns ThreeD */
	int		col;
	int		row;	/* rectind[col][row] data[col][row] */
	GC		draw_gc; /* font + foreground pour axe */
	GC		fill_gc; /* gc pour XFill */
} ThreeDPart;

/*
 * Full instance record declaration;
 */
typedef struct _ThreeDRec {
	CorePart		core;
	ThreeDPart	threeD;
} ThreeDRec;

#endif /* _TKSThreeDP_h */

#define	CorePart(w)		w->core
#define	TFPart(w)		w->threeD

#define TFDrawGC(w)		(TFPart(w).draw_gc)
#define TFFillGC(w)		(TFPart(w).fill_gc)
#define TFForeground(w)		(TFPart(w).foreground)
#define TFBackground(w)		((w)->core.background_pixel)

#define BorderWidth(w)		((w)->core.border_width)
#define	Width(w)		((w)->core.width)
#define	Height(w)		((w)->core.height)
