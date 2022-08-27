/************************************************************************

                              XRESSOURCES

							  Projet BIG

               Herve Poupon             Thomas Rohmer
			   poupon@email.enstfr      rohmer@email.enst.fr

************************************************************************/

#include <stdio.h>
#include <string.h>
#include <Xm/XmAll.h>

/* constantes modifiables */

#define MAX_LIGNES 20
#define MAX_OPTIONS 20
#define nb_lignes_xdefaults 500

#define _QUOTE(x) #x
#define QUOTE(X) _QUOTE(X)

char* nom_xdef    = QUOTE(FICHIER_XDEFAULTS);
char* fichier_rgb = QUOTE(FICHIER_RGB);

/* typedef */

typedef enum {texte, couleur, police, enumere} type_ressource;

typedef struct {
 char* nom;
 char* ressource;
 type_ressource type;
 char** liste_choix;
} une_ligne;

typedef struct {
 char* nom;
 int nb_lignes;
 une_ligne* lignes;
} une_page;


/****** V A R I A B L E S   G L O B A L E S ****************************/


char*  fichier_xdefaults[nb_lignes_xdefaults];

Widget warning_dialog;
Widget select_fichier;

Widget toplevel, row_col, option_menu, page_rowcol, form_pb, pb1, pb2;
Widget w[MAX_LIGNES], form[MAX_LIGNES], label[MAX_LIGNES], textf[MAX_LIGNES];
XtAppContext app;
int page_cour=-1;
Boolean sauvegarde_pas_faite=1;


/* ressources par defaut de l'application */

String fallbacks[] = {
  "*menu_choix*labelString: Page courante:",
  "*foreground : black",
  "*background : grey",
  NULL};


/* Definition des pages de ressources */ 

char* Booleen[3]={"True", "False", NULL};
char* booleen[3]={"true", "false", NULL};
char* ouinon[3]={"yes", "no", NULL};
char* E1[3]={"POINTER", "EXPLICIT", NULL};

une_ligne gestionnaire_fenetres_lignes[4]=
  {
	{"Placement interactif", "Mwm*interactivePlacement", enumere, Booleen},
    {"Activation des fenetres", "Mwm*keyboardFocusPolicy", enumere, E1},
    {"Fenetre active au 1er plan", "Mwm*focusAutoRaise", enumere, Booleen},
    {"Deplace la fenetre avec son contenu", "Mwm*moveOpaque",enumere,Booleen},
  };
 
une_page gestionnaire_fenetres=
{ "Gestionnaire de fenetres", 4, gestionnaire_fenetres_lignes };

une_ligne attributs_fenetres_lignes[6]=
  {
    {"Largeur des bordures (automatique par defaut)", "Mwm*resizeBorderWidth", texte, NULL},
	{"Police des menus et titres", "Mwm*fontList", police, NULL},
    {"Fond","Mwm*background",couleur, NULL},
	{"Bord","Mwm*foreground",couleur, NULL},
	{"Fond actif","Mwm*activeBackground",couleur, NULL},
	{"Bord actif","Mwm*activeForeground",couleur, NULL}
  };

une_page attributs_fenetres=
{ "Attributs des fenetres", 6, attributs_fenetres_lignes };

char* E2[5]={"left top", "right top", "left bottom", "right bottom", NULL};
char* E3[7]={"activelabel image", "image", "activelabel label image", "activelabel label", "label image", "label", NULL};

une_ligne icones_lignes[6]=
  {
	{"Placement automatique", "Mwm*iconAutoPlace", enumere, Booleen},
    {"Position (placement auto)", "Mwm*iconPlacement", enumere, E2},
	{"Boite a icones", "Mwm*useIconBox", enumere, Booleen},
    {"Affichage des icones", "Mwm*iconDecoration", enumere, E3},
    {"Couleur de l'image (fond)", "Mwm*iconImageBackground", couleur, NULL},
    {"Couleur de l'image (dessin)", "Mwm*iconImageForeground", couleur, NULL}
  };

une_page icones=
{ "Icones", 6, icones_lignes };

