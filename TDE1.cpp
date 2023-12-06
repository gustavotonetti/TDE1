#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#define TM_MAX_DATA 1000
#define NUM_LINHAS 44	//Colocar o numero que você quer + 4

typedef struct {
    char nome[100];
    char id[100];  
    char categoria[50];
    char avaliacao[10];
} Infrmc;

void buscarPorNumero(FILE *arquivo, int numeroProcurado) {
    rewind(arquivo);	//Indicador de posição para a posição inicial

    char linha[TM_MAX_DATA];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Extrai o número da linha
        int numeroNaLinha;
        sscanf(linha, "%d", &numeroNaLinha);

        if (numeroNaLinha == numeroProcurado) {
            printf("Linha: %s", linha);
        }
    }

}

void escreveLinhaNumero(void){
	FILE *arquivo = fopen("ArquivoFinal.dat", "rb");
    int numeroProcurado;
    
    printf("Digite o numero a ser procurado: ");
    scanf("%d", &numeroProcurado);

    buscarPorNumero(arquivo, numeroProcurado);

    fclose(arquivo);
}

// Comparação para qsort
int compararNomes(const void *a, const void *b) {
    return strcasecmp(((Infrmc *)a)->nome, ((Infrmc *)b)->nome);
}

void ordenaNomes(void){
	FILE *arquivoEntrada = fopen("saida.csv", "r");
    FILE *arquivoSaida = fopen("saida2.csv", "w");
	char linha[TM_MAX_DATA];

    // Le a primeira linha (cabeçalho)
    if (fgets(linha, sizeof(linha), arquivoEntrada) != NULL) {
        // Imprimir o cabeçalho
        fprintf(arquivoSaida, "%s", linha);

        // Processa cada linha do arquivo (excluindo a primeira linha)
        Infrmc Infrmcs[NUM_LINHAS];
        int numLinhas = 0;

        // Ler dados do arquivo (excluindo a primeira linha)
        while (fgets(linha, sizeof(linha), arquivoEntrada) != NULL && numLinhas < NUM_LINHAS) {
            // Extrair dados da linha
            sscanf(linha, "%100[^,],%100[^,],%50[^,],%10[^\n]", Infrmcs[numLinhas].nome,
                   Infrmcs[numLinhas].id, Infrmcs[numLinhas].categoria, Infrmcs[numLinhas].avaliacao);
            numLinhas++;
        }

        // Ordenar os dados por nome
        qsort(Infrmcs, numLinhas, sizeof(Infrmc), compararNomes);

        // Escrever os dados ordenados no novo arquivo (excluindo a primeira linha)
        for (int i = 0; i < numLinhas; i++) {
            fprintf(arquivoSaida, "%s,%s,%s,%s\n", Infrmcs[i].nome, Infrmcs[i].id,
                    Infrmcs[i].categoria, Infrmcs[i].avaliacao);
        }
    }
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}


void extrairDados4Virgula(const char *linha, char *dados) {
    int ContVirg = 0;
    size_t tmh = strlen(linha);

    // Copiar caracteres até antes da quarta vírgula
    for (size_t i = 0; i < tmh && ContVirg < 4; i++) {
        if (linha[i] == ',') {
            ContVirg++;
        }
        *dados = linha[i];
        dados++;
    }

    // Adicionar o caractere nulo ao final da string de destino
    *dados = '\0';
}

void removerVirgulaFinal(char *linha) {
    size_t tmh = strlen(linha);

    // Verificar se o último caractere é uma vírgula
    if (tmh > 0 && linha[tmh - 1] == ',') {
        // Remover a vírgula
        linha[tmh - 1] = '\0';
    }
}

void extrairDados(void){
	FILE *arquivoEntrada = fopen("ArquivoOrdenado.csv", "r");
    FILE *arquivoSaida = fopen("saida.csv", "w");
	char linha[TM_MAX_DATA]; 
	char dados[TM_MAX_DATA]; 

    // Ler e processar cada linha do arquivo de entrada
    while (fgets(linha, sizeof(linha), arquivoEntrada) != NULL) {
        // Extrair dados até antes da quarta vírgula
        extrairDados4Virgula(linha, dados);

        // Remover vírgula no final
        removerVirgulaFinal(dados);

        // Escrever os dados no arquivo de saída
        fprintf(arquivoSaida, "%s\n", dados);
    }

    // Fechar os arquivos
    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}

void quantidadeLinhas(void){
	FILE *inputArq;
    FILE *novoArq;
    char linha[TM_MAX_DATA]; // Variável temporária para ler cada linha
    int i = 0;


    // Abrir o arquivo de entrada
    inputArq = fopen("Google-Playstore.csv", "r");
    novoArq = fopen("ArquivoOrdenado.csv", "w");
    
    for (int j = 0; j < NUM_LINHAS; ++j) {
        if (fgets(linha, sizeof(linha), inputArq) == NULL) {
            break;
        }
        fwrite(linha, sizeof(char), strlen(linha), novoArq);
    }
    
    // Fechar o arquivo de entrada
    fclose(inputArq);
}

void adicionaChave(void){
	FILE *inputArq = fopen("saida2.csv", "r");
    FILE *novoArq = fopen("ArquivoFinal.dat", "wb");
    char linha[TM_MAX_DATA]; 
    char numero[8];			//Tamanho 8 por causa de ~2 milhoes de linhas
    int i = 0;

    // Abrir o arquivo de entrada
    

    for (int j = 0; j < NUM_LINHAS; ++j) {
        if (fgets(linha, sizeof(linha), inputArq) == NULL) {
            break;
        }
        
		if(j == 0){
			fputs("Numero,",novoArq);
		}
		else{
			sprintf(numero, "%d,", j);
			fputs(numero,novoArq);
		}
		
        // Escreve a linha diretamente no arquivo binário
        fwrite(linha, sizeof(char), strlen(linha), novoArq);
    }
    
    fclose(inputArq);
    fclose(novoArq);
}

int main() {
    quantidadeLinhas(); 	//Escreve em um arquivo a quantidade de linhas definida
    extrairDados();			//Tranfere os dados desejados para um novo arquivo (as primeiras 4 colunas)
    ordenaNomes();			//Ordena em ordem alfabetica de acordo com a coluna App Name
    adicionaChave();		//Adiciona uma coluna "chave" no arquivo
    	
    escreveLinhaNumero();	//Consulta uma linha em especifico e escreve ela.

    return 0;
}

