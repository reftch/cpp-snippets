#include <stdio.h>

unsigned long fib(unsigned long n)
{
  if (n < 2)
    return n;
  return fib(n - 1) + fib(n - 2);
}

int main(void)
{
  unsigned long n = 45;
  unsigned long result = fib(n);
  printf("Result fibonacci of %ld is %ld\n", n, result);
  return 0;
}
