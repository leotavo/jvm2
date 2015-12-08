// main.c

/*==========================================*/
/* 	DESCRIÇÃO

- Passagens de parâmetro de entrada pela linha de comando.



*/
/*==========================================*/

/*==========================================*/
//	INCLUDES
#include	"jvm.h"
#include	"debug.h"
#include	<stdio.h>
#include	<stdlib.h>
/*==========================================*/


int	main(int, char **);	// função principal do programa

/*==========================================*/
// função main()
// - Trata os dados de entrada.
int	main(int argc, char* argv[]){
	switch(argc){
		case	1:
			puts("Uso: ./jvm arquivo.class [args...]\n");
			exit(EXIT_FAILURE);
		default:
			jvmStart(argv[1], argc-2, argv+2);
			
	}
	#ifdef	DEBUG
	puts("Método main: OK");
	#endif
	return	0;
}// fim da função main()
/*==========================================*/
