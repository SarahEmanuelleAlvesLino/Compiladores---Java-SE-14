#ifndef _FRAME_FUNCAO_HPP_
#define _FRAME_FUNCAO_HPP_
#include "../src-gram-java/Funcao.hpp"
#include "FrameAcesso.hpp"
#include <vector>
#include <utility>
#include <string>
using namespace std;

class FrameFuncao {
public:
  int tamanho_frame;
  int n_param_entrada;
  int n_maximo_param_saida;
  int n_pseudo_registradores;
  int n_variaveis_no_frame;
  vector<pair<string, FrameAcesso*>> alocacoes;

  FrameFuncao();
  static FrameFuncao* gera_frame_de_funcao(Funcao* fun);
  void imprime();
};

#endif
