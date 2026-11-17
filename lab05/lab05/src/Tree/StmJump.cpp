#include "StmJump.hpp"
#include <iostream>

using namespace std;

StmJump::StmJump() {
  destino = NULL;
}

void StmJump::imprime() const {
  cout << "JUMP(";
  if (destino != NULL) destino->imprime(); else cout << "EXP_NAO_SUPORTADA";
  cout << ")";
}

StmJump* StmJump::gera_Stm_Jump(string label) {
  StmJump* res = new StmJump();
  res->destino = ExpName::gera_Exp_Name(label);
  res->alvos.push_back(label);
  return res;
}
