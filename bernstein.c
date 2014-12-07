#include "bernstein.h"

#include <math.h>
#include <stdio.h>

int fact(int v)
{
  if (v < 2)
    return 1;
  return v * fact(v - 1);
}

Flottant bernstein_factor(int n, int i, Flottant u)
{
  // Degré valide ?
  if (n < 0)
    fprintf(stderr, "bernstein(n, i, u): Degré invalide: n = %d\r\n", n);
  // Terme valide ?
  if (i > n)
    fprintf(stderr, "bernstein(n, i, u): Terme invalide: i = %d (n = %d)\r\n", i, n);
  // Paramètre valide ?
  if (u < 0.f || u > 1.f)
    fprintf(stderr, "bernstein(n, i, u): Paramètre invalide: u = %f\r\n", u);

  return (Flottant)(fact(n) / (fact(i) * fact(n - i))) * pow((1.f - u), n - i) * pow(u, i);
}
