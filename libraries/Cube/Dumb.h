#ifndef dumbfunctions_h
#define dumbfunctions_h

#include <Point.h>


// TODO: why do i have to define these? Shouldn't they come in from Math.h?
#ifndef min
int min(int a, int b) {
 if (a <= b)
   return a; 
 else
   return b;
}
#endif

#ifndef max
int max(int a, int b) {
 if (a > b)
   return a; 
 else
   return b;
}
#endif

#endif