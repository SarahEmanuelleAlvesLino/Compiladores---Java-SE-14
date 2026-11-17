class Principal {
    public static int loop() {
        int contador;
        while (continuarExecucao(contador)) {
            contador = contador + 1;
        }
    }
}