#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {
}

static void coleta_nos_decl_metodo(No_arv_parse* no, vector<No_arv_parse*>& resultado) {
  if (no == NULL) return;
  if (no->simb == "DeclMetodo") {
    resultado.push_back(no);
    return;
  }
  for (size_t i = 0; i < no->filhos.size(); ++i) {
    coleta_nos_decl_metodo(no->filhos[i], resultado);
  }
}

vector<Funcao*> Funcao::extrai_todas_funcoes(No_arv_parse *arv) {
  vector<Funcao*> resultado;
  if (arv == NULL) return resultado;
  vector<No_arv_parse*> nos_metodo;
  coleta_nos_decl_metodo(arv, nos_metodo);
  for (size_t i = 0; i < nos_metodo.size(); ++i) {
    Funcao* f = extrai_funcao(nos_metodo[i]);
    if (f != NULL) resultado.push_back(f);
  }
  return resultado;
}

No_arv_parse* busca_no_decl_metodo(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "DeclMetodo") return no;
  for (int i = 0; i < no->filhos.size(); ++i) {
    No_arv_parse* res = busca_no_decl_metodo(no->filhos[i]);
    if (res != NULL) return res;
  }
  return NULL;
}

Funcao* Funcao::extrai_funcao(No_arv_parse *no) {
  No_arv_parse* no_metodo = busca_no_decl_metodo(no);
  if (no_metodo == NULL) return NULL;

  Funcao* res = new Funcao();
  if (no_metodo->regra == 22) {
    res->tipo_retorno = Tipo::extrai_Tipo(no_metodo->filhos[2]);
    res->nome_funcao = ID::extrai_ID(no_metodo->filhos[3]);
    res->parametros = Variavel::extrai_lista_parametros(no_metodo->filhos[5]);
    
    res->comandos = Comando::extrai_lista_comandos(no_metodo->filhos[7]->filhos[1]);
  } else if (no_metodo->regra == 23) {
    res->tipo_retorno = NULL; // VOID
    res->nome_funcao = ID::extrai_ID(no_metodo->filhos[3]);
    res->parametros = Variavel::extrai_lista_parametros(no_metodo->filhos[5]);
    res->comandos = Comando::extrai_lista_comandos(no_metodo->filhos[7]->filhos[1]);
  } else {
    delete res;
    return NULL;
  }

  return res;
}

void debug_comandos(const vector<Comando*> &coms, int tab) {
  for (int ic = 0; ic < coms.size(); ++ic) {
    if (coms[ic] != NULL) coms[ic]->debug_com_tab(tab+1);
  }
}

void Funcao::debug() {
  if (tipo_retorno == NULL) cerr<< "TR NULL"<< endl;
  if (nome_funcao == NULL) cerr<< "NF NULL"<< endl;
  cerr << "Funcao:[retorno=" << (tipo_retorno == NULL ? "VOID" : tipo_retorno->nome()) << "][nome=" << (nome_funcao == NULL ? "NULL" : nome_funcao->nome) << "]" << endl;
  cerr << "      (Param:(";
  fflush(stderr);
  for (int i_par = 0; i_par < parametros.size(); ++i_par) {
    cerr << ((parametros[i_par])->tipo == NULL ? "TIPO_INVALIDO" : (parametros[i_par])->tipo->nome()) << " " <<
      ((parametros[i_par])->nome == NULL ? "NULL" : (parametros[i_par])->nome->nome) << ", ";
  }
  cerr << ") { " << endl;
  debug_comandos(comandos, 1);
  cerr << "}" <<  endl;
}
