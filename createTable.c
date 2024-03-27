#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CAMPO 100 
typedef struct {
    int id, idade;
    char nomeJogador[MAX_CAMPO], nacionalidade[MAX_CAMPO], nomeClube[MAX_CAMPO];
} Jogador;

void binarioNaTela(char *nomeArquivo) {
    FILE *file = fopen(nomeArquivo, "rb");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo %s\n", nomeArquivo);
        return;
    }
    fseek(file, 0, SEEK_END);
    long tamanho = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(tamanho);
    if (buffer == NULL) {
        fclose(file);
        return;
    }
    fread(buffer, sizeof(char), tamanho, file);
    fwrite(buffer, sizeof(char), tamanho, stdout);
    free(buffer);
    fclose(file);
}
//criar caso de erro para n ter info suficiente

//prototipo programa principal com a chamada do create table
int main(int argc, char *argv[]) {
    FILE *entrada = fopen(argv[2], "r");
    if (entrada == NULL) {
        printf("Falha no processamento do arquivo.\n");//arquivo.csv
        return 1;
    }
    FILE *saida = fopen(argv[3], "wb");
    if (saida == NULL) {
        printf("Falha no processamento do arquivo.\n");//arquivo saida
        fclose(entrada);
        return 1;
    }
    Jogador jogador;
    int aux;
    while ((aux = fscanf(entrada, "%[^,],%d,%d,%s,%s,%s\n", jogador.id, &jogador.idade, &jogador.nomeJogador, &jogador.nacionalidade, &jogador.nomeClube))!= EOF ) {
        if(aux != 5){
            printf("Falha no processamento do arquivo.\n");
            fclose(entrada);
            fclose(saida);
        }
        fwrite(&jogador, sizeof(Jogador), 1, saida);
    }
    fclose(entrada);
    fclose(saida);
    binarioNaTela(argv[3]);
    return 0;
}
