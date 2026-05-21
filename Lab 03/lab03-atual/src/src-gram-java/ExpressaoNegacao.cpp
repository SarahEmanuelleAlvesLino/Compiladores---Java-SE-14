#include "ExpressaoNegacao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoNegacao::ExpressaoNegacao() {
  op = "!";
}

void ExpressaoNegacao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << op << " [ INI Negacao"<< endl;
  expressao->debug_com_tab(tab+1);
  tab3(tab);
  cerr << op << " ] FIM Negacao"<< endl;
}
