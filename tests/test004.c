int fib(int n)
{
  if (n == 0 || n == 1)
    return n;
  else
    return (f(n-1) + f(n-2));
}

int Start() {
    return fib(33);
}
