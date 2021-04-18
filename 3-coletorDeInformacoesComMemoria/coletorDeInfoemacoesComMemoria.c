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

char* elementosDoAlfabeto;
int tamanhoDoAlfabeto = 0;

int profundidade = 0;

typedef struct{
    char *contexto;
    unsigned int *ocorrenciasDosSubsequentes;
    unsigned int somatorioDasOcorrenciasDosSubsequentes;
    double *razaoDeTransicao;
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
        printf("\tDigite o nome do arquivo em que esta a cadeia a ser lida: ");
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
        return ' ';
    }
}

void criaContextos(Contexto *contextos, int indiceContextosInicio, int indiceContextosFim, int indiceIdContexto){
    if(indiceIdContexto < profundidade){
        int faixaDeTrabalho = indiceContextosFim - indiceContextosInicio;
        int faixaDeRepeticao = faixaDeTrabalho / tamanhoDoAlfabeto;

        int indiceVetorAlfabeto = 0, indiceContextos = indiceContextosInicio;
        while(indiceVetorAlfabeto < tamanhoDoAlfabeto){
            for(int i = 0; i < faixaDeRepeticao; i++){
                contextos[indiceContextos].contexto[indiceIdContexto] = elementosDoAlfabeto[indiceVetorAlfabeto];
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
    bool entradaRealizadaComSucesso = false;
    while(!entradaRealizadaComSucesso){
        printf("\n\tDigite o tamanho do alfabeto da cadeia a ser analisada: ");
        scanf("%d", &tamanhoDoAlfabeto);
        if(tamanhoDoAlfabeto < 1 || tamanhoDoAlfabeto > 10){
            printf("Erro! O tamanho do alfabeto deve ser entre 1 e 10.\nPor favor, tente novamente!");
        }else{
            entradaRealizadaComSucesso = true;
        }
    }

    // Entrada para a profundidade da cadeia
    entradaRealizadaComSucesso = false;
    while(!entradaRealizadaComSucesso){
        printf("\n\tDigite a profundidade a ser analisada: ");
        scanf("%d", &profundidade);
        if(profundidade < 1){
            printf("Erro! A profundidade da cadeia deve smaior que 1.\nPor favor, tente novamente!");
        }else{
            entradaRealizadaComSucesso = true;
        }
    }

    // Definicao do vetor de elementos do alfabeto
    elementosDoAlfabeto = (char*) malloc(tamanhoDoAlfabeto * sizeof(char));
    for(int i = 0; i < tamanhoDoAlfabeto; i++){
        elementosDoAlfabeto[i] = algarismoParaChar(i);
    }

    // Cria contextos
    int quantidadeDeContextos = (int)pow(tamanhoDoAlfabeto, profundidade);
    Contexto* contextos = (Contexto*) malloc(quantidadeDeContextos * sizeof(Contexto));
    for(int i = 0; i < quantidadeDeContextos; i++){
        contextos[i].contexto = (char*) malloc(profundidade * sizeof(char));
        contextos[i].ocorrenciasDosSubsequentes = (unsigned int*) malloc(tamanhoDoAlfabeto * sizeof(unsigned int));
        contextos[i].razaoDeTransicao = (double*) malloc(tamanhoDoAlfabeto * sizeof(double));

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            contextos[i].ocorrenciasDosSubsequentes[j] = 0;
            contextos[i].somatorioDasOcorrenciasDosSubsequentes = 0;
            contextos[i].razaoDeTransicao[j] = 0.0;
        }
    }

    criaContextos(contextos, 0, quantidadeDeContextos, 0);

    // Ocorrencia de contextos e subsequentes
    char memoriaDaCadeia[profundidade];
    char elementoAtual;
    FILE *arquivo = fopen(caminhoParaArquivo, "r");
    for(int i = 0; i < profundidade; i++){
        memoriaDaCadeia[i] = fgetc(arquivo); // Carregando vetor memoria da cadeia
    }

    do{
        elementoAtual = fgetc(arquivo);
        
        // Verificando se o contexto atual
        if(charEhAlgarismo(elementoAtual)){
            for(int i = 0; i < quantidadeDeContextos; i++){
                bool contextoEncontrado = false;
                for(int j = 0; j < profundidade; j++){
                    if(memoriaDaCadeia[j] != contextos[i].contexto[j]){
                        break;
                    }else if(memoriaDaCadeia[j] == contextos[i].contexto[j] && j == profundidade - 1){
                        contextoEncontrado = true;
                        break;
                    }
                }

                if(contextoEncontrado){
                    contextos[i].ocorrenciasDosSubsequentes[charParaAlgarismo(elementoAtual)]++;
                    contextos[i].somatorioDasOcorrenciasDosSubsequentes++;
                    break;
                }
            }
        }
        
        // Reorganizando elementos da memoria da cadeia
        for(int i = 1; i < profundidade; i++){
            memoriaDaCadeia[i - 1] = memoriaDaCadeia[i];
        }
        memoriaDaCadeia[profundidade - 1] = elementoAtual;

    }while(elementoAtual != EOF);
    fclose(arquivo);
    
    // Calculo de estatisticas de transicao
    for(int i = 0; i < quantidadeDeContextos; i++){
        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextos[i].somatorioDasOcorrenciasDosSubsequentes == 0){
                contextos[i].razaoDeTransicao[j] = 0.0;
            }else{
                contextos[i].razaoDeTransicao[j] = (double) contextos[i].ocorrenciasDosSubsequentes[j] / (double) contextos[i].somatorioDasOcorrenciasDosSubsequentes;
            }
        }
    }

    // Imprimindo Resultados
    printf("\nResultados:");
    for(int i = 0; i < quantidadeDeContextos; i++){
        printf("\n\tContexto: ");
        for(int j = 0; j < profundidade; j++){
            printf("%c", contextos[i].contexto[j]);
        }
        printf("\t");

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            printf("subsequente %d : %lf\t", j, contextos[i].razaoDeTransicao[j]);
        }
    }
    printf("\n");

    return 0;
}
