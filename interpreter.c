// interpreter.c

#include	"interpreter.h"
#include	"classreader.h"
#include	"system.out.h"
#include	"opcode.h"
#include	"debug.h"

#include	<stdlib.h>
#include	<string.h>
#include	<math.h>
#include	<wchar.h>


/*	ARQUIVOS DE TESTE
nop_
	nao definido
Tconst e Tstore
	Exemplos/Tconst_Tstore.class
Tipush
	Exemplos/Tipush.class
ldc_
	Exemplos/ldc_class
*/

int	isWide = 0;

//Chapter 7. Opcode Mnemonics by Opcode
//https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-7.html

// 2.11. Instruction Set Summary
// https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-2.html#jvms-2.11

//6.5. Instructions => AKI TEM UMA EXPLICAÇÃO DETALHADA DE CADA INSTRUÇÃO
//https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5

/*==========================================*/
//	INTERPRETADOR
void	interpreter(METHOD_DATA	* method, THREAD * thread, JVM * jvm){
	thread->program_counter = method->bytecodes;

	#ifdef	DEBUG_INSTRUCAO
	printf("PC\tOPCODE\n");
	#endif

	OPERAND		* operand = (thread->jvm_stack)->operand_stack;


	while(thread->program_counter < (method->bytecodes + method->code_length)){	// enquanto houver instruções

		#ifdef	DEBUG_INSTRUCAO
		printf("%" PRIu8 "\t%s", thread->program_counter - method->bytecodes, opcodes[* thread->program_counter]);
		#endif

		func[* thread->program_counter](method, thread, jvm);

		#ifdef	DEBUG_INSTRUCAO
		puts("");
		#endif
	}
}


/*==========================================*/
//	INSTRUÇÕES

// nop		0x00
// Não faz nada.
void	nop_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.nop*/
	thread->program_counter++;
}

/*	INSTRUÇÕES QUE CARREGAM VALORES NA PILHA	*/
// Tconst	0x01 a 0x0F
//	Carregam constantes na pilha de operandos.
void	Tconst(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aconst_null*/	// Tconst_Tstore.class
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iconst_i*/	// Tconst_Tstore.class
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lconst_l*/	// Tconst_Tstore.class
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fconst_f*/	// Tconst_Tstore.class
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dconst_d*/	// Tconst_Tstore.class
	u4		* value = (u4*) malloc(sizeof(u4));
	float		f = 0.0;
	switch(*thread->program_counter){
		case aconst_null:
		case iconst_m1:
		case iconst_0:
		case iconst_1:
		case iconst_2:
		case iconst_3:
		case iconst_4:
		case iconst_5:
			if(*thread->program_counter == aconst_null){
				value = NULL;
			}
			else if(*thread->program_counter == iconst_m1){
				*value = -1;
			}
			else if(*thread->program_counter == iconst_0){
				*value = 0;
			}
			else if(*thread->program_counter == iconst_1){
				*value = 1;
			}
			else if(*thread->program_counter == iconst_2){
				*value = 2;
			}
			else if(*thread->program_counter == iconst_3){
				*value = 3;
			}
			else if(*thread->program_counter == iconst_4){
				*value = 4;
			}
			else if(*thread->program_counter == iconst_5){
				*value = 5;
			}

			if(!value){
				pushOperand(0, thread->jvm_stack);
			}
			else{
				pushOperand(*value, thread->jvm_stack);
			}
			break;
		case lconst_0:
		case lconst_1:
				*value = 0;
			if(*thread->program_counter == lconst_1){
				*value = 1;
			}
			pushOperand(0, thread->jvm_stack);
			pushOperand(* value, thread->jvm_stack);
			break;
		case fconst_0:
		case fconst_1:
		case fconst_2:
			if(*thread->program_counter == fconst_1){
				f = 1.0;
			}
			else if(*thread->program_counter == fconst_2){
				f = 2.0;
			}
			memcpy(value, &f, sizeof(u4));
			pushOperand(* value, thread->jvm_stack);
			break;
		case dconst_0:
		case dconst_1:
			*value = 0x00000000;
			if(*thread->program_counter == dconst_1){
				*value = 0x3FF00000;
			}
			pushOperand(* value, thread->jvm_stack);

			*value = 0x00000000;
			pushOperand(* value, thread->jvm_stack);
			break;
	}
	thread->program_counter++;
	free(value);
}

// Tipush	0x10 e 0x11
//	carregam inteiros com sinal (short ou byte) na pilha
void	Tipush(METHOD_DATA * method, THREAD * thread, JVM * jvm){ // TESTAR NEGATIVOS
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.bipush*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.sipush*/
	switch(*thread->program_counter){
		case bipush:;
			s1 aux1;
			aux1 = (s1) * (thread->program_counter + 1);
			pushOperand((u4) aux1, thread->jvm_stack);

			#ifdef	DEBUG_INSTRUCAO
			printf("\t%" PRId8, aux1);
			#endif

			break;
		case sipush:;
			u1 high;
			u1 low;
			s2 aux2;
			high = *(thread->program_counter + 1);
			low = * (thread->program_counter + 2);
			aux2 = (high << 8) | low;
			pushOperand((u4) aux2, thread->jvm_stack);

			#ifdef	DEBUG
			printf("\t%" PRId16, aux2);
			#endif

			thread->program_counter++;
			break;
	}
	thread->program_counter	+= 2;
}
// ldc_		0x12 a 0x14
//	carregam constantes single (int, float, literal string) ou double word (long, double) na pilha
//	tem como argumento um indice de 8 ou 16 bits da constant pool
void	ldc_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ldc*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ldc_w*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ldc2_w*/
	OPERAND	* operand = (OPERAND *) malloc(sizeof(OPERAND));
	u4 *value = (u4*) malloc(sizeof(u4));
	u1 high, low;
	u2 string_index, index;
	cp_info	* cp, * cp_aux;
	char * string;
	float f;
	switch(*thread->program_counter)
	{
	case ldc:
	case ldc_w:
	case ldc2_w:
		if(*thread->program_counter == ldc){
			index = (u2) * (thread->program_counter + 1);
		}else if(*thread->program_counter == ldc_w || *thread->program_counter == ldc2_w){
			high = * (thread->program_counter + 1);
			low = * (thread->program_counter + 2);

			index = (high << 8) | low;
			thread->program_counter ++;
		}
		#ifdef	DEBUG_INSTRUCAO
		printf("\t#%" PRIu16, index);
		#endif

		cp = (thread->jvm_stack)->current_constant_pool + index - 1;
		switch(cp->tag)	{
			case (CONSTANT_Integer):

				#ifdef	DEBUG
				printf("\t<%" PRId32 ">", cp->u.Integer_Float.bytes);
				#endif

			case (CONSTANT_Float):
				if(cp->tag == CONSTANT_Float){
					float aux;
					memcpy(&aux, &(cp->u.Integer_Float.bytes), sizeof(u4));

					#ifdef	DEBUG
					printf("\t<%E>", aux);
					#endif

				}
				*value = cp->u.Integer_Float.bytes;
				pushOperand(* value, thread->jvm_stack);
				break;
			case (CONSTANT_String):
				cp_aux = (thread->jvm_stack)->current_constant_pool + cp->u.String.string_index - 1;
				string = (char *) cp_aux->u.Utf8.bytes;
				string[cp_aux->u.Utf8.length] = '\0';

				#ifdef	DEBUG
				printf("\t<%s>", string);
				#endif

				*value = (u4) string;
				pushOperand(* value, thread->jvm_stack);
				break;
			case (CONSTANT_Long):

				#ifdef	DEBUG
				printf("\t<%" PRId64 ">", ((s8) cp->u.Long_Double.high_bytes << 32) | cp->u.Long_Double.low_bytes);
				#endif

			case (CONSTANT_Double):
				if(cp->tag == CONSTANT_Double){
					double	aux;
					u8 auxd = ((u8) cp->u.Long_Double.high_bytes << 32) | cp->u.Long_Double.low_bytes;
					memcpy(&aux, &auxd, sizeof(double));

					#ifdef	DEBUG
					printf("\t<%E>", aux);
					#endif
				}
				*value = cp->u.Long_Double.high_bytes;
				pushOperand(* value, thread->jvm_stack);

				*value = cp->u.Long_Double.low_bytes;
				pushOperand(* value, thread->jvm_stack);
				break;
			default:
				printf("VerifyError: invalid index constant pool.\n");
				exit(EXIT_FAILURE);
				break;
		}
		break;
	}
	thread->program_counter += 2;
	free(value);
}

// Tload	0x15 a 0x2D
//	carregam na pilha valores do vetor de variaveis locais
void	Tload(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iload_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lload_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fload_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dload_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aload_n*/
	OPERAND	* operand = (OPERAND *) malloc(sizeof(OPERAND));

	u2 index;
	u4 *value = (u4*) malloc(sizeof(u4));
	switch(*thread->program_counter){
		case iload:
		case lload:
		case fload:
		case dload:
		case aload:
			index = (u2) *(thread->program_counter + 1);
			if (isWide){
				index = index << 8;
				thread->program_counter++;
				index = index | *(thread->program_counter);
				isWide = 0;
			}
			#ifdef	DEBUG
			printf("\t%" PRIu16, index);
			#endif

			*value = (thread->jvm_stack)->local_variables[index];
			pushOperand(* value, thread->jvm_stack);

			if(*thread->program_counter == lload || *thread->program_counter == dload){
				*value = (thread->jvm_stack)->local_variables[index + 1];
				pushOperand(* value, thread->jvm_stack);
			}
			thread->program_counter++;
			break;
		case iload_0:
		case iload_1:
		case iload_2:
		case iload_3:
		case fload_0:
		case fload_1:
		case fload_2:
		case fload_3:
		case aload_0:
		case aload_1:
		case aload_2:
		case aload_3:
			if(*thread->program_counter == iload_0 ||
			*thread->program_counter == fload_0 ||
			*thread->program_counter == aload_0){
				index = 0;
			}
			else if(*thread->program_counter == iload_1 ||
			*thread->program_counter == fload_1 ||
			*thread->program_counter == aload_1){
				index = 1;
			}
			else if(*thread->program_counter == iload_2 ||
			*thread->program_counter == fload_2 ||
			*thread->program_counter == aload_2){
				index = 2;
			}
			else if(*thread->program_counter == iload_3 ||
			*thread->program_counter == fload_3 ||
			*thread->program_counter == aload_3){
				index = 3;
			}

			*value = (thread->jvm_stack)->local_variables[index];
			pushOperand(* value, thread->jvm_stack);

			break;
		case lload_0:
		case lload_1:
		case lload_2:
		case lload_3:
		case dload_0:
		case dload_1:
		case dload_2:
		case dload_3:
			if(*thread->program_counter == lload_0 ||
			 *thread->program_counter == dload_0){
				index = 0;
			}
			else if(*thread->program_counter == lload_1 ||
			 *thread->program_counter == dload_1){
				index = 1;
			}
			else if(*thread->program_counter == lload_2 ||
			 *thread->program_counter == dload_2){
				index = 2;
			}
			else if(*thread->program_counter == lload_3 ||
			 *thread->program_counter == dload_3){
				index = 3;
			}
			*value = (thread->jvm_stack)->local_variables[index];
			pushOperand(* value, thread->jvm_stack);

			*value = (thread->jvm_stack)->local_variables[index + 1];
			pushOperand(* value, thread->jvm_stack);

			break;
	}
	free(value);
	thread->program_counter++;
}

// Taload	0x2E a 0x35
//	carregam na pilha valores de arrays referenciados do vetor.
void	Taload(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iaload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.laload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.faload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.daload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aaload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.baload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.caload*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.saload*/
	u4	index;
	u4	* value = (u4*) malloc(sizeof(u4));
	ARRAY	* arrayref;

	// get index from operand_stack
	index = popOperand(thread->jvm_stack);

	// get reference from operand_stack
	arrayref = (ARRAY *) popOperand(thread->jvm_stack);
	switch(*thread->program_counter){
		case iaload:
			*value = (arrayref->entry)[index].u.Integer.integer;
			break;
		case faload:
			*value = (arrayref->entry)[index].u.Float.float_;
			break;
		case aaload:
			*value = (u4) (arrayref->entry)[index].u.InstanceReference.reference;
			break;
		case baload:
			*value = (arrayref->entry)[index].u.Byte.byte;
			break;
		case caload:
			*value = (arrayref->entry)[index].u.Char.char_;
			break;
		case saload:
			*value = (arrayref->entry)[index].u.Short.short_;
			break;
		case laload:
			*value = (arrayref->entry)[index].u.Long.high_bytes;
			pushOperand(* value, thread->jvm_stack);
			*value = (arrayref->entry)[index].u.Long.low_bytes;
			break;
		case daload:
			*value = (arrayref->entry)[index].u.Double.high_bytes;
			pushOperand(* value, thread->jvm_stack);
			*value = (arrayref->entry)[index].u.Double.low_bytes;
			break;
	}
	pushOperand(* value, thread->jvm_stack);
	free(value);
	thread->program_counter++;
}

