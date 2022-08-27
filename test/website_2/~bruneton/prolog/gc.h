#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifndef _GC_
#define _GC_
#endif

#ifndef _TYPES_
#include "types.h"
#endif
 
/* ---------------------------------------------------------------------- */
/*   FONCTION D'ALLOCATION MEMOIRE DE BAS NIVEAU                          */
/* ---------------------------------------------------------------------- */

boolean gc_init (longint size);
ptr gc_malloc (longint size);
void gc_collect (void);
