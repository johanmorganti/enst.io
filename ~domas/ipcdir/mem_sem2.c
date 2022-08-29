/***********************************************************
  mem_sem.c

              UTILISER la commande ipcs pour consulter les 
              outils System V fonctionnant sur la machine
  *********************************************************/


#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#define K	1024		/* 1K = 1024... */
#define NBLOCS 	4
#define NB_FILS 4

/**************** Noms des outils system V *********************/

#define MEMKEY  64
#define SEMKEY  64

/**************** Variables globales **************************/

int sem_id ;			
struct sembuf sem_oper_P;	/* Operation P */	
struct sembuf sem_oper_V;	/* Operation V */	
char *Ptr_part ; 		/* Pteur su la memoire commune */

/*****************      main      ****************************/
void main(void)
{

  void fils (char *Com, int i);
  int mem_id ;
  int	i, etat  ;
  int    *Ptr_int  ;

  /* Liste des commandes qui seront  executees par les processus fils */
  static char *Com [NB_FILS] = {"whoami", "date", "tty" , "df|grep dev"}  ; 
 
  union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
  } s_ctl;
  
  /* Initialisations des structures ad-hoc pour faire P et V */ 
  s_ctl.val = 1;
  sem_oper_P.sem_num = 0;
  sem_oper_P.sem_op = -1;
  sem_oper_V.sem_num = 0;
  sem_oper_V.sem_op = 1;
  
  /*     Creation et initialisation de la memoire partagee   */
  
  mem_id  =  shmget (MEMKEY, NBLOCS * K, 0777|IPC_CREAT);
  printf ("Pere %d -> mem_id = %d\n",(int)getpid(), mem_id);

  Ptr_part = shmat(mem_id  , 0 , 0);
  printf ("Pere %d -> Ptr_part = 0x%x\n",(int)getpid(), (int)Ptr_part);
  
/*     Creation et initialisation du semaphore   */

  sem_id      = semget (SEMKEY, 1, 0777|IPC_CREAT);

  /* Initialisation du semaphore a 1.
     Pour plus de details sur semun, faire man semctl.
  */
  printf ("Pere %d -> SETVAL = %d\n", (int)getpid(), 
           semctl (sem_id, 0, SETVAL, s_ctl ));
 
  /* Verification */
  printf ("Pere %d -> GETVAL = %d\n",(int)getpid(), 
           semctl (sem_id, 0, GETVAL, 0) ); 
  printf("\n\n\n");
  
  /*     Initialisation du pointeur commun dans la zone commune	:
            Ptr_int pointe sur le debut de la zone.
            Ptr_int[0] est donc le premier mot de 4 octets
            de cette zone.
            Il est initialise avec l'adresse de la premiere case libre,
            c'est a dire la suivante, puisque rien n'a encore
            ete fait.
   */
  
  Ptr_int     = (int *) Ptr_part ;         
  Ptr_int[0]  = (int) Ptr_int + 4  ;
  
  /* On affiche les valeurs de ces differentes cases */
  printf("Ptr_part  = 0x%x, Ptr_int = 0x%x\n", (int) Ptr_part, (int)Ptr_int);
  printf("Ptr_int[0]= 0x%x\n", Ptr_int[0]);
  
  /*     Creation des fils executant chacun une commande via popen.
         Chaque fils execute la fonction fils a laquelle on
         donne en parametre le nom  de la commande UNIX
         a executer.
  */
  
  for (i = 0; i < NB_FILS   ; i++)    fils (Com [i], i);

  /* On va maintenant attendre la fin de tous les processus
     qui viennent d'etre crees.
  */
  
  while (wait (&etat) != -1)
    printf("\n\t\t Pere -> Fin du fils %d\n\n", (etat >> 8));
  
  /* Tous les fils se sont termines.
     Afficher le contenu de la memoire partagee.
  */
  
