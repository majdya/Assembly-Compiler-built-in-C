/*==================== MAIN ========================= */
/* MMN 14 - i know the program is not working properly , but i'm stuck and cant progress any more .
   
   majd yaqoub id: 313228249
*/

#include "asm.h"
#include<stdlib.h>

int main(int argc, char *argv[]){
	int i;
	for(i=1;i<argc;i++){
		assemble(argv[i]);
		clean_tab();
		clean_ex();
		clean_en();
	}
	printf("\n");
	return 0; 
}

