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
    if (!entrada) erro("Não foi possível abrir o arquivo de entrada..");

    Leitura leituras[MAX_LEITURAS];
    int total = 0;
    int linha_atual = 1; // contador de linha para identificar erros

    char buffer[MAX_LINHA];
    while (fgets(buffer, sizeof(buffer), entrada)) {
        char id_sensor[16], valor[32];
        long timestamp;

        int campos = sscanf(buffer, "%ld %15s %31s", &timestamp, id_sensor, valor);
        if (campos != 3) {
            fprintf(stderr, "Linha %d ignorada: formato inválido -> %s", linha_atual, buffer);
        } else {
            leituras[total].timestamp = timestamp;
            strncpy(leituras[total].id_sensor, id_sensor, sizeof(leituras[total].id_sensor));
            strncpy(leituras[total].valor, valor, sizeof(leituras[total].valor));
            total++;
            if (total >= MAX_LEITURAS) erro("Número de leituras excede o limite.");
        }
        linha_atual++;
    }
    fclose(entrada);

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
