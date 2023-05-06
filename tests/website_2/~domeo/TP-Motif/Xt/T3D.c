#include <math.h>
#include <X11/IntrinsicP.h>
#include "ThreeD.h"

#define DegToRads(x) ((3.14159*x)/180.0)
void MatrixScale(x,y,z,m)
	float x,y,z;
	float m[4][4];
{
	m[0][0] = x; m[1][1] = y; m[2][2] = z; m[3][3] = 1.0;
	m[0][1] = m[0][2] = m[0][3] = 0.0;
	m[1][0] = m[1][2] = m[1][3] = 0.0;
	m[2][0] = m[2][1] = m[2][3] = 0.0;
	m[3][0] = m[3][1] = m[3][2] = 0.0;
}
/*
 *	outm = m1 * m2 ; multiplication de matrice
 */
void MatrixMult(m1,m2,outm)
	float m1[4][4], m2[4][4],outm[4][4];
{
	int i,j;
	float tmpm[4][4];	/* pour pouvoir faire m1 = m1*m2 */

	for(i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			tmpm[i][j] = m1[i][0] * m2[0][j] +
				     m1[i][1] * m2[1][j] +
				     m1[i][2] * m2[2][j] +
				     m1[i][3] * m2[3][j];
	/* copie le resultat dans outm */
	bcopy( (char *)tmpm, (char *)outm, sizeof(float[4][4]));
}


/**************************************************************
*
*       SUMMARY:  BL - Back Left  BR - Back Right
*                 FL - Front Left  FR - Front Right
*
*                         t  h  e  t  a
*
*                      | 0 - 90 |  90 - 180  180 - 270  270 - 360
*          -------------------------------------------------------
*       a   0 - 90     |  BL    |    FL     |    FR   |    BR
*          -------------------------------------------------------
*       l   90 - 180   |  BL    |    FL     |    FR   |    BR
*          -------------------------------------------------------
*       p   180 - 270  |  FL    |    FR     |    BR   |    BL
*          -------------------------------------------------------
*       h   270 - 360  |  FL    |    FR     |    BR   |    BL
*          -------------------------------------------------------
*       a
*
******************************************************************/

#define BACKLEFT        1
#define BACKRIGHT       2
#define FRONTLEFT       3
#define FRONTRIGHT      4

static int find_corner(alpha,theta)
	float alpha,theta;
{
        if ((theta >= 0) && (theta <= 90)) {
             if ((alpha >= 0) && (alpha <= 180))
                 return (BACKLEFT);
             else return (FRONTLEFT);
        }
        else if ((theta >= 90) && (theta <= 180)) {
             if ((alpha >= 0) && (alpha <= 180))
                 return (FRONTLEFT);
             else return (FRONTRIGHT);
        }
        else if ((theta >= 180) && (theta <= 270)) {
             if ((alpha >= 0) && (alpha <= 180))
                 return (FRONTRIGHT);
             else return (BACKRIGHT);
        }
        else if ((theta >= 270) && (theta <= 360)) {
             if ((alpha >= 0) && (alpha <= 180))
                 return (BACKRIGHT);
            else return (BACKLEFT);
        }
}
void TksX3DMesh(w,gcf,gcb,data,sizy,sizx,
		zmin,zmax,rotx,rotz,eyed,viewd,perspective)
	Widget w;
	GC gcf,gcb;
	float	**data;
	int	sizy,sizx;
	float	zmin,zmax;	/* world coord. Z */
	float	rotx,rotz,eyed,viewd;
	Boolean perspective;
{
	int x,y;
	float m[4][4], mt[4][4];
	float scale_x,scale_y,scale_z;
	float xd,yd,zd,sd;
	XPoint **points;
	XPoint polyg[5];
	int corner;

	if(perspective == False){
		eyed = 0.0; 
		viewd = 0.0;
	}
	points = (XPoint **) XtMalloc(sizy*sizeof(XPoint*));
	for(y=0; y<sizy; y++)
		points[y] = (XPoint *) XtMalloc(sizx*sizeof(XPoint));
	/* mettre les coord. periph. normalisees  dans m */
	/* la figure tient dans un cube de 1x1x1 */
	/* dont les coord.min du cube sont l'origine du repere */
	/* cette origine est situe au centre du viewport */

	scale_x = 0.0;
	scale_y = 0.0;
	scale_z = 0.0;
	if(sizx > 1)scale_x = 1.0 / (sizx -1.0); /* 1/(wc_maxX - wc_minX)*/
	if(sizy > 1)scale_y = 1.0 / (sizy -1.0); /* 1/(wc_maxY - wc_minY)*/
	if(zmin != zmax)scale_z = 1.0 / (zmax -zmin);/* 1/(wc_maxZ - wc_minZ)*/
	MatrixScale(scale_x,scale_y,scale_z,m);

	/* recadrage de la figure  pour que le 0,0,0 soit au bord*/
	/*m[3][0] = 0 - xmin*scale_x;	*/
	/*m[3][1] = 0 - ymin*scale_y;	*/
	m[3][2] =  - (zmin*scale_z);	/* m=(translate au bord)*scale */

	/* on se translate au centre de la figure */
	MatrixScale(1.0,1.0,1.0,mt);
	mt[3][0] = mt[3][1] = mt[3][2] = -0.5;
	MatrixMult(m,mt,m);	/* m = m*mt= m*(translate au centre) */

	/* rotation du repere et distance de l'oeil */
	rotz = DegToRads(rotz);
	rotx = DegToRads(rotx);
	MatrixScale(1.0,1.0,1.0,mt);
	mt[0][0] = cos(rotz);
        mt[0][1] = -(cos(rotx) * sin(rotz));
        mt[0][2] = -(sin(rotx) * cos(rotz));
        mt[1][0] =  sin(rotz);
        mt[1][1] =  cos(rotx) * cos(rotz);
        mt[1][2] = -(sin(rotx) * cos(rotz));
        mt[2][1] =  sin(rotx);
        mt[2][2] = -(cos(rotx));
        mt[3][2] =  eyed;
        mt[3][3] =  1.0;	/* mt = Rotation * (translate de l'oeil) */

	MatrixMult(m,mt,m);

	MatrixScale(1.0,1.0,1.0,mt);
	if (perspective == True)
        {
		mt[2][2] = 0.0; 
        	mt[2][3] = 1.0/viewd; /*viewd= distance de l'oeil au viewport*/
           	MatrixMult(m, mt, m);
        } else {
      	   /*
            *  Set the Parallel transformation
            */

        	mt[2][2] = 0.0;
           	MatrixMult(m, mt, m);
        }
/*
toutes les transformation sont dans un repere norme de telle sorte que
l'objet tienne dans un cude de 1x1x1
*/
        /*
         *  Set the translation to the origin
         */
	MatrixScale(1.0,1.0,1.0,mt);
	mt[3][0] = mt[3][1] = mt[3][2] = 0.5;
        MatrixMult(m, mt, m);

        /*
         *  Set the scale about the origin (view port)
         */
/*        xv = (xv_max - xv_min)/1.0;	*/
/*        yv = (yv_max - yv_min)/1.0;	*/
/*        zv = (yv_max - yv_min)/1.0;/*????????????*/
/*        MatrixScale(0.9, 0.9, 0.9, mt);
/*        MatrixMult(m, mt, m);

/*	MatrixScale(1.0,1.0,1.0,mt);
/*	mt[3][0] = mt[3][1] =  0.1;
/*       MatrixMult(m, mt, m);
       /*
         *  Set the device coordinate transformation matrix
         */
	MatrixScale(1.0,1.0,1.0,mt);
	mt[0][0] = (float)w->core.width-1  - 0;
	mt[1][1] = 0.0 - (float)w->core.height-1;
	mt[3][0] = 0;
	mt[3][1] = (float)w->core.height-1;
        MatrixMult(m, mt, m);

	/* trace avec un grillage (mesh) */

	/* conversion world coord. to device coord. */

	if (perspective == True){
		for(y=0; y<sizy; y++)
			for (x=0; x<sizx; x++){
			   xd=x*m[0][0]+y*m[1][0]+data[y][x]*m[2][0]+m[3][0];
			   yd=x*m[0][1]+y*m[1][1]+data[y][x]*m[2][1]+m[3][1];
/*			   zd=x*m[0][2]+y*m[1][2]+data[y][x]*m[2][2]+m[3][2]; */
			   sd=x*m[0][3]+y*m[1][3]+data[y][x]*m[2][3]+m[3][3];

			   if(sd != 0){
			   	points[y][x].x = xd/sd+0.5;
			   	points[y][x].y = yd/sd+0.5;
			   }else{
			   	points[y][x].x = xd+0.5;
			   	points[y][x].y = yd+0.5;
			   }
			}
	}else{
                for(y=0; y<sizy; y++)
                        for (x=0; x<sizx; x++){
                           xd=x*m[0][0]+y*m[1][0]+data[y][x]*m[2][0]+m[3][0];
                           yd=x*m[0][1]+y*m[1][1]+data[y][x]*m[2][1]+m[3][1];
                           points[y][x].x = xd+0.5;
                           points[y][x].y = yd+0.5;
                        }
	}

	/* fin de conversion. Dans Xpoints **points on a des coord X */

	/* on dessine maintenant */
	corner = find_corner(rotx,rotz);

	switch (corner){
	case BACKLEFT:
		for(x = 0; x < sizx -1; x++)
			for( y = sizy-1; y > 0; y--){
				polyg[0] =
				polyg[4] = points[y][x];
				polyg[1] = points[y][x+1];
				polyg[2] = points[y-1][x+1];
				polyg[3] = points[y-1][x];
				/* le dessine */
				XFillPolygon(XtDisplay(w),XtWindow(w),gcb,
					polyg, 4,Nonconvex, CoordModeOrigin);
				XDrawLines(XtDisplay(w),XtWindow(w),gcf,
					polyg, 5, CoordModeOrigin);
			}
		break;
	case BACKRIGHT:
		for( y = sizy-1; y > 0; y--)
			for(x = sizx-1; x > 0 ; x--){
				polyg[0] =
				polyg[4] = points[y][x];
				polyg[1] = points[y][x-1];
				polyg[2] = points[y-1][x-1];
				polyg[3] = points[y-1][x];
				/* le dessine */
				XFillPolygon(XtDisplay(w),XtWindow(w),gcb,
					polyg, 4,Nonconvex, CoordModeOrigin);
				XDrawLines(XtDisplay(w),XtWindow(w),gcf,
					polyg, 5, CoordModeOrigin);
			}
		break;
	case FRONTRIGHT:
		for(x=sizx-1; x>0; x--)
			for (y = 0; y < sizy -1; y++){
				polyg[0] =
				polyg[4] = points[y][x];
				polyg[1] = points[y][x-1];
				polyg[2] = points[y+1][x-1];
				polyg[3] = points[y+1][x];
				/* le dessine */
				XFillPolygon(XtDisplay(w),XtWindow(w),gcb,
					polyg, 4,Nonconvex, CoordModeOrigin);
				XDrawLines(XtDisplay(w),XtWindow(w),gcf,
					polyg, 5, CoordModeOrigin);
			}
		break;
	case FRONTLEFT:
		for(y=0; y<sizy-1; y++)
			for(x=0; x<sizx-1; x++){
				polyg[0] =
				polyg[4] = points[y][x];
				polyg[1] = points[y][x+1];
				polyg[2] = points[y+1][x+1];
				polyg[3] = points[y+1][x];
				/* le dessine */
				XFillPolygon(XtDisplay(w),XtWindow(w),gcb,
					polyg, 4,Nonconvex, CoordModeOrigin);
				XDrawLines(XtDisplay(w),XtWindow(w),gcf,
					polyg, 5, CoordModeOrigin);
			}
		break;
	} /*switch */
}

