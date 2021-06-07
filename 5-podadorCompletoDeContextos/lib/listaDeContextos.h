#ifndef _LISTA_DE_CONTEXTOS_H_
#define _LISTA_DE_CONTEXTOS_H_

typedef struct{
    char* identificacao;
    int profundidade;
    unsigned int *ocorrenciasDosSubsequentes;
    double *razaoDeTransicao;
    unsigned int somatorioDasOcorrenciasDosSubsequentes;
    bool podavel;
}Contexto;

typedef struct celula{
    bool disabled;
    Contexto* contexto;
    struct celula* proximo;
}Celula;

typedef struct{
    Celula* inicio;
    Celula* fim;
}ListaDeContextos;

typedef struct{
    char* identificacao;
    int profundidade;
    bool ignorar;
}IdentificacaoDosContextos;

typedef struct{
    Celula* celula;
}PacoteCelula;

bool contextoArmazenado(ListaDeContextos *lista, int tamanhoDaLista, char* identificacao, int profundidade);

/**
 * int getIndiceContexto(ListaDeContextos lista, int tamanhoDaLista, char* identificacao, int profundidade)
 *  Retorna -1 se o contexto nao estiver armazenado
 *  Retorna um indice >= 0 se o contexto estiver armazenado
*/
int getIndiceContexto(ListaDeContextos *lista, char* identificacao, int profundidade);

Contexto* getContextoPorIndice(ListaDeContextos *lista, int indice);

Contexto* getContextoPorIdentificacao(ListaDeContextos *lista, char* identificacao, int profundidade);

int* getIndicesContextos_child(ListaDeContextos *lista_child, char* identificacao_parent, int profundidade_parent, int* qtdIndices_child);

IdentificacaoDosContextos* getIdentificacaoContextos_child(ListaDeContextos *lista_child, char* identificacao_parent, int profundidade_parent, int* qtdIds_child);

void inicializarListaDeContextos(ListaDeContextos *lista);

bool listaDeContextosEstaVazia(ListaDeContextos *lista);

void inserirContextoNaListaDeContextos(ListaDeContextos *lista, char* idenficicacao, int profundidade, int tamanhoDoAlfabeto);

void removerDaLista_identificacao(ListaDeContextos *lista, char* identificacao, int profundidade);

void removerDaLista_indice(ListaDeContextos *lista, int indice);

void registrarOcorrenciaDeContexto(ListaDeContextos *lista, int subsequente, char *identificacao, int profundidade);

void calculaRazaoDeTransicao(ListaDeContextos *lista, int tamanhoDoAlfabeto);

#endif  // _LISTA_DE_CONTEXTOS_H_