
/* ********************************************************************* *
 *
 *  xplus.h header de xplus.c
 *  
 * Nicolas Romanetti, 1995.
 *
 * ********************************************************************* */


extern int ReadGraph(char *NomFichier);
extern void afficher(int n);
extern void SupprimerCurves(int n);
extern void ToutVider(void);
extern void AffiMem(Widget dialog, XtPointer drawing, XtPointer call_data);
extern void SelectScale(Widget dialog, XtPointer drawing, XtPointer call_data);
extern void SelectZoom(Widget dialog, XtPointer drawing, XtPointer call_data);
extern void Bilan(void);
extern void AfficherAxes(void);
extern void SupprimerAxes(void);
extern void NewScreen(void);
