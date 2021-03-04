#include "sha256.h"
#include <iostream>
using namespace std;

uint32_t rotate_left(uint32_t x, int k)
{
  int s = uint32_t(k) & 31;
  return x << s | x >> (32 - s);
}

void sha256_block(uint32_t r[8], uint32_t hh[8], uint8_t p[64])
{

  r[0] = hh[0];
  r[1] = hh[1];
  r[2] = hh[2];
  r[3] = hh[3];
  r[4] = hh[4];
  r[5] = hh[5];
  r[6] = hh[6];
  r[7] = hh[7];

  uint32_t w[64];

  for (int i = 0; i < 16; i++)
  {
    int j = i * 4;
    w[i] = uint32_t(p[j]) << 24 | uint32_t(p[j + 1]) << 16 | uint32_t(p[j + 2]) << 8 | uint32_t(p[j + 3]);
  }

  for (int i = 16; i < 64; i++)
  {
    uint32_t v1 = w[i - 2];
    uint32_t t1 = rotate_left(v1, -17) ^ rotate_left(v1, -19) ^ (v1 >> 10);
    uint32_t v2 = w[i - 15];
    uint32_t t2 = rotate_left(v2, -7) ^ rotate_left(v2, -18) ^ (v2 >> 3);
    w[i] = t1 + w[i - 7] + t2 + w[i - 16];
  }

  uint32_t a = r[0];
  uint32_t b = r[1];
  uint32_t c = r[2];
  uint32_t d = r[3];
  uint32_t e = r[4];
  uint32_t f = r[5];
  uint32_t g = r[6];
  uint32_t h = r[7];

  for (int i = 0; i < 64; i++)
  {
    uint32_t t1 = h + (rotate_left(e, -6) ^ rotate_left(e, -11) ^ rotate_left(e, -25)) + ((e & f) ^ (~e & g)) + K[i] + w[i];
    uint32_t t2 = (rotate_left(a, -2) ^ rotate_left(a, -13) ^ rotate_left(a, -22)) + ((a & b) ^ (a & c) ^ (b & c));
    h = g;
    g = f;
    f = e;
    e = d + t1;
    d = c;
    c = b;
    b = a;
    a = t1 + t2;
  }

  r[0] += a;
  r[1] += b;
  r[2] += c;
  r[3] += d;
  r[4] += e;
  r[5] += f;
  r[6] += g;
  r[7] += h;
}