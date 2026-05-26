#include "Funcao.hpp"
#include "../debug-util.hpp"
#include <iostream>

Funcao::Funcao() {
}

// Função auxiliar para buscar recursivamente o nó correspondente à declaração do método
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

  // DeclMetodo -> PUBLIC STATIC TipoVar ID LPAR OpcionalListaParam RPAR Bloco (Regra 22, índice 0-based)
  // DeclMetodo -> PUBLIC STATIC VOID ID LPAR OpcionalListaParam RPAR Bloco (Regra 23, índice 0-based)
  if (no_metodo->regra == 22) {
    res->tipo_retorno = Tipo::extrai_Tipo(no_metodo->filhos[2]);
    res->nome_funcao = ID::extrai_ID(no_metodo->filhos[3]);
    res->parametros = Variavel::extrai_lista_parametros(no_metodo->filhos[5]);
    
    // Bloco -> LBRACE ListaCmd RBRACE (Regra 29)
    // Bloco está em filhos[7], e ListaCmd está em filhos[7]->filhos[1]
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
    coms[ic]->debug_com_tab(tab+1);
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
