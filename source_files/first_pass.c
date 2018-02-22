/* ================= first_pass.c ===================*/ 

/*
The purpose of this source file is to complete the "first pass" as specified on the assignment project.
*/

#include "asm.h"

struct keywords instruction[] ={
  {"mov"},{"cmp"},{"add"},{"sub"},{"not"},{"clr"},{"lea"},{"inc"},
  {"dec"},{"jmp"},{"bne"},{"red"},{"prn"},{"jsr"},{"rts"},{"stop"},
};

struct keywords directive[] = {
  {"data"},{"string"},
  {"entry"},{"extern"},
};

struct keywords reg[]={
  {"r0"},{"r1"},{"r2"},{"r3"},{"r4"},{"r5"},{"r6"},{"r7"},
};


/*------------------------- ASSEMBLE ---------------------
assemble: This is the main function of this file.
          It activates the "first pass" on the input file (the file ending with "*.as") 
		  file. It then calls read_line, and write_error on
		  each iteration.
*/
int assemble(char *ch){
	FILE *as ;
	char as_name[NAME] ;
	char line[MAX];
		        
	int  ln=1;      /* ln counts line number in the source */
		
	struct boolian longline={0};
	

        er=0;	
	ic=0; /* initiating instruction counter. */
	dc=0; /* initiating data counter. */
	ac=100; /* initiating address counter. */
	
	
	as_extens(ch,as_name);
	as=fopen(as_name,"r");
	
	if(as == NULL){
		printf("\n\n Either \"%s\" does not exist or",as_name);
		printf("\n you have no permission to read it.\n\n");
		return 0;
	}
		               
	while(1){
		
		/* initiating instruction codes and directive values*/
		inscode=no_ins;
		dirtype=no_dir;		
		/*________________________________________________*/
		error=0;
		longline.flag=0;
		
		/*------- Set a breaking point.------------------*/
		if(fgets(line,MAX,as)==NULL)
			break;		/* Finish reading file, exiting loop. */
		/*_______________________________________________*/

		if(! ignore(line)){
			/*------- Checking line's length ---------------*/
			if(strlen(line)>MAX-2){
				longline.flag=1;  /* Marks long line.*/
				error=9;
				er++;
				write_error(ln);
				ln--;
			}			
			/*_____________________________________________*/

			if(longline.flag != 1){  /* If line is not to long, proceed...	*/
				read_line(line);  
				if (error != 0)  /* Count errors */
					er++;             
				write_error(ln);			
			}
		}
		ln++;	/*count line numbers in the source code.*/
	}
	/* End of loop. */
	
	fclose(as);
	second_pass(line,ch);  /* executing the second pass on "as" file.*/
	
	if( er > 1 )
		printf("\n File name %s contains %d errors.\n",as_name,er);
	if( er == 1 )
		printf("\n File name %s contains %d error.",as_name,er); 
	return 1; 
}



/*----------------------- read_line  -------------------------------
read_line: Read a line from "as" file determine whether a label exists
		   determine whether the line contains a directive or 
		   instruction and calls the appropriate function[s] 
		   accordingly.
--------------------------------------------------------------------
*/
int read_line(char *line){
	
	/*char *p=line  ;*/
	int temp=0;
	int label=0;
	
	/*----- Remarks and blank lines -----*/
	if (*line == ';')
		return 0 ;
	line=skipspace(line); 
	if(line == NULL)
		return 0;
	/*___________________________________*/
	

	/* first character can either be a dot or a letter. */
	if( isalpha(*line)==0 && *line != '.'  ){   
		error=1;
		return 0;
	}
	/*__________________________________________________ */

	/*If there is a label, "look4label" detect it, 
 	  put it in the symbol table, mark it with label.
	  flag and move to the next field. */	
	if( look4label(line) == 1 ){ 
		label=1 ;			/* Mark label.*/
		line=nextfield(line) ; /* Go to the next field.*/
	}
	
	if (line == NULL){
		error=11;
		return 0;
	}

	/* Looking for directive...-------------*/
	if(*line == '.' ){
		look4dir(line);
		if(dirtype == no_dir){ /*find_dir(line) == 0){*/
			error=6;    
			return 0;
		}
		dir2value(line,label);
		return 1;
	}
	/*--------------------------------------*/
	
	/* Looking for instruction...*/
	temp=look4ins(line);
	if(temp==0 && error == 0){
		error=7;
		return 0;
	}
	if( temp==1 ){
		ins2value(line,label);
	}
	return -13; /*late*/
}


