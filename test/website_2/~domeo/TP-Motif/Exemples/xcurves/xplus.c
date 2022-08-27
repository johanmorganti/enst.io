
/* ********************************************************************* *
 *
 *  xplus.c
 *  
 *  Nicolas ROMANETTI , Janvier 1995.
 *  
 * ********************************************************************* */

  /* ******************************************************** */
  /* ATTENTION VOUS DEVEZ MODIFIER LE CHEMIN D'ACCES AUX AXES */
  /* dans la fonction AfficherAxes                            */
  /* ******************************************************** */

#define InitAxes "axes.init"

/*---------------------------------*/
/*            include              */
/*---------------------------------*/

#include <Xm/XmAll.h>
#include "xmutil.h"

/*---------------------------------*/
/*           definitions           */
/*---------------------------------*/

#define nbr_curves 5              /* permet l'affichage simultane de nbr_curves
				     courbes */

/* les fichiers .graph seront lus et stockes sous forme de listes chainees */
/* a chaque point d'une courbe sera donc associee une structure */
 
typedef 
 struct point { 
               double abs;              /* abscisse */
	       double ord;              /* ordonnee */
	       struct point *suivant;   /* adresse de la structure suivante */
	     } point;

/*---------------------------------*/
/*        variables externes       */
/*---------------------------------*/

extern Display *display;     /* Display de l'application */
extern Window  draw_window;  /* Fenetre du Drawing */
extern GC      draw_gc;	     /* Graphic Context du Drawing */
extern int ExternCompteur;   /* Numero de la prochaine courbe stockee
                                en memoire */



/*---------------------------------*/
/* variables globales et statiques */
/*---------------------------------*/

/* l'adresse de la premiere structure d'une liste chainee est stockee
   dans le tableau bat. Ainsi par exemple bat[2] est l'adresse du
   de la stucture associee au premier point de la courbe numero 2. 
   bat[nbr_curves] correspond aux axes */   
        
static point 
    *bat[nbr_curves+1]={NULL,NULL, /* par defaut rien n'est stocke */
			NULL,NULL,       
                        NULL,NULL};   


char CurveName[nbr_curves+1][100];  /* nom du fichier attache a la courbe */
                                                   
char *Color[nbr_curves+1]={"blue",  /* couleurs par defaut */
			   "red",     
			   "green",
			   "yellow",
			   "orange",
			   "white"}; /* couleur des axes = blanc */



int ecran[nbr_curves+1]={0,0,0,0,0,1};  /* si 1, la courbe est a l'ecran */
                                        /* si 0, la courbe est invisble  */
                                        /* axes toujours visibles */

int LAST=0;                     /* numero de la derniere courbe affichee */


int x_MainScale=1;             /* echelle generale */
int y_MainScale=1;

double                         /* echelle par defaut de chaque courbe */
 x_scale[nbr_curves+1]={1,1,1,1,1,1},
 y_scale[nbr_curves+1]={1,1,1,1,1,1};

double ZoomChoice=1;                   /* zoom par defaut */ 

int Delta_x=0;                         /* position relative du zoom */
int Delta_y=0;


/*=======================================================================*/
/*============================= [ FONCTIONS ] ===========================*/  
/*=======================================================================*/






/*-----------------------------------------------------------------------*/
/*============================== [ ReadGraph ] ==========================*/
/*-----------------------------------------------------------------------*/

/* 
   -passer en argument le pointeur sur le fichier graphique a lire
   -lit ce fichier graphique et stocke les coordonnees des points sous forme
    d'une liste chainee de structures de type point.
   -l'adresse du debut de la chaine est stockee dans bat[no courbe] 
   -le fichier graphique lu doit etre de la forme: x1 y1 x2 y2 x3 y3 ...
   -elle associe a la courbe lue un numero qu'elle renvoie 
*/ 



void NewScreen(void);
void SupprimerCurves(int);


