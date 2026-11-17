#include "ComandoFor.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoFor::ComandoFor() {
  inicializacao = NULL;
  condicao = NULL;
  passo = NULL;
  corpo = NULL;
}

void ComandoFor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "ComandoFor" << endl;
  if (inicializacao != NULL) inicializacao->debug_com_tab(tab + 1);
  if (condicao != NULL)      condicao->debug_com_tab(tab + 1);
  if (passo != NULL)         passo->debug_com_tab(tab + 1);
  if (corpo != NULL)         corpo->debug_com_tab(tab + 1);
}
