// Patrick Russell pcrussel
// $Id: hashset.c,v 1.9 2014-03-13 11:55:33-07 - - $

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "debugf.h"
#include "hashset.h"
#include "strhash.h"

#define HASH_NEW_SIZE 15

typedef struct hashnode hashnode;
struct hashnode {
   char *word;
   hashnode *link;
};

struct hashset {
   size_t size;
   size_t load;
   hashnode **chains;
};

void check_hashset(hashset *this, int debug){
   unsigned int j_index = 0;
   size_t index = 0;
   int cluster[this->size/4];
   //for(index = 0; index < this->size/4; index++) {
      //cluster[index] = NULL;
   //}
   int cluster_count = 0;
   for(index = 0; index < this->size; index++){
      if((this->chains[index]->word) != NULL && 
         (this->chains[index]->word == NULL)){
         ++cluster[cluster_count];
         cluster_count = 0;
      }
      if((this->chains[index]->word == NULL) && 
         (this->chains[0]->word == NULL)){
         int first_clu_count = 0;
         size_t k_index = 0;
         for(k_index = 0; k_index < this->size; k_index++){
            if(this->chains[k_index]->word != NULL){
              ++first_clu_count;
            }else{
              --cluster[first_clu_count];
              ++cluster[first_clu_count + cluster_count];
            }
         }
      }else if(this->chains[index]->word != NULL){
         j_index++;
         ++cluster_count;
      }else{
         ++cluster[cluster_count];
         cluster_count = 0;
      }
   }

   printf("%10d words in hash\n", j_index);
   printf("%10zu length of the array\n", this->size);
   int sum = 0;
   for(index = 1; index < this->size/4; index++){
      if(cluster[index] != 0){
         //printf("%10d clusters of size %u\n", cluster[index], index);
         sum += (cluster[index] * index);
      }
   }

   if(debug > 1){
     for(index = 0; index < this->size; index++){
        if(this->chains[index] != NULL){
           char *word = this->chains[index]->word;
           printf("chains[%zd]->word = \"%s\"\n", index, word);
        }
     }
  }
}

//doubles the array and reorders the hashtable according 
//to the new size
void doublearray(hashset *this){
   size_t index = 0;
   size_t oldsize = this->size;
   //double the hashset->size
   this->size = (oldsize * 2) + 1;
   //make a new array, and initialize it to null
   hashnode **new_chains = malloc (this->size * sizeof(hashnode));
   for (index = 0; index < this->size; index++){ 
      new_chains[index] = NULL;
   }
   //set the new array to the values of the old,
   //divided by the new size
   for (index = 0; index < oldsize ; index++){
      if(this->chains[index] == NULL) continue; 
      hashnode *tmp = this->chains[index];
      if(tmp == NULL){
         break;
      }
      size_t new_index = strhash(tmp->word) % this->size;
      while (new_chains[new_index] != NULL){
         hashnode *new_tmp = new_chains[new_index];
         if (strcmp(new_tmp->word, 
                    tmp->word) == 0){
            break;
         }
         new_index = (new_index + 1) % this->size;
      }
      new_chains[new_index] = this->chains[index];
   }
   //make the swap, free the old array
   hashnode **j_tmp = this->chains;
   this->chains = new_chains;
   free(j_tmp);
}

hashset *new_hashset (void) {
   hashset *this = malloc (sizeof (struct hashset));
   assert (this != NULL);
   this->size = HASH_NEW_SIZE;
   this->load = 0;
   size_t sizeof_chains = this->size * sizeof (hashnode *);
   this->chains = malloc (sizeof_chains);
   assert (this->chains != NULL);
   memset (this->chains, 0, sizeof_chains);
   DEBUGF ('h', "%p -> struct hashset {size = %d, chains=%p}\n",
                this, this->size, this->chains);
   return this;
}

//frees the hashset
void free_hashset (hashset *this) {
   DEBUGF ('h', "free (%p)\n", this);
   int index = 0;
   //free all the strings in the array
   for (index = 0; index < (int)this->size; index++){
      free(this->chains[index]);
   }
   //free the array
   memset(this->chains, 0, this->size * sizeof(char*));
   free(this->chains);
   //free the hashset
   memset(this, 0, sizeof(struct hashset));
   free(this);
}

//places words into the hashset
void put_hashset (hashset *this, const char *item) {
   //if the array's load is too big, double it
   if((this->load * 4) > this->size){
      doublearray(this);
   }
   //find where the input word is in the hash table
   size_t index = (strhash(item) % this->size);
   //move down the table until an empty spot is found
   while (this->chains[index] != NULL){
      hashnode *compare = this->chains[index];
      if (strcmp (compare->word, item) == 0){
         break;
      }
      index = (index + 1) % this->size;
   }
   //set the empty spot to the input, increase the size of the 
   //hashset
   hashnode *tmp = malloc(sizeof(hashnode));
   tmp->word = strdup(item);
   this->chains[index] = tmp;
   this->load++;
}

//checks to see if the word is in the hash
bool has_hashset (hashset *this, const char *item) {
   int index = (strhash(item) % this->size);   
   while (this->chains[index] != NULL){
      if(strcmp(this->chains[index]->word, item) == 0) return true;
      index = (index + 1) % this->size;
   }
   //converts to to all lower case, then checks again
   char *lower = malloc(1+strlen(item));
   assert(lower != NULL);
   strcpy(lower, item);
   for(index = 0; item[index] != 0; index++){
      lower[index] = (tolower(item[index]));
   }

   index = (strhash(lower) % this->size);
   while(this->chains[index] != NULL){
      if(strcmp(this->chains[index]->word, lower) == 0){
         return true;
      }
      index = (index+1) % this->size;
   }
   return false;
}

