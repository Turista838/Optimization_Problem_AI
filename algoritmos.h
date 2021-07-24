//Introdução à Inteligência Artificial ISEC LEI - 20/21
//Trabalho Prático nº 2 Realizado por:
//Gonçalo Ramalho 2019106561

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

// Individuo
typedef struct individual chrom, * pchrom;
struct individual {
    int* mat;
    int qualidade;
    int valido;
};

int trepa_colinas(char* fich, int* m, int elementos, int subconjuntos, int runs_t, float prob_aceitar_pior);
void tournament(pchrom parents, int tam_pop_parents, char* fich, int elementos, int subconjuntos);
pchrom crossover(pchrom parents, pchrom children, int tam_pop, char* fich, int elementos, int subconjuntos);
void mutation(pchrom children, int tam_pop, int tipo, char* fich, int elementos, int subconjuntos);
#endif