printf("\t\
------------------------------------\n\t\
VISUALISATION DE LA MEMOIRE PARTAGEE\n\t\
------------------------------------\n\n");

  printf("Ptr_part = 0x%x, Ptr_int = 0x%x\n", (int)Ptr_part, (int)Ptr_int);
  
  printf ("\nPere -> Impression memoire commune de 0x%8x a 0x%8x : \n ",
	  (int)Ptr_part, Ptr_int[0] );
  printf ("\nPremier mot : 0x%8x\n", (int)Ptr_part + 4);
  printf (".... et tous les autres :\n");

  for (i=4; i < NBLOCS*K; i=i+80) write (1, Ptr_part+i, 80);
  
  /*  C'est fini on va rendre les objets partages.
  
  */
  
  semctl ( sem_id, 0, IPC_RMID);
  shmctl ( mem_id, IPC_RMID,0);

  printf ("\nPere -> memoire %d et semaphore %d detruits !\n", mem_id, sem_id);

  exit (0);
}
/*******************************************************1*****************
                            fonction fils executee par
                            tous les processus fils
		           *********************************************/
void fils (char *com, int i)
{
  static char format [] = "\n******* Ici fils Pid %d : \n "; 
  char *Ptr;
  int  *Ptr_i;
  FILE *p;
  int j,val_sem;
  
  if ( fork() == 0)  {
    
    /*    *******   P (sem_id )    *******      */
    
    printf("\n\t\t#### Avant oper_P : #### \n\t\
Fils %d  Valeur du semaphore ( GETVAL )= %d \
\n\t\t\t#### \n",i,semctl(sem_id,0,GETVAL,0));
    
    if ( i == 0 )
      {
	printf("Fils %d avant oper_P et DODO 1s : \
Valeur du semaphore ( GETVAL )= %d \n"\
	       ,i,semctl(sem_id,0,GETVAL,0));
	
	sleep(1);
	
	printf("Fils %d Je me reveille : \
Valeur du semaphore ( GETVAL )= %d \n"\
	       ,i,semctl(sem_id,0,GETVAL,0));
	
      }
    j = semop (sem_id , &sem_oper_P, 1);
    
    
    printf("\n\t\t @@@@ Apres oper_P :@@@@\n \t\
Fils %d Valeur du semaphore ( GETVAL )= %d \n\
\t\t\t@@@@\n"
	   ,i,semctl(sem_id,0,GETVAL,0));
    
    if ( i == 1 ) sleep(10);
    
    Ptr_i = (int *) Ptr_part;
		Ptr = (char *) Ptr_i[0]; 
		
		printf("\n\
Pointeurs sur memoire partagee :\n\
->->->->->->->->->->->->->->->->\n\
Fils %d ---> Ptr_part=%x  Ptr_i[0]=%x\n",i,(int)Ptr_part,Ptr_i[0]);

		sprintf (Ptr,format , (int)getpid() );
		Ptr = Ptr + sizeof (format) + sizeof (int);
		p = popen (com, "r");
		while ( fread (Ptr, sizeof(char), 80, p) )  Ptr = Ptr +80;
		Ptr =Ptr + 80;
		Ptr_i[0] =(int) Ptr ;
		
		printf("\
Pointeurs apres ecriture :\n\
Ptr_part=%x  Ptr_i[0]=%x\n\
->->->->->->->->->->->->->->->->\n\n\
",(int)Ptr_part,Ptr_i[0]);
		
		/*    *******   V (sem_id )    *******      */
		
		j = semop (sem_id , &sem_oper_V, 1);
		val_sem=semctl(sem_id,0,GETVAL,0);
		
		printf("\n\t\t$$$$ SORTIE DE SC :$$$$\n\
\tFils %d ,Valeur du semaphore ( GETVAL)= %d\n\
\t\t\t$$$$\n"\
		       , i,val_sem );
		
		exit (i);
  } 

  /****************************   fin de fils  ************************/

}



