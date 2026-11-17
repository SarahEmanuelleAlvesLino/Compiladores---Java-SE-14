#ifndef _EXPRESSAO_VARIAVEL_HPP_
#define _EXPRESSAO_VARIAVEL_HPP_
#include "FrameAcesso.hpp"
#include "Expressao.hpp"
#include "ID.hpp"
using namespace std;

class ExpressaoVariavel : public Expressao {
public:
  ID* nome;
  FrameAcesso* acesso;
  ExpressaoVariavel();
  void debug_com_tab(int tab);
};

#endif
