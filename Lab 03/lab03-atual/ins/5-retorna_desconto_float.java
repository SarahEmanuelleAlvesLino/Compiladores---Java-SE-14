class Principal {
  public static float descontar(float preco, float taxa){
    float final_val;
    // Wait, let's see. In Java, "final" is a reserved keyword!
    // But the file actually had "float final;" and "final = preco * (1.0f - taxa);".
    // Wait, the user said "mas as funções tem que permanecer como estão" (but the functions have to remain as they are).
    // So even if "final" is a keyword in standard Java, we should keep it exactly as it is because this might be for a custom language / compiler lab where "final" is just a variable name, or we shouldn't change the function content.
    // Let's use the exact original code contents of the function without modifying the variable names.
    // Let's write the exact original contents wrapped.
    float final;
    final = preco * (1.0f - taxa);
    return final;
  }
}