int ReadGraph(char *NomFichier)
{
  static int i=0,courbe;     /* numeros associes a la courbe lue */
  FILE *pt_file;
  double x,y;
  int lu;                    /* correspond a un nombre de caracteres lus */
  point *clef;               /* sert d'intermediaire */
  char s[100];               /* pour le nom du fichier graphique */


  sprintf(s,"%s",NomFichier);
  pt_file=fopen(s,"r");          /* ouverture du fichier graphique */


 if(i==nbr_curves)   /* i varie en boucle entre 0 et 4 inclus */     
             
	{	     /* i est le numero qu'on associe a la courbe lue */
	  i=0;
	}
                     
  if( fscanf(pt_file,"%lf",&x) != EOF )  /* on s'assure de l'existence
					    de la 1ere coordonnee     */
    {
     
      fprintf(stderr,"on rafraichit la courbe %d \n",i);

      SupprimerCurves(i);       /* on vide la chaine occupee par la courbe i
				   pour y stocker la nouvelle courbe i */ 

      
      if(ecran[i])                      /* on efface l'ancienne courbe i
                                           si elle est visible */
	{
	  ecran[i]=0;
	  NewScreen();
	}
	
      fscanf(pt_file,"%lf",&y);	   /* lecture de l'ordonnee du 1er point */

      bat[i]=(point *)malloc(sizeof(point)); /* on reserve la memoire pour
                                                notre 1er point */
      bat[i]->suivant=NULL;
      bat[i]->abs=x;
      bat[i]->ord=y;
      
      if( fscanf(pt_file,"%lf",&x) != EOF ) /* si il y a un 2eme point */
	{
	  bat[i]->suivant=(point *)malloc(sizeof(point));
	  /* la 1ere stucture est construite */

	  clef=bat[i]->suivant;
	  fscanf(pt_file,"%lf",&y);
	  
	  do          
	    {
	      clef->abs=x;
	      clef->ord=y;
	      
	      if( (lu=fscanf(pt_file,"%lf",&x)) != EOF)
		{
		  fscanf(pt_file,"%lf",&y);
		  clef->suivant=(point *)malloc(sizeof(point));
		  clef=clef->suivant;
		  
		}
	      else
		{
		  clef->suivant=NULL;
		}
	    }
	  while(lu!=EOF);
	}
      courbe=i;
      i++;
    
    }
fclose(pt_file);    /* fermeture du fichier */
return(courbe);     /* renvoie le numero de la courbe */
}


/*-------------------------------------------------------------------------*/
/*============================== [ afficher ] =============================*/
/*-------------------------------------------------------------------------*/

/* 
  -affiche la courbe dont le numero est passe en argument
 */


void afficher(int n)       /* n<=>numero de courbe */
{
  double x1,x2,y1,y2;
  int cx=275;            /* centre du repere, calcule a partir     */
  int cy=225;		 /* des dimensions de la fenetre graphique */
  
  point *passe;          /* sert d'intermediaire */
 

  if((n<=nbr_curves) && (n>=0))
    {
      passe=bat[n];
      if(passe!=NULL)
	{
	  ecran[n]=1;
	  do
	    {
	      x1=passe->abs;
	      y1=passe->ord;
	      if(passe->suivant != NULL)
		{
		  x2=passe->suivant->abs;
		  y2=passe->suivant->ord;
		}
	      XDrawLine(display,
			draw_window,
			draw_gc,
		/* on fait un 'cast' pour convertir les 'double' en 'int' */
		/*  XDrawLine trace une doite entre (x1,y1) et (x2,y2) */

	    (int)(ZoomChoice*x_scale[n]*x1+cx-Delta_x*ZoomChoice),  /* x1 */
	    (int)(cy+Delta_y*ZoomChoice-ZoomChoice*y_scale[n]*y1),  /* y1 */
	    (int)(ZoomChoice*x_scale[n]*x2+cx-Delta_x*ZoomChoice),  /* x2 */
	    (int)(cy+Delta_y*ZoomChoice-y_scale[n]*ZoomChoice*y2)); /* y2 */

	      passe=passe->suivant;
	    }
	  while(passe!=NULL);
	}
    }

}

/*-----------------------------------------------------------------------*/
/*=========================== [ SupprimerCurves ]  ======================*/
/*-----------------------------------------------------------------------*/
/* 
   -vide la chaine occupee par la courbe dont le numero est passe en argument
   -n'efface pas la courbe de l'ecran
   -re-initialise l'echelle
*/


