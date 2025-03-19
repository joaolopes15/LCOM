#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
  int n = 5;
  int *p = &n;
  printf("n=%d\n", n); // %d - format to a decimal
  printf("*p=%d\n", *p);
  printf("p=%p\n", p); 
  int **pp = &p;
  printf("p=%p\n", p);
  printf("*pp=%p\n", *pp);
  int s[100] = { 0 }; // all zeroes by the compiler
  int *d = malloc(100 * sizeof(int)); // new int[100] in C++
  memset(d, 0, 100); // all zeroes in run time
  s[1] = 1;
  printf("s[0]=%d\n", s[0]);
  printf("*s=%d\n", *s);
  printf("s[1]=%d\n", s[1]);
  printf("*(s+1)=%d\n", *(s+1));
  printf("sizeof(s)=%u\n", sizeof(s));
  printf("sizeof(d)=%u\n", sizeof(d));
}