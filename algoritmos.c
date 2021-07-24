//Introdução à Inteligência Artificial ISEC LEI - 20/21
//Trabalho Prático nº 2 Realizado por:
//Gonçalo Ramalho 2019106561

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algoritmos.h"
#include "utils.h"

int trepa_colinas(char* fich, int* m, int elementos, int subconjuntos, int runs_t, float prob_aceitar_pior) {

    int i, j, k, x, custo = 0;
    int random, distancia_maxima, distancia_minima, distancia_actual;
    int custo_inicial, custo_final;
    int m_min = 0, m_max = 0;

    int* p = malloc(sizeof(int) * elementos);
    if (p == NULL) {
        printf("Erro na aloca��o de mem�ria");
        exit(1);
    }

    for (i = 0; i < runs_t; i++) {

        p = copia(m, elementos);
        custo_inicial = calcula_solucao(fich, m, elementos, subconjuntos); //calcula solução inicial

        // Randomiza subconjunto a usar
        random = random_l_h(0, subconjuntos - 1);

        do {
            x = random_l_h(0, (elementos - 1));
        } while (m[x] != random);

        distancia_minima = 1000;
        distancia_maxima = 0;
        for (j = 0; j < elementos; j++) { //vai procurar o minimo e o m�ximo
            if (j != x && m[x] != m[j]) { //vai sempre ver m�ximos diferentes do seu subconjunto
                if (x < j)
                    distancia_actual = retorna_distancia(fich, x, j);
                else
                    distancia_actual = retorna_distancia(fich, j, x);
                if (distancia_actual > distancia_maxima) {
                    distancia_maxima = distancia_actual;
                    m_max = j;
                }
            }
            if (j != x && m[x] == m[j]) { //vai sempre ver minimos iguais ao seu subconjunto
                if (x < j)
                    distancia_actual = retorna_distancia(fich, x, j);
                else
                    distancia_actual = retorna_distancia(fich, j, x);
                if (distancia_actual < distancia_minima) {
                    distancia_maxima = distancia_actual;
                    m_min = j;
                }
            }
        }

        //troca de valores do subconjunto e calculo da solução final
        m[m_min] = m[m_max];
        m[m_max] = m[x];
        custo_final = calcula_solucao(fich, m, elementos, subconjuntos);

        //Verificação de aceitação ou não da nova matriz gerada, comparando a sua nova solução com a solução antiga
        if (custo_final <= custo_inicial || prob_aceitar_pior > rand_01()) {
            for (k = 0; k < elementos; k++)
                m[k] = p[k];
        }

    }

    free(p);

    custo = calcula_solucao(fich, m, elementos, subconjuntos);

    return custo;
}


void tournament(pchrom parents, int tam_pop_parents, char* fich, int elementos, int subconjuntos) { //torneio binario (tamanho de torneio: 2)
    int i, j = 0;
    int matriz_dos_melhores[2] = { 0, 0 };
    float qualidade_total = 0.0, x1 = 0.0, x2 = 0.0;
    // Realiza popsize torneios
    for (i = 0; i < tam_pop_parents; i++) {
        parents[i].qualidade = calcula_solucao(fich, parents[i].mat, elementos, subconjuntos);
        parents[i].valido = 0;
        qualidade_total = qualidade_total + parents[i].qualidade;
    }

    // Calcula dos 2 melhores pais
    for (i = 0; i < tam_pop_parents; i++) {
        if (parents[i].qualidade / qualidade_total > x1) {
            x1 = parents[i].qualidade / qualidade_total;
            matriz_dos_melhores[0] = i;
        }
    }

    for (i = 0; i < tam_pop_parents; i++) {
        if (parents[i].qualidade / qualidade_total > x2 && parents[i].qualidade / qualidade_total < x1) {
            x2 = parents[i].qualidade / qualidade_total;
            matriz_dos_melhores[1] = i;
        }
    }

    parents[matriz_dos_melhores[0]].valido = 1;
    parents[matriz_dos_melhores[1]].valido = 1;

}

