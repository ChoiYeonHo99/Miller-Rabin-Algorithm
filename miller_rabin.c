/*
 * Copyright 2020-2022. Heekuck Oh, all rights reserved
 * 이 프로그램은 한양대학교 ERICA 소프트웨어학부 재학생을 위한 교육용으로
 * 제작되었다.
 */
#include "miller_rabin.h"

/*
 * mod_add() - computes a+b mod m
 * a와 b가 m보다 작다는 가정하에서 a+b >= m이면 결과에서 m을 빼줘야 하므로
 * 오버플로가 발생하지 않도록 a-(m-b)를 계산하고, 그렇지 않으면 그냥 a+b를
 * 계산하면 된다. a+b >= m을 검사하는 과정에서 오버플로가 발생할 수 있으므로 a
 * >= m-b를 검사한다.
 */
uint64_t mod_add(uint64_t a, uint64_t b, uint64_t m) {
  // a와 b가 m보다 작다는 가정으로 시작한다.
  if (a < m && b < m) {
    /* a+b >= m이면 결과에서 m을 빼줘야 하므로 오버플로가
    발생하지 않도록 a-(m-b)를 계산한다. */
    if (a >= m - b)
      return a - (m - b);
    // 그렇지 않으면 그냥 a+b를 계산한다.
    else
      return a + b;
  }
  // 가정이 틀리다면 느리더라도 나머지 연산을 실행한다.
  else
    return ((a % m) + (b % m)) % m;
}

/*
 * mod_sub() - computes a-b mod m
 * 만일 a < b이면 결과가 음수가 되므로 m을 더해서 양수로 만든다.
 */
uint64_t mod_sub(uint64_t a, uint64_t b, uint64_t m) {
  // a와 b가 m보다 작다는 가정으로 시작한다.
  if (a < m && b < m) {
    // 만일 a < b이면 결과가 음수가 되므로 m을 더해서 양수로 만든다.
    if (a < b)
      return a + (m - b);
    // 그렇지 않으면 그냥 a-b를 계산한다.
    else
      return a - b;
  }
  // 가정이 틀리다면 느리더라도 나머지 연산을 실행한다.
  else {
    // 만일 a < b이면 결과가 음수가 되므로 m을 더해서 양수로 만든다.
    if (a < b)
      return (a + (m - b)) % m;
    // 그렇지 않으면 그냥 a-b를 계산한다.
    else
      return (a - b) % m;
  }
}

/*
 * mod_mul() - computes a*b mod m
 * a*b에서 오버플로가 발생할 수 있기 때문에 덧셈을 사용하여 빠르게 계산할 수
 * 있는 "double addition" 알고리즘을 사용한다. 그 알고리즘은 다음과 같다. r = 0;
 *     while (b > 0) {
 *         if (b & 1)
 *             r = mod_add(r, a, m);
 *         b = b >> 1;
 *         a = mod_add(a, a, m);
 *     }
 */
uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t m) {
  /* a*b에서 오버플로가 발생할 수 있기 때문에 덧셈을 사용하여
  빠르게 계산할 수 있는 "double addition" 알고리즘을 사용한다. */
  uint64_t r = 0;
  while (b > 0) {
    if (b & 1)
      r = mod_add(r, a, m);
    b = b >> 1;
    a = mod_add(a, a, m);
  }
  return r;
}

/*
 * mod_pow() - computes a^b mod m
 * a^b에서 오버플로가 발생할 수 있기 때문에 곱셈을 사용하여 빠르게 계산할 수
 * 있는 "square multiplication" 알고리즘을 사용한다. 그 알고리즘은 다음과 같다.
 *     r = 1;
 *     while (b > 0) {
 *         if (b & 1)
 *             r = mod_mul(r, a, m);
 *         b = b >> 1;
 *         a = mod_mul(a, a, m);
 *     }
 */
uint64_t mod_pow(uint64_t a, uint64_t b, uint64_t m) {
  /* a^b에서 오버플로가 발생할 수 있기 때문에 곱셈을 사용하여
  빠르게 계산할 수 있는 "square multiplication"
  알고리즘을 사용한다. */
  uint64_t r = 1;
  while (b > 0) {
    if (b & 1)
      r = mod_mul(r, a, m);
    b = b >> 1;
    a = mod_mul(a, a, m);
  }
  return r;
}

/*
 * Miller-Rabin Primality Testing against small sets of bases
 *
 * if n < 2^64,
 * it is enough to test a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, and 37.
 *
 * if n < 3,317,044,064,679,887,385,961,981,
 * it is enough to test a = 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, and 41.
 */
const uint64_t a[BASELEN] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};

/*
 * miller_rabin() - Miller-Rabin Primality Test (deterministic version)
 *
 * n > 3, an odd integer to be tested for primality
 * It returns PRIME if n is prime, COMPOSITE otherwise.
 */
int miller_rabin(uint64_t n) {
  /* a[0]의 값이 2 이므로 n <= 3에서 n - 1 = 2로
  a[0] < n - 1가 어긋나기에 n이 2, 3에서는
  바로 소수로 한다. */
  if (n == 2 || n == 3)
    return PRIME;

  // n이 1이거나 2가 아닌 짝수이면 소수가 아니다.
  if (n == 1 || n % 2 == 0)
    return COMPOSITE;

  // n-1 = 2^k * q를 알아낸다.
  uint64_t k = 0, q = n - 1;
  while (q % 2 == 0) {
    k++;
    q = q >> 1;
  }

  // a[BASELEN]를 돌리는 i, 0~k-1까지를 반복하는 j를 선언한다.
  // n이 소수일 가능성을 나타내는 inconclusive를 선언한다.
  int i, j, inconclusive;
  for (i = 0; i < BASELEN; i++) {
    if (a[i] < n - 1) {
      // 매 계산마다 가능성이 없다에서 test를 시작한다.
      inconclusive = 0;
      // test(a^q mod n == 1)에 통과하면 가능성을 1로 수정한다.
      if (mod_pow(a[i], q, n) == 1) {
        inconclusive = 1;
        continue;
      }

      /* test((a^q)^(2^j) mod n == n-1)에 통과하면
      가능성을 1로 수정한다. 이때 페르마의 정리로 (2^j)을
      mod n-1로 계산하여 속도를 향상시켰다. */
      for (j = 0; j < k; j++) {
        if (mod_pow(mod_pow(a[i], q, n), mod_pow(2, j, n - 1), n) == n - 1) {
          inconclusive = 1;
          break;
        }
      }
    }
    // 어느 test에도 통과하지 못했으면 합성수이다.
    if (!inconclusive)
      return COMPOSITE;
  }
  // 모든 test에 통과하면 소수로 판단한다.
  return PRIME;
}