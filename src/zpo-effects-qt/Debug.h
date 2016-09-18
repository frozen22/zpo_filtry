/* 
 * Soubor: Debug.h
 * Autor:  Frantisek Nemec (xnemec61@stud.fit.vutbr.cz)
 * Datum:  2014-02-02
 */

#ifndef DEBUG_H
#define	DEBUG_H

#include <iostream>

// Debugovaci vypisy
#define DBG_MSG
#ifdef DBG_MSG
    #define DBG(message) std::cerr << message << std::endl;
    #define DBG_NO_ENDL(message) std::cerr << message;
#else
    #define DBG(message)
    define DBG_NO_ENDL(message)
#endif

// Macro pro potlaceni upozorneni prekladace (unused)
#ifndef UNREFERENCED_PARAMETER
    // MinGW UNREFERENCED_PARAMETER definuje, ale na jinych prekladacich 
    // macro nemusi.
    #define UNREFERENCED_PARAMETER(x) (x)=(x);
#endif
            
#endif	/* DEBUG_H */

