// viewer.c

/*======================================================*/
/* 	DESCRIÇÃO

- Implementação dos métodos para exibição dos bytecodes de um ClassFile
*/
/*======================================================*/


#include "viewer.h"
#include "opcode.h"
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include <locale.h>
#include <stdlib.h>

// Array contendo strings com os mnemônicos da JVM, para utilização no Exibidor.
char	*	opcodes[] = {
		// 0 a 9
		"nop", "aconst_null", "iconst_m1", "iconst_0", "iconst_1", "iconst_2", "iconst_3", "iconst_4", "iconst_5", "lconst_0",
		// 10 a 19
		"lconst_1", "fconst_0", "fconst_1", "fconst_2", "dconst_0", "dconst_1", "bipush", "sipush", "ldc", "ldc_w",
		// 20 a 29
		"ldc2_w", "iload", "lload", "fload","dload", "aload", "iload_0", "iload_1", "iload_2", "iload_3",
		// 30 a 39
		"lload_0", "lload_1", "lload_2", "lload_3", "fload_0", "fload_1", "fload_2", "fload_3", "dload_0", "dload_1",
		// 40 a 49
		"dload_2", "dload_3", "aload_0", "aload_1", "aload_2", "aload_3", "iaload", "laload", "faload", "daload",
		// 50 a 59
		"aaload", "baload", "caload", "saload", "istore", "lstore", "fstore", "dstore", "astore", "istore_0",
		// 60 a 69
		"istore_1", "istore_2", "istore_3", "lstore_0", "lstore_1", "lstore_2", "lstore_3", "fstore_0", "fstore_1", "fstore_2",
		// 70 a 79
		"fstore_3", "dstore_0", "dstore_1", "dstore_2", "dstore_3", "astore_0", "astore_1", "astore_2", "astore_3", "iastore",
		// 80 a 89
		"lastore", "fastore", "dastore", "aastore", "bastore", "castore", "sastore", "pop", "pop2", "dup",
		// 90 a 99	
		"dup_x1", "dup_x2", "dup2", "dup2_x1", "dup2_x2", "swap", "iadd", "ladd", "fadd", "dadd",
		// 100 a 109
		"isub", "lsub", "fsub", "dsub", "imul", "lmul", "fmul", "dmul", "idiv", "ldiv",
		// 110 a 119
		"fdiv", "ddiv", "irem", "lrem", "frem", "drem", "ineg", "lneg", "fneg", "dneg",
		// 120 a 129
		"ishl", "lshl", "ishr", "lshr", "iushr", "lushr", "iand", "land", "ior", "lor",
		// 130 a 139
		"ixor", "lxor", "iinc", "i2l", "i2f", "i2d", "l2i", "l2f", "l2d", "f2i",
		// 140 a 149
		"f2l", "f2d", "d2i", "d2l", "d2f", "i2b", "i2c", "i2s", "lcmp", "fcmpl",
		// 150 a 159
		"fcmpg", "dcmpl", "dcmpg", "ifeq", "ifne", "iflt", "ifge","ifgt", "ifle", "if_icmpeq",
		// 160 a 169
		"if_icmpne", "if_icmplt", "if_icmpge", "if_icmpgt", "if_icmple", "if_acmpeq", "if_acmpne", "goto", "jsr", "ret",
		// 170 a 179
		"tableswitch", "lookupswitch", "ireturn", "lreturn", "freturn", "dreturn", "areturn", "return", "getstatic", "putstatic",
		// 180 a 189
		"getfield", "putfield", "invokevirtual", "invokespecial", "invokestatic", "invokeinterface", "invokedynamic", "new", "newarray", "anewarray",
		// 190 a 199
		"arraylength", "athrow", "checkcast", "instanceof", "monitorenter", "monitorexit", "wide", "multianewarray", "ifnull", "ifnonnull",
		// 200 a 209
		"goto_w", "jsr_w", "breakpoint", NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
		// 210 a 219 
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		// 220 a 229
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		// 230 a 239
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		// 240 a 249
		NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
		// 250 a 255
		NULL, NULL, NULL, NULL, "impdep1", "impdep2"
};


/*	COMO IMPRIMIR OS VALORES DOS TIPOS u1, u2 e u4

typedef	uint8_t		u1;	// 1 byte
typedef	uint16_t	u2;	// 2 bytes
typedef	uint32_t	u4;	// 4 bytes

usar o header inttypes.h

http://www.cplusplus.com/reference/cinttypes/

usar a macro PRIxN em fprintf();

x = d ou i (signed decimal integer), o (octal), u (unsigned decimal integer), x (unsigned hexadecimal).
N = 8, 16, 32, 64  (bits)

exemplo: u4 magic; FILE * out;

fprintf(out, "magic = %" PRIx32 "\n", magic);   // PRIx32 = imprime 4 bytes como unsigned hexadecimal

*/


void	showClassFile(ClassFile * cf, FILE * out){
	fprintf(out, "\n====================================================================================\n");
	fprintf(out, "\t\t\tLeitor/Exibidor de .class:\n");
	fprintf(out, "====================================================================================\n\n\n");
	
	fprintf(out, "====================================================================================\n");
	showGeneralInformation(cf, out);
	
	fprintf(out, "====================================================================================\n");
	showConstantPool(cf, out);
	
	
	fprintf(out, "====================================================================================\n");
	showInterfaces(cf, out);
	
	fprintf(out, "====================================================================================\n");
	showFields(cf, out);
	
	fprintf(out, "====================================================================================\n");
	showMethods(cf, out);
	
	fprintf(out, "====================================================================================\n");
	showAttributes(NULL, NULL, NULL, cf, out);
	
	fprintf(out, "====================================================================================\n");
}

void	showGeneralInformation(ClassFile * cf,  FILE * out){
	float version;
	cp_info * cp;
	int i;

/*
General Information
	*/
	fprintf(out,"General Information:\n");
	
	fprintf(out, "\tMagic: \t\t\t\t0x%" PRIX32 "\n", cf->magic);
	
	fprintf(out, "\tMinor version: \t\t\t%" PRId16 "\n", cf->minor_version);
	switch(cf->major_version){
		case 45:
			version = 1.1;
			break;
		case 46:
			version = 1.2;
			break;
		case 47:
			version = 1.3;
			break;
		case 48:
			version = 1.4;
			break;
		case 49:
			version = 1.5;
			break;
		case 50:
			version = 1.6;
			break;
		case 51:
			version = 1.7;
			break;
		case 52:
			version = 1.8;
			break;
		default: version = 0.0; // versão não definida
	}
	fprintf(out, "\tMajor version: \t\t\t%" PRIu16 "\t\t[%.1f]\n", cf->major_version, version);
	
	fprintf(out, "\tConstant pool count: \t\t%" PRIu16 "\n", cf->constant_pool_count);
	
	fprintf(out, "\tAccess flags: \t\t\t0x%.4" PRIX16 "\t\t[", cf->access_flags);

	u2	access_flags = cf->access_flags;
	if(access_flags >= 0x400){
		fprintf(out, "abstract ");
		access_flags -= 0x400;
	}
	if(access_flags >= 0x200){
		fprintf(out, "interface ");
		access_flags -= 0x200;
	}
	if(access_flags >= 0x20){
		fprintf(out, "super ");
		access_flags -= 0x20;
	}
	
	if(access_flags >= 0x10){
		fprintf(out, "final ");
		access_flags -= 0x10;
	}
	if(access_flags >= 0x1){
		fprintf(out, "public");
		access_flags -= 0x1;
	}
	fprintf(out,"]\n");
	
	setlocale (LC_ALL, "" );
	
	fprintf(out, "\tThis class: \t\t\tcp_info #%" PRIu16, cf->this_class);
	if(cf->constant_pool[cf->this_class-1].tag != CONSTANT_Class){
		puts("\nERRO: this_class não aponta para uma CONSTANT_Class_info.\n");
		exit(EXIT_FAILURE);
	}
	else{
		u2	name_index;
		name_index = cf->constant_pool[cf->this_class-1].u.Class.name_index;
		cp = cf->constant_pool + name_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: this_class não referencia um name_index válido.\n");
			exit(EXIT_FAILURE);
		}
		else{
			fprintf(out, "\t<");
			PrintConstantUtf8(cp, out);
			fprintf(out, ">\n");
		}
	}
	
	if(!cf->super_class){
		fprintf(out, "\tSuper class: \t\t\tNone\n");	
	}
	else{
		if(cf->constant_pool[cf->super_class-1].tag != CONSTANT_Class){
			puts("\nERRO: super_class não aponta para uma CONSTANT_Class_info.\n");
			exit(EXIT_FAILURE);
		}	
		else{
			fprintf(out, "\tSuper class: \t\t\tcp_info #%" PRIu16 , cf->super_class);
			u2	name_index;
			name_index = cf->constant_pool[cf->super_class-1].u.Class.name_index;
			cp = cf->constant_pool + name_index - 1;
			if(cp->tag != CONSTANT_Utf8){
				puts("\nERRO: super_class não referencia um name_index válido.\n");
				exit(EXIT_FAILURE);
			}
			else{
				fprintf(out, "\t<");
				PrintConstantUtf8(cp, out);
				fprintf(out, ">\n");
			}
		}
	}


	fprintf(out, "\tInterfaces count: \t\t%" PRIu16 "\n", cf->interfaces_count);
	fprintf(out, "\tFields count: \t\t\t%" PRIu16 "\n", cf->fields_count);
	fprintf(out, "\tMethods count: \t\t\t%" PRIu16 "\n", cf->methods_count);
	fprintf(out, "\tAttributes count: \t\t%" PRIu16 "\n", cf->attributes_count);
}


