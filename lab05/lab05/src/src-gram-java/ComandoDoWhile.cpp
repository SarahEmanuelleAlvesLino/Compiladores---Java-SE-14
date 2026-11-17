#include "ComandoDoWhile.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoDoWhile::ComandoDoWhile() {
  corpo = NULL;
  condicao = NULL;
}

void ComandoDoWhile::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "ComandoDoWhile" << endl;
  if (corpo != NULL)    corpo->debug_com_tab(tab + 1);
  if (condicao != NULL) condicao->debug_com_tab(tab + 1);
}