une_ligne xterm_lignes[7]=
  {
	{"Icone", "Mwm*xterm*iconImage", texte, NULL},
	{"Barre de defilement", "xterm*scrollBar", enumere, Booleen},
	{"Lignes sauvegardees", "xterm*saveLines", texte, NULL},
	{"Couleur du curseur", "xterm*cursorColor", couleur, NULL},
	{"Couleur du fond", "xterm*background", couleur, NULL},
	{"Couleur du texte", "xterm*foreground", couleur, NULL},
	{"Position et dimensions", "xterm*geometry", texte, NULL},
  };

une_page xterm=
{ "XTerm", 7, xterm_lignes };

une_ligne emacs_lignes[7]=
  {
	{"Icone", "Mwm*emacs*iconImage", texte, NULL},
	{"Police de caracteres", "emacs*font", police, NULL},
	{"Couleur du curseur", "emacs*cursorColor", couleur, NULL},
	{"Couleur du pointeur", "emacs*pointerColor", couleur, NULL},
	{"Couleur du fond", "emacs*background", couleur, NULL},
	{"Couleur du texte", "emacs*foreground", couleur, NULL},
	{"Position et dimensions", "emacs.geometry", texte, NULL},
  };

une_page emacs=
{ "Emacs", 7, emacs_lignes };

int nombre_pages=5;
une_page* tableau_de_pages[5]=
{&gestionnaire_fenetres, &attributs_fenetres, &icones, &xterm, &emacs};

char* selection_fonte(char* resultat)
{
 FILE *f;
 f=popen("xfontsel -print","r");
 fgets(resultat,79,f);
 fclose(f);
 return(resultat);
} 

void couleur_textf(Widget textf);
void police_pushbutton(Widget bouton, une_ligne *ligne);

/* pour interrompre le programme en cas d'erreur 
   (avec fermeture des fichier ouverts)                */
void erreur(char* message)
{
 printf("\nERREUR FATALE: %s\n", message);
 exit(1);
}

/* allocation d'un bloc de memoire avec test du pointeur retourne */
void*
alloc(int taille)
{
 void* pt;
 pt=(void*)malloc(taille);
 if (pt==NULL)
    erreur("allocation de variables dynamiques impossible (memoire insuffisante?)");
 return pt;
}

/* modifie une ligne du fichier .Xdefaults charge en memoire */
void
def_ligne_xdefaults(int num, char* texte)
{
 int i;
 char* s;
 if (fichier_xdefaults[num]!=NULL) free(fichier_xdefaults[num]);
 for (i=0; texte[i]!=0; i++);
 fichier_xdefaults[num]=(char*)alloc(i+1);
 i=0;
 s=fichier_xdefaults[num];
 do
   s[i]=texte[i];
 while (s[i++]!=0);
}

/* cherche la valeur d'une ressource dans le fichier .Xdefaults charge 
   en memoire */
char*
cherche_ressource(char* ressource, int* num)
{
 int i,j;
 char* s;


 for (i=0; fichier_xdefaults[i]!=NULL; i++)
   {
    s=fichier_xdefaults[i];
    while (*s==' ') s++;
	if (*s!='!')
	  {
		for (j=0; ((ressource[j]==s[j])&&(ressource[j]!=0)); j++);
		if (ressource[j]==0) 
			 {
			   while (s[j]==' ') j++;
			   if (s[j++]==':')
				 {
				   while (s[j]==' ') j++;
				   *num=i;
				   return s+j;
				 }
			 }
	  }
   }
 return NULL;
}

/* definit une ressource dans le .Xdefaults a partir du nom de la ressource
   et de sa valeur 
   si la ressouce n'est pas deja dans le .Xdefaults, une ligne est ajoutee */
void 
def_ressource(char* ressource, char* valeur)
{
 int n,i;
 char s[512];

 if (cherche_ressource(ressource, &n)==0)
   for (n=0; fichier_xdefaults[n]!=0; n++);
 for (i=0; ((s[i]=ressource[i])!=0); i++);
 s[i++]=':';
 s[i++]=' ';
 for (; ((s[i]=(*valeur++))!=0); i++);
 def_ligne_xdefaults(n, s);
}

/* charge en memoire le fichier .Xdefaults. Le nombre de lignes du fichier
   est limite par nb_lignes_xdefaults */
