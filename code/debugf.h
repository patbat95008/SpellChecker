// Patrick Russell pcrussel
// $Id: debugf.h,v 1.4 2014-03-05 19:24:07-08 - - $

#ifndef __DEBUGF_H__
#define __DEBUGF_H__

//
// DESCRIPTION
//    Debugging library containing miscellaneous useful things.
//

//
// Keep track of Exec_Name and Exit_Status.
//
extern char *Exec_Name;
extern int Exit_Status;

//
// Support for stub messages.
//
#define STUBPRINTF(...) \
        __stubprintf (__FILE__, __LINE__, __func__, __VA_ARGS__)
void __stubprintf (const char *file, int line, const char *func,
                   const char *format, ...);

//
// Debugging utility.
//

void set_debugflags (const char *flags);
   //
   // Sets a string of debug flags to be used by DEBUGF statements.
   // Uses the address of the string, and does not copy it, so it
   // must not be dangling.  If a particular debug flag has been set,
   // messages are printed.  The format is identical to printf format.
   // The flag "@" turns on all flags.
   //

#ifdef NDEBUG
#define DEBUGF(FLAG,...) // DEBUG (FLAG, __VA_ARGS__)
#else
#define DEBUGF(FLAG,...) \
        __debugprintf (FLAG, __FILE__, __LINE__, __func__, __VA_ARGS__)
void __debugprintf (char flag, const char *file, int line,
                    const char *func, const char *format, ...);
#endif

#endif

