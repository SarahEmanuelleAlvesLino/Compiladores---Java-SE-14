#include "StmLabel.hpp"
#include <iostream>

using namespace std;

StmLabel::StmLabel() {}

void StmLabel::imprime() const{
  cout << "LABEL(" << nome << ")";
}

StmLabel *StmLabel::gera_Stm_Label(string nome){
  StmLabel *res = new StmLabel();
  res->nome = nome;
  return res;
}