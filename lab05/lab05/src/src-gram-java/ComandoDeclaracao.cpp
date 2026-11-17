#include "ComandoDeclaracao.hpp"
#include <iostream>
#include "../debug-util.hpp"
using namespace std;

ComandoDeclaracao::ComandoDeclaracao() {
  inicializacao = NULL;
}

void ComandoDeclaracao::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << variavel->nome->nome << ":" << (variavel->tipo == NULL ? "TIPO_INVALIDO" : variavel->tipo->nome()) << "] Declaracao";
  if (inicializacao != NULL) {
    cerr << " = " << endl;
    inicializacao->debug_com_tab(tab + 1);
  } else {
    cerr << endl;
  }
}
