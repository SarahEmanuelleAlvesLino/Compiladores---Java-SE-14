class Principal {
  public static float descontar(float preco, float taxa){
    float final;
    final = preco * (1.0f - taxa);
    return final;
  }
}