void SupprimerCurves(int n)
{
  int i;                /* i = numero de la courbe a supprimer */
  point *passe,*clef;
  
  i=n;

  x_scale[n]=1;	   /* initialise l'echelle */
  y_scale[n]=1;

      if(bat[i]!=NULL) /* si il y a une chaine <=> une courbe stockee */
	{
	  passe=bat[i]->suivant;
	  free(bat[i]);
	  bat[i]=NULL;
	  do
	    {
	      clef=passe;
	      passe=passe->suivant;
	      free(clef);
	    }
	  while(passe!=NULL);

	}
  
}
/*------------------------------------------------------------------------*/
/*============================== [ ToutVider ] ===========================*/
/*------------------------------------------------------------------------*/

/*
   -libere toutes les chaines sauf l'axe
   -rafraichit l'ecran
   -re-initialise le zoom ainsi que l'echelle
   -ne re-initialise pas les couleurs
*/ 

void ToutVider(void)


{
  int i;
  char *vide;

  vide=NULL;

  
  for(i=0;i<=nbr_curves-1;i++)  /* re-initialisations */
    {
      ecran[i]=0;
      sprintf(CurveName[i], "%s",vide);
    }

  ZoomChoice=1;
  
  for(i=0;i<=nbr_curves-1;i++)
    {
      SupprimerCurves(i);
    }
  Message("Auncune courbe en memoire");

  NewScreen();  
}


/*-----------------------------------------------------------------------*/
/*============================= [ AffiMem ] =============================*/
/*-----------------------------------------------------------------------*/

/*
  -affiche une courbe memorisee
*/
 
void AfficherAxes(void);
void SupprimerAxes(void);

void AffiMem(Widget dialog, XtPointer drawing, XtPointer call_data)
{
  char s[100], *number = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);

  if (number)
    {
      int i,n;
      if (((i= sscanf(number, "%d", &n)) == 1) && (n<nbr_curves) && (n>=0))
	  {
	    sprintf(s, "Courbe %d",n);
	    ChangeColor(display, draw_gc, Color[n]);
	    LAST=n;	   
	    afficher(n);
	  
	    ChangeColor(display, draw_gc, Color[nbr_curves]);
	    AfficherAxes();
	    
	    Message(s);
	    XtUnmanageChild(dialog);		/* enlever le dialog */
	  }
    else Message("Entrez un numero de courbe");
	}
}

/*------------------------------------------------------------------------*/
/*========================== [ AfficherAxes ] ============================*/
/*------------------------------------------------------------------------*/

/*
   -lit le fichier axe.c et le stocke au premier appel de la fonction
   -affiche les axes a chaque appel
*/

void AfficherAxes(void)
{
  static FILE *pt_file;
  char s[200];
  double x,y;
  int lu,courbe;
  point *clef;
  static int i=0;

if(!i)
  {
    pt_file=fopen(InitAxes,"r");
    if (! pt_file) {
      fprintf(stderr, "fichier d'initialisation des axes pas trouve: '%s'\n",
	      InitAxes);
      return;
    }
    
    if( fscanf(pt_file,"%lf",&x) != EOF )
      {	
	fscanf(pt_file,"%lf",&y);
	bat[nbr_curves]=(point *)malloc(sizeof(point));
	bat[nbr_curves]->suivant=NULL;
	bat[nbr_curves]->abs=x;
	bat[nbr_curves]->ord=y;
	
	if( fscanf(pt_file,"%lf",&x) != EOF )
	  {
	    bat[nbr_curves]->suivant=(point *)malloc(sizeof(point));
	    clef=bat[nbr_curves]->suivant; 
	    fscanf(pt_file,"%lf",&y);
	    
	    do
	      {
		clef->abs=x;
		clef->ord=y;
		
		if( (lu=fscanf(pt_file,"%lf",&x)) != EOF)
		  {
		    fscanf(pt_file,"%lf",&y);
		    clef->suivant=(point *)malloc(sizeof(point));
		    clef=clef->suivant;
		    
		  }
		else
		  {
		    clef->suivant=NULL;
		  }
	      }
	    while(lu!=EOF);
	  }	
      }
    fclose(pt_file);

    fprintf(stderr,"on passe dans le if...\n");
  
  }
  i++;				/* ainsi des le 2eme appel la fonction
				   se contente uniquement d'afficher
				   les axes */

  fprintf(stderr,"on va afficher les axes\n");
  afficher(nbr_curves);




}

