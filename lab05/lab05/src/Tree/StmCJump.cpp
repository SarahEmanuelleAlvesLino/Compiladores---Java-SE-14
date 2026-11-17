#include "StmCJump.hh"
#include <iostream>

using namespace std;

StmCJump::StmCJump() {
  expressao_booleana = NULL;
}

void StmCJump::imprime() const {
  cout << "CJUMP(";
  if (expressao_booleana != NULL) expressao_booleana->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ", " << destino_verdade << ", " << destino_falso << ")";
}

StmCJump* StmCJump::gera_Stm_CJump(Exp* expressao_booleana, string destino_verdade, string destino_falso) {
  StmCJump* res = new StmCJump();
  res->expressao_booleana = expressao_booleana;
  res->destino_verdade = destino_verdade;
  res->destino_falso = destino_falso;
  return res;
}
