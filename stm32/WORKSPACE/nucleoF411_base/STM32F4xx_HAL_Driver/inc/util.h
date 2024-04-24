#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif 

/* num2str
 *   convert a number 'number' in base 'base' to the string s (the string
 *   variable must be large enough)
 */
void num2str(uint8_t *s, unsigned int number, unsigned int base);
unsigned int str2num(uint8_t *s, unsigned base);
double myPow(double x, int n);
void float2str( char *res, float n, int afterpoint);


#ifdef __cplusplus
}
#endif
#endif
