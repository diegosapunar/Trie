#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "trie.h"
#include <time.h>

// Esta linea permite usar MAX_LENGTH como una constante en el programa
// Uso 101 para considerar el caracter que indica el fin de un string
#define MAX_LENGTH 101

int main(int argc, char *argv[])
{
	clock_t begin = clock();

	if (argc != 4)
	{
		printf("Modo de uso: ./solver [database.txt] [queries.txt] [output.txt]\n");
		return 0;
	}
	FILE *database = fopen(argv[1], "r");
	FILE *queries  = fopen(argv[2], "r");
	FILE *output   = fopen(argv[3], "w");

	if (!database || !queries || !output)
	{
		printf("¡Error abriendo los archivos!");
		return 2;
	}	
	// Creo la raiz de mi Trie
	struct TrieNode *root = initTrieNode();

	// Leo el numero de entradas en la base de datos
	int n;
	fscanf(database, "%d", &n);

	clock_t begin_building = clock();

	// Para cada entrada:
	for (int i = 0; i < n; i++)
	{
		// Obtengo la frecuencia y el largo
		int freq, length;
		// Ojo que incluyo un espacio en el formato para que no lo considere como parte del string
		fscanf(database, "%d %d ", &freq, &length);

		// Leo el string aprovechando que se el largo maximo
		char chars[MAX_LENGTH];
		fgets(chars, MAX_LENGTH, database);

		// Insertamos la entrada de la Database a mi Trie
		insert(root, chars, length, freq);
	}
	clock_t end_building = clock();

	

	// Leo el numero de entradas en queries
	int n_q;
	fscanf(queries, "%d", &n_q);

	clock_t begin_queries = clock();

	// Para cada entrada:
	for (int i = 0; i < n_q; i++)
	{
		int length;
		char chars[MAX_LENGTH];
		// Ojo que incluyo un espacio en el formato para que no lo considere como parte del string
		fscanf(queries, "%d ",  &length);

		if (length == 0)
		{ // Entregar el mas frecuente
			search(root, chars, length, output);
		}
		else{
		// Leo el string aprovechando que se el largo maximo
		fgets(chars, MAX_LENGTH, queries);

		search(root, chars, length, output);
		}
	}
	fclose(database);
	fclose(queries);
	fclose(output);
	
	clock_t end = clock();

	double time_spent_building = (double)(end_building - begin_building) / CLOCKS_PER_SEC;
	double time_spent_queries = (double)(end - begin_queries) / CLOCKS_PER_SEC;
	double time_spent_total = (double)(end - begin) / CLOCKS_PER_SEC;

	printf("N: %d - TIME SPENT in building the Trie: %lf seconds (s)\n",n, time_spent_building);
	printf("M: %d - TIME SPENT in answeting the queries: %lf seconds (s)\n",n_q, time_spent_queries);
	printf("TIME SPENT in total: %lf seconds (s)\n", time_spent_total);

	free_all(root);
	
	return 0;
}
