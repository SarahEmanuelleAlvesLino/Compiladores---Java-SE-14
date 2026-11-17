#include "ExpressaoChamadaFuncao.hpp"
#include <iostream>
#include "../debug-util.hpp"

using namespace std;

ExpressaoChamadaFuncao::ExpressaoChamadaFuncao(){
    nome_funcao = NULL;
}

void ExpressaoChamadaFuncao::debug_com_tab(int tab){
    tab3(tab);
    cerr << "CHAMADA_FUNCAO[" << (nome_funcao ? nome_funcao->nome : "nulo") << "]" << endl;
    for (size_t i = 0; i < parametros.size(); ++i){
        if (parametros[i]){
            parametros[i]->debug_com_tab(tab + 1);
        }
    }
}