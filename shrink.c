#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "shrink.h"


//LINKED LIST STUFF
//struct for the linked list nodes within the hash table
struct elt
{
    struct elt *next;
    char* word; //CHECK THIS, DO I NEED ANOTHER * TO MAKE IT A POINTER
                //DO I NEED TO CREATE SPACE OR FREE ANYTHING FOR THE STRING?
};

//HASH TABLE PROPER STUFF
struct hashTable
{
	unsigned long long size; //size of pointer table
	unsigned long long n; //number of elements stored
	Elt **array; //pointer table to elements
};

#define INITIAL_SIZE (655536)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1.2)

HashTable
createHashTable (unsigned long long size) {
	//create (malloc/calloc) space for each pointer in the hashTable
	HashTable h;
	h = calloc(1, sizeof(struct hashTable));

	assert(h != 0);

	h->size = size;
	h->n = 0;
	h->array = calloc(h->size, sizeof(Elt *)); 

	assert (h->array != 0);

	return h;
}


//creates the initial hash table
//no parallel destroy function separate from destroyHash/specific to initHashTable
HashTable
initHashTable(void) { 
	return createHashTable(INITIAL_SIZE);
}


//destroys the Hash Table, freeing all the memory in it
void
destroyHash(HashTable h) {
	int i;
	Elt *e;
	Elt *next;

	for(i = 0; i < h->size; i++) { //goes through each index of the pointer array
        
		//goes through the linked list at any given index
        for(e = h->array[i]; e != 0; e = next) {
            next = e->next;

            free(e->word);
            free(e); //delete the temporary node
        }
    }

    free(h->array); //free the space for the pointer array
    free(h); //free the space for the entire hash table structure
}

//hash function that maps the word into a location in the hash table
unsigned long long hashFn (const char *word, HashTable a) { //SHOULD THIS BE A CONST?
    
    //do the sum of the chars (as int value) in the string
    unsigned long long sum = 0;

    for(int i = 0; word[i] != '\0'; i++) { //CHECK THIS
        sum += (word[i]);
    }

    //mod that by the hashtable size for the hash fn
	return ((sum + a->size) % a->size);
}

//function in case the hashtable gets too full

static void
grow(HashTable h)
{
    HashTable h2;     /* new hashtable we'll create */
    struct hashTable swap;   /* temporary structure for moving everything to new hashtable */
    int i;
    Elt *e;

    h2 = createHashTable(h->size * GROWTH_FACTOR);

    for(i = 0; i < h->size; i++) {
        for(e = h->array[i]; e != 0; e = e->next) {
            /* note: this recopies everything */
            hashInsert(h2, e->word); 
        }
    }

    /* the messy part */
    /* We'll swap the meat of h and h2 */
    /* then call hashDestroy on h2 */
    swap = *h; //WHY THE HASHTABLE RATHER THAN THE POINTER TO A TABLE? DOESN'T ALIGN WITH OTHER HASHTABLE USES
    *h = *h2;
    *h2 = swap;

    destroyHash(h2);
}


//hash retrieval function
//aka find a word at a given coordinate
char* 
hashGet (HashTable h, const char *word) {
	Elt *e;

	//for loop (e = h->array[hash key derived from hashfn]; e!=0; e = e->next)
	for (e = h->array[hashFn(word, h)]; e != 0; e = e->next) {
		//if(coordinates match)
		//return the pointer to the element
		if(!strcmp(e->word, word)) {
			return e->word;
		}
	}
	//otherwise continue through the loop
	//once the loop is done (and hasn't found anything) return 0
	return 0;
}

//insert a node into the hash table
void
hashInsert (HashTable h, char *word) {
	Elt *e;
    unsigned long key;

    e = calloc(1, sizeof(*e));
	e->word = calloc(strlen(word) +4, sizeof(char));
    strcpy(e->word, word);

    key = hashFn(word, h);

    e->next = h->array[key];
    h->array[key] = e;

    h->n++;

    /* grow table if there is not enough room */
    if(h->n >= h->size * MAX_LOAD_FACTOR) {
        grow(h);
    }
}	

//delete a node of the hash table
void
hashDelete (HashTable h, char *word) 
{
	Elt **prev;          /* what to change when elt is deleted */
    Elt *e;              /* what to delete */

    for(prev = &(h->array[(hashFn(word, h))]); 
        *prev != 0; 
        prev = &((*prev)->next)) {
        if(!strcmp((*prev)->word, word)) {
            /* got it */

            //change the pointer of the previous node to point 
			//to whatever the deleted node points to
            e = *prev;
            *prev = e->next;

            //PERHAPS YOU NEED TO FREE THE PARTS OF THE NODE?
            free(e->word);
            free(e); //free the space associated with this node

            return;
        }
    }
}



//SHRINKY PART


//checks vowels
int isVowel (char c) {
	char* vowels = "aeiou"; //IS THIS RIGHT? 

	for (int i = 0; i < 5; i++) { 
		if (vowels[i] == c) {
			return 1;
		}
	}

	return 0;
}

//compares the two chars to see if they fit the rules
int reducible (char a, char b){
	if (abs(b-a) <= 5) { //checks if they are w/in 5 ascii
		return 1;
	} else if (isVowel(a) && isVowel(b)){ //checks if both are vowels
		return 1;
	} 

	//fits neither rule
	return 0;
}

