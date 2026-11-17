#ifndef _EXPRESSAO_MENOR_HPP_
#define _EXPRESSAO_MENOR_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoMenor : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  string op;
  ExpressaoMenor();
  void debug_com_tab(int tab);
};

#endif
