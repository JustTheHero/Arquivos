#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char status;
    long int topo, proxByteOffset;
    int nroRegArq, nroRegRem;
}Cabecalho;

typedef struct{
    int id, idade, tamanhoRegistro, tamNomeJog, tamNacionalidade, tamNomeClube;
    long int prox;
    char removido, *nomeJogador, *nacionalidade, *nomeClube;
}Jogador;



//FUNCOES




void createTable(char *entrada, char *saida){

    //primeiro registro = descricao de cada campo (pular?)
    //campos sem dados/nulos = preencher com lixo? ($) 

    //...

    //adicionar um cabecalho inicial no arquivo em binario de saida
}

void select_from(char *arqbin){
    
    FILE *leitura = fopen(arqbin, "rb");
        if(leitura == NULL){
            printf("Falha no processamento do arquivo.\n");
            return 1;
        }

        //atualizar status cabecalho
        //se nroRegArq == 0 -> registro inexistente e sair

        //while !feof (?)

        //...

}

void select_where(char *arqbin, int n){

    //...

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