/*---------------------look4label---------------------------------
look4label: return 1 on label appearance. Else return 0.
*/
int look4label(char *field ){
	char *p=field;
	
	int  i=0, c=0;
	
	while( *p != ':' && *p != 10 && i<30 ){
		LABEL[i]=*p;
		p++;
		i++;
	}
	LABEL[i]='\0';
	
	while( *p != ':' && *p != 10 && *p!='\0')  /* problem here 1*/
		p++;
	if ( *p == ':' ){						   
		if (i >= 30){
			error=5;		/* label is to long.*/
			return 0;
		}
		for(c=0;c<i;c++){ /* checking label characters.*/
			if(!isdigit(LABEL[c]) && !isalpha(LABEL[c])){
			error=1;
			return 0;
			}
		}
			
		if ( *field == '.' || isdigit(*field)  ){ /* label cannot start with a dot.*/
			error=8;			
			return 0;
		}		
		if (look4ins(LABEL)==1){   /* label cannot be an instruction.*/
			error=2 ;
			return 0;
		}
		if (find_dir(LABEL)==1){   /* label cannot be a directive.*/
			error = 3 ;
			return 0;
		}
		if(find_reg(LABEL) == 1){  /* label cannot be a register.*/
			error=13;
			return 0;
		}
		if( error == 0 )
			return 1;
	}
	return -13; /*late*/
}



/*----------- SKIPSPACE --------------------------------
skipspace: Skips on blank character[s] and returns the
		   a pointer to the first non blank character.
--------------------------------------------------------
*/
char *skipspace(char *line){
	if(line == NULL)
		return NULL;
	if(strlen(line) <= 1)       
		return NULL;
	while(*line==' ' || *line=='\t'){
		if (*line == '\0' || *line==15)
			break;
		line++;
		
	} 
	return line;
}
/* ----------------------     ----------------------------------- */

int check_ext(char *ch){
	while(*ch != '\0'){
		ch++;
	}
	ch--; 
	if(*ch !='s'){ 
		return 0;}
	ch--;
	if(*ch !='a')
		return 0;
	ch--;
	if(*ch !='.')
		return 0;
	return 1;

}
	


/*------------------ MAKE_EXTENS ------------------------
add_extens: Generates the appropriate extension to the 
             files.
---------------------------------------------------------
*/
void add_extens(char *ch, char *s, char *ext){
	strcpy(s,ch) ;
	strcat(s,ext) ;	
}

/*------------------ MAKE_EXTENS ------------------------
as_extens: Add extension for the *.as filetype. 
---------------------------------------------------------
*/
void as_extens(char *ch, char *s){
	strcpy(s,ch) ;
	if(check_ext(ch) == 0)
		strcat(s,".as") ;	
}
/*  --------------- file_name------------------------
file_name: This function returns the file name without the extension.
------------------------------------------------------
 */
char *file_name(char *fname){
	int i, j=0;
	char *ch, *fn=fname;
	ch= (char *) malloc(sizeof(char*));
	while(*fn!='\0'){
		j++;
		fn++;
	}
	while(*fn!='.'){
		j--;
		fn--;
	}
	fn--;
	for(i=0;i<j;i++){
		*ch=*fname;
		ch++;
		fname++;
	}
	*ch='\0';
	for(i=0;i<j;i++)
		ch--;
	return ch;
}
/*----------------- NEXTFIELD -------------------------------
nextfield: Returns a pointer to the next "field" of the line
		   i.e. it choose the first character after "blank".		
*/
char *nextfield(char *line){
	if( line == NULL)
		return NULL;
	while( *line != ' ' && *line != '\t' && *line != 10 && *line != '\0' ){
		line++;
	}
	line++;
	line=skipspace(line);
	return line;
}


/*------------------- FIND_DIR --------------------------
find_dir: Finds out whether p is a legitimate directive 
          name.
---------------------------------------------------------
*/
int find_dir(char *p){

	int i; 
	for(i=0;i<4;i++){
		if ( strcmp(directive[i].word , p) == 0 ){
			return 1 ;
		}
	}
	return 0 ;
}


/*--------------------- look4ins --------------------------- 
look4ins: Returns 1 if p is a legitimate instruction name, else
		  it returns zero.  
-------------------------------------------------------------
*/
int look4ins(char *p){
	int i; 
	for(i=0;i<=15;i++){
		if (strncmp(instruction[i].word , p,3) == 0 && lgh2blank(p)==3 ){
			inscode=i;
			return 1 ;
			}
		  else if (strncmp(instruction[i].word , p,4) == 0 && lgh2blank(p)==4 ){
			inscode=i;
			return 1 ;
			}
		
	}	
	
	return 0 ;
}


