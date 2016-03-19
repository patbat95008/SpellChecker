// Patrick Russell pcrussel
// $Id: debugf.c,v 1.5 2014-03-05 19:24:07-08 - - $

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "debugf.h"

char *Exec_Name = NULL;
int Exit_Status = EXIT_SUCCESS;

static const char *debugflags = "";
static bool alldebugflags = false;

void __stubprintf (const char *filename, int line, const char *func,
                   const char *format, ...) {
   va_list args;
   fflush (NULL);
   fprintf (stdout, "%s: STUB (%s:%d) %s:\n",
            Exec_Name, filename, line, func);
   va_start (args, format);
   vfprintf (stdout, format, args);
   va_end (args);
   fflush (NULL);
}

void set_debugflags (const char *flags) {
   debugflags = flags;
   if (strchr (debugflags, '@') != NULL) alldebugflags = true;
   DEBUGF ('a', "Debugflags = \"%s\"\n", debugflags);
}

void __debugprintf (char flag, const char *file, int line,
                    const char *func, const char *format, ...) {
   va_list args;
   if (alldebugflags || strchr (debugflags, flag) != NULL) {
      fflush (NULL);
      fprintf (stderr, "%s: DEBUGF(%c) %s (%s:%d):\n",
               Exec_Name, flag, func, file, line);
      va_start (args, format);
      vfprintf (stderr, format, args);
      va_end (args);
      fflush (NULL);
   }
}

