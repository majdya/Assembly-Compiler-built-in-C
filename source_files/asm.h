#include<stdio.h>  
#include<ctype.h>
#include<string.h>
#include<malloc.h>
#include<math.h>


#define NAME 207  /*Max characters allowed for the name of the input file.*/

#define MAX 200	  /*Max characters allowed in one line.*/

#define DATA 0
#define STRING 1
#define ENTRY 2
#define EXTERN 3

#define no_ins 16
#define no_dir 4
#define no_arg 6
#define no_operand 7
#define no_reg 30

#define forever for( ; ; ) /* infinie loop.*/

#define HASHSIZE 101

#define BB  111
/*------------------------------------STRACTURES------------------------------------------*/

struct keywords {
	char *word;
};
	 
struct boolian{
	unsigned int flag : 1;
};

struct nlist{
	struct nlist *next;
	char *name;
	int value;
};



/*--------------------- GLOBAL VARIABLES ------------------------- */
/*******************
Definition at header file (-3) 
********************/
int ic, dc, ac ;  /* Global variables for "instruction counter", "data counter" and "address counter" (respectively).*/
int er;
int error  ;  /* check errors  */

char LABEL[30];

int inscode   ;
int dirtype   ;
int regist    ;





/* Function declaration */

/* first_pass.c */
int assemble(char *);
int read_line(char *);
int check_ext(char *);
void as_extens(char *, char *);
char *file_name(char *fname);
void add_extens(char *, char *, char *);
int find_dir(char *);
int find_reg(char *);
char *skipspace(char *);
int look4label(char *);
int look4dir(char *);
int look4ins(char *);
int dir2value(char *, int );
int ins2value(char *, int );
char *go2comma(char *);
int islabel(char *);
char *nextfield(char *);
int isnumber(char *);
int lgh2blank(char * );
int cont(char *);
int ignore(char *);
/* error.c */
void write_error(int);
int write_error2(int, char *);

/* second_pass.c  */
int addresscode(FILE *);
int print(FILE *, int, int, int, int, int , int);
int dir2ob(FILE *,char *);
int write2files(FILE *,FILE *,FILE *,char *,int);
int second_pass(char*,char*);
int write2ent(FILE *, char *);
char *cpyarg(char *,char);
int wrt(FILE *, FILE * , char *);
int addmeth(char *);
int addrval(FILE *, char *,int *,int);
int d2b(int n) ;
int b2d(int n);



/* LAB_tab.c */
int install(char *, int);
struct nlist *lookup(char *);
unsigned hash(char *);
void clean_tab();
/* en_tab.c  */
int ins2entry(char *);
struct nlist *look4entry(char *);
void clean_en();
char *strDup(char *);
/* ex_tab.c  */
int install2ext(char *);
struct nlist *look4ext(char *);
void clean_ex();
