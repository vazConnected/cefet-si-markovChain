#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../listaDeContextos.h"

bool contextoArmazenado(ListaDeContextos *lista, int tamanhoDaLista, char* identificacao, int profundidade){
    Celula* celulaAtual = lista->inicio;
    while(celulaAtual->proximo != NULL){
        if(celulaAtual->contexto->profundidade == profundidade){
            for(int i = 0; i < profundidade; i++){
                if(identificacao[i] != celulaAtual->contexto->identificacao[i]){
                    break;
                }else if(i == profundidade - 1 && identificacao[i] == celulaAtual->contexto->identificacao[i]){
                    return true;
                }
            }
        }
        celulaAtual = celulaAtual->proximo;
    }
    return false;
}

/**
 * int getIndiceContexto(ListaDeContextos lista, int tamanhoDaLista, char* identificacao, int profundidade)
 *  Retorna -1 se o contexto nao estiver armazenado
 *  Retorna um indice >= 0 se o contexto estiver armazenado
*/
int getIndiceContexto(ListaDeContextos *lista, char* identificacao, int profundidade){
    Celula* celulaAtual = lista->inicio;
    int contadorIndice = 0;

    while(celulaAtual != NULL){
        if(celulaAtual->contexto->profundidade == profundidade){
            for(int i = 0; i < profundidade; i++){
                if(identificacao[i] != celulaAtual->contexto->identificacao[i]){
                    break;
                }else if(i == profundidade - 1 && identificacao[i] == celulaAtual->contexto->identificacao[i]){
                    return contadorIndice;
                }
            }
        }
        celulaAtual = celulaAtual->proximo;
        contadorIndice++;
    }
    return -1;
}

Contexto* getContextoPorIndice(ListaDeContextos *lista, int indice){
    if(indice >= 0){
        Celula* celulaAtual = lista->inicio;
        int contadorIndice = 0;
        while(celulaAtual != NULL){
            if(contadorIndice == indice){
                return celulaAtual->contexto;
            }
            contadorIndice++;
            celulaAtual = celulaAtual->proximo;
        }
    }
    return NULL;
}

Contexto* getContextoPorIdentificacao(ListaDeContextos *lista, char* identificacao, int profundidade){
    if(profundidade > 0){
        Celula* celulaAtual = lista->inicio;

        while(celulaAtual != NULL){
            if(celulaAtual->contexto->profundidade == profundidade){
                bool iquais = true;
                for(int i = 0; i < profundidade; i++){
                    if(celulaAtual->contexto->identificacao[i] != identificacao[i]){
                        iquais = false;
                        break;
                    }
                }

                if(iquais){
                    return celulaAtual->contexto;
                }
            }
            
            celulaAtual = celulaAtual->proximo;
        }

    }
    return NULL;
}

bool listaDeContextosEstaVazia(ListaDeContextos *lista){
    return (lista->inicio == NULL);
}

int* getIndicesContextos_child(ListaDeContextos *lista_child, char* identificacao_parent, int profundidade_parent, int* qtdIndices_child){
    int* vetorDeIndices_child = (int*) calloc(1, sizeof(int));
    int _qtdIndices_child = 0;

    if(!listaDeContextosEstaVazia(lista_child)){

        Celula* celulaAtual_child = lista_child->inicio;
        int contadorDeIndices_child = 0;
        
        while(celulaAtual_child != NULL){           
            int profundidade_child = celulaAtual_child->contexto->profundidade;

            bool relacaoEncontrada = true;
            for(int indice_child = 1, indice_parent = 0; (indice_child < profundidade_child) && (indice_parent < profundidade_parent); indice_child++, indice_parent++){
                if(identificacao_parent[indice_parent] != celulaAtual_child->contexto->identificacao[indice_child]){
                    relacaoEncontrada = false;
                    break;
                }
            }
            if(relacaoEncontrada){
                _qtdIndices_child++;
                vetorDeIndices_child = (int*) realloc(vetorDeIndices_child, _qtdIndices_child * sizeof(int));
                vetorDeIndices_child[_qtdIndices_child - 1] = contadorDeIndices_child;
            }


            celulaAtual_child = celulaAtual_child->proximo;
            contadorDeIndices_child++;
        }
    }
    *qtdIndices_child = _qtdIndices_child;

    return vetorDeIndices_child;
}

