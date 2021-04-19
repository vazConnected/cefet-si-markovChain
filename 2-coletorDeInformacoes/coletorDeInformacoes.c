/*
 * Titulo: coletor de informacoes de uma cadeia de Markov
 * Descricao: algoritmo que extrai informacoes (matriz de transicao
 *   e distribuicao geral) de uma cadeia de algarismos salva em um arquivo
 * 
 * Autor: Pedro Henrique Estevam Vaz de Melo
 * Data: dezembro de 2020 
 * 
 * Software relacionado aa IC "Estimacao de Cadeias de Markov com
 *   Alcance Variavel e suas Aplicacoes"
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAMANHO 10 // Qtd de algarismos analisados

void imprimirIntro(){
    #ifdef __unix__
        system("clear");
    #elif defined(_WIN32) || defined(WIN32)
        system("cls");
    #endif

    printf("\nColetor de informacoes de uma cadeia de Markov");
    printf("\nDescricao: algoritmo que extrai informacoes (matriz de transicao e distribuicao geral) de uma cadeia salva em um arquivo;");
    printf("\nAutor: Pedro Henrique Estevam Vaz de Melo;");
    printf("\nData: dezembro de 2020.\n\n");
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

    long tamanhoDaCadeia = 0; // Elementos validos identificados
    long quantidadeDeElementosIgnorados = 0; // Elementos que nao sao algarismos

    // Declaracao e inicializacao dos elementos dos vetores e matrizes
    double distribuicaoGeral[TAMANHO];
    double matrizDeTransicoes[TAMANHO][TAMANHO];
    long contadorDeTransicoes[TAMANHO][TAMANHO];
    long quantidadeDeElementosCapturadosPorAlgarismo[TAMANHO];
    for(int i = 0; i < TAMANHO; i++){
        distribuicaoGeral[i] = 0.0;
        quantidadeDeElementosCapturadosPorAlgarismo[i] = 0;
        for(int j = 0; j < TAMANHO; j++){
            matrizDeTransicoes[i][j] = 0.0;
            contadorDeTransicoes[i][j] = 0.0;
        }
    }

    // Entrada para o caminho do arquivo
    char* caminhoParaArquivo = (char*) malloc(4096 * sizeof(char));
    entrada_caminhoParaArquivo(caminhoParaArquivo);

    printf("\n\nCalculando estatisticas sobre a cadeia armazenada no arquivo %s", caminhoParaArquivo);

    int ultimosElementosDaCadeia[2]; // Elementos, respectivamente, anterior e atual
    ultimosElementosDaCadeia[0] = -1;

    FILE* arquivo = fopen(caminhoParaArquivo, "r");

    // Capturando o 1o elemento cadeia
    char charCapturado;
    do{
        charCapturado = fgetc(arquivo);
        if(charCapturado == EOF){
            printf("\nERRO! O arquivo selecionado nao possui elementos validos.");
            return 1;
        }else if(charEhAlgarismo(charCapturado)){
            ultimosElementosDaCadeia[1] = charParaAlgarismo(charCapturado);
            quantidadeDeElementosCapturadosPorAlgarismo[ ultimosElementosDaCadeia[1] ]++;
            tamanhoDaCadeia++;
        }else if(!charEhAlgarismo(charCapturado) && charCapturado != EOF){
            quantidadeDeElementosIgnorados++;
        }
    }while(!charEhAlgarismo(charCapturado));

    // Capturando os demais elementos da cadeia    
    do{
        charCapturado = fgetc(arquivo);
        if(charEhAlgarismo(charCapturado)){
            int algarismoCapturado = charParaAlgarismo(charCapturado);

            contadorDeTransicoes[ ultimosElementosDaCadeia[1] ][ algarismoCapturado ]++;
            distribuicaoGeral[algarismoCapturado]++;

            quantidadeDeElementosCapturadosPorAlgarismo[ algarismoCapturado ]++;

            ultimosElementosDaCadeia[0] = ultimosElementosDaCadeia[1];
            ultimosElementosDaCadeia[1] = algarismoCapturado;

            tamanhoDaCadeia++;
        }else if(!charEhAlgarismo(charCapturado) && charCapturado != EOF){
            quantidadeDeElementosIgnorados++;
        }
    }while(charCapturado != EOF);

    fclose(arquivo);

    // Calcula a variedade da cadeia
    int variedadeDaCadeia = 0;
    for(int i = 0; i < TAMANHO; i++){
        if(quantidadeDeElementosCapturadosPorAlgarismo[i] != 0){
            variedadeDaCadeia++;
        }
    }

    // Calcula distribuicao geral da cadeia
    for(int i = 0; i < TAMANHO; i++){
        distribuicaoGeral[i] = quantidadeDeElementosCapturadosPorAlgarismo[i] / (double) tamanhoDaCadeia;
    }

    // Inicializando vetor com a quantidade de ocorrencias dos algarismos
    long qtdDeOcorrenciaDosAlgarismos[TAMANHO];
    for(int i = 0; i < TAMANHO; i++){
        qtdDeOcorrenciaDosAlgarismos[i] = 0;
    }

    // Preenche o vetor com a quantidade de ocorrencias dos algarismos
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            qtdDeOcorrenciaDosAlgarismos[i] += contadorDeTransicoes[i][j];
        }
    }
    
    // Calcula matriz de transicao
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(contadorDeTransicoes[i][j] == 0){
                matrizDeTransicoes[i][j] = 0.0;
            }else{
                matrizDeTransicoes[i][j] = (double) contadorDeTransicoes[i][j] / (double) qtdDeOcorrenciaDosAlgarismos[i];
            }
        }
    }
    
    // Imprimindo estatísticas coletadas
    printf("\n\nEstatísticas da cadeia :");
    printf("\n\tVariedade da cadeia : %d elemento(s)", variedadeDaCadeia);
    printf("\n\tQuantidade de elementos capturados : %lu", tamanhoDaCadeia);
    printf("\n\tQuantidade de elementos ignorados por nao serem algarismos : %lu", quantidadeDeElementosIgnorados);
    
    printf("\n\n\tOcupacao na cadeia :");
    for(int i = 0; i < TAMANHO; i++){
        if(distribuicaoGeral[i] != 0.0)
            printf("\n\t\tElemento %d : %lf", i, distribuicaoGeral[i]);
    }

    printf("\n\n\tMatriz de transicao :");
    for(int i = 0; i < TAMANHO; i++){
        for(int j = 0; j < TAMANHO; j++){
            if(matrizDeTransicoes[i][j] != 0.0)
                printf("\n\t\tDe %d para %d : %lf", i, j, matrizDeTransicoes[i][j]);
        }
    }

    printf("\n");

    return 0;
}
