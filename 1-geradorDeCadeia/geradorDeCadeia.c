/*
 * Titulo: gerador de cadeia de Markov
 * Descricao: algoritmo interativo para a interação de Cadeias de
 *   Markov e salvar informacoes em arquivo texto
 * 
 * Autor: Pedro Vaz
 * Data: dezembro de 2020 
 * 
 * Software relacionado aa IC "Estimacao de Cadeias de Markov com
 *   Alcance Variavel e suas Aplicacoes"
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

void imprimirIntro(){
    #ifdef __unix__
        system("clear");
    #elif defined(_WIN32) || defined(WIN32)
        system("cls");
    #endif

    printf("\nGerador de Cadeia de Markov");
    printf("\nDescricao: algoritmo interativo para a interação de Cadeias de Markov e salvar informacoes em arquivo texto;");
    printf("\nAutor: Pedro Henrique Estevam Vaz de Melo;");
    printf("\nData: dezembro de 2020.\n");
}

void entrada_tamanhoVariedade(long *tamanhoDaCadeia, int *variedadeDaCadeia){
    do{
        printf("\n\tDigite o espaco de estados E (0,1,2,...,E-1) (max.: 9) : ");
        scanf("%d", variedadeDaCadeia);

        if(*variedadeDaCadeia < 0 || *variedadeDaCadeia > 10)
            printf("\n\tERRO! O espaco de estados deve ser entre 1 e 10 (intervalo fechado).");
    }while(*variedadeDaCadeia < 0 || *variedadeDaCadeia > 10);
    
    printf("\tDigite o tamanho da cadeia a ser gerada : ");
    scanf("%ld", tamanhoDaCadeia);
}

void entrada_caminhoParaArquivo(char* caminhoParaArquivo){
    bool arquivoExiste = false;
    do{
        printf("\tDigite o nome do arquivo a ser salva a cadeia (max. 4096 char): ");
        scanf("%s", caminhoParaArquivo);

        FILE* arquivo = fopen(caminhoParaArquivo, "w");
        if(arquivo != NULL){
            arquivoExiste = true;
        }else{
            printf("\n\tERRO! O arquivo nao existe. Tente novamente.");
        }
    }while(!arquivoExiste);

    int tamanhoDaString = strlen(caminhoParaArquivo);
    caminhoParaArquivo = (char*) realloc (caminhoParaArquivo, tamanhoDaString);
}

int geradorDeElemento(double *copiaLinhaTransicao, int variedadeDaCadeia){
    int idElementos[variedadeDaCadeia];

    // Preenchendo o vetor de  identificacao dos elementos
    for(int i = 0; i < variedadeDaCadeia; i++){
        idElementos[i] = i;
    }
    
    // Organizando elementos da linha da matriz de transicao em ordem crescente
    for(int i = 0; i < variedadeDaCadeia; i++){
        for(int j = i; j < variedadeDaCadeia; j++){
            if(copiaLinhaTransicao[j] < copiaLinhaTransicao[i]){
                double auxLinhaTransicao = copiaLinhaTransicao[i];
                copiaLinhaTransicao[i] = copiaLinhaTransicao[j];
                copiaLinhaTransicao[j] = auxLinhaTransicao;

                // Associando a nova posicao do elemento a sua identificacao
                int auxIdElementos = idElementos[i];
                idElementos[i] = idElementos[j];
                idElementos[j] = auxIdElementos;
            }
        }
    }

    // Gerando numero aleatorio para a selecao do proximo elemento da cadeia
    double razao = (double) (rand() % 100001) / 100000.0;

    // Selecao do proximo elemento da cadeia
    int elementoGerado = -1;
    double somatorioDeRazoes = 0.0;
    for(int i = 0; i < variedadeDaCadeia ; i++){
        if(razao >= somatorioDeRazoes && razao <= ( somatorioDeRazoes + copiaLinhaTransicao[i] )){
            elementoGerado = idElementos[i];
            break;
        }else{
            somatorioDeRazoes += copiaLinhaTransicao[i];
        }
    }

    return elementoGerado;
}

char algarismoParaChar(int numero){
    if(numero == 0)
        return '0';
    else if(numero == 1)
        return '1';
    else if(numero == 2)
        return '2';
    else if(numero == 3)
        return '3';
    else if(numero == 4)
        return '4';
    else if(numero == 5)
        return '5';
    else if(numero == 6)
        return '6';
    else if(numero == 7)
        return '7';
    else if(numero == 8)
        return '8';
    else if(numero == 9)
        return '9';
    else
        return 'x';
}

void salvarElementoEmArquivo(char* caminhoParaArquivo, char elemento){
    FILE* arquivoTexto = fopen(caminhoParaArquivo, "a");
    fputc(elemento, arquivoTexto);
    fclose(arquivoTexto);
}

void calcularDistribuicaoGeral(long* ocupacaoNaCadeia, double* distribuicaoGeral, long tamanhoNaCadeia){
    for(long i = 0; i < tamanhoNaCadeia; i++){
        distribuicaoGeral[i] = (double) ocupacaoNaCadeia[i] / tamanhoNaCadeia;
    }
}

bool somatorioValido(double* vetor, int tamanho){
    /* Fix: bug que nao valida o somatorio em uma ordem especifica
     *   Motivado pelo erro na validacao ca comparacao  (0.7 + 0.2 + 0.1) != 1, que resultava como menor
     */
    
    double somatorio = 0.0;
    for(int i = 0; i < tamanho; i++){
        somatorio += vetor[i];
    }

    if( abs(somatorio - 1) == 0.0 )
        return true;

    return false;
}

