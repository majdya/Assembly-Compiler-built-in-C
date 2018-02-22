/* ================= second_pass.c ===================*/ 

/* 
The purpose of this file is to make the "second pass" on the "as" file. 
*/

#include "asm.h"
/****************************************************
It is recommended to avoid global variables. (-3)
*****************************************************/
int ln=1;

/* ---------------- is_label ---------------------
islabel: return 1 if the line contain a legitimate
		 syntax of a label, else it returns 0.  
--------------------------------------------------
*/
int is_label(char *line){
	int i=0;
	line=skipspace(line);
	if( line == NULL)
		return 0;
	if( *line == '.' )
		return 0;
	while( *line != ':' && i < 30 && *line!='\0'){
		i++ ;
		line++ ;
	}
	if( i == 30)
		return 0;
	if( *line == ':')
		return 1 ;
       return 0; /*late 	*/
}


/*--------------------- char2num --------------------------------
char2num: Takes a string of the form [#][-]d where d is a string
		  of digits and the character between [] are optional, 
		  and returns the numeric value of [-]d.
--------------------------------------------------------------*/
int char2num(char *line){
	int num=0;
	struct boolian minus={0};
	
	if( *line == '#' )
		line++ ;
	if(*line == '-'){
		minus.flag=1;
		line++;
	}
	
	do{		
		num=(num*10)+( *line - '0' );
		line++;
	} while( isdigit(*line) ); 
	
	if(minus.flag ==1)
		return -num;
	else
		return num;
}



/*--------- second_pass------------------------------- 
second_pass: The main function in this file.
             It makes the "second pass" on the "as" 
			 file, create the appropriate files and
			 uses other functions to write on these 
			 files. If the program detects error[s]
			 on the "as" file, then the function removes 
			 all file[s].
-------------------------------------------------------
*/
int second_pass(char *line, char *ch){
	
	FILE *as, *ob, *ex=NULL, *en=NULL;
	char as_name[NAME], ob_name[NAME], ex_name[NAME], en_name[NAME];
	char *fname=NULL;
	
	/*int ln;		 to count line number in source. */
	fname=(char *) malloc(sizeof(char));
	as_extens(ch,as_name);
	as=fopen(as_name,"r");
	fname=file_name(as_name);
	
	add_extens(fname,ob_name,".ob");
	add_extens(fname,en_name,".ent");
	add_extens(fname,ex_name,".ext");
	
	ob=fopen(ob_name,"w");
	fprintf(ob,"\t%hX %hX\n",ic,dc);
	if( ob == NULL ){
		printf("Error: permission denied");
		return 0;
	}

	/*--- Creating ".ext" and ".ent" files. ---- */
			
	ex=fopen(ex_name,"w");
	en=fopen(en_name,"w");
	
	ln=0;

	while(1){

		/* initiating instruction codes and directive values*/
		inscode=no_ins;
		dirtype=no_dir;
		regist=no_reg;
		error=0;
		/*__________________________________________________*/
		
		/*------- Set a breaking point.------------------*/
		if(fgets(line,MAX,as)==NULL)
			break;		/* Finish reading file, exiting loop.*/
		/*_______________________________________________*/
		if(! ignore(line)){
			write2files(ob,en,ex,line,ln);
			write_error2(ln,line);
			if (error != 0)  /* Counting errors */
				er++;
			
		}
		ln++;
	}

	
	
	fclose(ex) ;
	fclose(en) ;
	fclose(ob);
	fclose(as);
	if(er>0){
		remove(ob_name);
		remove(en_name);
		remove(ex_name);
	}
	return 1; /*late*/
}

/*------------------print-------------------------------------------
print: To make notation easier.
-------------------------------------------------------------------
*/
int print(FILE *ob, int f0, int f1, int f2, int f3, int f4, int f5 ){
	fprintf(ob, "%.4hX \n",f0*2048+f1*512+f2*32+f3*8+f4*2+f5);
	return 0;
}

