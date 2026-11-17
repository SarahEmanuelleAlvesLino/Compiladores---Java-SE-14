#include "Tipo.hpp"
#include <algorithm>

Tipo::Tipo(Valor valor) {
  this->valor = valor;
}

string Tipo::nome() const {
  switch(valor) {
  case INT:
    return "INT";
  case FLOAT:
    return "FLOAT";
  case BOOL:
    return "BOOL";
  }
  return "";
}

string normaliza_nome_tipo(string nome) {
  transform(nome.begin(), nome.end(), nome.begin(), ::toupper);
  return nome;
}

Tipo* tipo_por_nome(string nome) {
  nome = normaliza_nome_tipo(nome);
  if (nome == "INT" || nome == "LONG") return new Tipo(Tipo::INT);
  if (nome == "DOUBLE" || nome == "FLOAT") return new Tipo(Tipo::FLOAT);
  if (nome == "BOOL" || nome == "BOOLEAN") return new Tipo(Tipo::BOOL);
  return NULL;
}

Tipo* Tipo::extrai_Tipo(No_arv_parse* no) {
  if (no == NULL) return NULL;
  No_arv_parse* tb = no;
  if (no->simb == "TipoVar") {
    tb = no->filhos[0];
  }
  
  No_arv_parse* terminal = tb;
  if (!tb->filhos.empty()) {
    terminal = tb->filhos[0];
  }
  string nome = terminal->dado_extra.empty() ? terminal->simb : terminal->dado_extra;
  return tipo_por_nome(nome);
}

Tipo* Tipo::extrai_tipo_literal(No_arv_parse* no) {
  if (no == NULL) return NULL;
  if (no->simb == "DECIMAL") return new Tipo(INT);
  if (no->simb == "FLOAT_LITERAL") return new Tipo(FLOAT);
  if (no->simb == "TRUE" || no->simb == "FALSE") return new Tipo(BOOL);
  return NULL;
}
