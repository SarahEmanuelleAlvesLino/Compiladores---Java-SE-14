#include "StmSeq.hpp"
#include <iostream>

using namespace std;

StmSeq::StmSeq() {
  esq = NULL;
  dir = NULL;
}

void StmSeq::imprime() const {
  if (esq != NULL) esq->imprime(); else cout << "STM_NAO_SUPORTADA";
  cout << endl;
  if (dir != NULL) dir->imprime(); else cout << "STM_NAO_SUPORTADA";
}

StmSeq* StmSeq::gera_Stm_Seq(Stm* esq, Stm* dir) {
  StmSeq* res = new StmSeq();
  res->esq = esq;
  res->dir = dir;
  return res;
}