void
charge(void)
{
 FILE* f; 
 char s[512];
 int i,j,fini=0;

 for (i=0; i<nb_lignes_xdefaults; i++) fichier_xdefaults[i]=NULL;

 f=fopen(nom_xdef,"r");
 if (f==NULL) 
   {
	 sprintf(s, "ouverture de '%s' impossible", nom_xdef);
	 erreur(s);
   }
 i=0;
 while ((!feof(f))&&(i<nb_lignes_xdefaults))
   {
	 fgets(s, 510, f);
	 if (!feof(f))
	   {
		 for (j=0; (s[j]!=10); j++);
		 s[j]=0;

		 {
		   def_ligne_xdefaults(i,s);
		   i++;
		 }
	   }
   }
 fclose(f);
}

/* Sauvegarde le fichier .Xdefaults modifie en memoire et conserve le
   fichier precedent sous le nom .Xdefaults~.
   Ce fichier est ensuite pris en compte a l'aide de la commande 
   xrdb -load .Xdefaults */
void
sauve(void)
{
 char s[512] = "";
 int n;
 FILE* f;

 if (sauvegarde_pas_faite)
   {
	 sprintf(s,"cp %s %s~",nom_xdef,nom_xdef);
	 system(s);
	 sauvegarde_pas_faite=0;
   }

 f=fopen(nom_xdef,"w");
 for (n=0; fichier_xdefaults[n]!=NULL; n++)
   fprintf(f, "%s\n", fichier_xdefaults[n]);
 fclose(f);

 sprintf(s, "xrdb -load %s", nom_xdef);
 system(s);
}

/* verifie qu'un nom de couleur saisi au clavier se trouve bien dans le
   fichier /usr/lib/X11/rgb.txt */
int
verifie_couleur(char* nom_couleur)
{
  FILE* f;
  int r,g,b;
  char s[80], s1[80];

  if (! (f=fopen(fichier_rgb,"r"))) return -1;

  while (!feof(f))
    {
      fgets(s, 88, f);
      sscanf(s, "%d %d %d %s",&r, &g, &b, s1);
      if (!feof(f))
	{
	  if (strcmp(s1,nom_couleur)==0)
	    {
	      fclose(f);
	      return 1;
	    }
	}
    }
  
  fclose(f);
  return 0;			/* couleur pas trouvee */
}

/* definition d'un WarningDialog utilise lors de la verification des 
   couleurs */
void
initialise(void)
{
  Arg args[10];
  XmString message,nom_boite,titre;
 
  nom_boite=XmStringCreateSimple("WarningDialog"); 
  warning_dialog=XmCreateWarningDialog(row_col, nom_boite, args,0);
  XtDestroyWidget(XmMessageBoxGetChild(warning_dialog, XmDIALOG_CANCEL_BUTTON)); 
  
  XtDestroyWidget(XmMessageBoxGetChild(warning_dialog, XmDIALOG_HELP_BUTTON));
  
}

/* fait apparaitre le WarningDialog */
void 
manage_warning(char* titre, char* message)
{
  XmString msg, tit;
  tit = XmStringCreateSimple(titre);
  msg = XmStringCreateLtoR(message, XmFONTLIST_DEFAULT_TAG);
  XtVaSetValues(warning_dialog,
				XmNdialogTitle,   tit,
				XmNmessageString, msg,
				NULL);
  XtManageChild(warning_dialog);
} 

void
pb1_callback(Widget w, XtPointer client, XtPointer call )
{
  char s[256];
  sprintf(s,"Le fichier '%s' viens d'etre modifie\n\nRelancez Motif ou vos applications pour que ces modifications soient effectives\n",nom_xdef);
  manage_warning("Attention",s);

  sauve();
}
/* callback pour la saisie dans un textfield (nom de couleur ou simple texte)*/
void saisie_textf(Widget text_w, une_ligne* la_ligne)
{
  char* valeur = XmTextFieldGetString(text_w);
  
  if (la_ligne->type == couleur) {
    switch (verifie_couleur(valeur)) {
    case -1:
      manage_warning("Attention","Fichier rgb.txt pas trouve");
      break;
    case 0:
      manage_warning("Attention","Cette couleur n'est pas definie");
      break;
    default:
      couleur_textf(text_w);
      break;
    }
  }
  def_ressource(la_ligne->ressource, valeur);
  XtFree(valeur);
}

/* callback pour la selection de la valeur d'une ressource dans un
   OptionMenu */
