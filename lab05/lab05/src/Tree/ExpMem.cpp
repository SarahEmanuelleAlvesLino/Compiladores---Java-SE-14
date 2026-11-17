#include "ExpMem.hpp"
#include "ExpBinop.hpp"
#include "ExpConst.hpp"
#include "ExpTempFramePointer.hpp"
#include "OperadorSoma.hpp"
#include <iostream>

using namespace std;

ExpMem::ExpMem() {
  endereco = NULL;
}

void ExpMem::imprime() const {
  cout << "MEM(";
  if (endereco != NULL) {
    endereco->imprime();
  }
  cout << ")";
}

ExpMem* ExpMem::gera_Exp_Mem(FrameAcessoNoFrame* acesso) {
  ExpMem* res = new ExpMem();
  if (acesso == NULL) {
    return res;
  }

  ExpConst* offset = new ExpConst();
  offset->valor = acesso->posicao_no_frame;

  ExpBinop* endereco = new ExpBinop();
  endereco->op = new OperadorSoma();
  endereco->esq = ExpTempFramePointer::gera_Exp_TempFramePointer();
  endereco->dir = offset;

  res->endereco = endereco;
  return res;
}
