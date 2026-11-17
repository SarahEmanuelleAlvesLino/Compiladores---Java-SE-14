#ifndef _COMANDO_WHILE_HPP_
#define _COMANDO_WHILE_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"

class ComandoWhile : public Comando {
public:
  Expressao* condicao;
  Comando* bloco;
  ComandoWhile();
  void debug_com_tab(int tab);
};

#endif
