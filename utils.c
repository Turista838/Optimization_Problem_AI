//Introdução à Inteligência Artificial ISEC LEI - 20/21
//Trabalho Prático nº 2 Realizado por:
//Gonçalo Ramalho 2019106561

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

void init_rand()
{
	srand((unsigned)time(NULL));
}

int random_l_h(int min, int max) // Devolve valor inteiro aleatorio entre min e max
{
	return min + rand() % (max - min + 1);
}

float rand_01() // Devolve um valor real aleatorio do intervalo [0, 1]
{
	return ((float)rand()) / RAND_MAX;
}

int* copia(int matriz_orginal[], int elementos) //Faz uma copia do vector introduzido
{
	int* p = malloc(sizeof(int) * elementos);
	if (p == NULL) {
		printf("Erro na alocação de memória");
		exit(1);
	}
	int i;
	for (i = 0; i < elementos; i++)
		p[i] = matriz_orginal[i];
	return p;
}

void transfere(int* child, int* prts, int elementos) //Transforma a popula��o de filhos em pais
{
	int i;
	for (i = 0; i < elementos; i++)
		prts[i] = child[i];

}

void leitura_inicial(char* fich, int* elementos, int* subconjuntos)
{
	int* p;
	FILE* f;
	int valor, cont = 0;
	char enter;

	f = fopen(fich, "r");
	if (!f)
	{
		printf("Erro no acesso ao ficheiro dos dados\n");
		exit(1);
	}
	// Numero de elementos
	fscanf(f, " %d", elementos);
	// Numero de subconjuntos
	fscanf(f, " %d", subconjuntos);


	// Leitura do tamanho da matriz de distancias
	while (fscanf(f, "%c", &enter) == 1)// Ignorar o resto da primeira linha
		if (enter == '\n')
			break;
	int a, b;
	while (fscanf(f, " %d %d %d", &a, &b, &valor) == 3)
	{
		cont++;
	}

	fclose(f);
}

int* gera_matriz_inicial(int elementos, int subconjuntos)
{
	int v[20] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	int* p;
	p = malloc(sizeof(int) * elementos);

	if (p == NULL) {
		printf("Erro na alocacao de memoria");
		exit(1);
	}
	int i, n, a;

	n = elementos / subconjuntos;

	for (i = 0; i < elementos; i++) {
		a = random_l_h(0, subconjuntos - 1);
		if (v[a] != n) {
			p[i] = a;
			v[a] = v[a] + 1;
		}
		else
			i--;
	}
	return p;
}

int calcula_solucao(char* fich, int* m, int elementos, int subconjuntos)
{
	int i, j, soma = 0;

	int min = 0, max = 0, intermedio;

	for (j = 0; j < subconjuntos; j++) {
		for (i = 0; i < elementos; i++) { //minimo campo onde aparece
			if (m[i] == j) {
				min = i;
				break;
			}
		}
		for (i = elementos; i > 0; i--) {//maximo campo onde aparece
			if (m[i] == j) {
				max = i;
				break;
			}
		}
		soma = soma + retorna_distancia(fich, min, max);

		for (i = min + 1; i < elementos; i++) {//começa do minimo excluindo o maximo
			if (m[i] == j && i != max) {
				intermedio = i;
				soma = soma + retorna_distancia(fich, min, intermedio);
			}
		}
		for (i = max - 1; i > 0; i--) {//começa do maximo excluindo o minimo
			if (m[i] == j && i != min) {
				intermedio = i;
				soma = soma + retorna_distancia(fich, intermedio, max);
			}
		}
	}
	return soma;
}

int retorna_distancia(char* fich, int a, int b)
{
	FILE* f;
	int i, j, valor;
	char enter;

	f = fopen(fich, "r");
	if (!f)
	{
		printf("Erro no acesso ao ficheiro dos dados\n");
		exit(1);
	}
	while (fscanf(f, "%c", &enter) == 1)// Ignorar a primeira linha
		if (enter == '\n')
			break;

	while (fscanf(f, " %d %d %d", &i, &j, &valor) == 3)
	{
		if (i == a && j == b)
			break;
	}

	fclose(f);
	return valor;
}