/*	INSTRUÇÕES QUE ARMAZENAM VALORES NO VETOR DE VARIAVEIS LOCAIS	*/

// Tstore	0x36 a 0x4E
// grava valores do topo da pilha no vetor de variaveis locais ou em arrays referenciados no vetor.
void	Tstore(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.istore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lstore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fstore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dstore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.astore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.istore_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lstore_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fstore_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dstore_n*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.astore_n*/
    switch(*thread->program_counter)
    {
        case istore:
        case fstore:
        case astore:{
            u2 index;
            u4 value;
            thread->program_counter++;
            index = (u2) *(thread->program_counter);
            // get value from operand_stack
            value = popOperand(thread->jvm_stack);
            // store value into local variables
            (thread->jvm_stack)->local_variables[index] = value;

            thread->program_counter++;
            break;
        }
        case lstore:
        case dstore:{
            u4 high, low;
            u2 index;
            u4 value;

            thread->program_counter++;
            index = (u2) *(thread->program_counter);
            // get high and low from operand_stack
            low = popOperand(thread->jvm_stack);
            high = popOperand(thread->jvm_stack);
            // store high and lowinto local variables
            (thread->jvm_stack)->local_variables[index] = high;
            (thread->jvm_stack)->local_variables[index + 1] = low;

            thread->program_counter++;
            break;
        }
        case istore_0:
        case istore_1:
        case istore_2:
        case istore_3:
        case fstore_0:
        case fstore_1:
        case fstore_2:
        case fstore_3:
        case astore_0:
        case astore_1:
        case astore_2:
        case astore_3:{
            u2 index;
            u4 value;

            if(*thread->program_counter == istore_0 || *thread->program_counter == fstore_0 || *thread->program_counter == astore_0){
                index = 0;
            }else if(*thread->program_counter == istore_1 || *thread->program_counter == fstore_1 || *thread->program_counter == astore_1){
                index = 1;
            }else if(*thread->program_counter == istore_2 || *thread->program_counter == fstore_2 || *thread->program_counter == astore_2){
                index = 2;
            }else if(*thread->program_counter == istore_3 || *thread->program_counter == fstore_3 || *thread->program_counter == astore_3){
                index = 3;
            }
            // get value from operand_stack
            value = popOperand(thread->jvm_stack);
            // store value into local variables
            (thread->jvm_stack)->local_variables[index] = value;

            thread->program_counter++;
            break;
        }
        case lstore_0:
        case lstore_1:
        case lstore_2:
        case lstore_3:
        case dstore_0:
        case dstore_1:
        case dstore_2:
        case dstore_3:{
            u4 high, low;
            u2 index;
            u4 value;

            if(*thread->program_counter == lstore_0 || *thread->program_counter == dstore_0){
                index = 0;
            }else if(*thread->program_counter == lstore_1 || *thread->program_counter == dstore_1){
                index = 1;
            }else if(*thread->program_counter == lstore_2 || *thread->program_counter == dstore_2){
                index = 2;
            }else if(*thread->program_counter == lstore_3 || *thread->program_counter == dstore_3){
                index = 3;
            }
            // get value from operand_stack
            value = popOperand(thread->jvm_stack);
            // store value into local variables
            (thread->jvm_stack)->local_variables[index] = value;

            // get value from operand_stack
            value = popOperand(thread->jvm_stack);
            // store value into local variables
            (thread->jvm_stack)->local_variables[index + 1] = value;

            thread->program_counter++;
            break;
        }
    }
}

// Tastore	0x4F a 0x56
// grava valores do topo da pilha em arrays referenciados no vetor de variaveis locais.
void	Tastore(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.aastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.bastore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.castore*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.sastore*/
	u4 index;
	u4 value, value2;
	ARRAY	* arrayref;

	// get value from operand_stack
	value = popOperand(thread->jvm_stack);
	if(*thread->program_counter == lastore || *thread->program_counter == dastore){
		value2 = popOperand(thread->jvm_stack);
	}

	// get index from operand_stack
	index = popOperand(thread->jvm_stack);

	// get reference from operand_stack
	arrayref = (ARRAY *) popOperand(thread->jvm_stack);

	switch(*thread->program_counter){
		case iastore:
			(arrayref->entry)[index].u.Integer.integer = (s4) value;
			break;
		case fastore:
			(arrayref->entry)[index].u.Float.float_ = value;
			break;
		case astore:
			(arrayref->entry)[index].u.InstanceReference.reference = (OBJECT *) value;
			break;
		case bastore:
			(arrayref->entry)[index].u.Byte.byte = (s1) value;
			break;
		case castore:
			(arrayref->entry)[index].u.Char.char_ = (u2) value;
			break;
		case sastore:
			(arrayref->entry)[index].u.Short.short_ = (s2) value;
			break;
		case lastore:
			(arrayref->entry)[index].u.Long.high_bytes = value;
			(arrayref->entry)[index].u.Long.low_bytes = value2;
			break;
		case dastore:
			(arrayref->entry)[index].u.Double.high_bytes = value;
			(arrayref->entry)[index].u.Double.low_bytes = value2;
			break;
	}
	thread->program_counter++;
}

/*	MANIPULAÇÃO DA PILHA	*/

// handleStack	0x57 a 0x5F	BUG NAS INSTRUÇÕES DUP2
// instruções de manipulação da pilha (descarte, duplicação, troca de valores)
void	handleStack(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.pop*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.pop2*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup_x1*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup_x2*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup2*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup2_x1*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dup2_x2*/
	u4	value;

	u4 value1, value2, value3, value4;
	switch(*thread->program_counter){
		case pop:
			value = popOperand(thread->jvm_stack);
			break;
		case pop2:
			value = popOperand(thread->jvm_stack);
			value = popOperand(thread->jvm_stack);
			break;
		case dup:
			// get value from operand_stack
			value = popOperand(thread->jvm_stack);

			//push value 2x
			pushOperand(value, thread->jvm_stack);
			pushOperand(value, thread->jvm_stack);
			break;
		case dup_x1:
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);

			//push value 1, 2, 1
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			break;
		case dup_x2:
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);
			value3 = popOperand(thread->jvm_stack);

			//push value
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value3, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			break;
		case dup2:
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);

			//push value
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			break;
		case dup2_x1: // BUG SE FOR LONG OU DOUBLE
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);
			value3 = popOperand(thread->jvm_stack);

			//push value
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value3, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			break;
		case dup2_x2:	// BUG SE FOR LONG OU DOUBLE
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);
			value3 = popOperand(thread->jvm_stack);
			value4 = popOperand(thread->jvm_stack);

			//push value
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value4, thread->jvm_stack);
			pushOperand(value3, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			pushOperand(value1, thread->jvm_stack);
			break;
		case swap:
			// get value from operand_stack
			value1 = popOperand(thread->jvm_stack);
			value2 = popOperand(thread->jvm_stack);

			//push value
			pushOperand(value1, thread->jvm_stack);
			pushOperand(value2, thread->jvm_stack);
			break;
	}
	thread->program_counter++;
}

/*	INSTRUÇÕES ARITMETICAS	*/

// Tadd		0x60 a 0x63
//	instruções de adição	// DOUBLE INCOMPLETO
void	Tadd(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iadd*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ladd*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fadd*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dadd*/
	u4	high, low, value;

	switch(*thread->program_counter) {
		// INSTRUÇÃO IADD
		case iadd:;
			s4	first_int, second_int, total_int;
			// Desempilha operandos
			first_int = (s4) popOperand(thread->jvm_stack);
			second_int = (s4) popOperand(thread->jvm_stack);

			// Empilha soma
			total_int = first_int + second_int;
			pushOperand(total_int, thread->jvm_stack);
			break;
		case ladd:;
			s8	first_long, second_long, total_long;
			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_long = ((s8) high << 32) | low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_long = ((s8) high << 32) | low;

			total_long = first_long + second_long;
			high = (u4) (total_long >> 32);
			low = (u4) ((total_long << 32) >> 32);

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);
			break;
		case fadd:;
			float	first_float, second_float, total_float;
			// Desempilha operandos
			value = popOperand(thread->jvm_stack);
			memcpy(&first_float, &value, sizeof(u4));

			value = popOperand(thread->jvm_stack);
			memcpy(&second_float, &value, sizeof(u4));

			total_float = first_float + second_float;
			memcpy(&value, &total_float, sizeof(u4));

			// empilha operandos
			pushOperand(value, thread->jvm_stack);
			break;
		case dadd:;
			u8	first_double, second_double, total_double;
			s4	sign1, sign2, sign_total;
			s4	exponent1, exponent2, exponent_total;
			s8	mantissa1, mantissa2, mantissa_total;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_double = ((u8) high << 32) | low;

			sign1 = ((first_double >> 63) == 0) ? 1 : -1;
			exponent1 = ((first_double >> 52) & 0x7ffL);
			mantissa1 = (exponent1 == 0) ?
						(first_double & 0xfffffffffffffL) << 1 :
						(first_double & 0xfffffffffffffL) | 0x10000000000000L;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_double = ((u8) high << 32) | low;

			sign2 = ((second_double >> 63) == 0) ? 1 : -1;
			exponent2 = ((second_double >> 52) & 0x7ffL);
			mantissa2 = (exponent2 == 0) ?
						(second_double & 0xfffffffffffffL) << 1 :
						(second_double & 0xfffffffffffffL) | 0x10000000000000L;
			if(!mantissa1){
				total_double = second_double;
			}
			else if(!mantissa2){
				total_double = first_double;
			}
			else{
				// reduzir ao maior expoente
				// somar/subtrair as mantissas
				// normalizar o resultado
			}
			break;
	}
	thread->program_counter++;
}

// Tsub		0x64 a 0x67
//	instruções de subtração
void	Tsub(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.isub*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lsub*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fsub*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dsub*/
	u4	high, low, value;

	switch(*thread->program_counter){
		// INSTRUÇÃO ISUB
		case isub:;
			s4	first_int, second_int, total_int;
			// Desempilha operandos
			first_int = (s4) popOperand(thread->jvm_stack);
			second_int = (s4) popOperand(thread->jvm_stack);

			// Empilha diferenca
			total_int = first_int - second_int;
			pushOperand(total_int, thread->jvm_stack);
			break;
		case lsub:;
			s8	first_long, second_long, total_long;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_long = ((s8) high << 32) | low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_long = ((s8) high << 32) | low;

			total_long = first_long - second_long;
			high = (u4) (total_long >> 32);
			low = (u4) ((total_long << 32) >> 32);


			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);
			break;
		case fsub:;
			float	first_float, second_float, total_float;
			// Desempilha operandos
			value = popOperand(thread->jvm_stack);
			memcpy(&first_float, &value, sizeof(u4));

			value = popOperand(thread->jvm_stack);
			memcpy(&second_float, &value, sizeof(u4));

			// Empilha diferenca
			total_float = first_float - second_float;
			memcpy(&value, &total_float, sizeof(u4));
			pushOperand(value, thread->jvm_stack);
			break;
		case dsub:;
			u8	first_double, second_double, total_double;
			s4	sign1, sign2, sign_total;
			s4	exponent1, exponent2, exponent_total;
			s8	mantissa1, mantissa2, mantissa_total;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_double = ((u8) high << 32) | low;

			sign1 = ((first_double >> 63) == 0) ? 1 : -1;
			exponent1 = ((first_double >> 52) & 0x7ffL);
			mantissa1 = (exponent1 == 0) ?
						(first_double & 0xfffffffffffffL) << 1 :
						(first_double & 0xfffffffffffffL) | 0x10000000000000L;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_double = ((u8) high << 32) | low;

			sign2 = ((second_double >> 63) == 0) ? 1 : -1;
			exponent2 = ((second_double >> 52) & 0x7ffL);
			mantissa2 = (exponent2 == 0) ?
						(second_double & 0xfffffffffffffL) << 1 :
						(second_double & 0xfffffffffffffL) | 0x10000000000000L;
			if(!mantissa1){
				total_double = second_double;
				if(sign2 == 1){
					total_double |= 0x8000000000000000L;
				}
				else{
					total_double &= 0X7FFFFFFFFFFFFFFFL;
				}
			}
			else if(!mantissa2){
				total_double = first_double;
			}
			else{
				// reduzir ao maior expoente
				// somar/subtrair as mantissas
				// normalizar o resultado
			}
			break;
	}
	thread->program_counter++;
}

