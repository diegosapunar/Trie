#include "trie.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/* Creo un puntero a nuevo TrieNode. Con todo inizializado en NULL */
struct TrieNode *initTrieNode()
{
    // Creo un TrieNode asignandole el espacio de memoria correspondiente
    TrieNode *trie_node = malloc(sizeof(TrieNode));
    
    trie_node -> isEndOfWord = false;
    trie_node -> freq_personal = 0;
    trie_node -> freq_best = 0;

    // Asigno los hijos a NULL
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        trie_node -> children[i] = NULL;
    }

    return trie_node;
}




void insert(struct TrieNode *root, const char *chars, int length, int freq)
{
    int index;

    TrieNode *actual_node = root;

    for (int level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(chars[level]);
        
        if (index == -65) {
            index = 26;
        }

        // Reviso si el hijo esta inicializado, para crearlo.
        if (!actual_node->children[index])
        {
            actual_node->children[index] = initTrieNode();          // lo inizializo

            actual_node->children[index] -> freq_personal = freq;   // le asigno la freq personal
            actual_node->children[index]->freq_best = freq;
        }
        // SI ya esta inicializado actualizamos las frecuencias
        else
        {
            if (actual_node -> children[index] -> freq_best < freq)
            {
                actual_node->children[index]->freq_best = freq;
                actual_node->children[index]->freq_personal = freq;
            }
        }
        // Vamos al siguiente nodo
        actual_node = actual_node -> children[index];
    }
    // En el que quede le asigno EndFfWord true
    actual_node->isEndOfWord = true;
}

// Decidir que hijo irme (el con mayor best). Retorna el index
int decide_son(TrieNode *children[27])
{
    int best = 0;
    int index;
    for(int i = 0; i < 27; i++)
    {
        if (children[i] && best < children[i] -> freq_best)
        {
            best = children[i] -> freq_best;
            index = i;
        }
    }
    return index;
}

int have_children(struct TrieNode *root)
{
    for(int i = 0; i < 27; i++)
    {
        if (root->children[i]) {return 1;}
    }
    return 0;
}

void search(struct TrieNode *root, const char *chars, int length, FILE *output)
{
    char output_string[101] = {0};
    bool validador = true;

    if (length == 0) {                                          // Entregar el mas frecuente
        most_freq(root, output, 0, output_string);
    }
    else {
        char letters[27] = "abcdefghijklmnopqrstuvwxyz ";
        int index;
        int contador = 0;
    
        TrieNode *actual_node = root;

        // Recorro el prefijo en mi Trie
        for (int level = 0; level < length; level++)
        {
            index = CHAR_TO_INDEX(chars[level]);
            if (index == -65){index = 26;}

            if (actual_node -> children[index])                 // Esta la letra
            {
                output_string[contador] = letters[index];       // Agrego el caracter a mi Output
                actual_node = actual_node -> children[index];   // Avanzo a ese Nodo
            }
            else                                                // No esta la letra, entonces nisiquiera esta el prefijo
            {
                output_string[contador] = chars[level];         // agrego al string_output
                validador = false;
            }
            contador++;
        }

        // Tengo que ver si sigo o no a partir del nodo que quede
        if (have_children(actual_node) && validador)
        {
            int index_p = decide_son(actual_node->children);
            if (index_p == -65)
            {
                index_p = 26;
            }

            if (actual_node -> children[index_p]-> freq_best < actual_node -> freq_best) {
                fprintf(output, "%s\n", output_string);
            }
            else {
                most_freq(actual_node, output, contador, output_string);
            }
            
        }
        else{
            fprintf(output, "%s\n", output_string);
        }


        // printf("output: %s\n", output_string);
    }
}

void most_freq(struct TrieNode *root, FILE *output, int contador, char output_string[101])
{

    TrieNode *actual_node = root;
    int index;
    char letters[27] = "abcdefghijklmnopqrstuvwxyz ";

    while(true)
    {
        if (have_children(actual_node)) // tiene hijo
        { 
            index = decide_son(actual_node -> children);
            if (index == -65){index = 26;}

            output_string[contador] = letters[index];
            if (actual_node->children[index]->isEndOfWord)
            {
                if (actual_node->children[index]->freq_best <= actual_node->children[index]->freq_personal)
                {

                    if (have_children(actual_node->children[index]))
                    {
                        int index_p = decide_son(actual_node->children[index]->children);
                        if (index_p == -65)
                        {
                            index_p = 26;
                        }

                        if (actual_node->children[index]->children[index_p]->freq_best >= actual_node->children[index]->freq_best)
                        {
                        }
                        else {break;}
                    }
                    else 
                    {
                        break;
                    }
                }
                
            }
            contador ++;
            actual_node = actual_node -> children[index];
        }
        else{
            break;
        }
    }
    fprintf(output, "%s\n", output_string);
}

// Source: https://stackoverflow.com/questions/34691151/how-to-free-recursive-struct-trie
void free_all(TrieNode *curs)
{
    int i;
    if (!curs)
        return; // safe guard including root node.

    // recursive case (go to end of trie)
    for (i = 0; i < 27; i++)
        free_all(curs->children[i]);

    // base case
    free(curs);
}