void	PrintConstantUtf8(cp_info * cp, FILE * out){
	char	* string = (char *) cp->u.Utf8.bytes;
	string[cp->u.Utf8.length] = '\0';
	fprintf(out, "%s", string );
}


/*
    Metodo: PrintConstantClass
    Funcao: Imprime no arquivo de saida os valores apontados pelo Constant_Class
    Parametros de entrada: ClassFile * cf, cp_info * cp, FILE * out
    Retorna: void
*/
void    PrintConstantClass(ClassFile * cf, cp_info * cp, FILE * out)
{
    cp_info * cp_aux;
    cp_aux = cf->constant_pool + cp->u.Class.name_index - 1;
    if(cp_aux->tag != CONSTANT_Utf8)
    {
        puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
        exit(EXIT_FAILURE);
    }
    else
    {
        for(u2 i = 0; i < cp_aux->u.Utf8.length; i++)
        {
            fprintf(out, "%c" , cp_aux->u.Utf8.bytes[i]);
        }
    }
}
/*
    Metodo: PrintConstantString
    Funcao: Imprime no arquivo de saida os valores apontados pelo Constant_String
    Parametros de entrada: ClassFile * cf, cp_info * cp, FILE * out
    Retorna: void
*/
void    PrintConstantString(ClassFile * cf, cp_info * cp, FILE * out)
{
    cp_info * cp_aux;
    cp_aux = cf->constant_pool + cp->u.String.string_index - 1;
    if(cp_aux->tag != CONSTANT_Utf8)
    {
        puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
        exit(EXIT_FAILURE);
    }
    else
    {
	PrintConstantUtf8(cp_aux, out);
    }
}
/*
    Metodo: PrintConstantNameType
    Funcao: Imprime no arquivo de saida os valores apontados pelo Constant_NameType
    Parametros de entrada: ClassFile * cf, cp_info * cp, char name_descriptor, FILE * out
    Retorna: void
*/
void    PrintConstantNameType(ClassFile * cf, cp_info * cp, char name_descriptor, FILE * out)
{
    cp_info * cp_aux = cp;
    switch(name_descriptor)
    {
    case 'n':
        cp_aux = cf->constant_pool + cp_aux->u.NameAndType.name_index - 1;
        if(cp_aux->tag != CONSTANT_Utf8)
        {
            puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            for(u2 i = 0; i < cp_aux->u.Utf8.length; i++)
            {
                fprintf(out, "%c" , cp_aux->u.Utf8.bytes[i]);
            }
        }
        break;

    case 'd':
        cp_aux = cf->constant_pool + cp_aux->u.NameAndType.descriptor_index- 1;
        if(cp_aux->tag != CONSTANT_Utf8)
        {
            puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            for(u2 i = 0; i < cp_aux->u.Utf8.length; i++)
            {
                fprintf(out, "%c" , cp_aux->u.Utf8.bytes[i]);
            }
        }
        break;

    }
}
/*
    Metodo: PrintConstantRef (CONSTANT_Fieldref, CONSTANT_Methodref, CONSTANT_InterfaceMethodref)
    Funcao: Imprime no arquivo de saida os valores apontados pelos CONSTANT_Fieldref, CONSTANT_Methodref, CONSTANT_InterfaceMethodref
    Parametros de entrada: ClassFile * cf, cp_info * cp,char index_type, FILE * out
    Retorna: void
*/
void    PrintConstantRef(ClassFile * cf, cp_info * cp,char index_type, FILE * out)
{
    cp_info * cp_aux;
    cp_info * cp_aux2;
    switch(index_type)
    {
    case 'c':
        cp_aux = cf->constant_pool + cp->u.Ref.class_index - 1;
        if(cp_aux->tag != CONSTANT_Class)
        {
            puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Class).\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            cp_aux = cf->constant_pool + cp_aux->u.Class.name_index - 1;
            if(cp_aux->tag != CONSTANT_Utf8)
            {
                puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                for(u2 i = 0; i < cp_aux->u.Utf8.length; i++)
                {
                    fprintf(out, "%c" , cp_aux->u.Utf8.bytes[i]);
                }
            }
        }
        break;

    case 'n':
        cp_aux = cf->constant_pool + cp->u.Ref.name_and_type_index - 1;
        if(cp_aux->tag != CONSTANT_NameAndType)
        {
            puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_NameAndType).\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            cp_aux2 = cp_aux;
            cp_aux = cf->constant_pool + cp_aux->u.NameAndType.name_index - 1;
            if(cp_aux->tag != CONSTANT_Utf8)
            {
                puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                for(u2 i = 0; i < cp_aux->u.Utf8.length; i++)
                {
                    fprintf(out, "%c" , cp_aux->u.Utf8.bytes[i]);
                }
            }
            cp_aux2 = cf->constant_pool + cp_aux2->u.NameAndType.descriptor_index - 1;
            if(cp_aux2->tag != CONSTANT_Utf8)
            {
                puts("\nERRO: valor da tag invalido (Diferente de CONSTANT_Utf8).\n");
                exit(EXIT_FAILURE);
            }
            else
            {
                for(u2 i = 0; i < cp_aux2->u.Utf8.length; i++)
                {
                    fprintf(out, "%c" , cp_aux2->u.Utf8.bytes[i]);
                }
            }
        }
        break;

    }

}
/*
    Metodo: showConstantPool
    Funcao: Imprime no arquivo de saida a Constant_Pool do ClassFile
    Parametros de entrada: ClassFile * cf, FILE * out
    Retorna: void
*/
void	showConstantPool(ClassFile * cf, FILE * out){
	double double_number;
	int64_t bits;
	fprintf(out,"Constant Pool:\n");
	cp_info * cp;
	u2	i;
	for(cp = cf->constant_pool, i = 1; cp < (cf->constant_pool + cf->constant_pool_count - 1); cp++, i++){
		switch(cp->tag){
			case	CONSTANT_Class:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Class_info\n", i);
				fprintf(out, "\t\tClass name:\t\tcp_info #%" PRIu16 "\t<", cp->u.Class.name_index);
				PrintConstantClass(cf,cp,out);
				fprintf(out, ">\n\n");
				break;
			case	CONSTANT_Fieldref:
			case	CONSTANT_Methodref:
			case	CONSTANT_InterfaceMethodref:
				if(cp->tag == CONSTANT_Fieldref)
					fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Fieldref_info\n", i); 
				else if(cp->tag == CONSTANT_Methodref)
					fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Methodref_info\n", i);
				else if(cp->tag == CONSTANT_InterfaceMethodref)
					fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_InterfaceMethodref_info\n", i);
					
				fprintf(out, "\t\tClass name:\t\tcp_info #%" PRIu16 "\t<", cp->u.Ref.class_index);
				PrintConstantRef(cf,cp,'c',out);
				fprintf(out, ">\n\t\tName and type:\t\tcp_info #%" PRIu16 "\t<", cp->u.Ref.name_and_type_index);
				PrintConstantRef(cf,cp,'n',out);
				fprintf(out, ">\n\n");
				break;
			case	CONSTANT_String:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_String_info\n", i); 
				fprintf(out, "\t\tString:\t\t\tcp_info #%" PRIu16 "\t<", cp->u.String.string_index);
				PrintConstantString(cf,cp,out);
				fprintf(out, ">\n\n");
				break;
			case	CONSTANT_Integer:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Integer_info\n", i); 
				fprintf(out, "\t\tBytes:\t\t\t0x%.8" PRIx32, cp->u.Integer_Float.bytes);
				fprintf(out, "\n\t\tInteger:\t\t%" PRId32 "\n\n", cp->u.Integer_Float.bytes);
			break;
			case	CONSTANT_Float:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Float_info\n", i); 
				fprintf(out, "\t\tBytes:\t\t\t0x%.8" PRIx32, cp->u.Integer_Float.bytes);
				float	float_number;
				memcpy(&float_number,&cp->u.Integer_Float.bytes, sizeof(cp->u.Integer_Float.bytes));
				fprintf(out, "\n\t\tFloat:\t\t\t%f\n\n", float_number);
				break;
			case	CONSTANT_Long:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Long_info\n", i); 
				fprintf(out, "\t\tHigh bytes:\t\t0x%.8" PRIx32, cp->u.Long_Double.high_bytes);
				fprintf(out, "\n\t\tLow bytes:\t\t0x%.8" PRIx32, cp->u.Long_Double.low_bytes);
				bits = (((int64_t) cp->u.Long_Double.high_bytes) << 32) + cp->u.Long_Double.low_bytes;
				fprintf(out, "\n\t\tLong:\t\t\t%" PRId64 "\n\n", bits);
				fprintf(out, "\t[%" PRIu16 "]\t(large numeric continued)\n\n", ++i);
				cp++;
				break;
			case	CONSTANT_Double:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Double_info\n", i); 
				fprintf(out, "\t\tHigh bytes:\t\t0x%.8" PRIx32, cp->u.Long_Double.high_bytes);
				fprintf(out, "\n\t\tLow bytes:\t\t0x%.8" PRIx32, cp->u.Long_Double.low_bytes);
				bits = (((int64_t) cp->u.Long_Double.high_bytes) << 32) + cp->u.Long_Double.low_bytes;
				switch(bits){
					case 0x7ff0000000000000L:
						fprintf(out, "\n\t\tDouble:\t\t\t+∞\n\n");
						break;
					case 0xfff0000000000000L:
						fprintf(out, "\n\t\tDouble:\t\t\t-∞\n\n");
						break;
					default:
					if((bits >= 0x7ff0000000000001L && bits <= 0x7ffffffffffffL) ||
					(bits >= 0xfff0000000000001L && bits <= 0xffffffffffffffffL )){
						fprintf(out, "\n\t\tDouble:\t\t\tNaN\n\n");
					}
					else{
						int32_t s = ((bits >> 63) == 0) ? 1 : -1;
						int32_t e = ((bits >> 52) & 0x7ffL);
						int64_t m = (e == 0) ?
						(bits & 0xfffffffffffffL) << 1 :
						(bits & 0xfffffffffffffL) | 0x10000000000000L;
						fprintf(out, "\n\t\tDouble:\t\t\t%f\n\n", (double) s*m*pow(2, (e-1075)));
					}
				}
				fprintf(out, "\t[%" PRIu16 "]\t(large numeric continued)\n\n", ++i);
				cp++;
				break;
			case	CONSTANT_NameAndType:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_NameAndType_info\n", i); 
				fprintf(out, "\t\tName:\t\t\tcp_info #%" PRIu16 "\t<", cp->u.NameAndType.name_index);
				PrintConstantNameType(cf,cp,'n',out);
				fprintf(out, ">\n\t\tDescriptor:\t\tcp_info #%" PRIu16 "\t<", cp->u.NameAndType.descriptor_index);
				PrintConstantNameType(cf,cp,'d',out);
				fprintf(out, ">\n\n");
				break;
			case	CONSTANT_Utf8:
				fprintf(out, "\t[%" PRIu16 "]\tCONSTANT_Utf8_info\n", i);
				fprintf(out, "\t\tLength of byte array:\t%" PRIu16, cp->u.Utf8.length);
				fprintf(out, "\n\t\tString:\t\t\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n\n");
				break;
			}
		}
}