// Tmul		0x68 a 0x6B
//	instruções de multiplicação
void	Tmul(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.imul*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lmul*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fmul*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dmul*/
	u4	high, low, value;

	switch(*thread->program_counter){
		// INSTRUÇÃO IMUL
		case imul:;
			s4	first_int, second_int, total_int;
			// Desempilha operandos
			first_int = (s4) popOperand(thread->jvm_stack);
			second_int = (s4) popOperand(thread->jvm_stack);

			// Empilha produto
			total_int = first_int * second_int;
			pushOperand(total_int, thread->jvm_stack);
			break;
		case lmul:;
			s8	first_long, second_long, total_long;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_long = ((s8) high << 32) | low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_long = ((s8) high << 32) | low;

			total_long = first_long * second_long;
			high = (u4) (total_long >> 32);
			low = (u4) ((total_long << 32) >> 32);

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);
			break;
		case fmul:;
			float	first_float, second_float, total_float;
			// Desempilha operandos
			value = popOperand(thread->jvm_stack);
			memcpy(&first_float, &value, sizeof(u4));

			value = popOperand(thread->jvm_stack);
			memcpy(&second_float, &value, sizeof(u4));

			total_float = first_float * second_float;
			memcpy(&value, &total_float, sizeof(u4));

			// empilha operandos
			pushOperand(value, thread->jvm_stack);
			break;
		case dmul:;
			u8	first_double, second_double, total_double;
			s4	sign1, sign2, sign_total;
			s4	exponent1, exponent2, exponent_total;
			s8	mantissa1, mantissa2, mantissa_total;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_double = ((u8) high << 32) | low;

			sign1 = ((first_double >> 63) == 0) ? 1 : -1;
			exponent1 = ((first_double >> 52) & 0x7ffL);
			mantissa1 = (exponent1 == 0) ?
						(first_double & 0xfffffffffffffL) << 1 :
						(first_double & 0xfffffffffffffL) | 0x10000000000000L;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_double = ((u8) high << 32) | low;

			sign2 = ((second_double >> 63) == 0) ? 1 : -1;
			exponent2 = ((second_double >> 52) & 0x7ffL);
			mantissa2 = (exponent2 == 0) ?
						(second_double & 0xfffffffffffffL) << 1 :
						(second_double & 0xfffffffffffffL) | 0x10000000000000L;
			if(!mantissa1 || !mantissa2){
				total_double = 0;
			}
			else{
				// somar os expoentes
				// multiplicar as mantissas
				// normalizar o resultado
			}
			break;
	}
	thread->program_counter++;
}

// Tdiv		0x6C a 0x6F
//	instrucoes de divisão
void	Tdiv(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.idiv*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ldiv*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fdiv*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ddiv*/
	u4	high, low, value;

	switch(*thread->program_counter){
		// INSTRUÇÃO IMUL
		case	idiv:;
			s4	first_int, second_int, total_int;
			// Desempilha operandos
			first_int = (s4) popOperand(thread->jvm_stack);
			second_int = (s4) popOperand(thread->jvm_stack);

			// Empilha produto
			total_int = first_int * second_int;
			pushOperand(total_int, thread->jvm_stack);
			break;
		case	ldiv_:;
			s8	first_long, second_long, total_long;
			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_long = ((s8) high << 32) | low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_long = ((s8) high << 32) | low;

			total_long = first_long - second_long;
			high = (u4) (total_long >> 32);
			low = (u4) ((total_long << 32) >> 32);

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);
			break;
		case fdiv:;
			float	first_float, second_float, total_float;
			// Desempilha operandos
			value = popOperand(thread->jvm_stack);
			memcpy(&first_float, &value, sizeof(u4));

			value = popOperand(thread->jvm_stack);
			memcpy(&second_float, &value, sizeof(u4));

			total_float = first_float * second_float;
			memcpy(&value, &total_float, sizeof(u4));

			// empilha operandos
			pushOperand(value, thread->jvm_stack);
			break;
		case ddiv:;
			u8	first_double, second_double, total_double;
			s4	sign1, sign2, sign_total;
			s4	exponent1, exponent2, exponent_total;
			s8	mantissa1, mantissa2, mantissa_total;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_double = ((u8) high << 32) | low;

			sign1 = ((first_double >> 63) == 0) ? 1 : -1;
			exponent1 = ((first_double >> 52) & 0x7ffL);
			mantissa1 = (exponent1 == 0) ?
						(first_double & 0xfffffffffffffL) << 1 :
						(first_double & 0xfffffffffffffL) | 0x10000000000000L;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_double = ((u8) high << 32) | low;

			sign2 = ((second_double >> 63) == 0) ? 1 : -1;
			exponent2 = ((second_double >> 52) & 0x7ffL);
			mantissa2 = (exponent2 == 0) ?
						(second_double & 0xfffffffffffffL) << 1 :
						(second_double & 0xfffffffffffffL) | 0x10000000000000L;
			if(!mantissa2){
				puts("DivisionByZeroError");
				exit(EXIT_FAILURE);
			}
			else if(!mantissa2){
				total_double = 0;
			}
			else{
				// subtrair os expoentes
				// dividir as mantissas
				// normalizar o resultado
			}
			break;
	}
	thread->program_counter++;
}

