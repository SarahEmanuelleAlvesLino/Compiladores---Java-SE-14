class Principal {

    public static int testarFrame(int a, int b, int c) {
        int temp1;
        int temp2;
        int resultado;

        temp1 = a * b + c;
        temp2 = temp1 * 2;

        resultado = somar(temp1, temp2);
        resultado = processar(resultado, a, b, c);

        return resultado;
    }

    public static int somar(int x, int y) {
        return x + y;
    }

    public static int processar(int p1, int p2, int p3, int p4) {
        int res;
        res = p1 + p2 * p3 - p4;
        return res;
    }
}