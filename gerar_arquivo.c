#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Função de erro com encerramento
void erro(const char *msg) {
    fprintf(stderr, "Erro: %s\n", msg);
    exit(EXIT_FAILURE);
}

// Converte uma data e hora para timestamp
time_t converter_para_timestamp(int dia, int mes, int ano, int hora, int min, int seg) {
    struct tm t;
    t.tm_year = ano - 1900;
    t.tm_mon = mes - 1;
    t.tm_mday = dia;
    t.tm_hour = hora;
    t.tm_min = min;
    t.tm_sec = seg;
    t.tm_isdst = -1;
    time_t ts = mktime(&t);
    if (ts == -1) erro("Data inválida.");
    return ts;
}

// Gera um timestamp aleatório no intervalo dado
time_t gerar_timestamp_aleatorio(time_t inicio, time_t fim) {
    if (fim <= inicio) erro("Intervalo de tempo inválido.");
    return inicio + rand() % (fim - inicio + 1);
}

int main(int argc, char *argv[]) {
    if (argc < 5) erro("Uso: ./gerar_arquivo <inicio> <fim> <saida.txt> <SENSORES...>");

    int ai, mi, di, hi, mini, si;
    int af, mf, df, hf, minf, sf;

    // Validação e extração das datas de início e fim
    if (sscanf(argv[1], "%d-%d-%d:%d:%d:%d", &ai, &mi, &di, &hi, &mini, &si) != 6 ||
        sscanf(argv[2], "%d-%d-%d:%d:%d:%d", &af, &mf, &df, &hf, &minf, &sf) != 6)
        erro("Formato de data inválido. Use AAAA-MM-DD:HH:MM:SS");

    time_t inicio = converter_para_timestamp(di, mi, ai, hi, mini, si);
    time_t fim = converter_para_timestamp(df, mf, af, hf, minf, sf);

    FILE *f = fopen(argv[3], "w");
    if (!f) erro("Não foi possível criar o arquivo de saída.");

    srand(time(NULL)); // inicializa o gerador aleatório

    // Gera 2000 leituras para cada sensor fornecido
    for (int s = 4; s < argc; s++) {
        char *sensor = argv[s];
        for (int i = 0; i < 2000; i++) {
            time_t ts = gerar_timestamp_aleatorio(inicio, fim);
            float valor = (rand() % 10000) / 100.0;
            fprintf(f, "%ld %s %.2f\n", ts, sensor, valor);
        }
    }

    fclose(f);
    printf("Arquivo %s gerado com sucesso!\n", argv[3]);
    return 0;
}
