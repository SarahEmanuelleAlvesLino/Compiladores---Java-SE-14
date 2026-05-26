#ifndef _EXPRESSAO_NEGACAO_HPP_
#define _EXPRESSAO_NEGACAO_HPP_
#include "Expressao.hpp"
#include <string>
using namespace std;

class ExpressaoNegacao : public Expressao {
public:
  Expressao* expressao;
  string op;
  ExpressaoNegacao();
  void debug_com_tab(int tab);
};

#endif
