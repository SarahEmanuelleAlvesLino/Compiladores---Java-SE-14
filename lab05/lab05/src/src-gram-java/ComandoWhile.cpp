#include "ComandoWhile.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoWhile::ComandoWhile() {
  condicao = NULL;
  bloco = NULL;
}

void ComandoWhile::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "ComandoWhile" << endl;
}