int main(){
    imprimirIntro();
    srand(time(NULL));

    int ultimoElementoDaCadeia = -1;
    long tamanhoDaCadeia; // Quantidade de elementos a serem gerados e salvos no arquivo
    int variedadeDaCadeia; // Diversidade de elementos singulares a serem salvos na cadeia (varia de 1 a 10)
    char* caminhoParaArquivo = (char*) malloc(4096 * sizeof(char));

    entrada_tamanhoVariedade(&tamanhoDaCadeia, &variedadeDaCadeia);
    entrada_caminhoParaArquivo(caminhoParaArquivo);    

    double* distribuicaoInicial = (double*) malloc(variedadeDaCadeia * sizeof(double));
    double* distribuicaoGeral = (double*) malloc(variedadeDaCadeia * sizeof(double));
    long* ocupacaoNaCadeia = (long*) malloc(variedadeDaCadeia * sizeof(long));

    double** matrizDeTransicao = (double**) malloc(variedadeDaCadeia * sizeof(double*));
    for(int i = 0; i < variedadeDaCadeia; i++){
        matrizDeTransicao[i] = (double*) malloc(variedadeDaCadeia * sizeof(double));
    }
    
    // Zerando as distribuicaoes e ocupacao
    for(int i = 0; i < variedadeDaCadeia; i++){
        distribuicaoInicial[i] = 0.0;
        distribuicaoGeral[i] = 0.0;
        ocupacaoNaCadeia[i] = 0;
    }
    
    // Entradas para o vetor de distrubuicao inicial
    printf("\n\nEntradas para o vetor de distribuicao inicial :\n");
    bool somatorioValidado = false;
    while(!somatorioValidado){
        for(int i = 0; i < variedadeDaCadeia; i++){
            printf("\tDistribuicao inicial para o elemento %d : ", i);
            scanf("%lf", &distribuicaoInicial[i]);
        }
        somatorioValidado = somatorioValido(distribuicaoInicial, variedadeDaCadeia);
        if(!somatorioValidado){
            printf("\n\tERRO. O somatorio dos valores no vetor de distribuicao inicial deve totalizar 1.0 (100 porcento). Tente novamente!\n");
        }
    }

    // Entradas para a matriz de Transicao
    printf("\n\nEntradas para a matriz de transicao :\n");
    for(int i = 0; i < variedadeDaCadeia; i++){
        double vetorLinhaAtual[variedadeDaCadeia];
        for(int j = 0; j < variedadeDaCadeia; j++){
            printf("\tTransicao de %d para %d : ", i, j);
            scanf("%lf", &matrizDeTransicao[i][j]);
            vetorLinhaAtual[i] = matrizDeTransicao[i][j];
        }
        somatorioValidado = somatorioValido(vetorLinhaAtual, variedadeDaCadeia);
        if(!somatorioValidado){
            printf("\nERRO! o somatorio dos valores linha da matriz de transicao devem totalizar 1.0 (100 porcento). Tente novamente!\n");
            i--;
        }
        printf("\n");
    }

    // Geracao dos elementos da cadeia
    printf("\n\nGerando elementos da cadeia...");
    printf("\n\tCadeia: ");
    int elementosGerados = 0;
    do{
        if(elementosGerados == 0){
            ultimoElementoDaCadeia = geradorDeElemento(distribuicaoInicial, variedadeDaCadeia);
        }else{
            double copiaDaLinhaDaMatriz[variedadeDaCadeia];
            for(int i = 0; i < variedadeDaCadeia; i++){
                copiaDaLinhaDaMatriz[i] = matrizDeTransicao[ultimoElementoDaCadeia][i];
            }
            ultimoElementoDaCadeia = geradorDeElemento(copiaDaLinhaDaMatriz, variedadeDaCadeia);
        }
        salvarElementoEmArquivo(caminhoParaArquivo, algarismoParaChar(ultimoElementoDaCadeia));
        elementosGerados++;
        ocupacaoNaCadeia[ ultimoElementoDaCadeia ]++;

        printf("%d ", ultimoElementoDaCadeia);
    }while(elementosGerados < tamanhoDaCadeia);
    
    printf("\n\nCadeia Gerada! Elementos salvos em : %s", caminhoParaArquivo);

    // Preenchendo distribuicao geral
    printf("\n\nDistribuicao invariante da cadeia :");
    for(int i = 0; i < variedadeDaCadeia; i++){
        distribuicaoGeral[i] = (double) ocupacaoNaCadeia[i] / (double) tamanhoDaCadeia;
        printf("\n\tElemento %d : %lf", i, distribuicaoGeral[i]);
    }

    printf("\n");

    return 0;
}
