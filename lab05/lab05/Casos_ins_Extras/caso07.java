class Principal {
    public static int buscar() {
        int sinalizador;
        while (lerProximo()) {
            if (extrairValor() == 0) {
                return obterPadrao();
            }
        }
    }
}