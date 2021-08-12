# Optimization_Problem_AI

### Project developed for 'Introduction to Artificial Intelligence', a second year subject @ISEC

#### Guide:
Inside Main.c you need to setup which .txt to use by commenting/uncommenting the ```#define FILE```.

After that you have the following options:

- ```#define ALGORITMO``` -> '1' for hill-climbing algorithm || '2' for genetic algorithm (only this one can use hybrid method)

- ```#define DEFAULT_RUNS``` -> Number of runs to consider (used by both hill-climbing algorithm and genetic algorithm)

- ```#define RUNS_TREPA``` -> Number of neighbours to generate (hill-climbing algorithm)
- ```#define PROB_ACEITAR_PIOR``` -> Probability for the hill-climbing algorithm accepts a worse solution than the previous one (hill-climbing algorithm)

- ```#define TAM_POPULACAO``` -> Size of the initial population (genetic algorithm)
- ```#define NUM_GERACOES``` -> Number of generations to consider in each run (genetic algorithm)
- ```#define TIPO_MUT``` -> (Mutation type) '0' to deactivate || '1' binary mutation || '2' greedy mutation (genetic algorithm)

- ```#define M_HIBRIDO``` -> (Hibryd method) '0' to deactivate || '1' to use hill-climbing algorithm in the initial population || '2' to use hill-climbing algorithm in every individual at the end of each generation || '3' to use hill-climbing algorithm in the last generation

