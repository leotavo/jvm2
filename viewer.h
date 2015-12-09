// viewer.h

/*======================================================*/
/* 	DESCRIÇÃO

- Definições dos métodos para exibição dos bytecodes de um ClassFile
*/
/*======================================================*/

#ifndef	VIEWER_H
#define	VIEWER_H


#include	"classreader.h"

void	PrintConstantUtf8(cp_info *, FILE *);
void    PrintConstantClass(ClassFile *,cp_info *, FILE *);
void    PrintConstantString(ClassFile *, cp_info *, FILE *);
void    PrintConstantNameType(ClassFile *, cp_info *, char, FILE *);
void    PrintConstantRef(ClassFile *,cp_info *, char, FILE *);
void	showClassFile(ClassFile *, FILE *);
void	showGeneralInformation(ClassFile *, FILE *);
void	showConstantPool(ClassFile *, FILE *);
void	showInterfaces(ClassFile *, FILE *);
void	showFields(ClassFile *, FILE *);
void	showMethods(ClassFile *, FILE *);
void	showAttributes(field_info *, method_info *, attribute_info *, ClassFile *, FILE *);	// atributos de field, metodo,
													// de Code_Attribute ou de classe.



#endif


