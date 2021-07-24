//Introdução à Inteligência Artificial ISEC LEI - 20/21
//Trabalho Prático nº 2 Realizado por:
//Gonçalo Ramalho 2019106561

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmos.h"

//#define FILE "teste.txt"
//#define FILE "n010.txt"
#define FILE "n012.txt"
//#define FILE "n030.txt"
//#define FILE "n060.txt"
//#define FILE "n120.txt"
//#define FILE "n240.txt"

#define ALGORITMO 1 //1 para pesquisa local, 2 para algoritmo evolutivo (com possibilidade de ser hibrido)

#define DEFAULT_RUNS 1 //Runs a considerar

#define RUNS_TREPA 3 //Vizinhos a gerar
#define PROB_ACEITAR_PIOR 0.05 // Probabilidade de no Trepa Colinas o algoritmo aceitar uma solução pior

#define TAM_POPULACAO 10 //Tamanho da populaçãoo inicial
#define NUM_GERACOES 6 //Número de gerações a considerar para cada run do algoritmo evolutivo
#define TIPO_MUT 1 //0 para descativar, 1 para mutação binária, 2 para mutação sôfrega

#define M_HIBRIDO 0 //0 para desactivar, 1 pesquisa local para definir a população inicial, 
// 2 pesquisa local para melhorar os indivíduos no final de cada geração, 3 pesquisa local para melhorar os indivíduos da população final



int main() {
	int i, j, k;
	int elementos, subconjuntos, solucao, melhor_solucao = 0;
	float mbf = 0.0;
	char fich[100];
	strcpy(fich, FILE);

	init_rand();

	leitura_inicial(fich, &elementos, &subconjuntos);


	if (ALGORITMO == 1) {
		int* matriz_inicial = malloc(sizeof(int));
		int* melhor_matriz = malloc(sizeof(int));

		if (matriz_inicial == NULL || melhor_matriz == NULL) {
			printf("Erro na alocação de mem�ria");
			exit(1);
		}
		printf("Algoritmo de Pesquisa Local\n");
		printf("Elementos: %d\tSubconjuntos: %d\n", elementos, subconjuntos);
		for (i = 0; i < DEFAULT_RUNS; i++) {
			printf("Iteracao numero: %d\n", i + 1);
			matriz_inicial = gera_matriz_inicial(elementos, subconjuntos);

			solucao = trepa_colinas(fich, matriz_inicial, elementos, subconjuntos, RUNS_TREPA, PROB_ACEITAR_PIOR);

			mbf = mbf + solucao;
			if (i == 0 || melhor_solucao < solucao) {
				melhor_solucao = solucao;
				melhor_matriz = copia(matriz_inicial, elementos);
			}
		}
		printf("\n####################\n");
		printf("\nMBF: %f\n", mbf / DEFAULT_RUNS);
		printf("\nMelhor solucao encontrada:");
		for (i = 0; i < elementos; i++)
			printf(" %d ", melhor_matriz[i]);
		printf("\n");
		printf("Qualidade: %2d\n", melhor_solucao);
		free(matriz_inicial);
		free(melhor_matriz);
	}

	if (ALGORITMO == 2) {
		pchrom parents = malloc(sizeof(chrom) * TAM_POPULACAO);
		pchrom children = malloc(sizeof(chrom) * TAM_POPULACAO);
		int* melhor_matriz = malloc(sizeof(int));
		if (parents == NULL || children == NULL) {
			printf("Erro na alocação de memória");
			exit(1);
		}
		if (M_HIBRIDO != 0)
			printf("Metodo Hibrido Combinado\n");
		else
			printf("Algoritmo Evolutivo\n");
		printf("Elementos: %d\tSubconjuntos: %d\n", elementos, subconjuntos);
		for (i = 0; i < DEFAULT_RUNS; i++) {
			printf("Iteracao numero: %d\n", i + 1);
			for (j = 0; j < TAM_POPULACAO; j++) {
				parents[j].mat = gera_matriz_inicial(elementos, subconjuntos);
				if (M_HIBRIDO == 1)
					trepa_colinas(fich, parents[j].mat, elementos, subconjuntos, RUNS_TREPA, PROB_ACEITAR_PIOR);

			}

			for (k = 0; k < NUM_GERACOES; k++) {
				printf("num ger %d\n", k + 1);
				tournament(parents, TAM_POPULACAO, fich, elementos, subconjuntos);

				children = crossover(parents, children, TAM_POPULACAO, fich, elementos, subconjuntos);

				mutation(children, TAM_POPULACAO, TIPO_MUT, fich, elementos, subconjuntos);

				for (j = 0; j < TAM_POPULACAO; j++) {
					if (M_HIBRIDO == 2)
						trepa_colinas(fich, children[j].mat, elementos, subconjuntos, RUNS_TREPA, PROB_ACEITAR_PIOR);
					if (melhor_solucao < children[j].qualidade) {
						melhor_solucao = children[j].qualidade;
						melhor_matriz = copia(children[j].mat, elementos);
					}
				}

				transfere(children, parents, TAM_POPULACAO); //childrens passam a ser parents

			}

			for (j = 0; j < TAM_POPULACAO; j++) {
				if (M_HIBRIDO == 3)
					trepa_colinas(fich, children[j].mat, elementos, subconjuntos, RUNS_TREPA, PROB_ACEITAR_PIOR);
				mbf = mbf + children[j].qualidade;
			}


		}
		printf("\n####################\n");
		printf("\nMBF: %f\n", mbf / (TAM_POPULACAO * DEFAULT_RUNS));
		printf("\nMelhor solucao encontrada:");
		for (i = 0; i < elementos; i++)
			printf(" %d ", melhor_matriz[i]);
		printf("\n");
		printf("Custo melhor: %d\n", calcula_solucao(fich, melhor_matriz, elementos, subconjuntos));
		free(parents);
		free(children);
		free(melhor_matriz);
	}
}