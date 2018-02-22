/*==================== MAIN ========================= */


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