void saisie_option(Widget option_w, int num_item)
{
  une_ligne* la_ligne;

  option_w=XtParent(option_w);
  option_w=XtParent(option_w);
  option_w=XtParent(option_w);
  XtVaGetValues(option_w, XmNuserData, &la_ligne, NULL);
  def_ressource(la_ligne->ressource, la_ligne->liste_choix[num_item]);
}

/* callback pour le PushButton "Selectionnez une police" */
void saisie_police(Widget bouton, une_ligne* la_ligne)
{
  char valeur[256];
  
  def_ressource(la_ligne->ressource, selection_fonte(valeur));
  police_pushbutton(bouton, la_ligne);
}


/* c'est fini */
void Sortie()
{
  exit(0);
}

/* callback: creation de la page desiree */
void
cree_page(int item)
{

  /* ressources des labels  */
  Arg l_args[] = {
	{XmNalignment, XmALIGNMENT_BEGINNING},
    {XmNtopAttachment,    XmATTACH_FORM},
    {XmNbottomAttachment, XmATTACH_FORM},
    {XmNleftAttachment,   XmATTACH_FORM},
	{XmNrightAttachment,  XmATTACH_POSITION},
    {XmNrightPosition,    (XtArgVal) 1}
  };
  /* ressources des textfields  */
  Arg t_args[] = {
    {XmNtopAttachment,    XmATTACH_FORM},
    {XmNbottomAttachment, XmATTACH_FORM},
	{XmNrightAttachment,   XmATTACH_FORM},
	{XmNleftAttachment,   XmATTACH_POSITION},
    {XmNleftPosition,     (XtArgVal) 1}
  };

  char name[50];
  int k,l,i,n, rien, item_defaut;
  XmString str[MAX_OPTIONS], vide;
  char *tab[MAX_OPTIONS];
  char** options_possibles;
  char* option_defaut;
  Widget le_plus_long;
  int taille_du_plus_long=0;
  XtWidgetGeometry geom;

  vide = XmStringCreateSimple("");

	
  /*==  creation des widgets de saisie et d'affichage de la page ==*/
  for (l = 0; l < tableau_de_pages[item]->nb_lignes ; l++) 
  {
	/* forms contenant les textfield et les label */
    sprintf(name, "form%d", l);
	form[l] = XtVaCreateWidget
	  (name, xmFormWidgetClass, page_rowcol, 
	   XmNfractionBase,     (XtArgVal) 2, /* form "scinde" en deux */
	   NULL);
    n = 0;
	/* label */
    sprintf(name, "%s", tableau_de_pages[item]->lignes[l].nom);
    w[n] = XmCreateLabel(form[l], name, l_args, XtNumber(l_args));
        /* cherche le label le plus long */
	if (strlen(name)>taille_du_plus_long) 
	  {
		taille_du_plus_long=strlen(name);
		le_plus_long=w[n];
	  }
	n++;
	
	/* creation d'un optionMenu, d'un TextField ou d'un PushButton selon */
	/* le type de la ressource                                           */
	switch (tableau_de_pages[item]->lignes[l].type) {
	  
	case enumere: {  /* option menu */
	  
	  /* cree un tableau de valeurs possibles a afficher */
	  options_possibles=tableau_de_pages[item]->lignes[l].liste_choix;
	  
	  /* cherche la valeur deja attribuee dans le .Xdefaults de */
	  /* l'utilisateur et la prend pour l'afficher par defaut   */
	  option_defaut=cherche_ressource(tableau_de_pages[item]->lignes[l].ressource, &rien);
	  
	  i=0;
	  item_defaut=0;
	  while ((*options_possibles)!=NULL)
		{
		  tab[i]=XmVaPUSHBUTTON;
		  str[i]= XmStringCreateSimple(*options_possibles);
		  /* cherche le numero du pushbutton correspondant a l'option */
		  /* par defaut                                               */
		  if (strcmp(*options_possibles,option_defaut)==0) item_defaut=i;
		  
		  i++;
		  options_possibles++;
		}
	  tab[i]=NULL;
	  
	  k=0;
	  sprintf(name, "option_menu%d", l);
	  w[n] = XmVaCreateSimpleOptionMenu
		(
		 form[l], name, NULL, NULL,
		 item_defaut, saisie_option,
		 XmNleftAttachment,   XmATTACH_POSITION,
		 XmNleftPosition,     (XtArgVal) 1,
		 XmNlabelString, vide,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 tab[k],str[k++],NULL,NULL,NULL,
		 NULL);
	  XtVaSetValues(XtParent(w[n]), XmNuserData, (tableau_de_pages[item]->lignes)+l, NULL);
	  n++;
	  break;
	}
	case police: {
	  sprintf(name, "pushbutton%d", l);
	  str[0]=XmStringCreateSimple("Selectionnez une police");
	  w[n] = XtVaCreateManagedWidget
		(name,
		 xmPushButtonWidgetClass, form[l],
		 XmNleftAttachment,   XmATTACH_POSITION,
		 XmNleftPosition,     (XtArgVal) 1,
		 XmNlabelString, str[0], 
		 NULL);
	  police_pushbutton(w[n], (tableau_de_pages[item]->lignes)+l);
	  XtAddCallback(w[n], XmNactivateCallback, saisie_police, 
					(tableau_de_pages[item]->lignes)+l); 

	  n++;
	  break;}
	  
	default:  {      /* textfield */
	  sprintf(name, "text%d", l);
	  w[n] = XmCreateTextField(form[l], name, t_args, XtNumber(t_args));
	  XmTextFieldSetString
		(w[n], 
		 cherche_ressource(tableau_de_pages[item]->lignes[l].ressource,
						   &rien)
		 );
	  if (tableau_de_pages[item]->lignes[l].type==couleur) couleur_textf(w[n]);
	  
	  XtAddCallback(w[n], XmNactivateCallback, saisie_textf, 
					(tableau_de_pages[item]->lignes)+l);
	  n++;
	}
	}
	  XtManageChildren(w, n);
	}
	
  geom.request_mode=CWWidth;
  XtQueryGeometry(le_plus_long, NULL, &geom);
  XtVaSetValues(toplevel, XmNwidth, 2*geom.width, NULL);
  XtFree(vide);
  XtManageChildren(form, tableau_de_pages[item]->nb_lignes);
  XtManageChild(page_rowcol);

  page_cour=item;
} 

