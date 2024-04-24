#include "../inc/util.h"
#include <math.h>
/* num2str
 *   convert a number 'number' in base 'base' to the string s (the string
 *   variable must be large enough)
 */
void num2str(uint8_t *s, unsigned int number, unsigned int base)
{
	static char  hexChars[] = "0123456789ABCDEF";

	uint8_t *p=s;
	unsigned int cnt;
	unsigned int i;
	char tmp;

	// get digits
	do {
		*s++=hexChars[number % base];
	} while (number /= base);
	*s='\0';

	// reverse string
	cnt=s-p;
	for (i=0;i<cnt/2;i++) {
		tmp=p[i]; p[i] = p[cnt-i-1]; p[cnt-i-1]=tmp;
	}
}

unsigned int str2num(uint8_t *s, unsigned base)
{
	unsigned int u=0, d;
	uint8_t ch=*s++;
	while (ch) {
		if ((ch>='0') && (ch<='9')) d=ch-'0';
		else if ((base==16) && (ch>='A') && (ch<='F')) d=ch-'A'+10;
		else if ((base==16) && (ch>='a') && (ch<='f')) d=ch-'a'+10;
		else break;
		u=d+base*u;
		ch=*s++;
	}
	return u;
}
//=================================================================
void reverse(char *str, int len)
{
    int i=0, j=len-1, temp;
    while (i<j)
    {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++; j--;
    }
}

//=================================================================
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x)
    {
        str[i++] = (x%10) + '0';
        x = x/10;
    }

    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';

    reverse(str, i);
    str[i] = '\0';
    return i;
}
//=================================================================
void float2str( char *res, float n, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;

    // Extract floating part
    float fpart = n - (float)ipart;

    // convert integer part to string
    int i = intToStr(ipart, res, 0);

    // check for display option after point
    if (afterpoint != 0)
    {
        res[i] = '.';  // add dot

        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter is needed
        // to handle cases like 233.007
        fpart = fpart * (float)myPow(10.0, afterpoint);

        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
//=================================================================
double myPow(double x, int n) {
    unsigned int p = abs(n);
    double result = 1;
    while(p > 0)
    {
        if(p & 1) // if bit is set
        {
            result = result * x;
        }
        p = p >> 1;
        x = x * x;
    }

    if(n < 0)
    {
        return 1/result;
    }
    return result;
}
