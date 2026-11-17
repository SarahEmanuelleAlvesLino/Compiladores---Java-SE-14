#ifndef _EXPRESSAO_IGUALDADE_HPP_
#define _EXPRESSAO_IGUALDADE_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoIgualdade : public Expressao {
public:
  Expressao* esquerda;
  Expressao* direita;
  string op;
  ExpressaoIgualdade();
  void debug_com_tab(int tab);
};

#endif
