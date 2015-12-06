INTERPRETER - SITUAÇÃO:

nop_ = OK (DANIEL)
Tconst = OK (DANIEL)
Tipush = OK (DANIEL)
ldc_ = OK (DANIEL)
Tload = OK (DANIEL)
Taload = falta testar (arrays) (DANIEL)
Tstore = OK (DANIEL)
Tastore = falta testar (arrays) (DANIEL)
handleStack = testado dup, instruções dup2 com bug (long e double), falta testar as demais. (DANIEL)
Tadd = falta dadd, falta testar as demais (MATHEUS E LEO)
Tsub = falta dsub, falta testar as demais (MATHEUS E LEO)
Tmul = falta dmul, falta testar as demais (MATHEUS E LEO)
Tdiv = falta ddiv, falta testar as demais (MATHEUS E LEO)
Trem = NÃO IMPLEMENTADA (usa Tmul Tdiv e Tsub)
Tneg = falta testar (DANIEL)
Tshl = falta testar (YTALO)
Tshr = falta testar (YTALO)
Tushr = NÃO IMPLEMENTADA
Tand = falta testar (GABRIEL)
Tor = NÃO IMPLEMENTADA (MATHEUS)
Txor = NÃO IMPLEMENTADA (MATHEUS)
Tinc = NÃO IMPLEMENTADA (MATHEUS)
i2T = falta testar (DANIEL)
l2T = falta testar (DANIEL)
f2T = falta testar (DANIEL)
d2T = falta testar (DANIEL)
Tcmp = NÃO IMPLEMENTADA
TcmpOP = NÃO IMPLEMENTADA
ifOP = NÃO IMPLEMENTADA
if_icmOP = NÃO IMPLEMENTADA
if_acmOP = NÃO IMPLEMENTADA
jump = NÃO IMPLEMENTADA
switch = NÃO IMPLEMENTADA
Treturn = NÃO IMPLEMENTADA
accessField = falta getfield (LEO)
invoke = falta invokevirtual, invokedynamic, invokeinterface (LEO)
handleObject = falta newarray, anewarray, arraylength, multianewarray (LEO)
athrow = NÃO IMPLEMENTADA
properties = NÃO IMPLEMENTADA
monitor = NÃO IMPLEMENTADA
wide = INCOMPLETA
ifNull = NÃO IMPLEMENTADA
widejump = NÃO IMPLEMENTADA
breakpoint = NÃO IMPLEMENTADA
nonDefined = NÃO IMPLEMENTADA
impdep = NÃO IMPLEMENTADA



Corretude do interpretador:

( ) arrays simples; => Taload, Tastore, handleObject
( ) array length; => handleObject
( ) inteiros; => todas as instruções com byte, char, short, int
( ) floats; => todas as instruções com float
( ) desvios com cat 1; => jump, widejump, ifOP, if_icmOP, if_acmOP, ifnull
( ) recursao; => invoke
( ) metodos estaticos; => invoke
( ) operações de retorno; => Treturn
( ) operações lógicas com cat 1; => Tneg, Tand, Tor, Txor
( ) operações de deslocamento com cat 1; => Tshl, Tshr, Tushr
( ) operações de manipulação de pilha; => Tload, Taload, Tstore, Tastore, Tipush, ldc_, Tconst, accessField, handleStack
( ) iteracao e case => switch_
( ) arrays multidimensionais; => Taload, Tastore, handleObject
( ) arrays de referência; => Taload, Tastore, handleObject
( ) objetos; => handleObject
( ) métodos não estaticos; => invoke
( ) longs; => todas as instruções com long
( ) doubles; = todas as instruções com doubles
( ) operações aritmeticas de cat 2; => Tadd, Tsub, Tmul, Tdiv, Trem.
( ) operações de deslocamento de long; => Tshl, Tshr, Tushr
( ) system.out.println (int, float, string, booleano); => invokespecial
( ) conversao de e para cat 2; => i2T, l2T, f2T, d2T
( ) strings; => ????
( ) interfaces; => ????



(X) leitura do formato .class (30% do interpretador)

opcionais:
- Todas as demais instruções.
( ) threads;
( ) debugs; => breakpoint
( ) chamada às bibliotecas java;
( ) tratamento de exceções; => athrow, try_catch (tratamento de métodos)
( ) checagem dinâmica de tipos; => properties
( ) coletor de lixo; (monitoramento da heap)

SITUAÇÃO ATUAL:


Compilação:
$ gcc -std=c99 *.c -o jvm -lm

Execução:
Linux
$ ./jvm arquivo.class [args...]
Windows
$ jvm.exe arquivo.class [args...]
args = array de strings, argumento do método main da classe de entrada. (opcional)

Testes Prontos:
Linux
$ ./teste.sh


Links úteis (Quem tiver algum link útil, coloque aqui):

https://docs.oracle.com/javase/specs/jvms/se6/html/VMSpecTOC.doc.html (Especificação da Oracle)
Item 2.17 - Execução da JVM
Capítulo 3 - Estrutura da JVM
Capítulo 4 - Formato classfile
Capítulo 5 - Loading, Linking e Initializing (JVM)
Capítulo 6 - Conjunto de Instruções da JVM
Capítulo 9 - Mnemônicos dos Opcodes da JVM

http://www.cs.cornell.edu/courses/cs2112/2012sp/lectures/lec27-12sp.pdf
http://blog.jamesdbloom.com/JVMInternals.html (arquitetura interna da JVM)
http://blog.jamesdbloom.com/JavaCodeToByteCode_PartOne.html (relação entre código Java e Bytecodes)
https://www.artima.com/insidejvm/ed2/index.html (Inside the Java Virtual Machine by Bill Venners)
http://mentebinaria.com.br/zine/edicoes/1/ClassInjection.txt (explicações em portugues sobre o formato classfile)
http://homepages.inf.ed.ac.uk/kwxm/JVM/codeByNo.html (opcodes por número)