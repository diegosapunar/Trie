#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Source: https://www.geeksforgeeks.org/trie-insert-and-search/

// Alphabet size (# of symbols)
// Correspondiente a 26 letras del abecedario en minuscula, mas el caracter " " (espacio)
#define ALPHABET_SIZE (27)

#define CHAR_TO_INDEX(c) ((int)c - (int)'a')

/* Estructura de un nodo del Trie.  */
struct TrieNode
{
    // La letra del TrieNodo. El cual debe ser un caracter.
    char letter;
    // True si es End of Word.
    bool isEndOfWord;
    // Corresponde la frecuencia hasta ese nodo. Solo hace sentido en EndOfWord.
    int freq_personal;
    // Corresponde a la mejor frecuencia incluyendo todos los posibles hijos. Solo hace sentido en EndOfWord.
    int freq_best;
    // Puntero a un arreglo de largo 27, donde cada entrada es un TrieNode.
    struct TrieNode *children[ALPHABET_SIZE]; 
};

typedef struct TrieNode TrieNode;

/* Creo un puntero a nuevo TrieNode. Con todo inizializado en NULL */
struct TrieNode *initTrieNode();

void insert(struct TrieNode *root, const char *chars, int length, int freq);

// Decidir que hijo irme (el con mayor best). Retorna el index
int decide_son(TrieNode *children[27]);

void search(struct TrieNode *root, const char *chars, int length, FILE *output);

void most_freq(struct TrieNode *root, FILE *output, int contador, char output_string[101]);

void free_all(TrieNode *curs);