/*----------------------- look4dir --------------------------
look4dir: Returns 1 if p is a legitimate directive name, else
		  it returns zero.  
-------------------------------------------------------------
*/
int look4dir(char *p){
	if(p == NULL)
		return 0;
	if ( strncmp("data",p+1,4)==0 && lgh2blank(p+1)==4){  
		dirtype=0;  /* Mark directive type. */
		return 1 ;
	}
	if (strncmp("string",p+1,6)==0 && lgh2blank(p+1)==6 ){
		dirtype=1;
		return 1;
	}
	if( strncmp("entry",p+1,5)==0 &&  lgh2blank(p+1)==5){
		dirtype=2;
		return 1;
	}
	if( strncmp("extern",p+1,6)==0 && lgh2blank(p+1)==6){
		dirtype=3;
		return 1;
	}
	
	return 0 ;
}


/*-------------------- lgh2blank -------------------------
lghblank: Returns the length of the string up to "blank"
		  character.	 
*/
int lgh2blank(char *line){
	int i=0;
	while ( *line>32 && *line!=10 && *line!='\0'){
		line++;
		i++;
	}
	return i;
}



/*---------------- dir2value ---------------------
dir2value: Inserts value to label and updates 
           the value of dc.
--------------------------------------------------
*/
int dir2value(char *line, int i){
	int k=0 ;
	char *xlab;
	
	line=nextfield(line);
	
	if(i == 1){ /* label exists.*/
		if(dirtype==DATA || dirtype == STRING){ 
			if(look4ext(LABEL) != NULL ){  
				error=16;
				return 0;
			}
			if( install(LABEL,(dc+ic)) == 0 ){ /* insert label into the label symbol table.*/
				error = 4;		/* redifinition of label.*/
				return 0;
			}
		}
	}
	
	line=skipspace(line);
	if( line == NULL ){
		error=18; 
		return 0;
	}
	
	if(dirtype == DATA){
		while( line != NULL && *line!=10 && *line!='\0'){
			if(*line == '-')
				line++;
			while( isdigit(*line) ){
				line++;
				k++;
			}
			dc++;
			line=skipspace(line);
			if( line==NULL || *(line+1)=='\0')
				return 0;
			if(  *line != ',' || k==0 ){
				error=10; 
				return 0;
			}
			line++;
			line=skipspace(line);
			if( line==NULL)
				return 0;
			k=0;
		}
	}
	if(dirtype == STRING){
		if(line==NULL){
			error=10;
			return 0;
		}
		if(*line!='"'){
			error=10;
			return 0;
		}
	
		line++;

		if( *line == '"' ){
			error=14;
			return 0;
		}
		while(line!=NULL && *line != '"' && *line!=10 && *line!='\0'){
			/*count++;*/
			line++;
			dc++;
		}
		dc++;  /* length of the string plus 1.*/
		if( line==NULL ){
			error=10;
			return 0;
		}
		line++;
		line=skipspace(line);
		if(line==NULL)
			return 0;
		if( *line!=' ' && *line!='\t' && *line!=10 && *line!='\0'){
			error=10;
			return 0;
		}
		dc++; /* for the line that marks the end of string.*/
	} 
	if(dirtype == EXTERN){
		xlab=cpyarg(line,' ');
		if( lookup(xlab) != NULL){ 
			error=17; /* already defined as a non ext label.*/
			return 0;
		}
		install2ext(xlab); /* put label in external symbole tabel.*/
	}
	if(dirtype == ENTRY ){
		xlab=cpyarg(line,' ');
		ins2entry(xlab);
	}
	return -13; /*late*/
}


/*------------------ gotocomma ---------------------------
goto2comma: Takes a string and return a pointer, pointing 
			to where the comma appears in the string. If 
			no comma in the string, it returns zero.
*/
char *go2comma(char *line){
	if(line==NULL || *line == '\0')
		return NULL;
	if(*line==',')
		return line;
	line++;
	return go2comma(line);
}

