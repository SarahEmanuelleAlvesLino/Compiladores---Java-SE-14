#ifndef _COMANDO_FOR_HPP_
#define _COMANDO_FOR_HPP_
#include "Comando.hpp"
#include "Expressao.hpp"

class ComandoFor : public Comando {
public:
  Comando* inicializacao;
  Expressao* condicao;
  Comando* passo;
  Comando* corpo;
  ComandoFor();
  void debug_com_tab(int tab);
};

#endif
