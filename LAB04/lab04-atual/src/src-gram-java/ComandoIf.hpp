#ifndef _COMANDO_IF_HPP_
#define _COMANDO_IF_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"

class ComandoIf : public Comando {
public:
  Expressao* condicao;
  Comando* bloco_true;
  Comando* bloco_false;
  ComandoIf();
  void debug_com_tab(int tab);
};

#endif
