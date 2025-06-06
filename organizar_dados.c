#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINHA 256
#define MAX_LEITURAS 10000 // número máximo de leituras permitidas

// Estrutura que representa uma leitura de sensor
typedef struct {
    long timestamp;              // instante da leitura (epoch)
    char id_sensor[16];          // identificador do sensor
    char valor[32];              // valor da leitura
} Leitura;

// Função para comparar dois timestamps para ordenação
int comparar_por_timestamp(const void *a, const void *b) {
    return ((Leitura *)a)->timestamp - ((Leitura *)b)->timestamp;
}

// Função auxiliar para exibir erro e encerrar o programa
void erro(const char *msg) {
    fprintf(stderr, "Erro: %s\n", msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 2) erro("Uso: ./organizar_dados <arquivo_entrada>");

    FILE *entrada = fopen(argv[1], "r");
    if (!entrada) erro("Não foi possível abrir o arquivo de entrada.");

    Leitura leituras[MAX_LEITURAS];
    int total = 0;

    // Leitura de todas as linhas do arquivo
    while (fscanf(entrada, "%ld %15s %31s", &leituras[total].timestamp, leituras[total].id_sensor, leituras[total].valor) == 3) {
        total++;
        if (total >= MAX_LEITURAS) erro("Número de leituras excede o limite.");
    }
    fclose(entrada);

    // Laço superficial para separar sensores (não implementado completamente)
    for (int i = 0; i < total; i++) {
        for (int j = i + 1; j < total; j++) {
            if (strcmp(leituras[i].id_sensor, leituras[j].id_sensor) == 0) {
                continue;
            }
        }
    }

    // Salva as leituras separadamente em arquivos por sensor
    for (int i = 0; i < total; i++) {
        char nome_arquivo[32];
        snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", leituras[i].id_sensor);

        FILE *out = fopen(nome_arquivo, "a");
        if (!out) {
            fprintf(stderr, "Erro ao abrir arquivo %s\n", nome_arquivo);
            continue;
        }
        fprintf(out, "%ld %s %s\n", leituras[i].timestamp, leituras[i].id_sensor, leituras[i].valor);
        fclose(out);
    }

    return 0;
}