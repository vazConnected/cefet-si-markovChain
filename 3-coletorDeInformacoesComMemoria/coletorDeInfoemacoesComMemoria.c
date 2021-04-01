/*
 * Titulo: coletor de informacoes com memoria
 * Descricao: algoritmo que extrai a matriz de transicao de uma cadeia considerando contextos
 * 
 * Autor: Pedro Vaz
 * Data: marco de 2021
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

#define PROFUNDIDADE 2
#define TAMANHO_ALFABETO 2

typedef struct{
    char contexto[PROFUNDIDADE];
    unsigned int ocorrenciasDosSubsequentes[TAMANHO_ALFABETO]; // [0][X]: elementos; [1][X]: ocorrencias
    unsigned int somatorioDasOcorrenciasDosSubsequentes;
    double razaoDeTransicao[TAMANHO_ALFABETO];
}Contexto;

void imprimirIntro(){
    #ifdef __unix__
        system("clear");
    #elif defined(_WIN32) || defined(WIN32)
        system("cls");
    #endif

    printf("\nColetor de informacoes de uma cadeia de Markov");
    printf("\nDescricao: algoritmo que extrai a matriz de transicao de uma cadeia considerando contextos;");
    printf("\nAutor: Pedro Henrique Estevam Vaz de Melo;");
    printf("\nData: marco de 2020.\n\n");
}

void entrada_caminhoParaArquivo(char* caminhoParaArquivo){
    bool arquivoExiste = false;
    do{
        printf("\tDigite o nome do arquivo em que esta a cadeia a ser lida (max. 4096 char): ");
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

int main(){
    imprimirIntro();

    // Entrada para o caminho do arquivo
    char* caminhoParaArquivo = (char*) malloc(4096 * sizeof(char));
    entrada_caminhoParaArquivo(caminhoParaArquivo);

    // Criando contextos
    unsigned int quantidadeDeContextos = (unsigned int)pow(TAMANHO_ALFABETO, PROFUNDIDADE);
    Contexto* contextos = (Contexto*) malloc(quantidadeDeContextos * sizeof(Contexto));
    for(int i = 0; i < quantidadeDeContextos; i++){
        for(int j = 0; j < TAMANHO_ALFABETO; j++){
            contextos[i].ocorrenciasDosSubsequentes[j] = 0;
            contextos[i].somatorioDasOcorrenciasDosSubsequentes = 0;
            contextos[i].razaoDeTransicao[j] = 0.0;
        }
    }
    // Fixando a profundidade em 2 temporariamente
    contextos[0].contexto[0] = '0';
    contextos[0].contexto[1] = '0';

    contextos[1].contexto[0] = '0';
    contextos[1].contexto[1] = '1';

    contextos[2].contexto[0] = '1';
    contextos[2].contexto[1] = '0';

    contextos[3].contexto[0] = '1';
    contextos[3].contexto[1] = '1';

    // Ocorrencia de contextos e subsequentes
    char memoriaDaCadeia[PROFUNDIDADE];
    char elementoAtual;
    FILE *arquivo = fopen(caminhoParaArquivo, "r");
    memoriaDaCadeia[0] = fgetc(arquivo);
    memoriaDaCadeia[1] = fgetc(arquivo);
    do{
        elementoAtual = fgetc(arquivo);

        if(charEhAlgarismo(elementoAtual)){
            for(int i = 0; i < quantidadeDeContextos; i++){
                if(charEhAlgarismo(elementoAtual) && contextos[i].contexto[0] == memoriaDaCadeia[0] && contextos[i].contexto[1] == memoriaDaCadeia[1]){
                    contextos[i].ocorrenciasDosSubsequentes[charParaAlgarismo(elementoAtual)]++;
                    contextos[i].somatorioDasOcorrenciasDosSubsequentes++;
                }
            }
        }

        memoriaDaCadeia[0] = memoriaDaCadeia[1];
        memoriaDaCadeia[1] = elementoAtual;

    }while(elementoAtual != EOF);
    fclose(arquivo);
    
    // Calculo de estatisticas de transicao
    for(int i = 0; i < quantidadeDeContextos; i++){
        for(int j = 0; j < TAMANHO_ALFABETO; j++){
            contextos[i].razaoDeTransicao[j] = (double) contextos[i].ocorrenciasDosSubsequentes[j] / (double) contextos[i].somatorioDasOcorrenciasDosSubsequentes;
        }
    }

    // Imprimindo Resultados
    printf("\nResultados:");
    for(int i = 0; i < quantidadeDeContextos; i++){
        printf("\n\tContexto: ");
        for(int j = 0; j < PROFUNDIDADE; j++){
            printf("%c", contextos[i].contexto[j]);
        }
        printf("\t");

        for(int j = 0; j < TAMANHO_ALFABETO; j++){
            printf("subsequente %d : %lf\t", j, contextos[i].razaoDeTransicao[j]);
        }
    }
    printf("\n");

    return 0;
}
