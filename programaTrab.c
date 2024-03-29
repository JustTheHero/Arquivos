#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "funcoes_fornecidas.h"

typedef struct{
    char status;
    long int topo, proxByteOffset;
    int nroRegArq, nroRegRem;
}Cabecalho;

typedef struct{
    int id, idade, tamanhoRegistro, tamNomeJog, tamNacionalidade, tamNomeClube;
    long int prox;
    char removido, *nomeJogador, *nacionalidade, *nomeClube;
}Registro;

void abreArqRead(char *nomeArquivo, FILE **arquivo){
    *arquivo = fopen(nomeArquivo, "r");
    if(*arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }
}

void abreBinWrite(char *nomeArquivo, FILE **arquivo){
    *arquivo = fopen(nomeArquivo, "wb");
    if(*arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }
}

void abreBinRead(char *nomeArquivo, FILE **arquivo){
    *arquivo = fopen(nomeArquivo, "rb");
    if(*arquivo == NULL){
        printf("Falha no processamento do arquivo.\n");
        return 1;
    }
}


void binarioNaTela(char *nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if(nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *) malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for(i = 0; i < fl; i++) {
		cs += (unsigned long) mb[i];
	}
	printf("%lf\n", (cs / (double) 100));
	free(mb);
	fclose(fs);
}

