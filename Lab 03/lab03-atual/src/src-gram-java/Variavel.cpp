#include "Variavel.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

vector<Variavel*> Variavel::extrai_lista_nao_vazia_parametros(No_arv_parse* no) {
  // ListaParam. 26) ListaParam -> ListaParam COMMA Parametro, 27) ListaParam -> Parametro (0-based: 25 e 26)
  vector<Variavel*> res;
  if (no->regra == 27) { // 27 -> ListaParam -> Parametro (0-based: 26)
    res.push_back(extrai_variavel_P(no->filhos[0]));
    return res;
  }
  // Tem que ser regra 26 (0-based: 25). ListaParam -> ListaParam COMMA Parametro
  res = extrai_lista_nao_vazia_parametros(no->filhos[0]);
  res.push_back(extrai_variavel_P(no->filhos[2]));
  return res;
}

vector<Variavel *> Variavel::extrai_lista_parametros(No_arv_parse* no) {
  // OpcionalListaParam. 25) OpcionalListaParam -> ListaParam, 26) OpcionalListaParam -> '' (0-based: 24 e 25)
  if (no->regra == 25) { // 0-based: 25 (ou seja, regra 26 -> vazia)
    return vector<Variavel*>();
  }
  // Tem que ser regra 24 (0-based: 24). OpcionalListaParam -> ListaParam
  return extrai_lista_nao_vazia_parametros(no->filhos[0]);
}

Variavel* Variavel::extrai_variavel_P(No_arv_parse* no) {
  Variavel* res = new Variavel();
  // Parametro -> TipoVar ID (Regra index 28)
  res->tipo = Tipo::extrai_Tipo(no->filhos[0]);
  res->nome = ID::extrai_ID(no->filhos[1]);
  return res;
}

void Variavel::debug_com_tab(int tab) {
  tab3(tab);
  cerr << "[" << (nome == NULL ? "NULL" : nome->nome) << ":" <<  (tipo == NULL ? "TIPO_INVALIDO" : tipo->nome()) << "] Variavel Declarada" << endl;
}