/*----------------------- write2files -----------------------------
write2files: Takes a line from the "as" file and write output to 
			 "ob", "ex", and "en".   
*/
int write2files(FILE *ob,FILE *ent,FILE *ext,char *line, int ln){
		
	/* ----- skip label ----------------- */
	line=skipspace(line);
	if( is_label(line) == 1 ){
		line=nextfield(line);
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	/*if(line!=NULL)*/
		/*fprintf(ob,"%s",line);*/
	dir2ob(ob,line);
	write2ent(ent,line);
	wrt(ob,ext,line);	
	
	return 0;
}

/*-------------------- dir2ob ---------------------------------
dir2ob: if a line contains .data or .string directive, then
		dir2ob function writes the appropriate output line[s]
		to the "ob" file.
---------------------------------------------------------------
*/
int dir2ob(FILE *ob,char *line){
	int i=0;

	look4dir(line);  /* change dirtype value.*/
	/*--- DATA*/
	if( dirtype == DATA ){
		line=nextfield(line) ;
		line=skipspace(line); 
		if( line == NULL){
			return 0;
		}
		do{
			if(line == NULL ) 
				break;
			fprintf(ob,"%.2hX\t",ac);     
			fprintf(ob,"%.4hX \n",char2num(line));    
			ac++;
			line=go2comma(line);
			if(line == NULL ){
				
				break;
			}
			line++;
			line=skipspace(line);
			
		}
		while(line!=NULL);
	}
	/*--- STRING*/
	if( dirtype == STRING ){
		line=nextfield(line);
		line=skipspace(line);
		if( line == NULL ){
			return 0;
		}
		line++; /* skip on quotation mark.*/
		
		do{
			fprintf(ob,"%.2hX\t",ac);
			fprintf(ob,"%.4hX \n",*line);   
			line++;
			ac++;
			i++;
		}
		while(*line != '"' && i<MAX);
		if( i == MAX){
			error=1000;
			error=4;
		}
		fprintf(ob,"%.2hX\t",ac);
			fprintf(ob,"%.4hX \n",0);   
			line++;
			ac++;
	}
	return 0;
}

/*------------------- write2ent ----------------------------
write2ent: if a line contains .entry directive, then
		   write2ent function writes the appropriate output line
		   to the "en" file. 
------------------------------------------------------------
*/
int write2ent(FILE *ent, char *line){
	
	struct nlist *en;
	char *temp;

	/* to change dirtype value, write2file function calls to */
	/* dir2ob first and then it calls to the current function. */
	line=skipspace(line);
	/* ----- skip label ----------------- */
	if( is_label(line) == 1 ){
		line=nextfield(line);
	}
	/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	
	if( dirtype == ENTRY){
		line=skipspace(line);
		line=nextfield(line);
		if( line == NULL ){
			return 0;
		}
		temp=cpyarg(line,' ');
		en=lookup(temp); 
		if(en == NULL){
			error=5; 
			return 0;
		}
		fprintf(ent,"%s\t",temp);
		fprintf(ent,"%hX \n",en->value);   
	}
	return 1;


}

/*-------------------- wrt -------------------------------
wrt: If a line contains instruction, then wrt function
     writes the appropriate output line[s] to the "ob" file.
----------------------------------------------------------
*/
int wrt(FILE *ob, FILE *ext, char *line){  
	char *u ;
	int first_arg, second_arg ;
	int code ;
	int addtype1, addtype2;

	addtype1=no_operand;
	addtype2=no_operand;

	line=skipspace(line);  
	if(line==NULL || *line=='\0' || *line==10)
		return 0;
	look4ins(line); /* change inscode value.*/
	code=inscode;
		
	/* Instruction with no arguments.*/
	if(code==14 || code == 15){
	
		fprintf(ob,"%.2hX\t",ac);
		print(ob,BB,0,code,0,0,0);
		ac++;
		return 0;
	}
	
	line=nextfield(line);
	
	if(line == NULL){
		error=2;
		return 0;
	}
	
	u=cpyarg(line,' ');

	/* Instruction with one argument.*/
	
	if((code >= 7 && code <= 13) || code == 4||code == 5){
		addtype1=addmeth(line);
		if(addtype1 == no_operand){
			error=2;
			return 0;
		}
		fprintf(ob,"%.2hX\t",ac);
		find_reg(u);
		if(regist == no_reg)
			regist=0;
		print(ob,BB,1,code,addtype1,regist,0);
		ac++;
		addrval(ext, line,& first_arg, addtype1);
		if(first_arg>-1){
			ac++;
			fprintf(ob,"%.2hX\t",ac);
			if( addtype1 == 3 )
				first_arg=first_arg-(ac-1);
			fprintf(ob,"%.4hX   \n",first_arg);
			/*ac++;*/
		}
		ac++;
	}
	
	/*   Instruction with two arguments.*/
	
	if( (code >=0 && code<=3) || code == 6 ){
		
		addtype1=addmeth(line);
		if(addtype1 == no_operand){
			error=2 ;
			return 0 ;
		}

		fprintf(ob,"%.2hX\t",ac);
		/*__________________________________________________________*/
		find_reg(u);
		if(regist == no_reg)
			regist=0;
		addrval(ext, line,&first_arg, addtype1);
		/*__________________________________________________________*/
		if(line != NULL)
			line=go2comma(line);
		
		
		if(line != NULL)
			line++; /* skip comma.*/
		if(line != NULL)
			line=skipspace(line);
		if(line == NULL){
			error=3;
			return 0;
		}
		addtype2=addmeth(line);
		
		u=cpyarg(line,' ');		
		
		/*____________________________________________________________*/
		
		find_reg(u);
		if(regist == no_reg)
			regist=0;
		addrval(ext, line,&second_arg,addtype2);     	
		
		/*____________________________________________________*/
		print(ob,BB,2,code,addtype1,addtype2,0);
		ac++;
		if(first_arg>-1){
			fprintf(ob,"%.2hX\t",ac);
			if( addtype1 == 3 )
				first_arg=first_arg-(ac-1);
			fprintf(ob,"%.4hX    \n",first_arg);
			ac++;
		}

		if(second_arg>-1){
			fprintf(ob,"%.2hX\t",ac);
			if( addtype2 == 3 )
				second_arg=second_arg-(ac-2);
			fprintf(ob,"%.4hX    \n",second_arg);
			ac++;
		}
	}
	return 1; /*late	*/
}



/*---------------------- addrval ----------------------------------------
addrval: If an additional word shuld be written to file name "ob", then
		 addrval function puts the numeric value of this word in integer
		 name "first". Otherwise "first" gets -1. The addrval function 
		 also, write the appropriate output to the "ex" file.
-------------------------------------------------------------------------
*/
int addrval(FILE *ext, char *line,int *first, int operand){
	
	struct nlist *temp;
	char *u;
	int addm = addmeth(line);
	
	
	if(addm == no_operand){
		error=2 ;
		return 0;
	}
	u=cpyarg(line,','); 
	if( addm ==0  || addm == 2 )
		u++;
	if( ( temp=lookup(u) ) != NULL){ /* find out whether temp is in label symbol table.*/
		*first=temp->value ;
		
		if(addm == 1)
			
		return 0;
	} 
	if( ( temp = look4ext(u) ) != NULL){ /* finds out  whether temp is in external symbol table.*/
		
		
		if( operand==3 && operand != no_operand)
			fprintf(ext,"%s\t%hX \n",temp->name,ac);
		if( operand<3 && operand>=0 )
			fprintf(ext,"%s\t%hX \n",temp->name,ac+1); 

		*first=0 ;
		return 0;
	}
	if( addm == 0 ){
		
		if (isnumber(u) != 0) 
			*first=char2num(u);
		if (isnumber(u) == 0) 
			error=1;	
		return 0;	
	}	
	if( addm ==3 ){
		*first = -1; /* no additional word.*/
		return 0;
	}		
	
	return 0 ;
} 

/*------------------ cpyarg -------------------------------
cpyarg: Copy all the character in the input string up to
		c, space, \t, \0 character, which ever comes first.   
----------------------------------------------------------
*/	
char *cpyarg(char *line,char c){
	char *arg; 
	char *t;
	
	arg=(char*) malloc(sizeof(char));
	if(arg == NULL ){
		printf("Error: out of memory");
		return NULL;
	}
	t=arg;
	strcpy(arg,line);
	if( arg == NULL )
		return NULL;
	while( cont(arg) &&  *arg != c ){
		arg++ ;
	}
	*arg='\0' ;
	return t ; 
}
/* Function to convert a decinal number to binary number */
int d2b(int n) {
    int remainder; 
 int binary = 0, i = 1;
  
    while(n != 0) {
        remainder = n%2;
        n = n/2;
        binary= binary + (remainder*i);
        i = i*10;
    }
    return binary;
}