/* callback associe a tous les boutons de option_menu */
void page_cb(Widget w, int item_no, XtPointer call_data)
{
  int n;
  
  if (item_no==page_cour) 
  {return;}  /* evite de recreer la meme page */
  if (page_cour!=-1) 
	{
	  XtUnmanageChildren(form,tableau_de_pages[page_cour]->nb_lignes);
	  for (n=0;n<tableau_de_pages[page_cour]->nb_lignes ; n++) 
		{
		  XtDestroyWidget(form[n]);
		}
	}
  
  page_cour = item_no;
  
  if (item_no == nombre_pages) /* le bouton "Annuler et Quitter"  */
	{ Sortie();}
  if (item_no == nombre_pages+1) /* le bouton "Enregistrer et Quitter"  */
	{
	  sauve();
	  Sortie();
	}
  else 
	{
	  cree_page(item_no);   /* creation de la page correspondante */
	}
}

/* fonction modifiant les couleurs d'un textfield */
void
couleur_textf(Widget textf)
{
  char *color_name;
  XColor xcolor, unused;
  Colormap cmap = DefaultColormapOfScreen(XtScreen(textf));

  color_name = XmTextFieldGetString(textf);
  if (XAllocNamedColor(XtDisplay(textf), cmap, color_name, &xcolor, &unused))
    XtVaSetValues(textf, XmNbackground, xcolor.pixel, NULL);
  if (strcmp(color_name,"black")==0)
	{
	  if (XAllocNamedColor(XtDisplay(textf), cmap, "white", &xcolor, &unused))
		XtVaSetValues(textf, XmNforeground, xcolor.pixel, NULL);
	}
  else
	{
	  if (XAllocNamedColor(XtDisplay(textf), cmap, "black", &xcolor, &unused))
		XtVaSetValues(textf, XmNforeground, xcolor.pixel, NULL);
	}
}

/* fonction modifiant la police de caracteres d'un pushbutton */
void
police_pushbutton(Widget bouton, une_ligne *ligne)
{
  int n;
  char* police;

  police=cherche_ressource(ligne->ressource, &n);
  XtVaSetValues(bouton, XtVaTypedArg, XmNfontList, XmRString,
				police, strlen(police)+1,
				NULL);
}



