// $Id: hashfn.c,v 1.10 2013-08-12 13:30:08-07 - - $

//
// This program is not part of your project.  It exists just to 
// illustrate how to obtain and print hash values.  Each element
// of argv is hashed and printed along with its hashcode.
//

#include <stdio.h>
#include <stdlib.h>

#include "../code/strhash.h"

int main (int argc, char **argv) {
   for (int argi = 0; argi < argc; ++argi) {
      char *str = argv[argi];
      size_t hashcode = strhash (str);
      printf ("%20lu = strhash (\"%s\")\n", hashcode, str);
   }
   printf ("%20lu = 0xFFFFFFFFLU\n", 0xFFFFFFFFLU);
   printf ("%20lu = 0x%016lXLU\n", (size_t)-1L, (size_t)-1L);
   return EXIT_SUCCESS;
}

