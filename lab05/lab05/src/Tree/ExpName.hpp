#ifndef _EXP_NAME_HPP_
#define _EXP_NAME_HPP_
#include "Exp.hpp"
#include "../src-gram-java/Expressao.hpp"
#include <string>
using namespace std;

class ExpName : public Exp {
public:
  string nome;
  ExpName();
  void imprime() const;
  static ExpName* gera_Exp_Name(string nome);
};

#endif