void
main(int argc, char **argv)
{
  int k; 
  char name[100];
  XmString str[nombre_pages];
  Arg args[10];

  /* ************************************************ Initialisation *** */
  XtSetArg(args[0], XmNallowShellResize, True);
  toplevel = XtAppInitialize(&app, "XEnv", NULL, 0, &argc, argv,
			     fallbacks, args, 1);

  /* ******************* Parametre en ligne de commande ************/

  if (argc > 1)  nom_xdef    = argv[1];
  if (argc > 2)  fichier_rgb = argv[2];

  row_col = XmCreateRowColumn( toplevel, "rc", NULL, 0);
  XtManageChild(row_col);

  
 /* creation des choix du option_menu de choix des pages */  
  for (k=0;k< nombre_pages;k++)
  {
	sprintf(name,"%s", tableau_de_pages[k]->nom);
	str[k]= XmStringCreateSimple(name);
  }
  k=0;

  option_menu = 
	XmVaCreateSimpleOptionMenu
	  (row_col, "menu_choix", NULL, NULL,
	   0, page_cb,
	   XmVaPUSHBUTTON, str[k], (tableau_de_pages[k++]->nom)[0], NULL, NULL,
	   XmVaPUSHBUTTON, str[k], (tableau_de_pages[k++]->nom)[0], NULL, NULL,
	   XmVaPUSHBUTTON, str[k], (tableau_de_pages[k++]->nom)[0], NULL, NULL,
	   XmVaPUSHBUTTON, str[k], (tableau_de_pages[k++]->nom)[0], NULL, NULL,
	   XmVaPUSHBUTTON, str[k], (tableau_de_pages[k++]->nom)[0], NULL, NULL,
	   NULL);
  for (k=0;k< nombre_pages;k++){XmStringFree(str[k]);}

  XtManageChild(option_menu);
  
  /* cree et manage un separator fils du row_col */
  XtVaCreateManagedWidget("separator_haut",
	xmSeparatorGadgetClass,row_col, NULL);
  
  /* creation du rowcolumn  pour les pages */
  page_rowcol = XmCreateRowColumn( row_col, "page_rowcol", NULL, 0);
  
  /* cree et manage un separator fils du row_col */
  XtVaCreateManagedWidget("separator_bas",
	xmSeparatorGadgetClass,row_col, NULL);
 
  /* cree et manage un form fils du row_col */
  form_pb = XtVaCreateManagedWidget
	  ("form_pb", xmFormWidgetClass, row_col, 
	   XmNfractionBase,     (XtArgVal) 2, /* form "scinde" en deux */
	   NULL);
	  
  /* creation de deux PushButton fils du row_col*/
  str[0]= XmStringCreateSimple("Appliquer");
  pb1 = XtVaCreateManagedWidget("push_button_appliquer",
							   xmPushButtonWidgetClass,
							   form_pb,
							   XmNlabelString, str[0],
							   XmNtopAttachment,    XmATTACH_FORM,
							   XmNbottomAttachment, XmATTACH_FORM,
							   XmNleftAttachment,  XmATTACH_FORM,
							   NULL);
  XmStringFree(str[0]);
  XtAddCallback(pb1, XmNactivateCallback, pb1_callback, NULL);  
  str[0]= XmStringCreateSimple("Quitter");
  pb2 = XtVaCreateManagedWidget("push_button_annuler",
								xmPushButtonWidgetClass,
								form_pb,
								XmNlabelString, str[0],
								XmNtopAttachment,    XmATTACH_FORM,
								XmNbottomAttachment, XmATTACH_FORM,
								XmNleftAttachment,   XmATTACH_POSITION,
								XmNleftPosition,    (XtArgVal) 1,
								NULL);
  XmStringFree(str[0]);
  XtAddCallback(pb2, XmNactivateCallback, Sortie, NULL);  
  
  /* charge le .Xdefaults */
  charge();

  /* creation d'un warning dialog */
  initialise();
  
  /* premiere page (par defaut) */ 
  cree_page(0);
  
  XtRealizeWidget(toplevel);
  XtAppMainLoop(app);
}


