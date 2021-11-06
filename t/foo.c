int g;

/*
 * A function.
 */
int f(void) {
  // a variable
  int a, b, c;
  a = 1;
  b = 2;
  c = a + b;
  return c;
}

int sum_to_n(int n) {
  int sum, i;
  sum = 0;
  for (i = 1; i <= n; i++) {
    sum += i;
  }
  return sum;
}