pchrom crossover(pchrom parents, pchrom children, int tam_pop, char* fich, int elementos, int subconjuntos)
{
    int i, j, k;
    int max, min, intermedio, custo, subcj;
    int random;
    int acrescenta_n1 = 0, acrescenta_n2 = 0;
    int n1 = 0, n2 = 0;

    for (k = 0; k < tam_pop; k++) {

        //criar cada filho
        chrom p;

        //inicializar matriz random do filho
        p.mat = gera_matriz_inicial(elementos, subconjuntos);
        p.qualidade = calcula_solucao(fich, p.mat, elementos, subconjuntos);
        p.valido = 0;

        subcj = subconjuntos - 1;

        do { //ciclo de procura de o melhor de cada pai - 2 pais dãos os melhores genes a 1 filho, 1 pai de cada vez 
            for (i = 0; i < tam_pop; i++) {
                if (parents[i].valido == 1) {
                    for (j = 0; j < elementos; j++) { //minimo campo onde aparece
                        if (parents[i].mat[j] == subcj) {
                            min = j;
                            break;
                        }
                    }
                    for (j = elementos; j > 0; j--) {//maximo campo onde aparece
                        if (parents[i].mat[j] == subcj) {
                            max = j;
                            break;
                        }
                    }
                    custo = retorna_distancia(fich, min, max);

                    for (j = min + 1; j < elementos; j++) {//come�a do minimo excluindo o maximo
                        if (parents[i].mat[j] == subcj && j != max) {
                            intermedio = j;
                            if (retorna_distancia(fich, min, intermedio) > custo) {
                                custo = retorna_distancia(fich, min, intermedio);
                                max = intermedio;
                            }
                        }
                    }
                    for (j = max - 1; j > 0; j--) {//come�a do maximo excluindo o minimo
                        if (parents[i].mat[j] == subcj && j != min) {
                            intermedio = j;
                            if (retorna_distancia(fich, intermedio, max) > custo) {
                                custo = retorna_distancia(fich, intermedio, max);
                                min = intermedio;
                            }
                        }
                    }

                    //fazer as verificações nas childrens para não haver soluções inválidas e fazer a atribuição do melhor do pai
                    if (p.mat[max] != subcj) {
                        n1 = p.mat[max];
                        p.mat[max] = subcj;
                        acrescenta_n1++;
                    }
                    if (p.mat[min] != subcj) {
                        n2 = p.mat[min];
                        p.mat[min] = subcj;
                        acrescenta_n2++;
                    }

                    if (acrescenta_n1 == 1 || acrescenta_n2 == 1) {
                        do {
                            random = random_l_h(0, (elementos - 1));
                            if (random != max && random != min && p.mat[random] == subcj && acrescenta_n1 == 1) {
                                //Randomiza para não ser sempre o n1 primeiro no caso de ser preciso n1 e n2
                                if (0.5 > rand_01()) {
                                    p.mat[random] = n1;
                                    acrescenta_n1--;
                                }
                            }
                            if (random != max && random != min && p.mat[random] == subcj && acrescenta_n2 == 1) {
                                //Randomiza para não ser sempre o n1 primeiro no caso de ser preciso n1 e n2
                                if (0.5 > rand_01()) {
                                    p.mat[random] = n2;
                                    acrescenta_n2--;
                                }
                            }
                        } while (acrescenta_n1 + acrescenta_n2 != 0);
                    }
                }
            }

            subcj--;

        } while (subcj != -1);
        p.qualidade = calcula_solucao(fich, p.mat, elementos, subconjuntos);
        children[k] = p; //Atribuição da estrutura
    }
    return children;
}

void mutation(pchrom children, int tam_pop, int tipo, char* fich, int elementos, int subconjuntos) {
    int i, j, x, y, random;
    int n1, n2;
    int casa, temp;
    int custo = 0, menor_custo = 1000;

    //bin�ria, completamente aleatória, independente se a solução é boa ou má ou se o subconjunto é o mesmo
    if (tipo == 1) {
        for (i = 0; i < tam_pop; i++) {
            x = random_l_h(0, (elementos - 1));
            n1 = children[i].mat[x];
            do {
                y = random_l_h(0, (elementos - 1));
            } while (y == x);
            n2 = children[i].mat[y];
            children[i].mat[x] = n2;
            children[i].mat[y] = n1;
            children[i].qualidade = calcula_solucao(fich, children[i].mat, elementos, subconjuntos);

        }
    }

    //mutação sófrega/greedy, vai escolher um subconjunto aleat�rio e retirar a pior distancia desse conjunto
    if (tipo == 2) {
        for (j = 0; j < tam_pop; j++) {
            // Randomiza subconjunto a usar
            random = random_l_h(0, subconjuntos - 1);
            //procura aleatoriamente o 1� elemento a usar
            x = random_l_h(0, (elementos - 1));
            for (i = 0; i < elementos; i++) {
                if (children[j].mat[i] != x) {
                    if (i < x)
                        custo = retorna_distancia(fich, i, x);
                    if (i > x)
                        custo = retorna_distancia(fich, x, i);
                    if (custo < menor_custo) {
                        menor_custo = custo;
                        casa = i;
                    }
                }
            }
            do {
                y = random_l_h(0, (elementos - 1));
            } while (y == x || y == i);
            temp = children[j].mat[y];
            children[j].mat[y] = children[j].mat[casa];
            children[j].mat[casa] = temp;
            children[j].qualidade = calcula_solucao(fich, children[j].mat, elementos, subconjuntos);
        }
    }
}