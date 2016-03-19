// Patrick Russell pcrussel
// $Id: spellchk.c,v 1.8 2014-03-13 11:55:33-07 - - $

#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

#include "debugf.h"
#include "hashset.h"
#include "yyextern.h"

#define STDIN_NAME       "-"
#define DEFAULT_DICTNAME \
        "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words"
#define DEFAULT_DICT_POS 0
#define EXTRA_DICT_POS   1
#define NUMBER_DICTS     2

void print_error (const char *object, const char *message) {
   fflush (NULL);
   fprintf (stderr, "%s: %s: %s\n", Exec_Name, object, message);
   fflush (NULL);
   Exit_Status = EXIT_FAILURE;
}

FILE *open_infile (const char *filename) {
   FILE *file = fopen (filename, "r");
   if (file == NULL) print_error (filename, strerror (errno));
   DEBUGF ('m', "filename = \"%s\", file = 0x%p\n", filename, file);
   return file;
}

void spellcheck (const char *filename, hashset *hashset) {
   yylineno = 1;
   DEBUGF ('m', "filename = \"%s\", hashset = 0x%p\n",
                filename, hashset);
   
   for (;;) {
      int token = yylex ();
      if (token == 0) break;
      DEBUGF ('m', "line %d, yytext = \"%s\"\n", yylineno, yytext);
      //check to see if the word is in the hashset
      if(has_hashset(hashset, yytext)) continue;
      else{
      //if not, add to the error message to indicate a 
      //misspelled word, and print the word
      printf("%s: %d: misspelled word: %s\n", filename, yylineno, yytext);
      if (Exit_Status == 0) Exit_Status++;
      }
      //STUBPRINTF ("%s: %d: %s\n", filename, yylineno, yytext);
   }
}

void load_dictionary (const char *dictionary_name, hashset *hashset) {
   if (dictionary_name == NULL) return;
   DEBUGF ('m', "dictionary_name = \"%s\", hashset = %p\n",
           dictionary_name, hashset);
   char buff[1024];
   FILE *dictionary = open_infile(dictionary_name);
   assert(dictionary != NULL);
   int linenr = 1, j_index = 0;
   for(linenr = 1; ; linenr++){
      char *line_pos = fgets(buff, sizeof buff, dictionary);
      //check to see if words scanned are valid
      if (line_pos == NULL) break;
      line_pos = strchr (buff, '\n');
      //make sure the new line character is inserted correctly
      if (line_pos == NULL){
         fflush(NULL);
         fprintf (stderr, "%s: %s[%d]: broken line\n", Exec_Name,
                  dictionary_name, linenr);
         fflush(NULL);
         Exit_Status = 2;
      }else {
         *line_pos = '\0';
      }
      //line_pos is fed the buffer's string
      line_pos = strdup(buff);
      assert (line_pos != NULL);
      //puts the input into the hashset
      put_hashset(hashset, line_pos);
      //frees the line_position
      free(line_pos);
      j_index++;
   }
   fclose(dictionary);
   printf("# of words added: %d\n", j_index);
   //STUBPRINTF ("Open dictionary, load it, close it\n");
}


int main (int argc, char **argv) {
   Exec_Name = basename (argv[0]);
   char *default_dictionary = DEFAULT_DICTNAME;
   char *user_dictionary = NULL;
   hashset *hashset = new_hashset ();
   yy_flex_debug = false;
   int debug_dump = 0;

   // Scan the arguments and set flags.
   opterr = false;
   for (;;) {
      int option = getopt (argc, argv, "nxyd:@:");
      if (option == EOF) break;
      switch (option) {
         char optopt_string[16]; // used in default:
         case 'd': user_dictionary = optarg;
                   break;
         case 'n': default_dictionary = NULL;
                   break;
         case 'x': debug_dump++;
                   break;
         case 'y': yy_flex_debug = true;
                   break;
         case '@': set_debugflags (optarg);
                   if (strpbrk (optarg, "@y")) yy_flex_debug = true;
                   break;
         default : sprintf (optopt_string, "-%c", optopt);
                   print_error (optopt_string, "invalid option");
                   break;
      }
   }

   // Load the dictionaries into the hash table.
   load_dictionary (default_dictionary, hashset);
   load_dictionary (user_dictionary, hashset);

   if(debug_dump > 1){
     check_hashset(hashset, debug_dump);
     //yycleanup();
     free_hashset(hashset);
     return Exit_Status;
   }
   if(debug_dump == 1) check_hashset(hashset, debug_dump);
   // Read and do spell checking on each of the files.
   if (optind >= argc) {
      yyin = stdin;
      spellcheck (STDIN_NAME, hashset);
   }else {
      for (int fileix = optind; fileix < argc; ++fileix) {
         DEBUGF ('m', "argv[%d] = \"%s\"\n", fileix, argv[fileix]);
         char *filename = argv[fileix];
         if (strcmp (filename, STDIN_NAME) == 0) {
            yyin = stdin;
            spellcheck (STDIN_NAME, hashset);
         }else {
            yyin = open_infile (filename);
            if (yyin == NULL) continue;
            spellcheck (filename, hashset);
            fclose (yyin);
         }
      }
   }
   
   free_hashset(hashset);
   yylex_destroy ();
   return Exit_Status;
}

