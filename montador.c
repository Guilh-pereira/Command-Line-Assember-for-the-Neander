#include <stdio.h>
#include <locale.h>
#include <ctype.h>
#include <string.h>

/*
Feito por: Guilherme Pereira dos Santos e Enzo de Paulo Souto
*/

/*
Instruções de Compilação e Uso:
no Linux (terminal):
compile o código usando o comando:
    gcc montador.c -o montador
e execute o programa passando o arquivo assembly de entrada usando o comando:
    ./montador <arquivo_de_entrada>.asm

no Windows (powershell):
compile o código usando o comando:
    gcc montador.c -o montador.exe
e execute o programa passando o arquivo assembly de entrada usando o comando:
    .\montador.exe <arquivo_de_entrada>.asm
*/


/*
função para converter a string para maiúsculo
*/
void toUppercase(char *str) { 
    for(int i = 0; str[i]; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

/*
função para obter o opcode de uma instrução, retorna -1 se a instrução for inválida ou não reconhecida pelo Neander
*/
int getOpcode(char *mnemonico) {
    if (strcmp(mnemonico, "NOP") == 0) return 0x00;
    if (strcmp(mnemonico, "STA") == 0) return 0x10;
    if (strcmp(mnemonico, "LDA") == 0) return 0x20;
    if (strcmp(mnemonico, "ADD") == 0) return 0x30;
    if (strcmp(mnemonico, "OR")  == 0) return 0x40;
    if (strcmp(mnemonico, "AND") == 0) return 0x50;
    if (strcmp(mnemonico, "NOT") == 0) return 0x60;
    if (strcmp(mnemonico, "JMP") == 0) return 0x80;
    if (strcmp(mnemonico, "JN")  == 0) return 0x90;
    if (strcmp(mnemonico, "JZ")  == 0) return 0xA0;
    if (strcmp(mnemonico, "HLT") == 0) return 0xF0;
    return -1; // Instrução inválida
}


/*
função para mudar a extensão de um arquivo, recebe o nome do arquivo original e um endereço para armazenar o novo nome do arquivo com a extensão .mem
*/
void mudarExtensao(const char *filename, char *new_filename) {
    strcpy(new_filename, filename);
    char *ponto = strrchr(new_filename, '.');
    if (ponto != NULL) {
        *ponto = '\0'; // Termina a string no ponto encontrado, removendo a extensão original
    }
    strcat(new_filename, ".mem"); // Adiciona .mem ao final do nome do arquivo
}

/*
função para escrever uma instrução no arquivo de saída. Recebe o arquivo de saída, o opcode da instrução, o endereço (se houver) e o número de operadores lidos na linha.
*/
void escreverInstrucao(FILE *arq, int opcode, int endereco, int operadores) {
    unsigned char op[2] = {(unsigned char)opcode, 0x00}; //guarda o opcode em um array de 2 bytes, onde o primeiro byte é o opcode e o segundo byte é 0x00 (não utilizado)
    fwrite(op, sizeof(unsigned char), 2, arq); // Escreve o opcode no arquivo de saída
    if (operadores == 2 && opcode != 0x00 && opcode != 0x60 && opcode != 0xF0) // Verifica se a instrução tem um operador e se não é NOP, NOT ou HLT, que não possuem operandos
    {
        unsigned char end[2] = {(unsigned char)endereco,0x00}; //guarda o endereço em um array de 2 bytes, onde o primeiro byte é o endereço e o segundo byte é 0x00 (não utilizado)
        fwrite(end, sizeof(unsigned char), 2, arq); // Escreve o endereço no arquivo de saída
    }
}

/*
função para montar o arquivo de saída a partir do arquivo de entrada. Recebe o nome do arquivo de entrada
*/
void montarArquivo(const char *file_in) {
    char file_saida[200]; //guarda o nome do arquivo de saída com a extensão .mem
    mudarExtensao(file_in, file_saida);

    FILE *arq_in = fopen(file_in, "r"); // Abre o arquivo de entrada em modo leitura
    FILE *arq_out = fopen(file_saida, "wb"); // Abre o arquivo de saída em modo escrita binária


    if (arq_in == NULL || arq_out == NULL) {
        printf("Erro ao abrir o arquivos\n");
        return;
    }

    unsigned char cabecalho[4] = {0x03, 0x4E, 0x44, 0x52}; // Cabeçalho obrigatório
    fwrite(cabecalho, sizeof(unsigned char), 4, arq_out); // Escreve o cabeçalho no arquivo de saída
    char linha[200]; //guarda cada linha do arquivo de entrada lida pelo fgets, onde cada linha pode conter até 200 caracteres

    // Lê o arquivo de entrada linha por linha
    while(fgets(linha, sizeof(linha), arq_in)) {
        char mnemonico[20] = ""; //guarda a instrução lida na linha, onde o mnemônico pode conter até 20 caracteres
        int endereco = -1; //guarda o endereço lido na linha, tem valor -1 para indicar que não foi lido um endereço
        int operadores = sscanf(linha, "%s %x", mnemonico, &endereco); //guarda a quantidade de operadores lidos na linha

        if(operadores <= 0) continue; // Linha vazia ou sem mnemonico

        toUppercase(mnemonico);
        int opcode = getOpcode(mnemonico); //guarda o opcode da instrução lida na linha

        if(opcode == -1) {
            printf("Instrução inválida ou não reconhecida pelo Neander: %s\n", mnemonico);
            continue; // Pula para a próxima linha
        }
        escreverInstrucao(arq_out, opcode, endereco, operadores);
    }

    printf("Arquivo '%s' gerado com sucesso!\n", file_saida);

    fclose(arq_in);
    fclose(arq_out);
} 

int main(int argc, char const *argv[])
{
    setlocale(LC_ALL, "Portuguese"); //Configura a localidade para Português
    if (argc < 2) {
        printf("Uso: %s <arquivo_de_entrada>\n", argv[0]);
        return 1; // Encerra o programa com código de erro
    }

    montarArquivo(argv[1]);
    return 0;
}