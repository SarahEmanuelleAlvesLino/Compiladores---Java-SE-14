#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  
  vector<Variavel*> res;
  if (no->regra == 27) { // 27 -> ListaParam -> Parametro (0-based: 26)
    res.push_back(extrai_variavel_P(no->filhos[0]));
    return res;
  }
  
  res = extrai_lista_nao_vazia_parametros(no->filhos[0]);
  res.push_back(extrai_variavel_P(no->filhos[2]));
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  
  if (no->regra == 25) { 
    return vector<Variavel*>();
  }
  return extrai_lista_nao_vazia_parametros(no->filhos[0]);
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  Variavel* res = new Variavel();

  res->tipo = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome = ID::extrai_ID(no->filhos[1]);
  return res;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << (nome == NULL ? "NULL" : nome->nome) << ":" <<  (tipo == NULL ? "TIPO_INVALIDO" : tipo->nome()) << "] Variavel Declarada" << endl;
}
