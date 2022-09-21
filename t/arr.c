void fill(int *p, int n);

int main(void) {
  int arr[4];
  int *arr_of_ptr[10];
  int *p;
  int **pp;
  int x, y;

  arr[2] = 42;

  p = &x;
  *p = 17;

  pp = &p;
  p = &y;
  *pp = 121;

  return 0;
}

void fill(int *p, int n) {
  int i;
  for (i = 0; i < n; ++i) {
    p[i] = 0;
  }
}