void	showInterfaces(ClassFile * cf, FILE * out){
	fprintf(out,"Interfaces:\n\tinterfaces count:\t%" PRIu16 "\n\n", cf->interfaces_count);
	for(u2 i = 0; i < cf->interfaces_count; i++){
		fprintf(out, "\tInterface %" PRIu16, i);
		fprintf(out,"\t\t\tcp_info #%" PRIu16, cf->interfaces[i]);
		if(cf->constant_pool[cf->interfaces[i]-1].tag != CONSTANT_Class){
			puts("\nERRO: interface não referencia uma CONSTANT_Class_info.\n");
			exit(EXIT_FAILURE);
		}
		else{
			u2		name_index;
			cp_info *	cp;
			
			name_index = cf->constant_pool[cf->interfaces[i]-1].u.Class.name_index;
			cp = cf->constant_pool + name_index - 1;
			if(cp->tag != CONSTANT_Utf8){
				puts("\nERRO: interface não referencia um class_name_index válido.\n");
				exit(EXIT_FAILURE);
			}
			else{
				fprintf(out, "\t<");
				PrintConstantUtf8(cp, out);
				fprintf(out, ">\n\n");
			}
		}
	}
}

void	showFields(ClassFile * cf, FILE * out){
	fprintf(out,"Fields:\n\tfields count:\t\t%" PRIu16 "\n", cf->fields_count);
	for(u2 i = 0; i < cf->fields_count; i++){
		fprintf(out,"\n");
		u2		name_index = (cf->fields+i)->name_index;		
		cp_info *	cp = cf->constant_pool + name_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: field não referencia um name_index válido.\n");
			exit(EXIT_FAILURE);
		}
		fprintf(out, "\t[%" PRIu16 "]\t", i);
		PrintConstantUtf8(cp, out);
		fprintf(out, "\n");

		fprintf(out,"\t\tName:\t\t\tcp_info #%" PRIu16, (cf->fields+i)->name_index);
		fprintf(out, "\t<");
		PrintConstantUtf8(cp, out);
		fprintf(out, ">\n");
		
		u2	descriptor_index = (cf->fields+i)->descriptor_index;
		cp = cf->constant_pool + descriptor_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: field não referencia um descriptor_index válido.\n");
			exit(EXIT_FAILURE);
		}
		fprintf(out,"\t\tDescriptor:\t\tcp_info #%" PRIu16, (cf->fields+i)->descriptor_index);
		fprintf(out, "\t<");
		PrintConstantUtf8(cp, out);
		fprintf(out, ">\n");
		
		
		fprintf(out,"\t\tAccess Flags:\t\t0x%.4" PRIx16 "\t\t[", (cf->fields+i)->access_flags);
		u2	access_flags = (cf->fields+i)->access_flags;
		if(access_flags >= 0x80){
			fprintf(out, "transient ");
			access_flags -= 0x80;
		}
		if(access_flags >= 0x40){
			fprintf(out, "volatile");
			access_flags -= 0x40;
		}
		if(access_flags >= 0x10){
			fprintf(out, "final ");
			access_flags -= 0x10;
		}
		if(access_flags >= 0x8){
			fprintf(out, "static ");
			access_flags -= 0x8;
		}
		if(access_flags >= 0x4){
			fprintf(out, "protected ");
			access_flags -= 0x4;
		}
		if(access_flags >= 0x2){
			fprintf(out, "private ");
			access_flags -= 0x2;
		}
		if(access_flags >= 0x1){
			fprintf(out, "public");
			access_flags -= 0x1;
		}
		fprintf(out,"]\n");
		
		fprintf(out,"\n\t\tField's attributes count:\t%" PRIu16 "\n", (cf->fields+i)->attributes_count);
		field_info *	fd_in = (cf->fields+i);
		showAttributes(fd_in, NULL, NULL, cf, out);
	}
	fprintf(out, "\n");
}

