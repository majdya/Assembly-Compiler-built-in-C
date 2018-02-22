/* ================= LAB_tab.c =======================*/ 

#include "asm.h"

static struct nlist *hashtab[HASHSIZE];

/* hash: Building hash value for the string s */
unsigned hash(char *s){
	unsigned hashval;
	for (hashval = 0; *s != '\0'; s++)
		hashval = *s + 31*hashval;
	return hashval % HASHSIZE;
}


/* lookup: Searching s in hashtab */
struct nlist *lookup(char *s){
	struct nlist *np;
	for (np=hashtab[hash(s)]; np != NULL;	np = np -> next ){
		if (strcmp(s, np->name)== 0)
			return np; /* found */
	}
	return NULL;
}

/* install: Inserting the name to the hashtab */

int install(char *name,int v){
	struct nlist *np;
	unsigned hashval;
	if ( (np=lookup(name))== NULL){ /* name was not found, assigning np=name */
		np = (struct nlist *) malloc(sizeof(*np));
		if ( np == NULL ||  (np->name =strDup(name)) == NULL){
			printf("Out of memory");
			return 2; /* out of memory.*/
		}
		hashval = hash(name);
		np -> next = hashtab[hashval];
		hashtab[hashval]= np;
		np->value=v;
		return 1;  /* succesfully added */
	}
	else /* already there */
		return 0; 
}
	

/*---------------- clean_tab ------------------------
clean_tab: Cleans the "label symbol table".
-----------------------------------------------------
*/
void clean_tab(){
	struct nlist *np, *u ;
	int i=0;
	for( i=0; i<HASHSIZE; i++){
		np=hashtab[i]; 
		while(np != NULL){
			u=np->next ;
			free(np->name);
			free(np);
			np=u;
		}
		hashtab[i]=NULL;
	}
}
