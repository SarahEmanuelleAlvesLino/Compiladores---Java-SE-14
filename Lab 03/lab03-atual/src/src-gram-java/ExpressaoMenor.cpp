#include "ExpressaoMenor.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoMenor::ExpressaoMenor() {
  op = "<";
}

void ExpressaoMenor::debug_com_tab(int tab) {
  tab3(tab);
  cerr << op << " [ INI Menor"<< endl;
  esquerda->debug_com_tab(tab+1);
  tab3(tab);
  cerr << op << endl;
  direita->debug_com_tab(tab+1);
  tab3(tab);
  cerr << op << " ] FIM Menor"<< endl;
}
