#include "ExpTemp.hpp"
#include <iostream>

using namespace std;

ExpTemp::ExpTemp() {
  id = 0;
}

void ExpTemp::imprime() const {
  cout << "TEMP(t" << id << ")";
}

ExpTemp* ExpTemp::gera_Exp_Temp(FrameAcessoTemp* acesso) {
  ExpTemp* res = new ExpTemp();
  if (acesso != NULL) {
    res->id = acesso->id;
  }
  return res;
}
