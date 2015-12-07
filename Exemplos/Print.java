





//import	java.lang.System;

public class Print{
	public static void main(String[] args){	
		boolean	zf = false; 
		boolean	zt = true;
		byte	b = -128;
		char	c = '$';
		char	uc = 'φ';
		short	s = 32767;
		int	i = 2147483647;
		float	f = 3.4E38f;
		long	l = 9223372036854775807L;
		double	d = 1.7E308;
		String	str = "Hello World";
		
		System.out.println(zf);
		System.out.println(zt);
		System.out.println(b);
		System.out.println(c);
		System.out.println(uc);
		System.out.println(s);
		System.out.println(i);
		System.out.println(f);
		System.out.println(l);
		System.out.println(d);
		System.out.println(str);
		/*
  0 iconst_0
  1 istore_1
  2 iconst_1
  3 istore_2
  4 bipush -128
  6 istore_3
  7 bipush 36
  9 istore 4
 11 sipush 966
 14 istore 5
 16 sipush 32767
 19 istore 6
 21 ldc #2 <2147483647>
 23 istore 7
 25 ldc #3 <3.4E38>
 27 fstore 8
 29 ldc2_w #4 <852516352>
 32 lstore 9
 34 ldc2_w #6 <1.7E308>
 37 dstore 11
 39 ldc #8 <Hello World>
 41 astore 13
 43 getstatic #9 <java/lang/System.out>
 46 iload_1
 47 invokevirtual #10 <java/io/PrintStream.println>
 50 getstatic #9 <java/lang/System.out>
 53 iload_2
 54 invokevirtual #10 <java/io/PrintStream.println>
 57 getstatic #9 <java/lang/System.out>
 60 iload_3
 61 invokevirtual #11 <java/io/PrintStream.println>
 64 getstatic #9 <java/lang/System.out>
 67 iload 4
 69 invokevirtual #12 <java/io/PrintStream.println>
 72 getstatic #9 <java/lang/System.out>
 75 iload 5
 77 invokevirtual #12 <java/io/PrintStream.println>
 80 getstatic #9 <java/lang/System.out>
 83 iload 6
 85 invokevirtual #13 <java/io/PrintStream.println>
 88 getstatic #9 <java/lang/System.out>
 91 iload 7
 93 invokevirtual #14 <java/io/PrintStream.println>
 96 getstatic #9 <java/lang/System.out>
 99 fload 8
101 invokevirtual #15 <java/io/PrintStream.println>
104 getstatic #9 <java/lang/System.out>
107 lload 9
109 invokevirtual #16 <java/io/PrintStream.println>
112 getstatic #9 <java/lang/System.out>
115 dload 11
117 invokevirtual #17 <java/io/PrintStream.println>
120 getstatic #9 <java/lang/System.out>
123 aload 13
125 invokevirtual #18 <java/io/PrintStream.println>
128 return

		*/
	}
}

