class Principal {
    public static int processarMatriz() {
        int celula;
        while (possuiLinha()) {
            while (possuiColuna()) {
                celula = processarCelula(obterX(), obterY(maximo()));
            }
        }
    }
}