void inicializarListaDeContextos(ListaDeContextos *lista){
    lista->inicio = lista->fim = NULL;
}

void inserirContextoNaListaDeContextos(ListaDeContextos *lista, char* idenficicacao, int profundidade, int tamanhoDoAlfabeto){
    Celula* novaCelula = (Celula*) malloc(sizeof(Celula));

    novaCelula->contexto = (Contexto*) malloc(sizeof(Contexto));
    novaCelula->contexto->profundidade = profundidade;
    novaCelula->contexto->somatorioDasOcorrenciasDosSubsequentes = 0;

    novaCelula->contexto->ocorrenciasDosSubsequentes = (unsigned int*) calloc(tamanhoDoAlfabeto, sizeof(unsigned int));
    novaCelula->contexto->razaoDeTransicao = (double*) calloc(tamanhoDoAlfabeto, sizeof(double));

    novaCelula->contexto->identificacao = (char*) malloc(profundidade * sizeof(char));
    strncpy(novaCelula->contexto->identificacao, idenficicacao, profundidade);

    novaCelula->proximo = NULL;
    novaCelula->desabilitado = false;

    if(listaDeContextosEstaVazia(lista)){
        lista->inicio = lista->fim  = novaCelula;
    }else{ // 
        lista->fim = lista->fim->proximo = novaCelula;
    }
}

void registrarOcorrenciaDeContexto(ListaDeContextos *lista, int subsequente, char *identificacao, int profundidade){
    bool encontrado = false;

    Celula* celulaAtual = lista->inicio;
    while(celulaAtual != NULL){
        encontrado = true;

        if(celulaAtual->contexto->profundidade == profundidade){
            for(int indiceContexto = 0; indiceContexto < profundidade; indiceContexto++){
                if(celulaAtual->contexto->identificacao[indiceContexto] != identificacao[indiceContexto]){
                    encontrado = false;
                    break;
                }
            }
        }

        if(encontrado){
            celulaAtual->contexto->ocorrenciasDosSubsequentes[subsequente]++;
            celulaAtual->contexto->somatorioDasOcorrenciasDosSubsequentes++;
            break;
        }

        celulaAtual = celulaAtual->proximo;
    }
}

void calculaRazaoDeTransicao(ListaDeContextos *lista, int tamanhoDoAlfabeto){
    Celula* celulaAtual = lista->inicio;
    while(celulaAtual != NULL){

        for(int indiceSubsequente = 0; indiceSubsequente < tamanhoDoAlfabeto; indiceSubsequente++){
            if(celulaAtual->contexto->somatorioDasOcorrenciasDosSubsequentes != 0){
                celulaAtual->contexto->razaoDeTransicao[indiceSubsequente] = (double) celulaAtual->contexto->ocorrenciasDosSubsequentes[indiceSubsequente] / celulaAtual->contexto->somatorioDasOcorrenciasDosSubsequentes;
            }else{
                celulaAtual->contexto->razaoDeTransicao[indiceSubsequente] = 0.0;
            }
            
        }
        celulaAtual = celulaAtual->proximo;
    }
}

void getCelulas_child(PacoteCelula* celulas, ListaDeContextos *lista_child, char* identificacao_parent, int profundidade_parent, int* qtdContextos_child){

    Celula* celulaAtual = lista_child->inicio;
    while(celulaAtual != NULL){

        bool encontrado = true;
        for(int child = 1, parent = 0; parent < profundidade_parent; child++, parent++){
            if(identificacao_parent[parent] != celulaAtual->contexto->identificacao[child]){
                encontrado = false;
                break;
            }
        }

        if(encontrado){
            *qtdContextos_child++;
            celulas = (PacoteCelula*) realloc(celulas, *qtdContextos_child * sizeof(PacoteCelula));
            celulas[*qtdContextos_child - 1].celula = celulaAtual;
        }

        celulaAtual = celulaAtual->proximo;
    }
}
