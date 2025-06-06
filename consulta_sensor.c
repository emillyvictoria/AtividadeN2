#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEITURAS 10000 // limite de leituras por sensor

// Estrutura de leitura com timestamp e valor
typedef struct {
    long timestamp;
    char valor[32];
} Leitura;

// Calcula a diferença absoluta entre dois tempos
long diferenca(long a, long b) {
    return labs(a - b);
}

// Função auxiliar para erro fatal
void erro(const char *msg) {
    fprintf(stderr, "Erro: %s\n", msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc != 3) erro("Uso: ./consulta_sensor <ID_SENSOR> <TIMESTAMP>");

    char nome_arquivo[64];
    snprintf(nome_arquivo, sizeof(nome_arquivo), "%s.txt", argv[1]);

    long alvo = atol(argv[2]);
    if (alvo <= 0) erro("Timestamp inválido.");

    FILE *f = fopen(nome_arquivo, "r");
    if (!f) erro("Arquivo do sensor não encontrado.");

    Leitura leituras[MAX_LEITURAS];
    int total = 0;
    long ts;
    char sensor[16], valor[32];

    // Lê todas as linhas do arquivo do sensor
    while (fscanf(f, "%ld %15s %31s", &ts, sensor, valor) == 3) {
        leituras[total].timestamp = ts;
        strncpy(leituras[total].valor, valor, 32);
        total++;
        if (total >= MAX_LEITURAS) erro("Arquivo grande demais para consulta simples.");
    }
    fclose(f);

    if (total == 0) erro("Nenhuma leitura disponível.");

    // Busca binária pelo timestamp mais próximo
    int ini = 0, fim = total - 1, melhor = -1;
    long menor_diff = 9999999999;

    while (ini <= fim) {
        int meio = (ini + fim) / 2;
        long diff = diferenca(leituras[meio].timestamp, alvo);
        if (diff < menor_diff) {
            melhor = meio;
            menor_diff = diff;
        }
        if (leituras[meio].timestamp < alvo)
            ini = meio + 1;
        else
            fim = meio - 1;
    }

    // Imprime o resultado mais próximo
    printf("Mais próximo: %ld %s %s\n", leituras[melhor].timestamp, argv[1], leituras[melhor].valor);
    return 0;
}
