#include "ExpConst.hpp"
#include <iostream>

using namespace std;

void ExpConst::imprime() const {
  cout << "CONST(" << valor << ")";
}

ExpConst::ExpConst() {
  valor = 0;
}

ExpConst* ExpConst::gera_Exp_Const(ExpressaoValor* e) {
  ExpConst* res = new ExpConst();
  if (e != NULL && e->valor != NULL) {
    res->valor = e->valor->valor_int;
  }
  return res;
}
