/* ================= ex_tab.c =======================*/ 

#include "asm.h"

static struct nlist *exttab[HASHSIZE] ;

/* look4ext: looks for s in exttab */
struct nlist *look4ext(char *s){
	struct nlist *np;
	for (np=exttab[hash(s)]; np != NULL; np = np -> next ){
		if (strcmp(s, np->name)== 0)
			(np->value)++;
			return np; /* found */
	}
	return NULL;
}

/* install2ext: puts name in exttab */

int install2ext(char *name){
	struct nlist *np;
	unsigned hashval;
	if ( (np=look4ext(name))== NULL){ /* name was not found, assigning np=name */
		np = (struct nlist *) malloc(sizeof(*np));
		if ( np == NULL ||  (np->name =strDup(name)) == NULL){
			printf("Out of memory");
			return 2; /* out of memory. */
		}
		hashval = hash(name);
		np -> next = exttab[hashval];
		exttab[hashval]= np;
		(np->value)=0 ;
		return 1;  /* Successfully added */
	}
	else /* already there */
		return 0; 
}
	

/*------------- clean_ex ----------------------------
clean_ex: cleaning  the entry symbol table.
-----------------------------------------------------
*/
void clean_ex(){
	struct nlist *np, *u ;
	int i=0;
	for( i=0; i<HASHSIZE; i++){
		np=exttab[i]; 
		while(np != NULL){
			u=np->next ;
			free(np->name);
			free(np);
			np=u;
		}
		exttab[i]=NULL;
	}
}
