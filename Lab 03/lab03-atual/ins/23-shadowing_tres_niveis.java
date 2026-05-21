class Principal {
  public static int sombraTresNiveis(int x) {
    int v;
     v = x * 2;
    {
      int v;
      v = v * 3;
      {
        int v;
        v = v + 1;
        return v;
      }
    }
  }
}