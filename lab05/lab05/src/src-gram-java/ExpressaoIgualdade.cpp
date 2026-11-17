#include "ExpressaoIgualdade.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoIgualdade::ExpressaoIgualdade() {
  op = "==";
}

void ExpressaoIgualdade::debug_com_tab(int tab) {
  tab3(tab);
  cerr << op << " [ INI Igualdade"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << op << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << op << " ] FIM Igualdade"<< endl;
}
