

public class Append{
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
		
		System.out.println(zf + " - " + zt + " - " + b + " - " + c + " - " + uc);
		System.out.println(s + " - " + i + " - " + f + " - " + l + " - " + d + " - " + str);
		
	}
}
