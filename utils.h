//Introdução à Inteligência Artificial ISEC LEI - 20/21
//Trabalho Prático nº 2 Realizado por:
//Gonçalo Ramalho 2019106561

#ifndef UTILS_H
#define UTILS_H

void init_rand();
int random_l_h(int min, int max);
float rand_01();
int* copia(int matriz_orginal[], int elementos);
void transfere(int* matriz_orginal, int* matriz_copia, int elementos);
void leitura_inicial(char* fich, int* elementos, int* subconjuntos);
int* gera_matriz_inicial(int elementos, int subconjuntos);
int retorna_distancia(char* fich, int a, int b);
int calcula_solucao(char* fich, int* m, int elementos, int subconjuntos);

#endif