void	showMethods(ClassFile * cf, FILE * out){
	fprintf(out,"Methods:\n\tmethods count:\t\t%" PRIu16 "\n", cf->methods_count);
	for(u2 i = 0; i < cf->methods_count; i++){
		fprintf(out,"\n");
		u2		name_index = (cf->methods+i)->name_index;		
		cp_info *	cp = cf->constant_pool + name_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: método não possui um name_index válido.\n");
			exit(EXIT_FAILURE);
		}
		fprintf(out, "\t[%" PRIu16 "]\t", i);
		PrintConstantUtf8(cp, out);
		fprintf(out, "\n");
		
		fprintf(out,"\t\tName:\t\t\tcp_info #%" PRIu16, (cf->methods+i)->name_index);
		fprintf(out, "\t<");
		PrintConstantUtf8(cp, out);
		fprintf(out, ">\n");
		
		fprintf(out,"\t\tDescriptor:\t\tcp_info #%" PRIu16, (cf->methods+i)->descriptor_index);
		u2	descriptor_index = (cf->methods+i)->descriptor_index;
		cp = cf->constant_pool + descriptor_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: método não possui um descriptor_index válido.\n");
			exit(EXIT_FAILURE);
		}
		else{
			fprintf(out, "\t<");
			PrintConstantUtf8(cp, out);
			fprintf(out, ">\n");
		}
		
		fprintf(out,"\t\tAccess Flags:\t\t0x%.4" PRIx16 "\t\t[", (cf->methods+i)->access_flags);
		u2	access_flags = (cf->methods+i)->access_flags;
		if(access_flags >= 0x800){
			fprintf(out, "strict ");
			access_flags -= 0x800;
		}
		if(access_flags >= 0x400){
			fprintf(out, "abstract ");
			access_flags -= 0x400;
		}
		if(access_flags >= 0x100){
			fprintf(out, "native ");
			access_flags -= 0x100;
		}
		if(access_flags >= 0x20){
			fprintf(out, "synchronized ");
			access_flags -= 0x20;
		}
		if(access_flags >= 0x10){
			fprintf(out, "final ");
			access_flags -= 0x10;
		}
		if(access_flags >= 0x8){
			fprintf(out, "static ");
			access_flags -= 0x8;
		}
		if(access_flags >= 0x4){
			fprintf(out, "protected ");
			access_flags -= 0x4;
		}
		if(access_flags >= 0x2){
			fprintf(out, "private ");
			access_flags -= 0x2;
		}
		if(access_flags >= 0x1){
			fprintf(out, "public");
			access_flags -= 0x1;
		}
		fprintf(out,"]\n");
		
		fprintf(out,"\n\t\tMethod's attributes count:\t%" PRIu16 "\n", (cf->methods+i)->attributes_count);
		method_info *	mt_in = (cf->methods+i);
		showAttributes(NULL, mt_in, NULL, cf, out);
	}
}

#define		CLASS		1
#define		METHOD_FIELD	2
#define		ATTRIBUTE	3

#define		T_BOOLEAN	4
#define		T_CHAR		5
#define		T_FLOAT		6
#define		T_DOUBLE	7
#define		T_BYTE		8
#define		T_SHORT		9
#define		T_INT		10
#define		T_LONG		11