// Trem		0x70 a 0x73
//	instruções de resto
void	Trem(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.irem*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lrem*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.frem*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.drem*/
    switch(*thread->program_counter){
        case irem:{
            u4 value1, value2;
            // get value from operand_stack
            value2 = popOperand(thread->jvm_stack);
            value1 =  popOperand(thread->jvm_stack);
            //push value
			pushOperand( (value1 % value2), thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case lrem:{
			s8 long_value1, long_value2;
			u4 high, low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			long_value2 = (s8) high;
			long_value2 <<= 32;
			long_value2 += low;

            low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			long_value1 = (s8) high;
			long_value1 <<= 32;
			long_value1 += low;

			long_value1 = long_value1 % long_value2;

			high = long_value1 >> 32;
			low = long_value1 & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case frem:{
			float float_value1, float_value2;
			u4 value;

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value2, &value, sizeof(u4));

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value1, &value, sizeof(u4));

            float_value1 = fmodf( float_value1 , float_value2);

            memcpy(&value, &float_value1, sizeof(u4));
			pushOperand(value, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case drem:{
            u8 value;
            u4 high, low;
            double double_value1, double_value2;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);

            value = (((u8) high) << 32) + low;

            memcpy(&double_value2, &value, sizeof(u8));

            low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);

            value = (((u8) high) << 32) + low;

            memcpy(&double_value1, &value, sizeof(u8));

            double_value1 = fmod( double_value1 , double_value2 );

            memcpy(&value, &double_value1, sizeof(u8));

			high = value >> 32;
			low = value & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
	}
}

/*	INSTRUÇÕES BIT A BIT	*/
// Tneg		0x74 a 0x77
//	Instruções de Negação
void	Tneg(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ineg*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lneg*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fneg*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dneg*/
    switch(*thread->program_counter){
        case ineg:{
            s4 value;
            // get value from operand_stack
            value = (s4) popOperand(thread->jvm_stack);
            value = -value;
            //push value
			pushOperand( (u4)value, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case lneg:{
			s8 long_value;
			u4 high, low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			long_value = (s8) high;
			long_value <<= 32;
			long_value += low;

			long_value = -long_value;

			high = long_value >> 32;
			low = long_value & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case fneg:{
			float float_value;
			u4 value;

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value, &value, sizeof(u4));

            float_value = -float_value;
            //copy bits from float_value into value
            memcpy(&value, &float_value, sizeof(u4));

			pushOperand(value, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case dneg:{
            u8 double_value;
            u8 mask = 1;
            u4 high, low;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);

            double_value = (((u8) high) << 32) + low;
            double_value |= (mask << 63);

			high = double_value >> 32;
			low = double_value & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
	}
}

// Tshl		0x78 e 0x79
//	Instruções de Deslocamento à Esquerda
void	Tshl(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ishl*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lshl*/
	int32_t operand1, operand2, auxiliar2;
	int64_t auxiliar1;
	u4 auxU4, aux2U4;

	switch(*thread->program_counter) {
		case ishl:
			operand2 = (int32_t) popOperand(thread->jvm_stack);
			operand1 = (int32_t) popOperand(thread->jvm_stack);
			operand2 = operand2 & 0x1f;
			pushOperand((operand1 << operand2),thread->jvm_stack);
			thread->program_counter++;
		break;
		case lshl:

			auxiliar2 	= (signed) popOperand(thread->jvm_stack);
			aux2U4 		= popOperand(thread->jvm_stack);
			auxiliar1 	= (signed) popOperand(thread->jvm_stack);
			auxiliar1 	= auxiliar1 << 32;
			auxiliar1 	|= aux2U4;
			auxiliar2 	= auxiliar2 & 0x3f;
			auxiliar1 	= auxiliar1 << auxiliar2;
			auxU4 		= auxiliar1 >> 32;
			pushOperand(auxU4, thread->jvm_stack);
			auxU4 		= auxiliar1 & 0xffffffff;
			pushOperand(auxU4, thread->jvm_stack);
			thread->program_counter++;
		break;
	}
}

// Tshr		0x7A e 0x7B
//	Instruções de Deslocamento à Direita
void	Tshr(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ishr*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lshr*/
	int32_t operand1, operand2, auxiliar2;
	int64_t auxiliar1;
	u4 auxU4, aux2U4;

	switch(*thread->program_counter) {
		case ishr:
			operand2 = (int32_t) popOperand(thread->jvm_stack);
			operand1 = (int32_t) popOperand(thread->jvm_stack);
			operand2 = operand2 & 0x1f;
			pushOperand((operand1 >> operand2),thread->jvm_stack);
			thread->program_counter++;
		break;
		case lshr:
			auxiliar2 	= (signed) popOperand(thread->jvm_stack);
			aux2U4 		= popOperand(thread->jvm_stack);
			auxiliar1 	= (signed) popOperand(thread->jvm_stack);
			auxiliar1 	= auxiliar1 << 32;
			auxiliar1 	|= aux2U4;
			auxiliar2 	= auxiliar2 & 0x3f;
			auxiliar1 	= auxiliar1 >> auxiliar2;
			auxU4 = auxiliar1 >> 32;
			pushOperand(auxU4, thread->jvm_stack);
			auxU4 		= auxiliar1 & 0xffffffff;
			pushOperand(auxU4, thread->jvm_stack);
			thread->program_counter++;
		break;
	}
}

// Tushr	0x7C e 0x7D
//	Instruções de Deslocamento à Direita, sem sinal
void	Tushr(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iushr*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lushr*/
	int32_t operand1, operand2;
	int64_t auxiliar1, auxiliar2;
	u4 auxU4, aux2U4;

	switch(*thread->program_counter) {
		case iushr:
			operand2 = (int32_t) popOperand(thread->jvm_stack);
			operand1 = (int32_t) popOperand(thread->jvm_stack);
			operand2 = operand2 & 0x1f;
			pushOperand((operand1 >> operand2),thread->jvm_stack);
			thread->program_counter++;
		break;
		case lushr:
			aux2U4 = popOperand(thread->jvm_stack);
			auxiliar2 = popOperand(thread->jvm_stack);
			auxiliar1 = popOperand(thread->jvm_stack);
			auxiliar1 = auxiliar1 << 32;
			auxiliar1 |= auxiliar2;
			aux2U4 = aux2U4 & 0x3f;
			auxiliar1 = auxiliar1 >> aux2U4;
			auxU4 = auxiliar1 >> 32;
			pushOperand(auxU4, thread->jvm_stack);
			auxU4 = auxiliar1 & 0xffffffff;
			pushOperand(auxU4, thread->jvm_stack);
			thread->program_counter++;
		break;
	}
}


// Tand		0x7E e 0x7F
//	Instruções And bit a bit
void	Tand(METHOD_DATA * method, THREAD * thread, JVM * jvm){
		u4 value,aux1,aux2;
		int32_t first_operand, second_operand, result;
		int64_t oper1,oper2;

	OPERAND	* operand = (OPERAND *) malloc(sizeof(OPERAND));
	switch(*thread->program_counter) {

		// INSTRUÇÃO IAND
		case iand:
			// Desempilha operando
		popOperand(thread->jvm_stack);
			first_operand = (int32_t) value;
			// Desempilha operando
		popOperand(thread->jvm_stack);
			second_operand = (int32_t) value;
			// Empilha resultado
		result = first_operand &= second_operand;
		pushOperand(result, thread->jvm_stack);
			thread->program_counter++;
			break;

		//Intrução LAND
		case land:

		//Desempilha aux2
		popOperand(thread->jvm_stack);
			aux2 = (int32_t) value;

		//Desempilha oper1
		popOperand(thread->jvm_stack);
			oper1 = (signed)(int32_t) value;

			oper1 = oper1 << 32;
			oper1 |= aux1;

		//Desempilha aux2
		popOperand(thread->jvm_stack);
			aux2 = (signed)(int32_t) value;

		//Desempilha oper2
		popOperand(thread->jvm_stack);
			oper2 = (signed)(int32_t) value;

			oper2 = oper2 << 32;
			oper2 |= aux2;
			oper1 = oper2 &= oper1;
			aux1 = oper1 >> 32;

		//Empilha aux1
			pushOperand(aux1, thread->jvm_stack);

			aux1 = oper1 & 0xffffffff;

		//Empilha aux1
			pushOperand(aux1, thread->jvm_stack);
			thread->program_counter++;
			break;
}
}

// Tor		0x80 e 0x81
//	Instruções Or bit a bit
void	Tor(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ior*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lor*/
	switch(*thread->program_counter) {
		case ior:;
			u4	first_op, second_op;

			first_op = popOperand(thread->jvm_stack);
			second_op = popOperand(thread->jvm_stack);
			pushOperand((first_op | second_op), thread->jvm_stack);
			break;
		case lor:;
			u4 result, aux;
			s8 first_l, second_l;

			aux = popOperand(thread->jvm_stack);
			first_l = popOperand(thread->jvm_stack);
			first_l = first_l << 32;
			first_l |= aux;
			aux = popOperand(thread->jvm_stack);
			second_l = popOperand(thread->jvm_stack);
			second_l = second_l << 32;
			second_l |= aux;
			first_l = first_l | second_op;
			result = first_l >> 32;
			pushOperand(result, thread->jvm_stack);
			result = first_l & 0xffffffff;
			pushOperand(result, thread->jvm_stack);
			break;
	}
	thread->program_counter++;
}

// Txor		0x82 e 0x83
//	Instruções Xor bit a bit
void	Txor(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ixor*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lxor*/
	switch(*thread->program_counter) {
		case ior:;
			u4	first_op, second_op;

			first_op = popOperand(thread->jvm_stack);
			second_op = popOperand(thread->jvm_stack);
			pushOperand((first_op ^ second_op), thread->jvm_stack);
			break;
		case lor:;
			u4 result, aux;
			s8 first_l, second_l;

			aux = popOperand(thread->jvm_stack);
			first_l = popOperand(thread->jvm_stack);
			first_l = first_l << 32;
			first_l |= aux;
			aux = popOperand(thread->jvm_stack);
			second_l = popOperand(thread->jvm_stack);
			second_l = second_l << 32;
			second_l |= aux;
			first_l = first_l ^ second_op;
			result = first_l >> 32;
			pushOperand(result, thread->jvm_stack);
			result = first_l & 0xffffffff;
			pushOperand(result, thread->jvm_stack);
			break;
	}
	thread->program_counter++;
}

/*	INCREMENTO	*/

// Tinc		0x84
//	Incremento de variável local
void	Tinc(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iinc*/
	u1	index = (u1) *(thread->program_counter + 1);
	s1	const_ = (s1) *(thread->program_counter + 2);
	printf("\t%" PRIu8 "\tby\t%" PRId8, index, const_);
	(thread->jvm_stack)->local_variables[index] += (s4) const_;
	thread->program_counter += 3;
}

/*	CONVERSÕES DE TIPOS	*/

// i2T		0x85 a 0x87; 0x91 a 0x93
//	Converte inteiro pra um outro tipo
void	i2T(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2l*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2f*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2d*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2b*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2c*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.i2s*/
	switch(*thread->program_counter){
		case i2l:{
			u1 isNegative;
			u4 value, high, low;
			s8 long_value;

			value = popOperand(thread->jvm_stack);

			isNegative = (u1)(value >> 31);

			long_value = (s8) value;

			if(isNegative){
					long_value = long_value | 0xffffffff00000000;
			}

			high = long_value >> 32;
			low = long_value & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case i2f:{
			float float_value;
			u4 value;

            value = popOperand(thread->jvm_stack);
            float_value = (float) value;
            //copy bits from float_value into value
            memcpy(&value, &float_value, sizeof(u4));

			pushOperand(value, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case i2d:{
            u8 double_value = 0;
            u4 value,high, low, bits;
            float float_value;

            value = popOperand(thread->jvm_stack);

            float_value = (float) value;

            memcpy(&bits, &float_value, sizeof(u4));
            s4 s = ((bits >> 31) == 0) ? 1 : -1;
            s4 e = ((bits >> 23) & 0xff);
            s4 m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;

            s4 exp = 1023 + (e - 127);

            double_value = ((double_value + s) << 63) + ((double_value + exp) << 52) + ((double_value + m) << 29);

			high = double_value >> 32;
			low = double_value & 0xffffffff;

			pushOperand(high, thread->jvm_stack);
			pushOperand(low, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case i2b:{
			s4 value;
			value = (s4)((s1) popOperand(thread->jvm_stack));

			pushOperand((u4)value, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
		case i2c:
		case i2s:{
			s4 value;
			value = (s4)((s2) popOperand(thread->jvm_stack));

			pushOperand((u4)value, thread->jvm_stack);

			thread->program_counter++;
			break;
		}
	}
}

// l2T		0x88 a 0x8A
//	Converte long pra um outro tipo
void	l2T(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.l2i*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.l2f*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.l2d*/
	switch(*thread->program_counter)
	{
		case l2i:{
			u4 high, low;
			high = popOperand(thread->jvm_stack);
			low = popOperand(thread->jvm_stack);

			pushOperand(low, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case l2f:{
            u8 long_value;
            s8 value;
            u4 high, low, aux;
            float float_value;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);

            value = (((s8) high) << 32) + low;
            long_value = (u8) value;
            float_value = (float) long_value;

			memcpy(&aux, &float_value, sizeof(u4));

			pushOperand(aux, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case l2d:{
            u8 double_value = 0;
            u8 value;
            u4 high, low, bits;
            float float_value;

            low = popOperand(thread->jvm_stack);
            high = popOperand(thread->jvm_stack);

            value = ((u8)(high) << 32) + low;
			float_value = (float) value;

            memcpy(&bits, &float_value, sizeof(u4));
            s4 s = ((bits >> 31) == 0) ? 1 : -1;
            s4 e = ((bits >> 23) & 0xff);
            s4 m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;

            s4 exp = 1023 + (e - 127);

            double_value = ((double_value + s) << 63) + ((double_value + exp) << 52) + ((double_value + m) << 29);

            high = double_value >> 32;
            low = double_value & 0xffffffff;

            pushOperand(high, thread->jvm_stack);
            pushOperand(low, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
    }
}

// f2T		0x8B a 0x8D
//	Converte float pra um outro tipo
void	f2T(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.f2i*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.f2l*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.f2d*/
    switch(*thread->program_counter)
    {
        case f2i:{
            u4 value;
            float float_value;

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value, &value, sizeof(u4));

            value = (u4)((s4)float_value);
            pushOperand(value, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case f2l:{
            u4 value;
            u8 long_value;
            float float_value;

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value, &value, sizeof(u4));

            long_value = (u8) ((s8)float_value);
            pushOperand(long_value, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case f2d:{
            u8 double_value = 0;
            u4 value,high, low, bits;
            float float_value;

            value = popOperand(thread->jvm_stack);

            float_value = (float) value;

            memcpy(&bits, &float_value, sizeof(u4));
            s4 s = ((bits >> 31) == 0) ? 1 : -1;
            s4 e = ((bits >> 23) & 0xff);
            s4 m = (e == 0) ? (bits & 0x7fffff) << 1 : (bits & 0x7fffff) | 0x800000;

            s4 exp = 1023 + (e - 127);

            double_value = ((double_value + s) << 63) + ((double_value + exp) << 52) + ((double_value + m) << 29);

            high = double_value >> 32;
            low = double_value & 0xffffffff;

            pushOperand(high, thread->jvm_stack);
            pushOperand(low, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
    }
}

// d2T		0x8E a 0x90
//	Converte double pra um outro tipo
void	d2T(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.d2i*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.d2l*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.d2f*/
switch(*thread->program_counter)
    {
        case d2i:
        case d2l:
        case d2f:{
            s8 value;
            u4 high, low;
            s4 s, e;
            s8 m;

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);

			value = (((s8) high) << 32) + low;
				switch(value){
					case 0x7ff0000000000000L:
						//printf("\n\t\tDouble:\t\t\t+∞\n\n");
						break;
					case 0xfff0000000000000L:
						//printf("\n\t\tDouble:\t\t\t-∞\n\n");
						break;
					default:
					if((value >= 0x7ff0000000000001L && value <= 0x7ffffffffffffL) ||
					(value >= 0xfff0000000000001L && value <= 0xffffffffffffffffL )){
						//printf("\n\t\tDouble:\t\t\tNaN\n\n");
					}
					else{
						s = ((value >> 63) == 0) ? 1 : -1;
						e = ((value >> 52) & 0x7ffL);
						m = (e == 0) ?
						(value & 0xfffffffffffffL) << 1 :
						(value & 0xfffffffffffffL) | 0x10000000000000L;
						//printf("\n\t\tDouble:\t\t\t%f\n\n", (double) s*m*pow(2, (e-1075)));
					}
				}

            if(*thread->program_counter == d2i){
                s4 int_value;

                int_value = (s4)(s*m*pow(2, (e-1075)));
                pushOperand((u4)int_value, thread->jvm_stack);

            }else if(*thread->program_counter == d2l){
                u8 long_value = (u8)((s8)s*m*pow(2, (e-1075)));

                u4 high = long_value >> 32;
                u4 low = long_value & 0xffffffff;

                pushOperand(high, thread->jvm_stack);
                pushOperand(low, thread->jvm_stack);
            }else if(*thread->program_counter == d2f){
                float float_value;
                u4 value;

                float_value = s*m*pow(2, (e-1075));

                memcpy(&value, &float_value, sizeof(u4));

                pushOperand(value, thread->jvm_stack);
            }
            thread->program_counter++;
            break;
        }
    }
}

/*	COMPARAÇÃO	*/

// Tcmp		0x94
//	comparação tipo integral (long)
void	Tcmp(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lcmp*/
    u8 value1, value2;
    u4 high, low;
    s4 result;

    low = popOperand(thread->jvm_stack);
    high = popOperand(thread->jvm_stack);

    value2 = (((u8) high) << 32) + low;

    low = popOperand(thread->jvm_stack);
    high = popOperand(thread->jvm_stack);

    value1 = (((u8) high) << 32) + low;

	if ( value1 == value2 ){
        result = 0;
	}else if ( value1 > value2 ){
        result = 1;
	}
	else{
        result = -1;
	}

	pushOperand((u4)result, thread->jvm_stack);

	thread->program_counter++;
}

// TcmpOP	0x95 a 0x98
//	comparação ponto flutuante
void	TcmpOP(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fcmpl*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.fcmpg*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dcmpl*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dcmpg*/
switch(*thread->program_counter)
    {
        case fcmpl:
        case fcmpg:{
            s4 result;
            u4 value;
            float float_value1, float_value2;

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value2, &value, sizeof(u4));

            value = popOperand(thread->jvm_stack);
            memcpy(&float_value1, &value, sizeof(u4));

            if ( float_value1 == float_value2 ){
                result = 0;
            }else if ( float_value1 > float_value2 ){
                result = 1;
            }
            else{
                result = -1;
            }

            pushOperand((u4)result, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
        case dcmpl:
        case dcmpg:{
            u8	first_double, second_double;
			double double_value1, double_value2;
			s4 result;
			u4 low, high;

			// desempilha operandos
			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			first_double = ((u8) high << 32) | low;
            memcpy(&double_value2, &first_double, sizeof(u8));

			low = popOperand(thread->jvm_stack);
			high = popOperand(thread->jvm_stack);
			second_double = ((u8) high << 32) | low;
            memcpy(&double_value1, &first_double, sizeof(u8));

			if ( double_value1 == double_value2 ){
                result = 0;
            }else if ( double_value1 > double_value2 ){
                result = 1;
            }
            else{
                result = -1;
            }

            pushOperand((u4)result, thread->jvm_stack);

            thread->program_counter++;
            break;
        }
    }
}

// ifOP		0x99 a 0x9E
//	compara um valor inteiro com zero, e realiza um jump
void	ifOP(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifeq*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifne*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.iflt*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifge*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifgt*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifle*/
    switch(*thread->program_counter)
    {
        case ifeq:
        case ifne:
        case iflt:
        case ifge:
        case ifgt:
        case ifle:{
            u1	branchbyte1 = * (thread->program_counter + 1);
            u1	branchbyte2 = * (thread->program_counter + 2);
            s2	branch = (branchbyte1 << 8) | branchbyte2;

            s4 value = (s4)popOperand(thread->jvm_stack);

            if(*thread->program_counter == ifeq){
                if(value == 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }else if(*thread->program_counter == ifne){
                if(value != 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }else if(*thread->program_counter == iflt){
                if(value < 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }else if(*thread->program_counter == ifge){
                if(value >= 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }else if(*thread->program_counter == ifgt){
                if(value > 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }else if(*thread->program_counter == ifle){
                if(value <= 0){
                    thread->program_counter += branch;
                }else{
                    thread->program_counter += 3;
                }
            }

            break;
        }
    }
}

// if_icmOP	0x9F a 0xA4
//	compara dois valores inteiros, e realiza um jump
void	if_icmOP(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmpeq*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmpne*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmplt*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmpge*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmpgt*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_icmple*/
	u1	branchbyte1 = * (thread->program_counter + 1);
	u1	branchbyte2 = * (thread->program_counter + 2);
	s2	branch = (branchbyte1 << 8) | branchbyte2;

	#ifdef	DEBUG
	printf("\t%" PRId16 "\t(%+" PRId16 ")", (u2) (thread->program_counter - method->bytecodes) + branch, branch);
	#endif

	s4	value2	= (s4) popOperand(thread->jvm_stack);
	s4	value1	= (s4) popOperand(thread->jvm_stack);

	switch(* thread->program_counter){
		case	if_icmpeq:
			if(value1 == value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
        case	if_icmpne:
			if(value1 != value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
        case	if_icmplt:
			if(value1 < value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
		case	if_icmpge:
			if(value1 >= value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
        case	if_icmpgt:
			if(value1 > value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
        case	if_icmple:
			if(value1 <= value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
	}
}

// if_acmOP	0xA5 e 0xA6
//	compara dois valores do tipo reference.
void	if_acmOP(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_acmpeq*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.if_acmpne*/
    u1	branchbyte1 = * (thread->program_counter + 1);
	u1	branchbyte2 = * (thread->program_counter + 2);
	s2	branch = (branchbyte1 << 8) | branchbyte2;

	#ifdef	DEBUG
	printf("\t%" PRId16 "\t(%+" PRId16 ")", (u2) (thread->program_counter - method->bytecodes) + branch, branch);
	#endif

	s4	value2	= (s4) popOperand(thread->jvm_stack);
	s4	value1	= (s4) popOperand(thread->jvm_stack);

	switch(* thread->program_counter){
		case	if_acmpeq:
			if(value1 == value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
        case	if_acmpne:
			if(value1 != value2){
				thread->program_counter += branch;
			}else{
                thread->program_counter += 3;
			}
			break;
	}
}

// jump		0xA7 a 0xA9
//	jumps incondicionais
void	jump(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.goto*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.jsr*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ret*/
    u1	branchbyte1 = * (thread->program_counter + 1);
	u1	branchbyte2 = * (thread->program_counter + 2);
	s2	branch = (branchbyte1 << 8) | branchbyte2;

	switch(* thread->program_counter){
		case	goto_:
            thread->program_counter += branch;
			break;
        case	jsr:
            pushOperand((u4)(thread->program_counter += 3), thread->jvm_stack);
            thread->program_counter += branch;
			break;
        case	ret:{
            thread->program_counter++;
            u2 index = (u2)*(thread->program_counter);

            if (isWide == 1){
                thread->program_counter++;
                index = (index << 8) | *(thread->program_counter);
                isWide = 0;
            }

            *thread->program_counter = (thread->jvm_stack)->local_variables[index];
			break;
        }
	}
}

// switch_	0xAA e 0xAB
//	jump table
void	switch_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.tableswitch*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lookupswitch*/
}

// Treturn	0xAC a 0xB1
//	retorna valor do metodo e retoma a execuçao do método invocador
void	Treturn(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ireturn*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.lreturn*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.freturn*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.dreturn*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.areturn*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.return*/
	FRAME	*	aux = thread->jvm_stack;
	u4	value, value_h, value_l;
	switch(* thread->program_counter){// PARA TESTAR O HELLOWORLD
		case	ireturn:
		case	freturn:
		case	areturn:
			value = popOperand(thread->jvm_stack);
			// retorna a execução ao método invocador;
			thread->jvm_stack = (thread->jvm_stack)->prox;
			free(aux->local_variables);
			if(aux->operand_stack){
				free(aux->operand_stack);
			}
			free(aux);
			pushOperand(value, thread->jvm_stack);
			break;
		case	lreturn:
		case	dreturn:
			value_l = popOperand(thread->jvm_stack);
			value_h = popOperand(thread->jvm_stack);
			// retorna a execução ao método invocador;
			thread->jvm_stack = (thread->jvm_stack)->prox;
			free(aux->local_variables);
			if(aux->operand_stack){
				free(aux->operand_stack);
			}
			free(aux);
			pushOperand(value_h, thread->jvm_stack);
			pushOperand(value_l, thread->jvm_stack);
			break;
		case	return_:;
			// retorna a execução ao método invocador;
			FRAME	*	aux = thread->jvm_stack;
			thread->jvm_stack = (thread->jvm_stack)->prox;
			free(aux->local_variables);
			if(aux->operand_stack){
				free(aux->operand_stack);
			}
			free(aux);
			break;
	}
	thread->program_counter += method->code_length;
}

// accessField	0xB2 a 0xB5
//	manipula fields
void	accessField(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.getstatic*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.putstatic*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.getfield*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.putfield*/

	if((method->bytecodes + method->code_length - 2) <= thread->program_counter){
		puts("VerifyError: instrução sem a quantidade de argumentos correta.");
		exit(EXIT_FAILURE);
	}

	u1	indexbyte1 = *(thread->program_counter + 1);
	u1	indexbyte2 = *(thread->program_counter + 2);
	u2	index = (indexbyte1 << 8) | indexbyte2;

	#ifdef	DEBUG_INSTRUCAO
	printf("\t#%" PRIu16, index);
	#endif

	// RESOLUÇÃO DO FIELD
		// Nome da classe do field
	cp_info	* cp_aux = (thread->jvm_stack)->current_constant_pool;
	cp_aux = cp_aux + index - 1; // falta verificar se o indice está nos limites da constant pool
	cp_aux = (thread->jvm_stack)->current_constant_pool + cp_aux->u.Ref.class_index - 1;
	cp_info	* cp_class_name = (thread->jvm_stack)->current_constant_pool + cp_aux->u.Class.name_index - 1;
	char	* class_name = cp_class_name->u.Utf8.bytes;
	class_name[cp_class_name->u.Utf8.length] = '\0';



		// nome do field
	cp_aux = (thread->jvm_stack)->current_constant_pool;
	cp_aux = cp_aux + index - 1;
	cp_aux = (thread->jvm_stack)->current_constant_pool + cp_aux->u.Ref.name_and_type_index - 1;
	cp_info	* cp_field_name = (thread->jvm_stack)->current_constant_pool + cp_aux->u.NameAndType.name_index - 1;
	char	* field_name = cp_field_name->u.Utf8.bytes;
	field_name[cp_field_name->u.Utf8.length] = '\0';

		// descritor do field
	cp_info * cp_field_descriptor = (thread->jvm_stack)->current_constant_pool + cp_aux->u.NameAndType.descriptor_index - 1;
	char	* field_descriptor = cp_field_descriptor->u.Utf8.bytes;
	field_descriptor[cp_field_descriptor->u.Utf8.length] = '\0';

	#ifdef	DEBUG_INSTRUCAO
	printf("\t<%s.%s>", class_name, field_name);
	#endif

	// CONTROLE DE ACESSO
	u1	* backupPC = thread->program_counter;
	CLASS_DATA	* field_class = getClass(cp_class_name, jvm);
	if(!field_class){// se a classe do field não foi carregada

		#ifdef	DEBUG
		puts("\n");
		#endif

		char	* string = malloc((strlen(class_name) + 7) * sizeof(CHAR));
		strcpy(string, class_name);
		strcat(string, ".class");


		classLoading(string, &field_class, method->class_data, jvm);
		free(string);
		classLinking(field_class, jvm);
		classInitialization(field_class, jvm, thread);
		thread->program_counter = backupPC;

		#ifdef	DEBUG_METODO
		puts("\n=======================");
		printf("Resume\t");
		PrintConstantUtf8((method->class_data)->class_name, stdout);
		char	* method_name = (method->method_name)->u.Utf8.bytes;
		method_name[(method->method_name)->u.Utf8.length] = '\0';
		printf(".%s\n", method_name);
		puts("=======================");
		#endif

	}
	else{
		if(field_class != method->class_data){// Se o Field não for da mesma classe
			// verifica se a classe do field é acessível pelo método corrente
			if(!(field_class->modifiers & ACC_PUBLIC) &
			(field_class->classloader_reference != (method->class_data)->classloader_reference)){
				puts("IllegalAccessError: acesso indevido à classe ou interface.");
				exit(EXIT_FAILURE);
			}
		}
	}

	VARIABLE	* var;
	switch(* thread->program_counter){
		case	getstatic:;
			var = getClassVariable(cp_field_name, field_class);
			if(!var){
				puts("NoSuchFieldError");
				exit(EXIT_FAILURE);
			}
			switch((var->value).type){
				case	BYTE:
					pushOperand((s4) (var->value).u.Byte.byte, thread->jvm_stack);
					break;
				case	CHAR:
					pushOperand((u4) (var->value).u.Char.char_, thread->jvm_stack);
					break;
				case	FLOAT:
					pushOperand((u4) (var->value).u.Float.float_, thread->jvm_stack);
					break;
				case	INT:
					pushOperand((s4) (var->value).u.Integer.integer, thread->jvm_stack);
					break;
				case	REF_INST:
					pushOperand((u4) (var->value).u.ArrayReference.reference, thread->jvm_stack);
					break;
				case	SHORT:
					pushOperand((s4) (var->value).u.Short.short_, thread->jvm_stack);
					break;
				case	BOOLEAN:
					pushOperand((u4) (var->value).u.Boolean.boolean, thread->jvm_stack);
					break;
				case	REF_ARRAY:
					pushOperand((u4) (var->value).u.InstanceReference.reference, thread->jvm_stack);
					break;
				case	DOUBLE:
				case	LONG:;
					if((var->value).type == DOUBLE){
						pushOperand((u4) (var->value).u.Double.high_bytes, thread->jvm_stack);
						pushOperand((u4) (var->value).u.Double.low_bytes, thread->jvm_stack);
					}
					else{
						pushOperand((u4) (var->value).u.Long.high_bytes, thread->jvm_stack);
						pushOperand((u4) (var->value).u.Long.low_bytes, thread->jvm_stack);
					}
					break;
				default:
					puts("VerifyError: descritor de field inválido");
					exit(EXIT_FAILURE);
			}
			break;
		case	putstatic:
			var = getClassVariable(cp_field_name, field_class);
			if(!var){
				puts("NoSuchFieldError");
				exit(EXIT_FAILURE);
			}
			switch((var->value).type){
				case	BYTE:
					(var->value).u.Byte.byte = (s1) popOperand(thread->jvm_stack);
					break;
				case	CHAR:
					(var->value).u.Char.char_ = (u1) popOperand(thread->jvm_stack);
					break;
				case	FLOAT:
					(var->value).u.Float.float_ = popOperand(thread->jvm_stack);
					break;
				case	INT:
					(var->value).u.Integer.integer = (s4) popOperand(thread->jvm_stack);
					break;
				case	REF_INST:
					(var->value).u.InstanceReference.reference = (OBJECT *) popOperand(thread->jvm_stack);
					break;
				case	SHORT:
					(var->value).u.Short.short_ = (s2) popOperand(thread->jvm_stack);
					break;
				case	BOOLEAN:
					(var->value).u.Boolean.boolean = (u1) popOperand(thread->jvm_stack);
					break;
				case	REF_ARRAY:
					(var->value).u.ArrayReference.reference = (ARRAY *) popOperand(thread->jvm_stack);

					break;
				case	DOUBLE:
					(var->value).u.Double.high_bytes = popOperand(thread->jvm_stack);
					(var->value).u.Double.low_bytes = popOperand(thread->jvm_stack);
					break;
				case	LONG:
					(var->value).u.Long.high_bytes = popOperand(thread->jvm_stack);
					(var->value).u.Long.low_bytes = popOperand(thread->jvm_stack);
					break;
				default:
					puts("VerifyError: descritor de field inválido");
					exit(EXIT_FAILURE);
			}
			break;
		case	getfield:
			break;
		case	putfield:;
			VALUE	value;
			switch(* field_descriptor){
				case	BYTE:
					value.u.Byte.byte = (s1) popOperand(thread->jvm_stack);
					break;
				case	CHAR:
					value.u.Char.char_ = (u1) popOperand(thread->jvm_stack);
					break;
				case	FLOAT:
					value.u.Float.float_ = popOperand(thread->jvm_stack);
					break;
				case	INT:
					value.u.Integer.integer = (s4) popOperand(thread->jvm_stack);
					break;
				case	REF_INST:
					value.u.InstanceReference.reference = (OBJECT *) popOperand(thread->jvm_stack);
					break;
				case	SHORT:
					value.u.Short.short_ = (s2) popOperand(thread->jvm_stack);
					break;
				case	BOOLEAN:
					value.u.Boolean.boolean = (u1) popOperand(thread->jvm_stack);
					break;
				case	REF_ARRAY:
					value.u.ArrayReference.reference = (ARRAY *) popOperand(thread->jvm_stack);

					break;
				case	DOUBLE:
					value.u.Double.high_bytes = popOperand(thread->jvm_stack);
					value.u.Double.low_bytes = popOperand(thread->jvm_stack);
					break;
				case	LONG:
					value.u.Long.high_bytes = popOperand(thread->jvm_stack);
					value.u.Long.low_bytes = popOperand(thread->jvm_stack);
					break;
				default:
					puts("VerifyError: descritor de field inválido");
					exit(EXIT_FAILURE);
			}
			OBJECT	* objectref = (OBJECT *) popOperand(thread->jvm_stack);
			if(!objectref){
				puts("NullPointerException");
				exit(EXIT_FAILURE);
			}
			var = getInstanceVariable(cp_field_name, objectref);
			if(!var){
				puts("NoSuchFieldError");
				exit(EXIT_FAILURE);
			}
			var->value = value;
			break;
	}

	thread->program_counter += 3;
}

// invoke	0xB6 a 0xBA
//	invocação de métodos
void	invoke(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokevirtual*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokespecial*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokestatic*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokeinterface*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.invokedynamic*/

	u1	indexbyte1 = *(thread->program_counter + 1);
	u1	indexbyte2 = *(thread->program_counter + 2);
	u2	index = (indexbyte1 << 8) | indexbyte2;
	
	#ifdef	DEBUG_INSTRUCAO
	printf("\t#%" PRIu16, index);
	#endif
	
/*	https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-5.html#jvms-5.4.3.3*/
	// RESOLUÇÃO DO MÈTODO

	// nome da classe do método
	cp_info	* cp = (thread->jvm_stack)->current_constant_pool;
	cp_info * cp_method_ref = cp + index - 1; // falta verificar se o indice está nos limites da constant pool
	cp_info * cp_class = cp + cp_method_ref->u.Ref.class_index - 1;
	cp_info	* cp_class_name = cp + cp_class->u.Class.name_index - 1;
	char	* class_name = cp_class_name->u.Utf8.bytes;
	class_name[cp_class_name->u.Utf8.length] = '\0';

	// nome do método
	cp_info	* cp_name_and_type = cp + cp_method_ref->u.Ref.name_and_type_index - 1;
	cp_info * cp_method_name = cp + cp_name_and_type->u.NameAndType.name_index - 1;

	char	* method_name = cp_method_name->u.Utf8.bytes;
	method_name[cp_method_name->u.Utf8.length] = '\0';

	#ifdef	DEBUG_INSTRUCAO
	printf("\t<%s.%s>\n", class_name, method_name);
	#endif

	// descritor do método
	cp_info	* cp_method_descriptor = cp + cp_name_and_type->u.NameAndType.descriptor_index - 1;
	char	* method_descriptor = cp_method_descriptor->u.Utf8.bytes;
	method_descriptor[cp_method_descriptor->u.Utf8.length] = '\0';

	// CONTROLE DE ACESSO
/*	https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-5.html#jvms-5.4.4*/
	u1	* backupPC = thread->program_counter;
	CLASS_DATA	* method_class = getClass(cp_class_name, jvm);
	if(!method_class){// se a classe do método não foi carregada, mesmo package

		#ifdef	DEBUG
		puts("");
		#endif

		char	* string = malloc((strlen(class_name) + 7) * sizeof(CHAR));
		strcpy(string, class_name);
		strcat(string, ".class");

		classLoading(string, &method_class, method->class_data, jvm);
		free(string);
		classLinking(method_class, jvm);
		classInitialization(method_class, jvm, thread);
		thread->program_counter = backupPC;
		
		#ifdef	DEBUG_METODO
		puts("=======================");
		printf("Resume\t");
		PrintConstantUtf8((method->class_data)->class_name, stdout);
		char	* current_method_name = (method->method_name)->u.Utf8.bytes;
		current_method_name[(method->method_name)->u.Utf8.length] = '\0';
		printf(".%s\n", current_method_name);
		puts("=======================");
		#endif
	}
	bool	is_print = false;
	bool	is_append = false;
	bool	is_toString = false;

	METHOD_DATA	* invoked_method = getMethod(method_name, method_descriptor, method_class);

	if(!invoked_method){
		puts("AbstractMethodError: método não encontrado");
		exit(EXIT_FAILURE);
	}

	if(!(invoked_method->modifiers & ACC_PUBLIC)){// SE O MÉTODO NÃO É PUBLICO
		if(invoked_method->modifiers & ACC_PROTECTED){ // SE O MÉTODO É PROTECTED
			if(method->class_data != method_class){ // Se a classe do método invokado é diferente da classe método corrente
				if(!isSuperClass(method->class_data, method_class)){
				// Se a classe do método invokado é não é subclasse da classe método corrente
					if((method->class_data)->classloader_reference != method_class->classloader_reference){
						if(!(invoked_method->modifiers & ACC_PRIVATE)){
							puts("IllegalAccessError");
							exit(EXIT_FAILURE);
						}
					}
				}
			}
			else if(1/*NOT restricao static*/){
				if(!(invoked_method->modifiers & ACC_PRIVATE)){
					puts("IllegalAccessError");
					exit(EXIT_FAILURE);
				}
			}
			// restriçao static
		}
		else if(!invoked_method->modifiers){// SE O MÉTODO É DEFAULT
			if((method->class_data)->classloader_reference != (method_class)->classloader_reference){
				if(!(invoked_method->modifiers & ACC_PRIVATE)){
					puts("IllegalAccessError");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	else{
		if(!strcmp(class_name, "java/io/PrintStream")){
			bool	ln = false;
			if(!strcmp(method_name, "println")){
				is_print = true;
				ln = true;
			}
			else if(!strcmp(method_name, "print")){
				is_print = true;
			}
			if(is_print){
				switch(method_descriptor[1]){
					case	BOOLEAN:;
						u1	boolean_ = (u1) popOperand(thread->jvm_stack);
						if(boolean_){
							printf("true");
						}
						else{
							printf("false");
						}
						break;
					case	BYTE:
						printf("%" PRId8, (s1) popOperand(thread->jvm_stack));
						break;
					case	CHAR:; // BUG PARA UNICODE CHAR
						u2	char_ = popOperand(thread->jvm_stack);
					
						char * utf8_char = (char *) &char_;
						utf8_char[2] = '\0';
						printf("%s", utf8_char);
						break;
					case	SHORT:
						printf("%" PRId16, (s2) popOperand(thread->jvm_stack));
						break;
					case	INT:
						printf("%" PRId32, (s4) popOperand(thread->jvm_stack));
						break;
					case	FLOAT:;
						u4 float_bits = popOperand(thread->jvm_stack);
						bool	isValidFloat = true;
						switch(float_bits){
							case	0x7f800000:
								printf("+∞");
								isValidFloat = false;
								break;
							case	0xff800000:
								printf("-∞");
								isValidFloat = false;
								break;
							default:
								if((float_bits >= 0x7f800001 && float_bits <= 0x7fffffff) ||
									(float_bits >= 0xff800001 && float_bits <= 0xffffffff)){
									printf("NaN");
									isValidFloat = false;
								}
								
						}
						if(isValidFloat){
							s4 s = ((float_bits >> 31) == 0) ? 1 : -1;
							s4 e = ((float_bits >> 23) & 0xff);
							s4 m = (e == 0) ?
								(float_bits & 0x7fffff) << 1 :
								(float_bits & 0x7fffff) | 0x800000;
							printf("%E", (float) s * m * pow(2,e - 150));
						}
						break;
					case	LONG:;
						u4	long_low_bytes = popOperand(thread->jvm_stack);
						u4	long_high_bytes = popOperand(thread->jvm_stack);
						s8	long_ = ((u8) long_high_bytes << 32) | long_low_bytes;
	
						printf("%" PRId64, long_);
						break;
					case	DOUBLE:;
						u4	double_low_bytes = popOperand(thread->jvm_stack);
						u4	double_high_bytes = popOperand(thread->jvm_stack);
						u8	double_bits = ((u8) double_high_bytes << 32) | double_low_bytes;

						bool	isValidDouble = true;
						switch(double_bits){
							case	0x7ff0000000000000L:
								printf("+∞");
								isValidDouble = false;
								break;
							case	0xfff0000000000000L:
								printf("-∞");
								isValidDouble = false;
								break;
							default:
								if((double_bits >= 0x7ff0000000000001L && double_bits <= 0x7ffffffffffffL) ||
									(double_bits >= 0xfff0000000000001L && double_bits <= 0x7ffffffffffffL)){
									printf("NaN");
									isValidDouble = false;
								}

						}
						if(isValidDouble){
							s4 s = ((double_bits >> 63) == 0) ? 1 : -1;
							s4 e = ((double_bits >> 52) & 0x7ffL);
							s8 m = (e == 0) ?
								(double_bits & 0xfffffffffffffL) << 1 :
								(double_bits & 0xfffffffffffffL) | 0x10000000000000L;
							printf("%E", (double) s * m * pow(2,e - 1075));
						}
						break;
					case	REF_INST: // STRING
						printf("%s", (char *) popOperand(thread->jvm_stack));
				}
				if(ln){
					puts("");
				}
			}
		}
		else if(!strcmp(class_name, "java/lang/StringBuffer")){
			if(!strcmp(method_name, "append")){
				is_append = true;
				char	* append_string;
				u8	length;
				switch(method_descriptor[1]){
					case	BOOLEAN:;
						u1	boolean = (u1) popOperand(thread->jvm_stack);
						
						if(boolean){
							append_string = (char *) malloc(5 * sizeof(char));
							append_string[0] = '\0';
							strcpy(append_string, "true");
						}
						else{
							append_string = (char *) malloc(6 * sizeof(char));
							append_string[0] = '\0';
							strcpy(append_string, "false");
						}
						break;
					case	BYTE:;
						s1	byte = (s1) popOperand(thread->jvm_stack);

						if(byte < 0){
							length = -(byte/10) + 2;
							length++;
						}
						else{
							length = byte/10 + 2;
						}
						append_string = (char *) malloc(length * sizeof(char));
						sprintf(append_string, "%" PRId8, byte);
						break;
					case	CHAR:;
						u2	char_ = (u2) popOperand(thread->jvm_stack);
					
						append_string = (char *) malloc(2 * sizeof(char));
						sprintf(append_string, "%c", (char) char_);
						break;
					case	SHORT:;
						s2	short_ = (s2) popOperand(thread->jvm_stack);
						
						if(byte < 0){
							length = -(short_/10) + 2;
							length++;
						}
						else{
							length = short_/10 + 2;
						}
						append_string = (char *) malloc(length * sizeof(char));
						sprintf(append_string, "%" PRId16, short_);
						break;
					case	INT:;
						s4	int_ = (s4) popOperand(thread->jvm_stack);
						
						if(byte < 0){
							length = -(int_/10) + 2;
							length++;
						}
						else{
							length = int_/10 + 2;
						}
						
						append_string = (char *) malloc(length * sizeof(char));
						sprintf(append_string, "%" PRId32, int_);
						break;
					case	LONG:;
						u4	long_low_bytes = popOperand(thread->jvm_stack);
						u4	long_high_bytes = popOperand(thread->jvm_stack);
						s8	long_ = ((u8) long_high_bytes << 32) | long_low_bytes;
						
						if(byte < 0){
							length = -(long_/10) + 2;
							length++;
						}
						else{
							length = long_/10 + 2;
						}
						
						append_string = (char *) malloc(length * sizeof(char));
						sprintf(append_string, "%" PRId64, long_);
						break;
					case	FLOAT:;
						u4 float_bits = popOperand(thread->jvm_stack);
						bool	isValidFloat = true;
						switch(float_bits){
							case	0x7f800000:
								append_string = (char *) malloc(3 * sizeof(char));
								sprintf(append_string, "+∞");
								isValidFloat = false;
								break;
							case	0xff800000:
								append_string = (char *) malloc(3 * sizeof(char));
								sprintf(append_string, "-∞");
								isValidFloat = false;
								break;
							default:
								if((float_bits >= 0x7f800001 && float_bits <= 0x7fffffff) ||
									(float_bits >= 0xff800001 && float_bits <= 0xffffffff)){
									append_string = (char *) malloc(4 * sizeof(char));
									sprintf(append_string, "NaN");
									isValidFloat = false;
								}
								
						}
						if(isValidFloat){
							s4 s = ((float_bits >> 31) == 0) ? 1 : -1;
							s4 e = ((float_bits >> 23) & 0xff);
							s4 m = (e == 0) ?
								(float_bits & 0x7fffff) << 1 :
								(float_bits & 0x7fffff) | 0x800000;
							append_string = (char *) malloc(13 * sizeof(char));
							sprintf(append_string, "%E", (float) s * m * pow(2,e - 150));
						}
						break;
					case	DOUBLE:;
						u4	double_low_bytes = popOperand(thread->jvm_stack);
						u4	double_high_bytes = popOperand(thread->jvm_stack);
						u8	double_bits = ((u8) double_high_bytes << 32) | double_low_bytes;
						
						bool	isValidDouble = true;
						switch(double_bits){
							case	0x7ff0000000000000L:
								append_string = (char *) malloc(3 * sizeof(char));
								sprintf(append_string, "+∞");
								isValidDouble = false;
								break;
							case	0xfff0000000000000L:
								append_string = (char *) malloc(3 * sizeof(char));
								sprintf(append_string, "-∞");
								isValidDouble = false;
								break;
							default:
								if((double_bits >= 0x7ff0000000000001L && double_bits <= 0x7ffffffffffffL) ||
									(double_bits >= 0xfff0000000000001L && double_bits <= 0x7ffffffffffffL)){
									append_string = (char *) malloc(4 * sizeof(char));
									sprintf(append_string, "NaN");
									isValidDouble = false;
								}

						}
						if(isValidDouble){
							s4 s = ((double_bits >> 63) == 0) ? 1 : -1;
							s4 e = ((double_bits >> 52) & 0x7ffL);
							s8 m = (e == 0) ?
								(double_bits & 0xfffffffffffffL) << 1 :
								(double_bits & 0xfffffffffffffL) | 0x10000000000000L;
							append_string = (char *) malloc(14 * sizeof(char));
							sprintf(append_string, "%E", (double) s * m * pow(2,e - 1075));
						}
						break;
					case	REF_INST:
						append_string = (char *) popOperand(thread->jvm_stack);
						break;
				}
				OBJECT * string_buffer = (OBJECT *) popOperand(thread->jvm_stack);
				string_buffer = (OBJECT *) realloc(string_buffer, strlen(append_string) + strlen((char *) string_buffer) + 1);
				strcat((char *) string_buffer, append_string);
				pushOperand((u4) string_buffer, thread->jvm_stack);
				
				#ifdef	DEBUG
				printf("string_buffer:%s", (char *) string_buffer);
				#endif
			}
			else if(!strcmp(method_name, "toString")){
				is_toString = true;
			}
		
		}
	}
	u2	nargs;
	u4	* args; 
	if(!is_print && !is_append && !is_toString){
		// desempilha operandos e coloca no vetor de variaveis locais;
		nargs = 0;
		args = (u4 *) malloc(invoked_method->locals_size * sizeof(u4));
		u2	i = 1;
		u2	locals_size = invoked_method->locals_size;
		
		while(method_descriptor[i] != ')'){
			switch(method_descriptor[i]){
				case	REF_INST:
					while(method_descriptor[i] != ';'){
						i++;
					}
				case	BOOLEAN:
				case	BYTE:
				case	CHAR:
				case	FLOAT:
				case	INT:
				case	SHORT:
					args[locals_size - nargs + 1] = popOperand(thread->jvm_stack);
					nargs++;
					break;
				case	LONG:
				case	DOUBLE:
					args[locals_size - nargs + 1] = popOperand(thread->jvm_stack);
					nargs++;
					args[locals_size - nargs + 1] = popOperand(thread->jvm_stack);
					nargs++;
					break;
				case	REF_ARRAY:
					while(method_descriptor[i] == '['){
						i++;
					}
					switch(method_descriptor[i]){
						case	REF_INST:
							while(method_descriptor[i] != ';'){
								i++;
							}
						case	BOOLEAN:
						case	BYTE:
						case	CHAR:
						case	FLOAT:
						case	INT:
						case	SHORT:
						case	LONG:
						case	DOUBLE:
							args[locals_size - nargs + 1] = popOperand(thread->jvm_stack);
							nargs++;
							break;
					}
					break;
			}
			i++;
		}	
	}
	OBJECT	* objectref;
	CLASS_DATA	* super_class;
	bool		findMethod;
	switch(* thread->program_counter){// PARA TESTAR O HELLOWORLD
		case	invokevirtual:
			if(is_print || is_append || is_toString){ // NATIVE METHOD
				break;
			}
			if((!strcmp(method_name, "<init>")) || (!strcmp(method_name, "<clinit>"))){
				puts("InvokeVirtualInitError");
				exit(EXIT_FAILURE);
			}
			objectref = (OBJECT *) popOperand(thread->jvm_stack);
			if(!objectref){
				puts("NullPointerException");
				exit(EXIT_FAILURE);
			}
			if(invoked_method->modifiers & ACC_STATIC){
				puts("IncompatibleClassChangeError");
				exit(EXIT_FAILURE);
			}
			
			CLASS_DATA	* class_objectref = objectref->class_data_reference;
			if((invoked_method->modifiers & ACC_PROTECTED)){
				CLASS_DATA	* super_class = getSuperClass((method->class_data)->classfile, jvm);
				bool	isSuperClass = false;
				while(super_class && !isSuperClass){
					if(method_class == super_class){
						isSuperClass = true;
					}
					else{
						super_class =  getSuperClass(super_class->classfile, jvm);
					}
				}				
				if(isSuperClass && ((invoked_method->class_data)->classloader_reference !=
							 (method->class_data)->classloader_reference )){
					if(class_objectref != method->class_data){
						isSuperClass = false;
						super_class = getSuperClass(class_objectref->classfile, jvm);
						while(super_class && !isSuperClass){
							if(method->class_data == super_class){
								isSuperClass = true;
							}
							else{
								super_class = getSuperClass(super_class->classfile, jvm);
							}
						}
						if(!isSuperClass){
							puts("IllegalAccessError: acesso indevido à método protegido");
							exit(EXIT_FAILURE);
						}
					}
				}
			}
			findMethod	 = false;
			super_class = class_objectref;
			while(super_class && !findMethod){
				if(invoked_method = getMethod(method_name, method_descriptor, super_class)){
					if(!(invoked_method->modifiers & ACC_STATIC)){
						backupPC = thread->program_counter;
						if(invoked_method->modifiers & ACC_NATIVE){
							puts("UnsatisfiedLinkError");
							exit(EXIT_FAILURE);
						}
			
						if(invoked_method->modifiers & ACC_ABSTRACT){
							puts("AbstractMethodError: método abstrato");
							exit(EXIT_FAILURE);
						}
						executeMethod(method_name, method_descriptor, super_class, 
								jvm, thread, objectref, nargs, args);
						thread->program_counter = backupPC;
						findMethod = true;
					}
					else{
						super_class = getSuperClass(super_class->classfile, jvm);
					}
				}
				else{
					super_class = getSuperClass(super_class->classfile, jvm);
				}
			}
			if(!findMethod){
				puts("AbstractMethodError:");
				exit(EXIT_FAILURE);
			}

			break;
		case	invokespecial:;
			objectref= (OBJECT *) popOperand(thread->jvm_stack);
			if(!objectref){
				puts("NullPointerException");
				exit(EXIT_FAILURE);
			}

			if(invoked_method->modifiers & ACC_STATIC){
				puts("IncompatibleClassChangeError");
				exit(EXIT_FAILURE);
			}

			if((invoked_method->modifiers & ACC_PROTECTED)){
				CLASS_DATA	* super_class = getSuperClass((method->class_data)->classfile, jvm);
				bool	isSuperClass = false;
				while(super_class && !isSuperClass){
					if(method_class == super_class){
						isSuperClass = true;
					}
					else{
						super_class =  getSuperClass(super_class->classfile, jvm);
					}
				}
				if(isSuperClass && ((invoked_method->class_data)->classloader_reference !=
							 (method->class_data)->classloader_reference )){
					CLASS_DATA	* class_objectref = objectref->class_data_reference;
					if(class_objectref != method->class_data){
						isSuperClass = false;
						super_class = getSuperClass(class_objectref->classfile, jvm);
						while(super_class && !isSuperClass){
							if(method->class_data == super_class){
								isSuperClass = true;
							}
							else{
								super_class = getSuperClass(super_class->classfile, jvm);
							}
						}
						if(!isSuperClass){
							puts("IllegalAccessError: acesso indevido à método protegido");
							exit(EXIT_FAILURE);
						}
					}
				}
			}
			CLASS_DATA	* super_class = getSuperClass((method->class_data)->classfile, jvm);
				bool	isSuperClass = false;
				while(super_class && !isSuperClass){
					if(method_class == super_class){
						isSuperClass = true;
					}
					else{
						super_class =  getSuperClass(super_class->classfile, jvm);
					}
				}
			// se a classe corrente é super;
			// a classe do método invokado é superclasse da classe corrente;
			// e o metodo nao é <init>

			if(((method->class_data)->modifiers & ACC_SUPER) && (isSuperClass) && strcmp(method_name, "<init>")){
				bool	findMethod = false;
				super_class = getSuperClass((method->class_data)->classfile, jvm);
				while(super_class && !findMethod){
					if(invoked_method = getMethod(method_name, method_descriptor, super_class)){
						if(!(invoked_method->modifiers & ACC_STATIC)){
							if(invoked_method->modifiers & ACC_ABSTRACT){
								puts("AbstractMethodError: método abstrato");
								exit(EXIT_FAILURE);
							}
							if(invoked_method->modifiers & ACC_NATIVE){
								puts("UnsatisfiedLinkError");
								exit(EXIT_FAILURE);
							}
						
							backupPC = thread->program_counter;
							executeMethod(method_name, method_descriptor, super_class,
									jvm, thread, objectref, nargs, args);
							thread->program_counter = backupPC;
							findMethod = true;
						}
						else{
							super_class = getSuperClass(super_class->classfile, jvm);
						}
					}
					else{
						super_class = getSuperClass(super_class->classfile, jvm);
					}
				}
				if(!findMethod){
					puts("AbstractMethodError:");
					exit(EXIT_FAILURE);
				}

			}
			else{
				backupPC = thread->program_counter;
				executeMethod(method_name, method_descriptor, method_class, jvm, thread, objectref, nargs, args);
				thread->program_counter = backupPC;
			}


			break;
		case	invokestatic:
			if((!strcmp(method_name, "<init>")) || (!strcmp(method_name, "<clinit>"))){
				puts("InvokeStaticInitError");
				exit(EXIT_FAILURE);
			}
			if(!(invoked_method->modifiers & ACC_STATIC) || (invoked_method->modifiers & ACC_ABSTRACT)){
				puts("IllegalAccessError");
				exit(EXIT_FAILURE);
			}
			
			if(invoked_method->modifiers & ACC_NATIVE){
				puts("UnsatisfiedLinkError");
				exit(EXIT_FAILURE);
			}
			
			backupPC = thread->program_counter;
			executeMethod(method_name, method_descriptor, method_class, jvm, thread, NULL, nargs, args);
			thread->program_counter = backupPC;
			break;
		case	invokeinterface:
			if(cp_method_ref->tag != CONSTANT_InterfaceMethodref){
				puts("VerifyError: invokeinterface invalid method");
				exit(EXIT_FAILURE);
			}
			objectref = (OBJECT *) popOperand(thread->jvm_stack);
			if(!objectref){
				puts("NullPointerException");
				exit(EXIT_FAILURE);
			}
			
			if((!strcmp(method_name, "<init>")) || (!strcmp(method_name, "<clinit>"))){
				puts("InvokeInterfaceInitError");
				exit(EXIT_FAILURE);
			}
			u1	count = * (thread->program_counter + 3);
			if(!count){
				puts("InvokeInterfaceError");
				exit(EXIT_FAILURE);
			}
			
			#ifdef	DEBUG
			printf("\tcount	%" PRIu8, count);
			#endif
			
			u4	fourth_operand = * (thread->program_counter + 4);
			if(fourth_operand){
				printf("\n%" PRIu32, fourth_operand);
				puts("InvokeInterfaceError");
				exit(EXIT_FAILURE);
			}
			class_objectref = objectref->class_data_reference;
			
			findMethod = false;
			super_class = class_objectref;
			while(super_class && !findMethod){
				if(invoked_method = getMethod(method_name, method_descriptor, super_class)){
					if(!(invoked_method->modifiers & ACC_STATIC)){
						backupPC = thread->program_counter;
						if(invoked_method->modifiers & ACC_ABSTRACT){
							puts("AbstractMethodError: método abstrato");
							exit(EXIT_FAILURE);
						}
						if(invoked_method->modifiers & ACC_NATIVE){
							puts("UnsatisfiedLinkError");
							exit(EXIT_FAILURE);
						}
						
						if(!invoked_method->modifiers & ACC_PUBLIC){
							puts(" IllegalAccessError");
							exit(EXIT_FAILURE);
						}
						
						executeMethod(method_name, method_descriptor, super_class, 
								jvm, thread, objectref, nargs, args);
						thread->program_counter = backupPC;
						findMethod = true;
					}
					else{
						super_class = getSuperClass(super_class->classfile, jvm);
					}
				}
				else{
					super_class = getSuperClass(super_class->classfile, jvm);
				}
			}
			if(!findMethod){
				puts("AbstractMethodError:");
				exit(EXIT_FAILURE);
			}
			thread->program_counter += 2;
			break;
		case	invokedynamic:
			thread->program_counter += 2;
			break;
	}
	thread->program_counter += 3;

}

// handleObject	0xBB a 0xBE; 0xC5
//	criação e manipulação de objetos e arrays
void	handleObject(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.new*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.newarray*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.anewarray*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.arraylength*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.multianewarray*/
	switch(* thread->program_counter){// PARA TESTAR O HELLOWORLD
		case	new:;
			u1	indexbyte1 = * (thread->program_counter + 1);
			u1	indexbyte2 = * (thread->program_counter + 2);
			u2	index = (indexbyte1 << 8) | indexbyte2;

			#ifdef	DEBUG_INSTRUCAO
			printf("\t#%" PRIu16, index);
			#endif

			cp_info	* cp =(thread->jvm_stack)->current_constant_pool;
			cp_info	* cp_class = cp + index - 1;

			if(cp_class->tag != CONSTANT_Class){
				puts("InstantiationError: Referência inválida para classe do objeto");
				exit(EXIT_FAILURE);
			}

			cp_info	* cp_class_name = cp + cp_class->u.Class.name_index - 1;

			char	* class_name = cp_class_name->u.Utf8.bytes;
			class_name[cp_class_name->u.Utf8.length] = '\0';

			#ifdef	DEBUG_INSTRUCAO
			printf("\t<%s>\n", class_name);
			#endif

			OBJECT	*	newObject;
			if(!strcmp(class_name, "java/lang/StringBuffer")){
				char * string = (char *) malloc(sizeof(char));
				string[0] = '\0';
				newObject = (OBJECT *)  string;
				pushOperand((u4) newObject, thread->jvm_stack);
				thread->program_counter += 3;
				return;
			}

			// CONTROLE DE ACESSO
			u1	* backupPC = thread->program_counter;
			CLASS_DATA	* object_class = getClass(cp_class_name, jvm);
			if(!object_class){// se a classe do objeto não foi carregada

				char	* string = malloc((strlen(class_name) + 7) * sizeof(CHAR));
				strcpy(string, class_name);
				strcat(string, ".class");

				classLoading(string, &object_class, method->class_data, jvm);
				free(string);
				classLinking(object_class, jvm);
				classInitialization(object_class, jvm, thread);

				thread->program_counter = backupPC;
			}
			else{
				if(object_class != method->class_data){// Se o objeto não for da mesma classe do método
					// verifica se a classe do objeto é acessível pelo método corrente
					if(!(object_class->modifiers & ACC_PUBLIC) & // se a classe não é public e não é do mesmo package
					(object_class->classloader_reference != (method->class_data)->classloader_reference)){
						puts("IllegalAccessError: acesso indevido à classe ou interface.");
						exit(EXIT_FAILURE);
					}
				}
			}
			if((object_class->modifiers == ACC_INTERFACE) || (object_class->modifiers == ACC_ABSTRACT)){
				puts("InstantiationError: Criação de objeto de interface ou class abstrata");
			}

			newObject = (OBJECT *) malloc(sizeof(OBJECT));

			newObject = (OBJECT *) malloc(sizeof(OBJECT));
			newObject->class_data_reference = object_class;
			newObject->prox = (jvm->heap)->objects;
			(jvm->heap)->objects = newObject;

			// CRIA INSTANCE_VARIABLES
			if(!(object_class->classfile)->fields_count){
				newObject->instance_variables = NULL;
			}
			else{
				for(u2 i = 0; i < (object_class->classfile)->fields_count; i++){
					VARIABLE	* var = (VARIABLE *) malloc(sizeof(VARIABLE));
					var->field_reference = object_class->field_data + i;
					(object_class->field_data + i)->var = var;

					u2	descriptor_index = ((object_class->field_data + i)->info)->descriptor_index;
					(var->value).type = (object_class->runtime_constant_pool + descriptor_index - 1)->u.Utf8.bytes[0];
					switch((var->value).type){
						case	BOOLEAN:
							(var->value).u.Boolean.boolean = 0;
							break;
						case	BYTE:
							(var->value).u.Byte.byte = 0;
							break;
						case	CHAR:
							(var->value).u.Char.char_ = 0;
							break;
						case	DOUBLE:
							(var->value).u.Double.high_bytes = 0;
							(var->value).u.Double.low_bytes = 0;
							break;
						case	FLOAT:
							(var->value).u.Float.float_ = 0;
							break;
						case	INT:
							(var->value).u.Integer.integer = 0;
							break;
						case	LONG:
							(var->value).u.Long.high_bytes = 0;
							(var->value).u.Long.low_bytes = 0;
							break;
						case	REF_INST:;
							(var->value).u.InstanceReference.reference = NULL;
							break;
						case	SHORT:
							(var->value).u.Short.short_ = 0;
							break;
						case	REF_ARRAY:
							(var->value).u.ArrayReference.reference = NULL;
							break;
						default:
							puts("VerifyError: Unknown type");
							exit(EXIT_FAILURE);
					}
					u2	access_flags = (object_class->field_data + i)->modifiers;
/*			if(!(access_flags & ACC_FINAL)){*/
					if(!(access_flags & ACC_STATIC)){
						var->prox = newObject->instance_variables;
							newObject->instance_variables = var;
					}
					else{
						free(var);
					}
/*			}*/
				}
			}

			// coloca novo objeto no heap
			newObject->prox = (jvm->heap)->objects;
			(jvm->heap)->objects = newObject;

			// coloca referencia do objeto na pilha
			pushOperand((u4) newObject, thread->jvm_stack);
			thread->program_counter += 3;
			break;
		case	anewarray:
			thread->program_counter += 3;
			break;
		case	newarray:
			s4	count = popOperand(thread->jvm_stack);
			
			if(count < 0){
				puts("NegativaArraySizeException");
				exit(EXIT_FAILURE);
			}
			ARRAY *	new_array = (ARRAY *) malloc(sizeof(ARRAY));
			new_array->atype = * (thread->program_counter + 1);
			new_array->count = count;
			printf("\t%" PRIu32, new_array->atype);
			switch(new_array->atype){
				case	T_BOOLEAN:
					printf("\t(boolean)");
					break;
				case	T_BYTE:
					printf("\t(byte)");
					break;
				case	T_CHAR:
					printf("\t(char)");
					break;
				case	T_DOUBLE:
					printf("\t(double)");
					break;
				case	T_FLOAT:
					printf("\t(float)");
					break;
				case	T_INT:
					printf("\t(int)");
					break;
				case	T_LONG:
					printf("\t(long)");
					break;
				case	T_SHORT:
					printf("\t(short)");
					break;
				default:
					puts("\nUnknown array type");
					exit(EXIT_FAILURE);
			}
			new_array->entry = (VALUE *) malloc(count * sizeof(VALUE));
			new_array->prox = (jvm->heap)->arrays;
			(jvm->heap)->arrays = new_array;
			
			pushOperand((u4) new_array, thread->jvm_stack);
			
			thread->program_counter += 2;
			break;
		case	arraylength:
			break;
		case	multianewarray:
			thread->program_counter += 4;
			break;
	}
}

// athrow	0xBF
//	lançamento de exceções
void	athrow_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.athrow*/
}

// properties	0xC0 e 0xC1
//	checa propriedades de objetos
void	properties(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.checkcast*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.instanceof*/
}

// monitor	0xC2 e 0xC3
//	implementa monitores para objetos (multithreading)
void	monitor(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.monitorenter*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.monitorexit*/
}

// wide		0xC4
//	modifica tamanho de argumentos de certas instruções aritmeticas
void	wide_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.wide*/
	isWide = 1;

	thread->program_counter++;
}

// ifNull	0xC6 e 0xC7
//	verifica se uma referencia é ou não NULL, e realiza jump
void	ifNull(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifnull*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.ifnonnull*/
    u1	branchbyte1 = * (thread->program_counter + 1);
	u1	branchbyte2 = * (thread->program_counter + 2);
	s2	branch = (branchbyte1 << 8) | branchbyte2;

    s4 value = (s4) popOperand(thread->jvm_stack);
	switch(* thread->program_counter){
		case	ifnull:
            if(value == 0){
                thread->program_counter += branch;
            }else{
                thread->program_counter += 3;
            }
			break;
        case	ifnonnull:
            if(value != 0){
                thread->program_counter += branch;
            }else{
                thread->program_counter += 3;
            }
			break;
	}
}

// widejump	0xC8 a 0xC9
//	jumps incondicionais de maior tamanho de offset
void	widejump(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.goto_w*/
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.5.jsr_w*/
    u1	branchbyte1 = * (thread->program_counter + 1);
	u1	branchbyte2 = * (thread->program_counter + 2);
	u1	branchbyte3 = * (thread->program_counter + 3);
	u1	branchbyte4 = * (thread->program_counter + 4);

	s4	branch = (s4)(((branchbyte1 & 0xFF)<<24) | ((branchbyte2 & 0xFF)<<16) | ((branchbyte3 & 0xFF)<<8) | (branchbyte1 & 0xFF));

	switch(* thread->program_counter){
		case	goto_w:
            thread->program_counter += branch;
			break;
        case	jsr_w:
            pushOperand((u4)(thread->program_counter += 5), thread->jvm_stack);
            thread->program_counter += branch;
			break;
	}
}

// breakpoint	0xCA
//	(opcode reservado) utilizado por debuggers para implementar breakpoints
void	breakpoint_(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.2*/
}

// nonDefined	0xCB a 0xFD
//	faixa de Opcodes não utilizada pela Oracle
void	nonDefined(METHOD_DATA * method, THREAD * thread, JVM * jvm){
	puts("Unknown jvm instruction.");
	exit(EXIT_FAILURE);
}

// impdep	0xFE e 0xFF
//	(opcodes reservados) instruções que provêem backdoors para implementações específicas
void	impdep(METHOD_DATA * method, THREAD * thread, JVM * jvm){
/*https://docs.oracle.com/javase/specs/jvms/se7/html/jvms-6.html#jvms-6.2*/
}


// ARRAY DE PONTEIROS PRA FUNÇÃO
INSTRUCTION	func[] = {
		// 0x00		nop
		nop_,

		// 0x01 a 0x0F	Tconst
		Tconst, Tconst, Tconst, Tconst, Tconst, Tconst, Tconst,Tconst,
		Tconst,	Tconst, Tconst, Tconst, Tconst, Tconst, Tconst,

		// 0x10 e 0x11	Tipush
		Tipush, Tipush,

		// 0x12 a 0x14	ldc_
		ldc_, ldc_, ldc_,

		// 0x15 a 0x2D	Tload
		Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload,
		Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload, Tload,
		Tload, Tload, Tload, Tload, Tload,

		// 0x2E a 0x35	Taload
		Taload, Taload, Taload, Taload, Taload, Taload, Taload, Taload,

		// 0x36 a 0x4E	Tstore
		Tstore, Tstore, Tstore, Tstore, Tstore, Tstore,	Tstore, Tstore, Tstore, Tstore,
		Tstore, Tstore, Tstore, Tstore, Tstore, Tstore,	Tstore, Tstore, Tstore, Tstore,
		Tstore, Tstore, Tstore, Tstore, Tstore,

		// 0x4F a 0x56
		Tastore, Tastore, Tastore, Tastore, Tastore, Tastore, Tastore, Tastore,

		// 0x57 a 0x5F
		 handleStack, handleStack, handleStack, handleStack, handleStack, handleStack, handleStack, handleStack, handleStack,

		// 0x60 a 0x63
		Tadd, Tadd, Tadd, Tadd,

		// 0x64 a 0x67
		Tsub, Tsub, Tsub, Tsub,

		// 0x68 a 0x6N
		Tmul, Tmul, Tmul, Tmul,

		// 0x6C a 0x6F
		Tdiv, Tdiv, Tdiv, Tdiv,

		// 0x70 a 0x73
		Trem, Trem, Trem, Trem,

		// 0x74 a 0x77
		Tneg, Tneg, Tneg, Tneg,

		// 0x78 e 0x79
		Tshl, Tshl,

		// 0x7A e 0x7B
		Tshr, Tshr,

		// 0x7C e 0x7D
		Tushr, Tushr,

		// 0x7E e 0x7F
		Tand, Tand,

		// 0x80 e 0x81
		Tor, Tor,

		// 0x82 e 0x83
		Txor, Txor,

		// 0x84
		Tinc,

		// 0x85 a 0x87
		i2T, i2T, i2T,

		// 0x88 a 0x8A
		l2T, l2T, l2T,

		// 0x8B a 0x8D
		f2T, f2T, f2T,

		// 0x8E a 0x90
		d2T, d2T, d2T,

		// 0x91 a 0x93
		i2T, i2T, i2T,

		// 0x94
		Tcmp,

		// 0x95 a 0x98
		TcmpOP, TcmpOP, TcmpOP, TcmpOP,

		// 0x99 a 0x9E
		ifOP, ifOP, ifOP, ifOP, ifOP, ifOP,

		// 0x9F a 0xA4
		if_icmOP, if_icmOP, if_icmOP, if_icmOP, if_icmOP, if_icmOP,

		// 0xA5 e 0xA6
		if_acmOP, if_acmOP,

		// 0xA7, 0xA8, 0xA9
		jump, jump, jump,

		// 0xAA e 0xAB
		switch_, switch_,

		// 0xAC a 0xB1
		Treturn, Treturn, Treturn, Treturn, Treturn, Treturn,

		// 0xB2 a 0xB5
		accessField, accessField, accessField, accessField,

		// 0xB6 a 0xBA
		invoke, invoke, invoke, invoke, invoke,

		// 0xBB a 0xBE
		handleObject, handleObject, handleObject, handleObject,

		// 0xBF
		athrow_,

		// 0xC0 e 0xC1
		properties, properties,

		// 0xC2 e 0xC3
		monitor, monitor,

		// 0xC4
		wide_,

		// 0xC5
		handleObject,

		// 0xC6 e 0xC7
		ifNull, ifNull,

		// 0xC8 a 0xC9
		widejump, widejump,

		// 0xCA
		breakpoint_,

		// 0xCB a 0xFD
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined,	nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined,	nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined, nonDefined, nonDefined, nonDefined,
		nonDefined, nonDefined,	nonDefined, nonDefined, nonDefined,
		nonDefined,

		// 0xFE e 0xFF
		impdep, impdep
};
