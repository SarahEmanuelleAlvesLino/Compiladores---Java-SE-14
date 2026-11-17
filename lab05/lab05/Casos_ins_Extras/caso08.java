class Principal {
    public static float calcularTaxa(int tipo) {
        if (estaAtivo(tipo)) {
            if (obterDesconto() > 50) {
                return aplicarBonus(total() + 10);
            }
        }
    }
}