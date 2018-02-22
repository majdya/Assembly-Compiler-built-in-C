/* =================== er.c =========================*/ 

/* 
The purpose of this file is to handle possible error[s] on "as" file.
*/

#include "asm.h"

/*---------------- WRITE_ERROR ---------------------
write_error: This function uses the global variable 
             "error" to determine whether an error 
			 should be written to the screen. 
*/
void write_error(int ln){

	switch (error){
		case 1:						
			printf("line %d: Syntax error. \n",ln);
			break ;
		case 2:
			printf("line %d: label cannot be an instruction.\n", ln);
			break ;
		case 3:
			printf("line %d: label cannot be a directive. \n", ln);
			break;
		case 4:
			printf("line %d: Previously defined label. \n",ln);
			break;
		case 5:
			printf("line %d: label contains more than 30 characters. \n",ln);
			break;
		case 6:
			printf("line %d: Unrecognized directive. \n",ln);
			break;
		case 7:
			printf("line %d: unrecognized insruction. \n",ln);
			break;
		case 8:
			printf("line %d: The first character of the label must be a letter. \n", ln);
			break;
		case 9:
			printf("line %d: To many characters for one line. The number of characters per line must be less than 80.\n",ln	);
			break;
		case 10:
			printf("line %d: bad argument[s]\n",ln);
			break;
		case 11:
			printf("Line %d: line should contain more than a label\n",ln);
			break;
		case 12:
			printf("Line %d: Illegal addressing, check operands. \n",ln);
			break;
		case 13: 
			printf("Line %d: label cannot be a register \n",ln);
			break;
		case 14:
            printf("Line %d:no character[s] between the quotation marks ",ln);
            break;
		case 15:
			printf("Line %d: label was not defined in the file \n",ln);
			break;
		case 16:
             printf("Line %d: label already defined as external \n",ln);
             break;
        case 17:
             printf("Line %d: cannot define label as external. ",ln);
             printf("label already defined as internal.\n");
             break;
		case 18:
			printf("Line %d: Error: expecting argument...\n",ln);
			break;
	}

}


/*---------------- write_error2 ------------------------
write_error2: This function uses the global variable 
             "error" to determine whether an error 
	         should be written to the screen.
--------------------------------------------------------
*/
int write_error2(int ln, char *line){
	
	switch( error ){
		case 1:
			printf("Line %d: unrecognized label \n",ln);
			break;
		case 2:
			printf("Line %d: Error: expecting argument.. \n",ln);
			break;
		case 3:
			printf("Line %d: To few actual argument[s] \n",ln);
			break;
		case 4:
			printf("Line %d: misseing quotation[s] character on .string directive",ln);
			break;
		case 5:
			line=skipspace(line);
			line=nextfield(line);
			if( line == NULL )
				return 0;
			printf("Line %d: file has no internal label name \"%s\" ",ln,line);
			break;
	
	}
	return -13; /* late */
}

