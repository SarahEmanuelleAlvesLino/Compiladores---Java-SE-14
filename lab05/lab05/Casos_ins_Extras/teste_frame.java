class Principal {

  public static int calcular(int a, int b) {
    int x; 
    int y; 
    int z; 

    y = a + b;
    x = y * 2;

    z = somar(x, y);

    return z;
  }

  public static int somar(int c, int d) {
    return c + d;
  }
}