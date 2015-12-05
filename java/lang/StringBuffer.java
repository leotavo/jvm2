package	java.lang;


public	class	StringBuffer{
	private	String	str;
	
	public	StringBuffer(){
		this.str = new String();
	}
		
	public	native	StringBuffer	append(boolean z);
	public	native	StringBuffer	append(byte b);
	public	native	StringBuffer	append(char c);
	public	native	StringBuffer	append(short s);
	public	native	StringBuffer	append(int i);
	public	native	StringBuffer	append(long l);
	public	native	StringBuffer	append(float f);
	public	native	StringBuffer	append(double d);

	public	String		toString(){
		return	str;
	}
}


