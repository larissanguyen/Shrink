typedef struct hashTable *HashTable;
typedef struct elt Elt;
HashTable createHashTable (unsigned long long size);
HashTable initHashTable(void);
void destroyHash(HashTable h);
unsigned long long hashFn (const char* word, HashTable h);
static void grow(HashTable h);
char* hashGet (HashTable h, const char *word);
void hashInsert (HashTable h, char *word);
void hashDelete (HashTable h, char *word);
int isVowel(char c);
int reducible(char a, char b);
char * stringRdcAdj (char* word, int fstLetLoc, int size);
char* stringRdcFar(char* word, int fstLetLoc, int sndLetLoc, int size);
int reduces(char **printA, char *word, int addHere, HashTable h, int strSize);
void printStrings(char **printA, int length);

