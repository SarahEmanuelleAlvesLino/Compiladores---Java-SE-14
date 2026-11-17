#include "ComandoIf.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoIf::ComandoIf() {
  condicao = NULL;
  bloco_true = NULL;
  bloco_false = NULL;
}

void ComandoIf::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "ComandoIf" << endl;
}
