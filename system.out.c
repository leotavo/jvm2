#include <stdio.h>

#include	"system.out.h"

void	print(char * string){
	printf("%s", string);
}

void	println(char * string){
	print(string);
	printf("\n");
}