/*--------------------- addmeth --------------------------
addmeth: returns the code number for each 
         operand.
---------------------------------------------------------- 
*/
int addmeth(char *line){
	if(line == NULL )
		return no_operand; 	
	if(*line=='#'){
			line++;
			if( *line == '-')
				line++;
			if( isnumber(line) == 0 )
				error=12;
			return 0;
	}
	if( find_reg(line) == 1)
	{
		line++;
		if(*line%2==1&&*(line+1)=='['&&*(line+4)==']'){

			if(find_reg(line+2)==1&&*(line+3)%2==0){			
			return 2;
			
			}
			}
		return 3;
			
	}
		
	if(islabel(line) == 1)
		return 1;
	
	return 7;	
	
}

	
/*---------------- ins2value ------------------------
ins2value: Inserting  a value to label and updating the 
           value of ic. 
-----------------------------------------------------
*/
int ins2value(char *line, int lab_in_tab){
	int first=no_arg, second=no_arg;
	int code=inscode; 
	
    line=nextfield(line);
	if( line == NULL && code <14 ){
		error=12;
		return 0;
	}
	if(lab_in_tab == 1){ /* label exists.*/
		if(look4ext(LABEL) != NULL ){
			error=16;
			return 0;
		}
		if( install(LABEL,(ic+dc)) == 0 ){ /* put label in the label symbol table.   */
			error = 4;			/* redefinition of label.*/
			return 0;
		}
	}

	/*-Instruction with no operands.-*/
	if( code == 14 || code == 15){
		ic++;
	}
	/*_____________________________*/
	
	/*-Instruction with one operand.-*/
	if((code >= 7 && code <= 13) || code == 4||code == 5){
		second=addmeth(line);
		
		if (second == 7){
			error=12;
			return 0;
		}
		if( (code >= 7 && code <= 13 && code !=12) && second == 0){
			error=12;
			return 0;
		}
		if( ( code == 9 || code == 10 || code == 13 ) && second ==3 ){
			error=12;
			return 0;
		}
		ic++ ;
		if( second <=3)
			ic++;
	}
	/*______________________________*/

	/*-Instruction with two operands.-*/
	if( (code >=0 && code<=3) || code == 6 ){
		first=addmeth(line);
		line=go2comma(line);
		if(line == NULL){
			error=12;
			return 0;
		}
		line++;
		if(line == NULL){
			error=12;
			return 0;
		}
		line=skipspace(line);
		if(line == NULL){
			error=12;
			return 0;
		}
		second=addmeth(line);
		if( ((code >=0 && code<=3)||code==6) && code != 1 ){
			if( second == 0 ){
				error=12;
				return 0;
			}
			if( code == 6 && first != 1  ){
				error=12;
				return 0;
			}
		}
		if( code == 11 && first == 0){
			return 0;
		}
		ic++;
		if(first<2)
			ic++;
		if(second<2)
			ic++; 
	}
	
	return -13; /*late*/
}
	
/*------------cont---------------------------
cont: To make notation easier.
*/
int cont(char *line){
	if(line==NULL)
		return 0;
	if(*line != ' ' && *line != '\t'){
		if(*line != 10 &&  *line !='\0' && line != NULL)
			return 1;
	}
	return 0;
}


/*--------------- isnumber ----------------------
isnumber: Check to see whether a string is a positive 
	  number or not. Return 1 if the string is a 
	  positive number 0 otherwise.       
-------------------------------------------------
*/
int isnumber(char *line){
	if(line == NULL)
		return 0;
	while( cont(line) && *line != ',' ){
		if( ! isdigit(*line) )
			return 0;
		line++;
	}
	return 1;
}

/*-------------------- islabel ----------------------
islabel: returns 1 if the syntax of the string compliance  with the syntax of label.
----------------------------------------------------
				
*/
int islabel(char *line){
	int i=0;
	if(line == NULL)
		return 0;
	while( cont(line) && *line != ',' ){
		if(i==0 &&  isalpha(*line) == 0 )
			return 0;
		i++;
		if( isdigit(*line) == 0 &&  isalpha(*line) == 0 )
			return 0;
		if(i == 30){
			error=5;
			return 0;
		}
		line++;
	}
	return 1;
}

/*----------------------- find_reg ------------------------------
find_reg: Returns 1, if register appears, 0 otherwise.
----------------------------------------------------------------
*/
int find_reg(char *p){
	int i; 
	for(i=0;i<8;i++){
		if ( strncmp(reg[i].word, p ,2) == 0 ){
			regist=i;
			return 1 ;
		}
	}
	return 0 ;
}

/*-------------------------ignore----------------------------------
 * */
int ignore(char *line){
	if(line == NULL)
		return 1; /* ignore the line it doesn't exist.*/
	if(*line==';')
		return 1;
	while(line!=NULL && *line<=32 && *line!='\0' && *line=='\t'){
		line++;
		if(*line==';')
			return 1;
	}
	if(*line=='\0')
		return  1;
	line++;
	if(*line=='\0')
		return  1;
	else 
		return 0;
}

