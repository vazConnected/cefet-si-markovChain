/*
 * Titulo: podador de contextos em um nivel
 * Descricao: analisar uma cadeia e apresentar os contextos com poda de profundidade menos 1
 * 
 * Autor: Pedro Vaz
 * Data: abril de 2021
 * 
 * Software relacionado aa IC "Estimacao de Cadeias de Markov com
 *   Alcance Variavel e suas Aplicacoes"
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "lib/listaDeContextos.h"


char* elementosDoAlfabeto;
int tamanhoDoAlfabeto = 0;

int profundidade = 0;

double erro = 0.0;

void imprimirIntro(){
    #ifdef __unix__
        system("clear");
    #elif defined(_WIN32) || defined(WIN32)
        system("cls");
    #endif

    printf("\nPodador de contextos");
    printf("\nDescricao: analisar uma cadeia e apresentar os contextos com poda ate a raiz;");
    printf("\nAutor: Pedro Henrique Estevam Vaz de Melo;");
    printf("\nData: maio de 2020.\n\n");
}

void entrada_caminhoParaArquivo(char* caminhoParaArquivo){
    bool arquivoExiste = false;
    do{
        printf("\tNome do arquivo em que esta a cadeia a ser lida: ");
        scanf("%s", caminhoParaArquivo);

        FILE* arquivo = fopen(caminhoParaArquivo, "r");
        if(arquivo != NULL){
            arquivoExiste = true;
        }else{
            printf("\n\tERRO! O arquivo nao existe. Tente novamente.\n");
        }
    }while(!arquivoExiste);

    caminhoParaArquivo = (char*) realloc(caminhoParaArquivo, strlen(caminhoParaArquivo));
}

void entrada_profundidade(int *profundidade){
    bool entradaRealizadaComSucesso = false;
    while(!entradaRealizadaComSucesso){
        printf("\tProfundidade a ser analisada: ");
        scanf("%d", profundidade);
        if(*profundidade < 2){
            printf("Erro! A profundidade da cadeia deve ser maior ou igual a 2.\nPor favor, tente novamente!\n\n");
        }else{
            entradaRealizadaComSucesso = true;
        }
    }
}

void entrada_tamanhoDoAlfabeto(int *tamanhoDoAlfabeto){
    bool entradaRealizadaComSucesso = false;
    while(!entradaRealizadaComSucesso){
        printf("\tTamanho do alfabeto da cadeia a ser analisada: ");
        scanf("%d", tamanhoDoAlfabeto);
        if(*tamanhoDoAlfabeto < 1 || *tamanhoDoAlfabeto > 10){
            printf("Erro! O tamanho do alfabeto deve ser entre 1 e 10.\nPor favor, tente novamente!\n");
        }else{
            entradaRealizadaComSucesso = true;
        }
    }
}

void entrada_erro(double *erro){
    bool entradaRealizadaComSucesso = false;
    while(!entradaRealizadaComSucesso){
        printf("\tFaixa de erro para a poda da arvore: ");
        scanf("%lf", erro);
        if(*erro < 0.0 || *erro > 1.0){
            printf("Erro! O erro deve ser entre 0.0 e 1.0.\nPor favor, tente novamente!\n\n");
        }else{
            entradaRealizadaComSucesso = true;
        }
    }
}

bool charPertenceAoAlfabeto(char c){
    for(int i = 0; i < tamanhoDoAlfabeto; i++){
        if(elementosDoAlfabeto[i] == c){
            return true;
        }
    }
    return false;
}

bool charEhAlgarismo(char c){
    if(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9'){
        return true;
    }
    return false;
}

int charParaAlgarismo(char c){
    if(c == '0')
        return 0;
    else if(c == '1')
        return 1;
    else if(c == '2')
        return 2;
    else if(c == '3')
        return 3;
    else if(c == '4')
        return 4;
    else if(c == '5')
        return 5;
    else if(c == '6')
        return 6;
    else if(c == '7')
        return 7;
    else if(c == '8')
        return 8;
    else if(c == '9')
        return 9;
    else
        return -1;
}

char algarismoParaChar(int algarismo){
    if(algarismo == 1){
        return '1';
    }else if(algarismo == 2){
        return '2';
    }else if(algarismo == 3){
        return '3';
    }else if(algarismo == 4){
        return '4';
    }else if(algarismo == 5){
        return '5';
    }else if(algarismo == 6){
        return '6';
    }else if(algarismo == 7){
        return '7';
    }else if(algarismo == 8){
        return '8';
    }else if(algarismo == 9){
        return '9';
    }else if(algarismo == 0){
        return '0';
    }else{
        return 'x';
    }
}

void criaContextos(IdentificacaoDosContextos* contextos, int indiceContextosInicio, int indiceContextosFim, int indiceIdContexto){
    if(indiceIdContexto < contextos[indiceContextosInicio].profundidade){

        int faixaDeTrabalho = indiceContextosFim - indiceContextosInicio;
        int faixaDeRepeticao = faixaDeTrabalho / tamanhoDoAlfabeto;

        int indiceVetorAlfabeto = 0, indiceContextos = indiceContextosInicio;
        while(indiceVetorAlfabeto < tamanhoDoAlfabeto){
            for(int i = 0; i < faixaDeRepeticao; i++){
                contextos[indiceContextos].identificacao[indiceIdContexto] = elementosDoAlfabeto[indiceVetorAlfabeto];
                indiceContextos++;
            }
            indiceVetorAlfabeto++;
        }

        // Condicao de parada da recurcao
        if(faixaDeTrabalho == 1){
            return;
        }else{
            for(int i = indiceContextosInicio; i < indiceContextosFim; i += faixaDeRepeticao){
                criaContextos(contextos, i, i + faixaDeRepeticao, indiceIdContexto + 1);
            }
        }
    }
}

int main(){
    imprimirIntro();

    // Entrada para o caminho do arquivo
    char* caminhoParaArquivo = (char*) malloc(4096 * sizeof(char));
    entrada_caminhoParaArquivo(caminhoParaArquivo);

    // Entrada para o alfabeto da cadeia
    entrada_tamanhoDoAlfabeto(&tamanhoDoAlfabeto);

    // Entrada para a profundidade da cadeia
    entrada_profundidade(&profundidade);

    // Entrada para a faixa de erro para a poda da arvore
    entrada_erro(&erro);

    // Definicao do vetor de elementos do alfabeto
    elementosDoAlfabeto = (char*) malloc(tamanhoDoAlfabeto * sizeof(char));
    for(int i = 0; i < tamanhoDoAlfabeto; i++){
        elementosDoAlfabeto[i] = algarismoParaChar(i);
    }
    
    // Calcula a quantidade de contextos por profundidade
    unsigned int* qtdDeContextosPorProfundidade = malloc(profundidade * sizeof(unsigned int));
    for(int i = 0; i < profundidade; i++){
        qtdDeContextosPorProfundidade[i] = (unsigned int)pow(tamanhoDoAlfabeto, i + 1);
    }

    // Aloca memoria para contextos para cada profundidade
    IdentificacaoDosContextos** idContextos = (IdentificacaoDosContextos**) malloc(profundidade * sizeof(IdentificacaoDosContextos*)); // [profundidade][indiceIdentidade]
    for(int i = 0; i < profundidade; i++){
        idContextos[i] = (IdentificacaoDosContextos*) malloc(qtdDeContextosPorProfundidade[i] * sizeof(IdentificacaoDosContextos));
        for(int j = 0; j < qtdDeContextosPorProfundidade[i]; j++){
            idContextos[i][j].profundidade = i + 1;
            idContextos[i][j].identificacao = (char*) malloc(idContextos[i][j].profundidade * sizeof(char));
        }
    }
    
    // Criando identificacao dos contextos
    for(int i = 0; i < profundidade; i++){
        criaContextos(idContextos[i], 0, qtdDeContextosPorProfundidade[i], 0);
    }
    
    // Aloca memoria para a lista de contextos de diferentes profundidades e as inicializa
    ListaDeContextos* listas = (ListaDeContextos*) malloc(profundidade * sizeof(ListaDeContextos));
    for(int i = 0; i < profundidade; i++){
        inicializarListaDeContextos(&listas[i]);
    }
    
    // Registra identificacao dos contextos na lista de contextos da respectiva profundidade
    for(int i = 0; i < profundidade; i++){
        for(int j = 0; j < qtdDeContextosPorProfundidade[i]; j++){
            inserirContextoNaListaDeContextos(&listas[i], idContextos[i][j].identificacao, idContextos[i][j].profundidade, tamanhoDoAlfabeto);
        }
    }
    
    // Exibe os contextos que serao considerados em cada profundidade
    printf("\nContextos considerados para a analise da cadeia:");
    for(int _profundidade = 0; _profundidade < profundidade; _profundidade++){
        Celula* celulaAtual = listas[_profundidade].inicio;
        printf("\n\tprofundidade %d : ", _profundidade + 1);
        while(celulaAtual != NULL){
            for(int i = 0; i < celulaAtual->contexto->profundidade; i++){
                printf("%c", celulaAtual->contexto->identificacao[i]);
            }
            printf(" ");
            celulaAtual = celulaAtual->proximo;
        }
    }

    // Abre arquivo e inicia a memoria da cadeia
    FILE *arquivo = fopen(caminhoParaArquivo, "r");

    // Alocando memória e reenchendo bufferMemoriaCadeia
    char* bufferMemoriaDaCadeia = (char*) malloc( (profundidade + 1) * sizeof(char) );
    for(int indiceBuffer = 0; indiceBuffer <= profundidade; indiceBuffer++){
        bufferMemoriaDaCadeia[indiceBuffer] = fgetc(arquivo);
    }
    
    // Ocorrencia dos subsequentes
    char* auxId = (char*) malloc(sizeof(char));
    do{
        // Registra contextos
        for(int _profundidade = 0; _profundidade < profundidade; _profundidade++){

            // Preenche auxId
            auxId = (char*) realloc(auxId, (_profundidade + 1) * sizeof(char));
            for(int indiceCopia = 0; (indiceCopia <= _profundidade); indiceCopia++){
                auxId[indiceCopia] = bufferMemoriaDaCadeia[indiceCopia];
            }
            int subsequente = charParaAlgarismo(bufferMemoriaDaCadeia[_profundidade + 1]);

            // Inverte auxId (adequacao de literatura)
            if(_profundidade > 0){
                for(int i = 0, j = _profundidade; i < j; i++, j--){
                    char aux = auxId[i];
                    auxId[i] = auxId[j];
                    auxId[j] = aux;
                }
            }
            
            // Registro de ocorrencias
            registrarOcorrenciaDeContexto(&listas[_profundidade], subsequente, auxId, _profundidade + 1);
        }

        // Atualiza a bufferMemoriaDaCadeia
        for(int indiceBuffer = 0; indiceBuffer < profundidade; indiceBuffer++){
            bufferMemoriaDaCadeia[indiceBuffer] = bufferMemoriaDaCadeia[indiceBuffer + 1];
        }

        do{
            bufferMemoriaDaCadeia[profundidade] = fgetc(arquivo);
            if(bufferMemoriaDaCadeia[profundidade] == EOF)
                break;
        }while( !charPertenceAoAlfabeto(bufferMemoriaDaCadeia[profundidade]) );
        
    }while(bufferMemoriaDaCadeia[profundidade] != EOF);
    free(auxId);
    fclose(arquivo);
    
    // Calcula as razões de transição para os contextos
    for(int indiceListas = 0; indiceListas < profundidade; indiceListas++){
        calculaRazaoDeTransicao(&listas[indiceListas], tamanhoDoAlfabeto);
    }
    //int contador1 = 0;
    // Imprime estado dos contextos antes da poda
    printf("\n\nContextos antes da poda: ");
    for(int indiceListas = 0; indiceListas < profundidade; indiceListas++){
        printf("\n\tProfundidade %d: ", indiceListas + 1);
        Celula *celulaAtual = listas[indiceListas].inicio;
        
        while(celulaAtual != NULL){
            //if(celulaAtual->contexto->somatorioDasOcorrenciasDosSubsequentes != 0) contador1++;
            printf("\n\t  Id. (w): ");
            for(int indiceIdenficacao = 0; indiceIdenficacao < indiceListas + 1; indiceIdenficacao++){
                printf("%c", celulaAtual->contexto->identificacao[indiceIdenficacao]);
            }
            printf("\t");
            
            for(int indiceRazaoTransicao = 0; indiceRazaoTransicao < tamanhoDoAlfabeto; indiceRazaoTransicao++){
                if(celulaAtual->contexto->razaoDeTransicao[indiceRazaoTransicao] > 0)
                    printf("p(w|%d): %lf\t", indiceRazaoTransicao, celulaAtual->contexto->razaoDeTransicao[indiceRazaoTransicao]);
                else
                    printf("p(w|%d): --      \t", indiceRazaoTransicao);
            }
            celulaAtual = celulaAtual->proximo;
        }
        printf("\n");
    }    

    // Podando contextos
    int indiceVetorListasDeContexto = profundidade - 1;
    while(indiceVetorListasDeContexto > 0){
        Celula* celulaAtual_parent = listas[indiceVetorListasDeContexto - 1].inicio;
        
        while(celulaAtual_parent != NULL){
            
            PacoteCelula* celulas_child;
            int qtdCelulas_child = 0;

            // Adquire celulas "depentendes" da celulaAtual_parent
            Celula* celulaAtual_child = listas[indiceVetorListasDeContexto].inicio;
            while(celulaAtual_child != NULL){

                bool encontrado = true;
                for(int child = 1, parent = 0; parent < celulaAtual_parent->contexto->profundidade; child++, parent++){
                    if(celulaAtual_parent->contexto->identificacao[parent] != celulaAtual_child->contexto->identificacao[child]){
                        encontrado = false;
                        break;
                    }
                }

                if(encontrado){
                    qtdCelulas_child++;
                    celulas_child = (PacoteCelula*) realloc(celulas_child, qtdCelulas_child * sizeof(PacoteCelula));
                    celulas_child[qtdCelulas_child - 1].celula = celulaAtual_child;
                }

                celulaAtual_child = celulaAtual_child->proximo;
            }
            
            // Desabilita contextos podaveis
            if(qtdCelulas_child != 0){

                for(int indiceContexto = 0; indiceContexto < qtdCelulas_child; indiceContexto++){
                    double maxDiferenca = 0.0;
                    for(int idSubsequente = 0; idSubsequente < tamanhoDoAlfabeto; idSubsequente++){
                        double diferencaAtual = fabs(celulaAtual_parent->contexto->razaoDeTransicao[idSubsequente] - celulas_child[indiceContexto].celula->contexto->razaoDeTransicao[idSubsequente]);
                        if(maxDiferenca < diferencaAtual){
                            maxDiferenca = diferencaAtual;
                        }
                    }

                    if(maxDiferenca < erro || celulas_child[indiceContexto].celula->contexto->somatorioDasOcorrenciasDosSubsequentes == 0){
                        celulas_child[indiceContexto].celula->disabled = true;
                    }
                }
            }
            
            celulaAtual_parent = celulaAtual_parent->proximo;
        }
        indiceVetorListasDeContexto--;
    }

    
    // Imprimindo depois da poda
    printf("\n\nContextos depois da poda: ");
    //int contador2=0;
    for(int indiceListas = 0; indiceListas < profundidade; indiceListas++){
        printf("\n\tProfundidade %d: ", indiceListas + 1);
        Celula *celulaAtual = listas[indiceListas].inicio;
        while(celulaAtual != NULL){
            //if(celulaAtual->disabled) contador2++;
            if(!celulaAtual->disabled){
                printf("\n\t  Id. (w): ");
                for(int indiceIdenficacao = 0; indiceIdenficacao < indiceListas + 1; indiceIdenficacao++){
                    printf("%c", celulaAtual->contexto->identificacao[indiceIdenficacao]);
                }
                printf("\t");
                
                for(int indiceRazaoTransicao = 0; indiceRazaoTransicao < tamanhoDoAlfabeto; indiceRazaoTransicao++){
                    if(celulaAtual->contexto->razaoDeTransicao[indiceRazaoTransicao] > 0)
                        printf("p(w|%d): %lf\t", indiceRazaoTransicao, celulaAtual->contexto->razaoDeTransicao[indiceRazaoTransicao]);
                    else
                        printf("p(w|%d): --      \t", indiceRazaoTransicao);
                }
            }
            
            celulaAtual = celulaAtual->proximo;
        }
        printf("\n");
    }    
  
    //printf("\ndiferenca = %d", contador1 - contador2);
    return 0;
}
