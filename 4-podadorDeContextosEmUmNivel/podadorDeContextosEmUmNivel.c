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

char* elementosDoAlfabeto;
int tamanhoDoAlfabeto = 0;

int profundidade = 0;

double erro = 0.0;

char coringa = ' '; // Caractere utilizado para a comparacao entre diferentes profundidades

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

    printf("\nPodador de contextos em um nivel");
    printf("\nDescricao: analisar uma cadeia e apresentar os contextos com poda de profundidade menos 1;");
    printf("\nAutor: Pedro Henrique Estevam Vaz de Melo;");
    printf("\nData: abril de 2020.\n\n");
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

void buscaIndiceDeContextosDerivados(Contexto* contextos, int qtdContextos, int profundidadePresente, char* contextoProcurado, int *indices){
    int indiceDosIndices = 0;
    for(int x = 0; x < qtdContextos; x++){
        bool encontrado = true;
        for(int y = 0; y < profundidadePresente; y++){
            if(contextos[x].contexto[y] != contextoProcurado[y]){
                encontrado = false;
                break;
            }
        }
        if(encontrado){
            indices[indiceDosIndices] = x;
            indiceDosIndices++;
        }
    }
}

bool contextoJaArmazenado(Contexto* vetorContextos, int tamanhoVetorContexto, char* contextoProcurado){
    for(int x = 0; x < tamanhoVetorContexto; x++){
        for(int y = 0; y < profundidade; y++){
            if(vetorContextos[x].contexto[y] != contextoProcurado[y]){
                break;
            }else if( (y == profundidade - 1) && (vetorContextos[x].contexto[y] == contextoProcurado[y]) ){
                return true;
                break;
            }
        }
    }
    return false;
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

    // Cria contextos para a profundidade escolhida
    int quantidadeDeContextosProfundidade = (int)pow(tamanhoDoAlfabeto, profundidade);
    Contexto* contextosProfundidade = (Contexto*) malloc(quantidadeDeContextosProfundidade * sizeof(Contexto));
    for(int i = 0; i < quantidadeDeContextosProfundidade; i++){
        contextosProfundidade[i].contexto = (char*) malloc(profundidade * sizeof(char));
        contextosProfundidade[i].ocorrenciasDosSubsequentes = (unsigned int*) malloc(tamanhoDoAlfabeto * sizeof(unsigned int));
        contextosProfundidade[i].razaoDeTransicao = (double*) malloc(tamanhoDoAlfabeto * sizeof(double));

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            contextosProfundidade[i].ocorrenciasDosSubsequentes[j] = 0;
            contextosProfundidade[i].somatorioDasOcorrenciasDosSubsequentes = 0;
            contextosProfundidade[i].razaoDeTransicao[j] = 0.0;
        }
    }
    criaContextos(contextosProfundidade, 0, quantidadeDeContextosProfundidade, 0);

    // Cria contextos para a profundidade menos 1
    int quantidadeDeContextosProfundidadeMenos1 = (int)pow(tamanhoDoAlfabeto, (profundidade - 1) );
    Contexto* contextosProfundidadeMenos1 = (Contexto*) malloc(quantidadeDeContextosProfundidadeMenos1 * sizeof(Contexto));
    for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){
        contextosProfundidadeMenos1[i].contexto = (char*) malloc( (profundidade - 1) * sizeof(char));
        contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes = (unsigned int*) malloc(tamanhoDoAlfabeto * sizeof(unsigned int));
        contextosProfundidadeMenos1[i].razaoDeTransicao = (double*) malloc(tamanhoDoAlfabeto * sizeof(double));

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes[j] = 0;
            contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes = 0;
            contextosProfundidadeMenos1[i].razaoDeTransicao[j] = 0.0;
        }
    }
    criaContextos(contextosProfundidadeMenos1, 0, quantidadeDeContextosProfundidadeMenos1, 0);

    // Abre arquivo e inicia a memória da cadeia
    char memoriaDaCadeia[profundidade];
    FILE *arquivo = fopen(caminhoParaArquivo, "r");
    for(int i = profundidade - 1; i <= 0; i--){
        memoriaDaCadeia[i] = fgetc(arquivo);
    }
    
    // Registra a ocorrencia do primeiro contexto com a profundidade menos 1
    bool contextoEncontrado = false;
    for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){
        for(int j = 1, k = 0; j < profundidade; j++, k++){
            if(memoriaDaCadeia[j] != contextosProfundidadeMenos1[i].contexto[k]){
                break;
            }else if(memoriaDaCadeia[j] == contextosProfundidadeMenos1[i].contexto[k] && j == (profundidade - 1) ){
                contextoEncontrado = true;
                break;
            }
        }

        if(contextoEncontrado && charPertenceAoAlfabeto(contextosProfundidadeMenos1[i].contexto[0])){
            contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes[charParaAlgarismo(contextosProfundidadeMenos1[i].contexto[0])]++;
            contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes++;
            break;
        }
    }
    
    // Ocorrencia dos subsequentes
    char elementoAtual;
    do{
        elementoAtual = fgetc(arquivo);

        if(charEhAlgarismo(elementoAtual)){

            // Verificando se o contexto atual na profundidade menos 1 corresponde a algum contexto descito 
            for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){
                contextoEncontrado = false;
                for(int indiceMemoria = 1, k = 0; indiceMemoria < profundidade & k < profundidade - 1; indiceMemoria++, k++){
                    if(memoriaDaCadeia[indiceMemoria] != contextosProfundidadeMenos1[i].contexto[k]){
                        break;
                    }else if(memoriaDaCadeia[indiceMemoria] == contextosProfundidadeMenos1[i].contexto[k] && indiceMemoria == profundidade - 1){
                        contextoEncontrado = true;
                        break;
                    }
                }

                if(contextoEncontrado && charPertenceAoAlfabeto(elementoAtual)){
                    contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes[charParaAlgarismo(elementoAtual)]++;
                    contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes++; 
                    break;
                }
            }

            // Verificando se o contexto atual na profundidade selecionada corresponde a algum contexto descrito
            for(int i = 0; i < quantidadeDeContextosProfundidade; i++){
                contextoEncontrado = false;
                for(int j = 0; j < profundidade; j++){
                    if(memoriaDaCadeia[j] != contextosProfundidade[i].contexto[j]){
                        break;
                    }else if(memoriaDaCadeia[j] == contextosProfundidade[i].contexto[j] && j == profundidade - 1){
                        contextoEncontrado = true;
                        break;
                    }
                }

                if(contextoEncontrado && charPertenceAoAlfabeto(elementoAtual)){
                    contextosProfundidade[i].ocorrenciasDosSubsequentes[charParaAlgarismo(elementoAtual)]++;
                    contextosProfundidade[i].somatorioDasOcorrenciasDosSubsequentes++;
                    break;
                }
            }
        }
        
        // Reorganizando elementos da memoria da cadeia
        int indiceMemoriaDaCadeia = profundidade - 2;
        while(indiceMemoriaDaCadeia >= 0){
            memoriaDaCadeia[indiceMemoriaDaCadeia + 1] = memoriaDaCadeia[indiceMemoriaDaCadeia];
            indiceMemoriaDaCadeia--;
        }
        memoriaDaCadeia[0] = elementoAtual;

    }while(elementoAtual != EOF);
    fclose(arquivo);
    
    // Calculo de estatisticas de transicao para a profundidade selecionada
    for(int i = 0; i < quantidadeDeContextosProfundidade; i++){
        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextosProfundidade[i].somatorioDasOcorrenciasDosSubsequentes == 0){
                contextosProfundidade[i].razaoDeTransicao[j] = 0.0;
            }else{
                contextosProfundidade[i].razaoDeTransicao[j] = (double) contextosProfundidade[i].ocorrenciasDosSubsequentes[j] / (double) contextosProfundidade[i].somatorioDasOcorrenciasDosSubsequentes;
            }
        }
    }

    // Calculo de estatisticas de transicao para a profundidade menos 1
    for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){
        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes == 0){
                contextosProfundidadeMenos1[i].razaoDeTransicao[j] = 0.0;
            }else{
                contextosProfundidadeMenos1[i].razaoDeTransicao[j] = (double) contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes[j] / (double) contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes;
            }
        }
    }
    
    // Declaração do vetor com contextos depois da poda
    Contexto* contextosPodados;
    int quantidadeDeContextosAposPoda = 0;

    // Decobre os valores max tomando como referenca o vetor contextosProfundidadeMenos1
    char contextoProcurado[profundidade];
    for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){

        // Determinacao do contextoProcurado para descobrir os indices correspondentes no contextosProfundidade
        contextoProcurado[0] = coringa;
        for(int j = 1, k = 0; (j < profundidade) && (k < profundidade - 1); j++, k++){
           contextoProcurado[j] = contextosProfundidadeMenos1[i].contexto[k];
        }

        // Busca dos indices dos contestos correspondentes ao contextosProfundiadeMenos1[i] no contextosProfundidade
        int indiceContextosDerivados[tamanhoDoAlfabeto]; // Contextos que partem do contextosProfundidadeMenos1[i]
        for(int j = 0, indiceDoVetorDeIndices = 0; j < quantidadeDeContextosProfundidade; j++){
            bool encontrado = true;
            for(int k = 1; k < profundidade; k++){
                if(contextosProfundidade[j].contexto[k] != contextoProcurado[k]){
                    encontrado = false;
                }
            }
            if(encontrado){
                indiceContextosDerivados[indiceDoVetorDeIndices] = j;
                indiceDoVetorDeIndices++;
            }
        }

        // Determina a diferenca maxima entre as razoes de transicao dos contextoProfundidadeMenos1[i] e contextosProfundidade[ indiceContextosDerivados[j] ]
        bool algumDosIndicesFoiSalvoNosContextosPodados = false;
        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            double maxDiferenca = 0.0;

            for(int k = 0; k < tamanhoDoAlfabeto; k++){
                double diferencaAtual = fabs(contextosProfundidadeMenos1[i].razaoDeTransicao[k] - contextosProfundidade[ indiceContextosDerivados[j] ].razaoDeTransicao[k]);
                if(diferencaAtual > maxDiferenca){
                    maxDiferenca = diferencaAtual;
                }
            }

            if(maxDiferenca > erro && !contextoJaArmazenado(contextosPodados, quantidadeDeContextosAposPoda, contextoProcurado)){
                // realiza a poda
                quantidadeDeContextosAposPoda++;
                contextosPodados = (Contexto*) realloc(contextosPodados, quantidadeDeContextosAposPoda * sizeof(Contexto));

                contextosPodados[quantidadeDeContextosAposPoda - 1].contexto = (char*) malloc(profundidade * sizeof(char));
                contextosPodados[quantidadeDeContextosAposPoda - 1].contexto[0] = coringa;
                for(int j = 0, k = 1; j < profundidade - 1; k++, j++){
                    contextosPodados[quantidadeDeContextosAposPoda - 1].contexto[k] = contextosProfundidadeMenos1[i].contexto[j];
                }

                contextosPodados[quantidadeDeContextosAposPoda - 1].somatorioDasOcorrenciasDosSubsequentes = contextosProfundidadeMenos1[i].somatorioDasOcorrenciasDosSubsequentes;

                contextosPodados[quantidadeDeContextosAposPoda - 1].ocorrenciasDosSubsequentes = (unsigned int*) malloc(tamanhoDoAlfabeto * sizeof(unsigned int));
                memcpy(contextosPodados[quantidadeDeContextosAposPoda - 1].ocorrenciasDosSubsequentes, contextosProfundidadeMenos1[i].ocorrenciasDosSubsequentes, tamanhoDoAlfabeto * sizeof(unsigned int));

                contextosPodados[quantidadeDeContextosAposPoda - 1].razaoDeTransicao = (double*) malloc(tamanhoDoAlfabeto * sizeof(double));
                memcpy(contextosPodados[quantidadeDeContextosAposPoda - 1].razaoDeTransicao, contextosProfundidadeMenos1[i].razaoDeTransicao, tamanhoDoAlfabeto * sizeof(double));

                algumDosIndicesFoiSalvoNosContextosPodados = true;
            
            }
            
        }

        if(!algumDosIndicesFoiSalvoNosContextosPodados){
            // nao realiza a poda
            for(int j = 0; j < tamanhoDoAlfabeto; j++){
                if( !contextoJaArmazenado(contextosPodados, quantidadeDeContextosAposPoda, contextosProfundidade[ indiceContextosDerivados[j] ].contexto) ){
                    quantidadeDeContextosAposPoda++;
                    contextosPodados = (Contexto*) realloc(contextosPodados, quantidadeDeContextosAposPoda * sizeof(Contexto));

                    contextosPodados[quantidadeDeContextosAposPoda - 1].contexto = (char*) malloc(profundidade * sizeof(char));
                    memcpy(contextosPodados[quantidadeDeContextosAposPoda - 1].contexto, contextosProfundidade[ indiceContextosDerivados[j] ].contexto, profundidade * sizeof(char));

                    contextosPodados[quantidadeDeContextosAposPoda - 1].somatorioDasOcorrenciasDosSubsequentes = contextosProfundidade[ indiceContextosDerivados[j] ].somatorioDasOcorrenciasDosSubsequentes;

                    contextosPodados[quantidadeDeContextosAposPoda - 1].ocorrenciasDosSubsequentes = (unsigned int*) malloc(tamanhoDoAlfabeto * sizeof(unsigned int));
                    memcpy(contextosPodados[quantidadeDeContextosAposPoda - 1].ocorrenciasDosSubsequentes, contextosProfundidade[ indiceContextosDerivados[j] ].ocorrenciasDosSubsequentes, tamanhoDoAlfabeto * sizeof(unsigned int));

                    contextosPodados[quantidadeDeContextosAposPoda - 1].razaoDeTransicao = (double*) malloc(tamanhoDoAlfabeto * sizeof(double));
                    memcpy(contextosPodados[quantidadeDeContextosAposPoda - 1].razaoDeTransicao, contextosProfundidade[ indiceContextosDerivados[j] ].razaoDeTransicao, tamanhoDoAlfabeto * sizeof(double));
                }
            }
        }

        
    }

    // Imprimindo probabilidades para os contextos da profundidade escolhida (w)
    printf("\nResultados para os contextos w:");
    for(int i = 0; i < quantidadeDeContextosProfundidade; i++){
        printf("\n\tContexto w: ");
        for(int j = 0; j < profundidade; j++){
            printf("%c", contextosProfundidade[i].contexto[j]);
        }
        printf("\t");

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextosProfundidade[i].razaoDeTransicao[j] == 0.0){
                printf("p( %d | w ): --      \t", j);
            }else{
                printf("p( %d | w ): %lf\t", j, contextosProfundidade[i].razaoDeTransicao[j]);
            }
            
        }
    }
    printf("\n");

    // Imprimindo probabilidades para os contextos da profundidade do nivel podado (u)
    printf("\nResultados para os contextos u:");
    for(int i = 0; i < quantidadeDeContextosProfundidadeMenos1; i++){
        printf("\n\tContexto u: ");
        for(int j = 0; j < profundidade - 1; j++){
            printf("%c", contextosProfundidadeMenos1[i].contexto[j]);
        }
        printf("\t");

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextosProfundidadeMenos1[i].razaoDeTransicao[j] == 0.0){
                printf("p( %d | u ): --      \t", j);
            }else{
                printf("p( %d | u ): %lf\t", j, contextosProfundidadeMenos1[i].razaoDeTransicao[j]);
            }
            
        }
    }
    printf("\n");

    // Imprimindo contextos depois da poda (z)
    printf("\nResultados para os contextos z:");
    for(int i = 0; i < quantidadeDeContextosAposPoda; i++){
        printf("\n\tContexto u: ");
        for(int j = 0; j < profundidade; j++){
            printf("%c", contextosPodados[i].contexto[j]);
        }
        printf("\t");

        for(int j = 0; j < tamanhoDoAlfabeto; j++){
            if(contextosPodados[i].razaoDeTransicao[j] == 0.0){
                printf("p( %d | z ): --      \t", j);
            }else{
                printf("p( %d | z ): %lf\t", j, contextosPodados[i].razaoDeTransicao[j]);
            }
            
        }
    }
    printf("\n");

    return 0;
}