void scan_quote_string(char *str) {

	/*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da seguinte forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

	char R;

	while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

	if(R == 'N' || R == 'n') { // campo NULO
		getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
		strcpy(str, ""); // copia string vazia
	} else if(R == '\"') {
		if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
			strcpy(str, "");
		}
		getchar(); // ignorar aspas fechando
	} else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
		str[0] = R;
		scanf("%s", &str[1]);
	} else { // EOF
		strcpy(str, "");
	}
}


//FUNCOES
char *lerString(FILE *arquivo, int tamanho){
    char *string = (char *)malloc(tamanho);
    fread(string, tamanho, 1, arquivo);
    return string;
}
void exibirString(char *string, int  tamanho){
    if (tamanho == -1) {
        printf("Falha no processamento do arquivo.\n");
    } else {
        printf("%s\n", string);
    }
}

void inicializarCabecalho(Cabecalho *cabecalho){
    cabecalho->status = '0';
    cabecalho->topo = -1;
    cabecalho->proxByteOffset = 0;
    cabecalho->nroRegArq = 0;
    cabecalho->nroRegRem = 0;
}

void escreverCabecalho(Cabecalho *cabecalho, FILE *arquivo){
    fwrite(&cabecalho->status, sizeof(char), 1, arquivo);
    fwrite(&cabecalho->topo, sizeof(long int), 1, arquivo);
    fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, arquivo);
    fwrite(&cabecalho->nroRegArq, sizeof(int), 1, arquivo);
    fwrite(&cabecalho->nroRegRem, sizeof(int), 1, arquivo);
}


void createTable(char *entrada, char *saida){

    FILE *arquivoEntrada;
    abreArqRead(entrada, &arquivoEntrada);
    
    FILE *arquivoSaida;
    abreBinWrite(saida, &arquivoSaida);

    Cabecalho cabecalho;
    inicializarCabecalho(&cabecalho);
    escreverCabecalho(&cabecalho, saida);

    //primeiro registro = descricao de cada campo (pular?)
    //campos sem dados/nulos = preencher com lixo? ($)
    //ate quando ler do arquivo base?
    //lidar com tamanhos strings 

    //atualizacao do status do cabecalho
    cabecalho.status = '1';
    fseek(arquivoSaida, 0, SEEK_SET);
    fwrite(&cabecalho.status, sizeof(char), 1, arquivoSaida);

    fclose(arquivoEntrada);
    fclose(arquivoSaida);
}

void select_from(char *arqbin){
        FILE *arquivo;
        abreBinRead(arqbin, &arquivo);
        
        fseek(arquivo, 16, SEEK_SET);
        int nroRegArq, nroRegRem;
        fread(&nroRegArq, sizeof(int), 1, arquivo);
        fread(&nroRegRem, sizeof(int), 1, arquivo);
        if(nroRegArq == 0){
            printf("Registro inexistente.\n");
            return 0;
        }   
        Registro registro;//fazer casos de erro, int -1 se for int, $ se for string, etc
        for (int i = 0; i < nroRegArq+nroRegRem; i++) {
            fread(&registro.removido, sizeof(char), 1, arquivo);
            if(registro.removido == '0'){
                fread(&registro.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&registro.prox, sizeof(long int), 1, arquivo);
                fread(&registro.id, sizeof(int), 1, arquivo);
                fread(&registro.idade, sizeof(int), 1, arquivo);
                fread(&registro.tamNomeJog, sizeof(int), 1, arquivo);       //MODULARIZAR FREADS FUNCAO - copiar plano b
                registro.nomeJogador = lerString(arquivo, registro.tamNomeJog);
                fread(&registro.tamNacionalidade, sizeof(int), 1, arquivo);
                registro.nacionalidade = lerString(arquivo, registro.tamNacionalidade);
                fread(&registro.tamNomeClube, sizeof(int), 1, arquivo);
                registro.nomeClube = lerString(arquivo, registro.tamNomeClube);
                if(registro.tamNomeJog == 0)
                    printf("Nome do Jogador: SEM DADO\n");
                else{
                    printf("Nome do Jogador: ");
                    exibirString(registro.nomeJogador, registro.tamNomeJog);
                }
                if(registro.tamNacionalidade == 0)
                    printf("Nacionalidade: SEM DADO\n");
                else{
                    printf("Nacionalidade: ");
                    exibirString(registro.nacionalidade, registro.tamNacionalidade);
                }
                if(registro.tamNomeClube == 0)
                    printf("Nome do Clube: SEM DADO\n");
                else{
                    printf("Nome do Clube: ");
                    exibirString(registro.nomeClube, registro.tamNomeClube);
                }
                printf("\n");
                free(registro.nomeJogador);
                free(registro.nacionalidade);
                free(registro.nomeClube);
            }
            else{
                fseek(arquivo, registro.tamanhoRegistro, SEEK_CUR);
            }
    }
    fclose(arquivo);
}


int verificarRegistro(FILE *arquivo, int n) {
    Registro registro;//codar caso de mais de uma chave de busca
    int todosCamposAtendidos = 1;
    for (int i = 0; i < n; i++) {
        int campoAtendido = 0;
        char nomeCampo[100];
        fscanf(arquivo, "%s", nomeCampo);
        if (strcmp(nomeCampo, "nomeJogador") == 0 || strcmp(nomeCampo, "nacionalidade") == 0 || strcmp(nomeCampo, "nomeClube") == 0) {
            char valorCampo[100];
            scan_quote_string(valorCampo);
            if (strcmp(nomeCampo, "nomeJogador") == 0) {
                if (registro.tamNomeJog != -1 && strcmp(registro.nomeJogador, valorCampo) == 0) {
                    campoAtendido = 1;
                }
            } else if (strcmp(nomeCampo, "nacionalidade") == 0) {
                if (registro.tamNacionalidade != -1 && strcmp(registro.nacionalidade, valorCampo) == 0) {
                    campoAtendido = 1;
                }
            } else if (strcmp(nomeCampo, "nomeClube") == 0) {
                if (registro.tamNomeClube != -1 && strcmp(registro.nomeClube, valorCampo) == 0) {
                    campoAtendido = 1;
                }
            }
        } else {
            char valorCampo[100];
            fscanf(arquivo, "%s", valorCampo);
            if (strcmp(nomeCampo, "id") == 0) {
                int valorCampoInt = atoi(valorCampo);
                if (registro.id == valorCampoInt) {
                    campoAtendido = 1;
                }
            } else if (strcmp(nomeCampo, "idade") == 0) {
                int valorCampoInt = atoi(valorCampo);
                if (registro.idade == valorCampoInt) {
                    campoAtendido = 1;
                }
            } 
        }
        if (!campoAtendido) {
            todosCamposAtendidos = 0;
            break;
        }
    }
    return todosCamposAtendidos;
}

void select_where(char *arqbin, int n){//codar interação com verificacao de registros

        FILE *arquivo;
        abreBinRead(arqbin, &arquivo);
        
        fseek(arquivo, 16, SEEK_SET);
        int nroRegArq, nroRegRem;
        fread(&nroRegArq, sizeof(int), 1, arquivo);
        fread(&nroRegRem, sizeof(int), 1, arquivo);
        if(nroRegArq == 0){
            printf("Registro inexistente.\n");
            return 0;
        }   
        Registro registro;
        for (int i = 0; i < nroRegArq+nroRegRem; i++) {
            fread(&registro.removido, sizeof(char), 1, arquivo);
            if(registro.removido == '0'){
                fread(&registro.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&registro.prox, sizeof(long int), 1, arquivo);
                fread(&registro.id, sizeof(int), 1, arquivo);
                fread(&registro.idade, sizeof(int), 1, arquivo);
                fread(&registro.tamNomeJog, sizeof(int), 1, arquivo);
                registro.nomeJogador = lerString(arquivo, registro.tamNomeJog);
                fread(&registro.tamNacionalidade, sizeof(int), 1, arquivo);
                registro.nacionalidade = lerString(arquivo, registro.tamNacionalidade);
                fread(&registro.tamNomeClube, sizeof(int), 1, arquivo);
                registro.nomeClube = lerString(arquivo, registro.tamNomeClube);
                if (registro.id == n) {
                    printf("ID: %d\n", registro.id);
                    printf("Idade: %d\n", registro.idade);
                    printf("Nome do Jogador: ");
                    exibirString(registro.nomeJogador, registro.tamNomeJog);
                    printf("Nacionalidade: ");
                    exibirString(registro.nacionalidade, registro.tamNacionalidade);
                    printf("Nome do Clube: ");
                    exibirString(registro.nomeClube, registro.tamNomeClube);
                    printf("\n");
                }
                free(registro.nomeJogador);
                free(registro.nacionalidade);
                free(registro.nomeClube);
            }
            else{
                fseek(arquivo, registro.tamanhoRegistro, SEEK_CUR);
            }
    fclose(arquivo);
}
}


int main(int argc, char* argv[]){

    //MENU DE ESCOLHA DE FUNCAO BASEADO EM TD MESMA LINHA
    //se entrada for parametro de execucao trocar p argv
    char entrada[50], arg1[30], arg2[30];
    int op=0, n=0;
    fgets(entrada, sizeof(entrada), stdin); //leitura entrada tudo mesma linha
    entrada[strcspn(entrada, "\n")] = 0; //remocao de /n
    
    //separacao entrada em op e nome arqs -> muito mais simples se ler primeiro operacao e dps decidir
    int aux = sscanf(entrada, "%d %s %s", &op, arg1, arg2);

    //verifica se foram lidos 2 ou 3 argumentos
    if(aux == 3){
        if(op == 1){
            createTable(arg1, arg2);
        }
        if(op == 3){
            n = atoi(arg2);
            select_where(arg1, n);
        }
    }
    else if(op == 2){
        select_from(arg1);
    }
        
    return 0;
}

