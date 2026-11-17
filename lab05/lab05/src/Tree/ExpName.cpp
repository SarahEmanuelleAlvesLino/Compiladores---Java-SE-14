#include "ExpName.hpp"
#include <iostream>

using namespace std;

ExpName::ExpName() { }

void ExpName::imprime() const {
  cout << "NAME(" << nome << ")";
}

ExpName* ExpName::gera_Exp_Name(string nome) {
  ExpName* res = new ExpName();
  res->nome = nome;
  return res;
}