void	showAttributes(field_info * fd_in, method_info * mt_in, attribute_info * attr_in, ClassFile * cf, FILE * out){
	u2			attributes_count;
	attribute_info *	attributes;
	int	attribute_tab;
	
	if(fd_in != NULL){		
		attributes_count = fd_in->attributes_count;
		attributes = fd_in->attributes;
		attribute_tab = METHOD_FIELD;
	}	
	else if(mt_in != NULL){		
		attributes_count = mt_in->attributes_count;
		attributes = mt_in->attributes;
		attribute_tab = METHOD_FIELD;	
	}
	else if(attr_in != NULL){
		attributes_count = attr_in->u.Code.attributes_count;
		attributes = attr_in->u.Code.attributes;
		attribute_tab = ATTRIBUTE;
	}
	else if(cf != NULL){
		fprintf(out, "Class's Attributes:");
		fprintf(out,"\n\tClass's attributes count:\t%" PRIu16 "\n", cf->attributes_count);
		attributes_count = cf->attributes_count;
		attributes = cf->attributes;
		attribute_tab = CLASS;
	}
	
	u2	name_index;
	cp_info *	cp;
	attribute_info	* attributes_aux = attributes;
	for(u2 i = 0; i < attributes_count; i++){
		name_index = attributes_aux->attribute_name_index;		
		cp = cf->constant_pool + name_index - 1;
		if(cp->tag != CONSTANT_Utf8){
			puts("\nERRO: atributo não referencia um name_index válido.\n");
			exit(EXIT_FAILURE);
		}
		
		switch(attribute_tab){
			case	CLASS:
				fprintf(out,"\n");
				fprintf(out, "\t[%" PRIu16 "]\t", i);
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\tGeneric info:\n");
				fprintf(out, "\t\t\tAttribute name index:\tcp_info #%" PRIu16, attributes_aux->attribute_name_index);
				fprintf(out, "\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\t\tAttribute length:\t%" PRIu16 "\n", attributes_aux->attribute_length);
				fprintf(out, "\t\tSpecific info:\n");
				break;
			case	METHOD_FIELD:
				fprintf(out,"\n");
				fprintf(out, "\t\t[%" PRIu16 "]", i);
				fprintf(out, "\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\t\tGeneric info:\n");
				fprintf(out, "\t\t\t\tAttribute name index:\tcp_info #%" PRIu16, attributes_aux->attribute_name_index);
				fprintf(out, "\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\t\t\tAttribute length:\t%" PRIu16 "\n", attributes_aux->attribute_length);
				fprintf(out, "\t\t\tSpecific info:\n");
				break;
			case	ATTRIBUTE:
				fprintf(out,"\n");
				fprintf(out, "\t\t\t[%" PRIu16 "]", i);
				fprintf(out, "\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\t\t\tGeneric info:\n");
				fprintf(out, "\t\t\t\t\tAttribute name index:\tcp_info #%" PRIu16, attributes_aux->attribute_name_index);
				fprintf(out, "\t");
				PrintConstantUtf8(cp, out);
				fprintf(out, "\n");
				
				fprintf(out, "\t\t\t\t\tAttribute length:\t%" PRIu16 "\n", attributes_aux->attribute_length);
				fprintf(out, "\t\t\t\tSpecific info:\n");
				break;
		}
		cp_info *	cp_aux;
		int64_t		bits;
		int32_t		branch;
		switch(getAttributeType(attributes_aux, cf)){
			case	CONSTANT_VALUE:
				;
				u2	constantvalue_index = attributes_aux->u.ConstantValue.constantvalue_index;
				fprintf(out, "\t\t\t\tConstant value index:\tcp_info #%" PRIu16, constantvalue_index);
				cp_aux = cf->constant_pool + constantvalue_index - 1;
				switch(cp_aux->tag){
					case	CONSTANT_Integer:
						fprintf(out, "\t<%" PRId32 ">\n", cp_aux->u.Integer_Float.bytes);
						break;
					case	CONSTANT_Long:
						bits = (((int64_t) cp_aux->u.Long_Double.high_bytes) << 32)
							+ cp_aux->u.Long_Double.low_bytes;
						fprintf(out,"\t<%" PRId64 ">\n", bits);
						break;
					case	CONSTANT_Float:
						;
						float	float_number;
						memcpy(&float_number,&cp_aux->u.Integer_Float.bytes, sizeof(u4));
						fprintf(out, "\t<%f>\n", float_number);
						break;
					case	CONSTANT_Double:
						bits = (((int64_t) cp_aux->u.Long_Double.high_bytes) << 32)
								+ cp_aux->u.Long_Double.low_bytes;
						switch(bits){
							case 0x7ff0000000000000L:
								fprintf(out, "\t<+∞>\n");
								break;
							case 0xfff0000000000000L:
								fprintf(out, "\t<-∞>\n");
								break;
							default:
								if((bits >= 0x7ff0000000000001L && 
									bits <= 0x7ffffffffffffL) ||
									(bits >= 0xfff0000000000001L &&
									bits <= 0xffffffffffffffffL )){
									fprintf(out, "\t<NaN>\n");
								}
								else{
									int32_t s = ((bits >> 63) == 0) ? 1 : -1;
									int32_t e = ((bits >> 52) & 0x7ffL);
									int64_t m = (e == 0) ?
										(bits & 0xfffffffffffffL) << 1 :
										(bits & 0xfffffffffffffL) |
											0x10000000000000L;
									fprintf(out, "\t<%f>\n",
									(double) s*m*pow(2, (e-1075)));
								}
						}
						break;
					case	CONSTANT_String:
						fprintf(out,"\t<");
						PrintConstantString(cf, cp_aux, out);
						fprintf(out,">\n");
						break;
				}
				break;
			case	CODE:
				fprintf(out, "\t\t\t\tBytecode:\n");
				fprintf(out,"\t\t\t\t\tline\toffset\n");
				u1 *	nextCode;
				u4	lineNumber = 1;
				u2	index;
				u4	i;
				int32_t default_, n;
				int16_t	const_;
				
				for(u4 offset = 0; offset < attributes_aux->u.Code.code_length;){
					fprintf(out,"\t\t\t\t\t%" PRIu32 "\t%" PRIu32 "\t", lineNumber++, offset);
					nextCode = attributes_aux->u.Code.code + offset;
					switch(*nextCode){
					// 0 bytes
						case	aaload:
						case	aastore:
						case	aconst_null:
						case	aload_0:
						case	aload_1:
						case	aload_2:
						case	aload_3:
						case	areturn:
						case	arraylength:
						case	astore_0:
						case	astore_1:
						case	astore_2:
						case	astore_3:
						case	athrow:
						case	baload:
						case	bastore:
						case	breakpoint:
						case	caload:
						case	castore:
						case	d2f:
						case	d2i:
						case	d2l:
						case	dadd:
						case	daload:
						case	dastore:
						case	dcmpg:
						case	dcmpl:
						case	dconst_0:
						case	dconst_1:
						case	ddiv:
						case	dload_0:
						case	dload_1:
						case	dload_2:
						case	dload_3:
						case	dmul:
						case	dneg:
						case	drem:
						case	dreturn:
						case	dstore_0:
						case	dstore_1:
						case	dstore_2:
						case	dstore_3:
						case	dsub:
						case	dup:
						case	dup_x1:
						case	dup_x2:
						case	dup2:
						case	dup2_x1:
						case	dup2_x2:
						case	f2d:
						case	f2i:
						case	f2l:
						case	fadd:
						case	faload:
						case	fastore:
						case	fcmpg:
						case	fcmpl:
						case	fconst_0:
						case	fconst_1:
						case	fconst_2:
						case	fdiv:
						case	fload_0:
						case	fload_1:
						case	fload_2:
						case	fload_3:
						case	fmul:
						case	fneg:
						case	frem:
						case	freturn:
						case	fstore_0:
						case	fstore_1:
						case	fstore_2:
						case	fstore_3:
						case	fsub:
						case	i2b:
						case	i2c:
						case	i2d:
						case	i2f:
						case	i2l:
						case	i2s:
						case	iadd:
						case	iaload:
						case	iand:
						case	iastore:
						case	iconst_m1:
						case	iconst_0:
						case	iconst_1:
						case	iconst_2:
						case	iconst_3:
						case	iconst_4:
						case	iconst_5:
						case	idiv:
						case	iload_0:
						case	iload_1:
						case	iload_2:
						case	iload_3:
						case	impdep1:
						case	impdep2:
						case	imul:
						case	ineg:
						case	ior:
						case	irem:
						case	ireturn:
						case	ishl:
						case	ishr:
						case	istore_0:
						case	istore_1:
						case	istore_2:
						case	istore_3:
						case	isub:
						case	iushr:
						case	ixor:
						case	l2d:
						case	l2f:
						case	l2i:
						case	ladd:
						case	laload:
						case	land:
						case	lastore:
						case	lcmp:
						case	lconst_0:
						case	lconst_1:
						case	ldiv_:
						case	lload_0:
						case	lload_1:
						case	lload_2:
						case	lload_3:
						case	lmul:
						case	lneg:
						case	lor:
						case	lrem:
						case	lreturn:
						case	lshl:
						case	lshr:
						case	lstore_0:
						case	lstore_1:
						case	lstore_2:
						case	lstore_3:
						case	lsub:
						case	lushr:
						case	lxor:
						case	monitorenter:
						case	monitorexit:
						case	nop:
						case	pop:
						case	pop2:
						case	return_:
						case	saload:
						case	sastore:
						case	swap:
							fprintf(out, "%s\n", opcodes[*nextCode]);
							offset++;
							break;
					// 1 byte
						//	local variable index (u1)
						case	aload:
						case	astore:
						case	dload:
						case	dstore:
						case	fload:
						case	fstore:
						case	iload:
						case	istore:
						case	lload:
						case	lstore:
						case	ret:
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out, "\t%" PRIu8 "\n", *(nextCode+1));
							offset +=2;
							break; 
							
						//	signed integer (s1)
						case	bipush:						
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out, "\t%" PRId8 "\n", *(nextCode+1));
							offset +=2;
							break;
							
						//	array type (u1)
						case	newarray:
							fprintf(out, "%s\t", opcodes[*nextCode]);
							fprintf(out,"%" PRId8, *(nextCode+1));
							switch(*(nextCode+1)){
								case	T_BOOLEAN:
									fprintf(out," (boolean)\n");
									break;
								case	T_CHAR:
									fprintf(out," (char)\n");
									break;
								case	T_FLOAT:
									fprintf(out," (float)\n");
									break;
								case	T_DOUBLE:
									fprintf(out," (double)\n");
									break;
								case	T_BYTE:
									fprintf(out," (byte)\n");
									break;
								case	T_SHORT:
									fprintf(out," (short)\n");
									break;
								case	T_INT:
									fprintf(out," (int)\n");
									break;
								case	T_LONG:
									fprintf(out," (long\n");
									break;
								default:
									printf("ERRO: newarray aplicado à tipo inválido.\n");
									exit(EXIT_FAILURE);
									break;
							}
							offset +=2;
							break;
							
						//	constant pool index (integer, float, string) (u1)
						case	ldc:
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out, "\t#%" PRIu8, *(nextCode+1));
							cp_aux = cf->constant_pool + *(nextCode+1) - 1;
							switch(cp_aux->tag){
								case	CONSTANT_Integer:
									fprintf(out, "\t<%" PRId32 ">\n", cp_aux->u.Integer_Float.bytes);
									break;
								case	CONSTANT_Float:
									;
									float	float_number;
									memcpy(&float_number,&cp_aux->u.Integer_Float.bytes, sizeof(u4));
									fprintf(out, "\t<%f>\n", float_number);
									break;
								case	CONSTANT_String:
									fprintf(out,"\t<");
									PrintConstantString(cf, cp_aux, out);
									fprintf(out,">\n");
									break;
								default:
									puts("ERRO: instrução ldc com argumento inválido\n");
									exit(EXIT_FAILURE);
							}
							offset +=2;
							break;
							
					// 2 bytes
						//	constant_pool index (class) (u2)
						case	anewarray:
						case	checkcast:
						case	instanceof:
						case	new:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16, index);
							cp_aux = cf->constant_pool + index - 1;
							if(cp_aux->tag != CONSTANT_Class){
								printf("\nERRO: %s não referencia um CONSTANT_Class válido.\n",
									opcodes[*nextCode]);
								exit(EXIT_FAILURE);
							}
							else{
								u2	name_index = cp_aux->u.Class.name_index;
								cp_aux = cf->constant_pool + name_index - 1;
									
								fprintf(out, "\t<");
								PrintConstantUtf8(cp_aux, out);
								fprintf(out, ">\n");						
							}
							offset +=3;
							break;	
						
						//	branch offset (s2) 					
						case	goto_: 
						case	if_acmpeq: 
						case	if_acmpne:						
						case	if_icmpeq:
						case	if_icmpge:
						case	if_icmpgt:
						case	if_icmple:
						case	if_icmplt:
						case	if_icmpne:
						case	ifeq:
						case	ifge:
						case	ifgt:
						case	ifle:
						case	iflt:
						case	ifne:
						case	ifnonnull:
						case	ifnull:
						case	jsr: 	
							fprintf(out, "%s", opcodes[*nextCode]);
							branch = ( (int8_t) *(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t%" PRId32 "\t(%+" PRId32 ")\n", (int32_t) offset + branch, branch);
							offset +=3;
							break;							
						
						//	constant pool index (integer, float, string) (u2)			
						case	ldc_w:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16 "\n", index);
							cp_aux = cf->constant_pool + *(nextCode+1) - 1;
							switch(cp_aux->tag){
								case	CONSTANT_Integer:
									fprintf(out, "\t<%" PRId32 ">\n", cp_aux->u.Integer_Float.bytes);
									break;
								case	CONSTANT_Float:
									;
									float	float_number;
									memcpy(&float_number,&cp_aux->u.Integer_Float.bytes, sizeof(u4));
									fprintf(out, "\t<%f>\n", float_number);
									break;
								case	CONSTANT_String:
									fprintf(out,"\t<");
									PrintConstantString(cf, cp_aux, out);
									fprintf(out,">\n");
									break;
								default:
									puts("ERRO: instrução ldc_w com argumento inválido\n");
									exit(EXIT_FAILURE);
							}
							offset +=3;
							break;	
						
						//	constant pool index (fieldref; methodref) (u2)
						case	getfield:
						case	getstatic:
						case	invokespecial:
						case	invokestatic:
						case	invokevirtual:
						case	putfield:
						case	putstatic:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16, index);
							cp_aux = cf->constant_pool + index - 1;
							if(cp_aux->tag != CONSTANT_Methodref && cp_aux->tag != CONSTANT_Fieldref){
								printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",
									opcodes[*nextCode]);
								exit(EXIT_FAILURE);
							}
							else{
								u2	class_index = cp_aux->u.Ref.class_index;
								u2	name_and_type_index = cp_aux->u.Ref.name_and_type_index;
								
								cp_aux = cf->constant_pool + class_index - 1;
								if(cp_aux->tag != CONSTANT_Class){
									printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",
									opcodes[*nextCode]);
									exit(EXIT_FAILURE);
								}
								else{
									u2	name_index = cp_aux->u.Class.name_index;
									cp_aux = cf->constant_pool + name_index - 1;
									
									fprintf(out, "\t<");
									PrintConstantUtf8(cp_aux, out);
									
									cp_aux = cf->constant_pool + name_and_type_index - 1;
									if(cp_aux->tag != CONSTANT_NameAndType){
										printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",
											opcodes[*nextCode]);
										exit(EXIT_FAILURE);
									}
									else{
										name_index = cp_aux->u.NameAndType.name_index;
										cp_aux = cf->constant_pool + name_index - 1;
										
										fprintf(out, ".");
										PrintConstantUtf8(cp_aux, out);
										fprintf(out, ">\n");
									}
								}
								
							}							
							offset +=3;
							break;
							
						//	constant pool index (long, double) (u2)
						case	ldc2_w:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16, index);
							cp_aux = cf->constant_pool + index - 1;
							switch(cp_aux->tag){
								case	CONSTANT_Long:
									bits = (((int64_t) cp_aux->u.Long_Double.high_bytes) << 32)
											+ cp_aux->u.Long_Double.low_bytes;
									fprintf(out,"\t<%" PRId64 ">\n", bits);
									break;
								case	CONSTANT_Double:
									bits = (((int64_t) cp_aux->u.Long_Double.high_bytes) << 32)
											+ cp_aux->u.Long_Double.low_bytes;
									switch(bits){
										case 0x7ff0000000000000L:
											fprintf(out, "\t<+∞>\n");
											break;
										case 0xfff0000000000000L:
											fprintf(out, "\t<-∞>\n");
											break;
										default:
											if((bits >= 0x7ff0000000000001L && 
											bits <= 0x7ffffffffffffL) ||
											(bits >= 0xfff0000000000001L &&
											bits <= 0xffffffffffffffffL )){
												fprintf(out, "\t<NaN>\n");
											}
											else{
												int32_t s = ((bits >> 63) == 0) ? 1 : -1;
												int32_t e = ((bits >> 52) & 0x7ffL);
												int64_t m = (e == 0) ?
													(bits & 0xfffffffffffffL) << 1 :
													(bits & 0xfffffffffffffL) |
														0x10000000000000L;
												fprintf(out, "\t<%f>\n",
													(double) s*m*pow(2, (e-1075)));
											}
									}
									break;
								default:
									puts("ERRO: instrução ldc2_w com argumento inválido\n");
									exit(EXIT_FAILURE);
							}
							offset +=3;
							break;			
						
						//	signed short (s2)			
						case	sipush:
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out, "\t%" PRId16 "\n", *(nextCode+1));
							offset +=3;
							break;
							
						//	unsigned byte (u1); signed byte (s1)
						case	iinc: // 1 byte 1 byte
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out, "\t%" PRIu8 , *(nextCode+1));	
							fprintf(out, " by %" PRId8 "\n", *(nextCode+2));					
							offset +=3;						
							break;
					// 3 bytes
						// constant_pool index (class) (u2) ; unsigned byte (u1)
						case	multianewarray:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16, index);
							cp_aux = cf->constant_pool + index - 1;
							if(cp_aux->tag != CONSTANT_Class){
								printf("\nERRO: %s não referencia um CONSTANT_Class válido.\n",
									opcodes[*nextCode]);
								exit(EXIT_FAILURE);
							}
							else{
								u2	name_index = cp_aux->u.Class.name_index;
								cp_aux = cf->constant_pool + name_index - 1;
									
								fprintf(out, "\t<");
								PrintConstantUtf8(cp_aux, out);
								fprintf(out, ">");						
							}
							fprintf(out, "\tdim %" PRIu8 "\n", *(nextCode+3));
							offset +=4;
							break;
						
					// 4 bytes
						// branch offset (u4)
						case	goto_w:
						case	jsr_w:
							fprintf(out, "%s", opcodes[*nextCode]);
							branch = ((u4) *(nextCode+1) << 24) + ((u4) *(nextCode+2) << 16) +
									((u4) *(nextCode+3) << 8) + *(nextCode+4);
							fprintf(out, "\t%" PRIu32 "\t(+%" PRIu32 ")\n", offset + branch, branch);
							offset +=5;
							break;
						
						// constant_pool index (u2); unsigned byte (u1); unsigned byte (u1)	
						case	invokeinterface:
						case	invokedynamic:
							fprintf(out, "%s", opcodes[*nextCode]);
							index = (u2) (*(nextCode+1) << 8) + *(nextCode+2);
							fprintf(out, "\t#%" PRIu16, index);
							cp_aux = cf->constant_pool + index - 1;
							if(cp_aux->tag != CONSTANT_InterfaceMethodref && cp_aux->tag != CONSTANT_Methodref){
								printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",
									opcodes[*nextCode]);
								exit(EXIT_FAILURE);
							}
							else{
								u2	class_index = cp_aux->u.Ref.class_index;
								u2	name_and_type_index = cp_aux->u.Ref.name_and_type_index;
								
								cp_aux = cf->constant_pool + class_index - 1;
								if(cp_aux->tag != CONSTANT_Class){
									printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",
									opcodes[*nextCode]);
									exit(EXIT_FAILURE);
								}
								else{
									u2	name_index = cp_aux->u.Class.name_index;
									cp_aux = cf->constant_pool + name_index - 1;
									
									fprintf(out, "\t<");
									PrintConstantUtf8(cp_aux, out);
									
									cp_aux = cf->constant_pool + name_and_type_index - 1;
									if(cp_aux->tag != CONSTANT_NameAndType){
										printf("\nERRO: %s não referencia um CONSTANT_ref válido.\n",opcodes[*nextCode]);
										exit(EXIT_FAILURE);
									}
									else{
										name_index = cp_aux->u.NameAndType.name_index;
										cp_aux = cf->constant_pool + name_index - 1;
										
										fprintf(out, ".");
										PrintConstantUtf8(cp_aux, out);
										fprintf(out, ">");
									}
									fprintf(out,"\t<%" PRIu8  ">\n", *(nextCode+3));
								}								
							}							
							offset +=5;
							break;
							
					// 4 ou + bytes
						//	<0-3 byte pad>; default (s4); n (s4); key1, offset_label1 ... keyn, offset_labeln (s4)
						case	lookupswitch: 
							fprintf(out, "%s", opcodes[*nextCode]);
							i = (offset+1) % 4;
							default_ = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) |
									(*(++i + nextCode) << 8) | *(++i + nextCode);
							
							n = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
								(*(++i + nextCode) << 8) | *(++i + nextCode);
									
							fprintf(out,"\t%" PRId32 "\n", n);
							
							int32_t	key, offset_label;
							for(u4 j = 0; j < n; j++){
								fprintf(out,"\t\t\t\t\t%" PRIu32, lineNumber++);
								key = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
									(*(++i + nextCode) << 8) | *(++i + nextCode);
								fprintf(out, "\t\t\t%" PRId32, key);
								offset_label = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
										(*(++i + nextCode) << 8) | *(++i + nextCode);
								fprintf(out, ":\t%" PRId32 " (%+" PRId32 ")\n",
									offset + offset_label, offset_label);
							}
							fprintf(out, "\t\t\t\t\t%" PRIu32, lineNumber++);
							fprintf(out, "\t\t\tdefault:\t%" PRId32 " (+%" PRId32 ")\n", 
								offset + default_, default_);							
							offset +=i;
							offset++;
							break;

						//	<0-3 byte pad>; default (s4); low (s4); high (s4); label1 ... labeln (s4)
						case	tableswitch: 
							fprintf(out, "%s", opcodes[*nextCode]);
							i = (offset+1) % 4;
							default_ = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) |
									(*(++i + nextCode) << 8) | *(++i + nextCode);
							
							int32_t	low = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
								(*(++i + nextCode) << 8) | *(++i + nextCode);
							
							int32_t	high = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
								(*(++i + nextCode) << 8) | *(++i + nextCode);
								
							fprintf(out, "\t%" PRId32 " to %" PRId32 "\n", low, high);
							
							n = high - low + 1;
							
							int32_t	label;
							for(u4 j = 0; j < n; j++){
								fprintf(out,"\t\t\t\t\t%" PRIu32, lineNumber++);
								label = (*(++i + nextCode) << 24) | (*(++i + nextCode) << 16) | 
									(*(++i + nextCode) << 8) | *(++i + nextCode);
								fprintf(out, "\t\t\t%" PRId32 ":\t%" PRId32 " (%+" PRId32 ")\n",
									 j, offset + label, label);
							}
							fprintf(out, "\t\t\t\t\t%" PRIu32, lineNumber++);
							fprintf(out, "\t\t\tdefault:\t%" PRId32 " (%+" PRId32 ")\n",
									offset + default_, default_);
							offset +=i;
							offset++;
							break;
					// 3 ou 5 bytes
						case	wide:
							fprintf(out, "%s", opcodes[*nextCode]);
							fprintf(out,"\t\t\t\t\t%" PRIu32, lineNumber++);
							fprintf(out, "%s", opcodes[*(nextCode+1)]);
							index = (u2) (*(nextCode+2) << 8) + *(nextCode+3);
							fprintf(out, "\t%" PRIu16, index);
							switch(*(nextCode+1)){
								case	aload:
								case	dload:
								case	iload:
								case	fload:
								case	lload:
								case	astore:
								case	dstore:
								case	istore:
								case	fstore:
								case	lstore:
								case	ret:
									offset +=4;
									break;
								case	iinc:
									const_ = (u2) (*(nextCode+4) << 8) + *(nextCode+5);
									fprintf(out,"\t%" PRId16, const_);
									offset +=6;
									break;
								default:
									printf("ERRO: wide aplicado à uma instrução inválida\n");
									exit(EXIT_FAILURE);
							}
							fprintf(out, "\n");
							break;
					}
				}
				fprintf(out, "\n\t\t\t\tException table length:\t\t%" PRId16 "\n", attributes_aux->u.Code.exception_table_length);
				if(attributes_aux->u.Code.exception_table_length){
					fprintf(out, "\n\t\t\t\tException table:\n");
					fprintf(out, "\t\t\t\tNr.\tstart_pc\tend_pc\thandler_pc\tcatch_type\tverbose\n");
					for(u2 i = 0; i < attributes_aux->u.Code.exception_table_length; i++){
						fprintf(out,"\t\t\t\t%" PRId16, i);
						fprintf(out,"\t%" PRId16,
							(attributes_aux->u.Code.exception_table + i)->start_pc);	
						fprintf(out,"\t\t%" PRId16,
							(attributes_aux->u.Code.exception_table + i)->end_pc);
						fprintf(out,"\t%" PRId16,
							(attributes_aux->u.Code.exception_table + i)->handler_pc);
						fprintf(out,"\t\tcp_info #%" PRId16,	
							(attributes_aux->u.Code.exception_table + i)->catch_type);	
						cp_info	* cp_aux = cf->constant_pool + (attributes_aux->u.Code.exception_table + i)->catch_type - 1;
						PrintConstantClass(cf, cp_aux, out);
						fprintf(out,"\n");
								
					}
				}	
				fprintf(out, "\n\t\t\t\tMiscellaneous:\n");
				fprintf(out, "\t\t\t\t\tMaximum stack depth:\t\t%" PRId16 "\n", attributes_aux->u.Code.max_stack);
				fprintf(out, "\t\t\t\t\tMaximum local variables:\t%" PRId16 "\n", attributes_aux->u.Code.max_locals);
				fprintf(out, "\t\t\t\t\tCode length:\t\t\t%" PRId32 "\n", attributes_aux->u.Code.code_length);
				fprintf(out,"\n\t\t\tCode's attributes count:\t%" PRId16 "\n", attributes_aux->u.Code.attributes_count);
				showAttributes(NULL, NULL, attributes_aux, cf, out);
				break;
			case	DEPRECATED:
				/* */
				break;
			case	EXCEPTIONS:
				fprintf(out, "\t\t\t\tNumber of exceptions: %" PRId16, attributes_aux->u.Exceptions.number_of_exceptions);
				fprintf(out,"\t\t\t\tException index table:\n");
				for(u2 i = 0; i < attributes_aux->u.Exceptions.number_of_exceptions; i++){
					fprintf(out,"\t\t\t\t cp_info #%" PRId16 "\n", attributes_aux->u.Exceptions.exception_index_table[i]);
				}
				break;
			case	INNER_CLASSES:
				fprintf(out, "\t\t\tNumber of classes: %" PRId16 "\n", attributes_aux->u.InnerClasses.number_of_classes);
				fprintf(out, "\t\t\t\tClasses:\n");
				classes_type	* classes_aux = attributes_aux->u.InnerClasses.classes;
				for(u2 i = 0; i < attributes_aux->u.InnerClasses.number_of_classes; i++){
					fprintf(out, "\t\t\t\t\tClass %" PRId16 "\n", i);
					fprintf(out, "\t\t\t\t\t\tinner_class_info_index:\t\tcp_info #%" PRId16 "\t",
						(classes_aux)->inner_class_info_index);
					PrintConstantClass(cf, cf->constant_pool +
							 (classes_aux)->inner_class_info_index - 1, out);
					fprintf(out, "\n\t\t\t\t\t\touter_class_info_index:\t\tcp_info #%" PRId16 "\t",
						(classes_aux)->outer_class_info_index);
					PrintConstantClass(cf, cf->constant_pool +
							 (classes_aux)->outer_class_info_index - 1, out);
					fprintf(out, "\n\t\t\t\t\t\tinner_name_index:\t\tcp_info #%" PRId16 "\t",
						(classes_aux)->inner_name_index);
					PrintConstantUtf8(cf->constant_pool +
							 (classes_aux)->inner_name_index - 1, out);		
					fprintf(out, "\n\t\t\t\t\t\tinner_class_access_flags:\t0x%.4" PRIx16 "\t\t",
						(classes_aux)->inner_class_access_flags);
					u2	access_flags = (classes_aux)->inner_class_access_flags;
					fprintf(out, "[");
					if(access_flags >= 0x400){
						fprintf(out, "abstract ");
						access_flags -= 0x400;
					}
					if(access_flags >= 0x200){
						fprintf(out, "interface ");
						access_flags -= 0x200;
					}
					if(access_flags >= 0x10){
						fprintf(out, "final ");
						access_flags -= 0x10;
					}
					if(access_flags >= 0x8){
						fprintf(out, "static ");
						access_flags -= 0x08;
					}
					if(access_flags >= 0x4){
						fprintf(out, "protected ");
						access_flags -= 0x4;
					}
					if(access_flags >= 0x2){
						fprintf(out, "private ");
						access_flags -= 0x2;
					}
					if(access_flags >= 0x1){
						fprintf(out, "public");
						access_flags -= 0x1;
					}			
					fprintf(out, "]\n");
					classes_aux++;
				}
				break;
			case	LINE_NUMBER_TABLE:
				fprintf(out, "\t\t\t\t\tLine number table length:\t%" PRId16 "\n",
					attributes_aux->u.LineNumberTable.line_number_table_length);
				fprintf(out, "\t\t\t\t\tLine number table:\n");
				fprintf(out, "\t\t\t\t\tNr.\tstart_pc\tline_number\n");
				for(u2 i = 0; i < attributes_aux->u.LineNumberTable.line_number_table_length; i++){
					fprintf(out,"\t\t\t\t\t%" PRId16, i);
					fprintf(out,"\t%" PRId16, ((attributes_aux->u.LineNumberTable.line_number_table)+i)->start_pc);		
					fprintf(out,"\t\t%" PRId16 "\n",
						((attributes_aux->u.LineNumberTable.line_number_table)+i)->line_number);			
				}
				break;
			case	LOCAL_VARIABLE_TABLE:
				fprintf(out, "\t\t\t\tLocal variable table length:\t%" PRId16 "\n",
					attributes_aux->u.LocalVariableTable.local_variable_table_length);
				fprintf(out, "\t\t\t\tLocal variable table:\n");
				fprintf(out, "\t\t\t\tNr.\tstart_pc\tlength\tindex\tname_index\tdescriptor_index\n");
				for(u2 i = 0; i < attributes_aux->u.LocalVariableTable.local_variable_table_length; i++){
					fprintf(out,"\t\t\t\t%" PRId16, i);
					fprintf(out,"\t%" PRId16,
						((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->start_pc);		
					fprintf(out,"\t\t%" PRId16,
						((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->length);
					fprintf(out,"\t%" PRId16,
						((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->index);
					fprintf(out,"\tcp_info #%" PRId16,
						((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->name_index);
					fprintf(out,"\tcp_info #%" PRId16 "\n",
						((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->descriptor_index);
					cp_aux = cf->constant_pool + (((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->name_index) - 1;
					fprintf(out,"\t\t\t\t\t\t\t\t\t");
					PrintConstantUtf8(cp_aux, out);
					fprintf(out,"\t\t");
					cp_aux = cf->constant_pool + (((attributes_aux->u.LocalVariableTable.local_variable_table)+i)->descriptor_index) - 1;
					PrintConstantUtf8(cp_aux, out);	
					fprintf(out, "\n");
							
				}
				break;
			case	SOURCE_FILE:
				fprintf(out, "\t\t\tSource file name index:\tcp_info #%" PRId16,
					attributes_aux->u.SourceFile.sourcefile_index);
				u2	sourcefile_index = attributes_aux->u.SourceFile.sourcefile_index;
				cp_aux = cf->constant_pool + sourcefile_index - 1;
				if(cp_aux->tag != CONSTANT_Utf8){
					puts("\nERRO: SourceFile Attribute não referencia um nome válido.\n");
					exit(EXIT_FAILURE);
				}
				else{
					fprintf(out, "\t");
					PrintConstantUtf8(cp_aux, out);
					fprintf(out, "\n");	
				}
				break;
			case	UNKNOWN:
/*				// IGNORANDO ATRIBUTOS DESCONHECIDOS*/
				break;			
		}
		attributes_aux++;
	}
}
