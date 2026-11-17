#ifndef _COMANDO_DO_WHILE_HPP_
#define _COMANDO_DO_WHILE_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"

class ComandoDoWhile : public Comando {
public:
  Comando* corpo;
  Expressao* condicao;
  ComandoDoWhile();
  void debug_com_tab(int tab);
};

#endif