/*------------------------------------------------------------------------*/
/*============================= [ SelectScale ] ==========================*/
/*------------------------------------------------------------------------*/

/* 
  -permet via le dialogue scale_dialog de modifier l'echelle d'une courbe
   et d'en voir les effets
*/


void NewScreen(void);

void SelectScale(Widget dialog, XtPointer drawing, XtPointer call_data)
{
  char s[100], *scale_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);
  int i,no;

  if (scale_string)
    {
      int n;
      double a,o;
      if (((n = sscanf(scale_string, "%d %lf %lf",&no, &a, &o)) == 3)
	    && 
	   (no<nbr_curves) && (no>=0))
	  {
	    sprintf(s, "Courbe %d :Echelle *%lf *%lf ",no,a, o);
	 
	    x_scale[no]=a;
	    y_scale[no]=o;
	  
	    Message(s);
	    NewScreen();
	    XtUnmanageChild(dialog);		/* enlever le dialog */
	  }
    else Message("No_courbe Echelle_x Echelle_y");
	}
}

/*-----------------------------------------------------------------------*/
/*============================ [ SelectZoom  ] ==========================*/
/*-----------------------------------------------------------------------*/

/*  
   -permet via le dialogue scale_zoom de modifier le zoom
    et d'en voir les effets
*/
  
void SelectZoom(Widget dialog, XtPointer drawing, XtPointer call_data)
{
  char s[100], *zoom_string = 
    StringGet(((XmSelectionBoxCallbackStruct*)call_data)->value);
 /* int i=0;*/

  if (zoom_string)
    {
      int n;
      double z;
      if ((n = sscanf(zoom_string, "%lf %d %d",&z,&Delta_x, &Delta_y))== 3)
	  {
	    sprintf(s,
		    "Zoom *%lf centre autour de (%d,%d)",z,Delta_x,Delta_y);
	 
	    ZoomChoice=z;
	    Message(s);
	    XtUnmanageChild(dialog);		/* enlever le dialog */
	    NewScreen();
	  }
    else
      {
	Message("Attention: Saisir Zoom et coordonnees");
      }

    }



}


/*-----------------------------------------------------------------------*/
/*============================== [ Bilan ] ==============================*/
/*-----------------------------------------------------------------------*/

/*
  -ecrit sur la sortie erreur stderr, serait preferable
   d'ecrire ailleurs
*/

void Bilan(void)
{
  int j;
  
  j=0;

  Message("Bilan disponible sur Stderr");
  fprintf(stderr,
	  "*======================= [ BILAN ] =======================*\n");
  fprintf(stderr,"\n");
  fprintf(stderr," # Vous etes en Zoom *%lf\n",ZoomChoice);
  fprintf(stderr," # Derniere courbe affichee = %d\n",LAST);

  fprintf(stderr,"\n");
  while(j<=nbr_curves-1)
    {
      fprintf(stderr,"\n");
      fprintf(stderr,"-> COURBE numero %d\n" ,j);
      fprintf(stderr,"     * Fichier = %s\n", CurveName[j]);
      fprintf(stderr,"     * Echelle = %lf*%lf\n",x_scale[j],y_scale[j]);
      fprintf(stderr,"     * Couleur = %s\n",Color[j]);
      if(ecran[j])
	{
	  fprintf(stderr,"     * Visible \n");
	}
      else
	{
	  fprintf(stderr,"     * Non Visible \n");
	}


      j++;
    }
  fprintf(stderr,
	  "*=========================================================*\n");
}



/*-----------------------------------------------------------------------*/
/*=========================== [ NewScreen ] =============================*/
/*-----------------------------------------------------------------------*/

/*
  -efface l'ecran sans modifier les valeurs de ecran[numero de courbe] 
  -re affiche les courbes qui etaient visibles en prenant en compte 
   les nouveux parametres.
*/

void NewScreen(void)
{
  int j;
  
  XClearWindow(display, draw_window);
  for(j=0;j<=nbr_curves-1;j++)
    {
      if(ecran[j])
	{
	  ChangeColor(display, draw_gc, Color[j]);
	  
	  afficher(j);
	}
    }
  ChangeColor(display, draw_gc, Color[nbr_curves]);
  AfficherAxes();
}
/*======================================================================*/



