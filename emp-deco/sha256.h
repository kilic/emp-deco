
#ifndef EMP_DECO_SHA256_H_
#define EMP_DECO_SHA256_H_
#include "constant.h"
#include "emp-sh2pc/emp-sh2pc.h"

template <class T>
T inverse(T a);

template <class T>
T rotate_left(T x, int k)
{
    return ((x >> k) | (x << (32 - k)));
}

class sha256
{

private:
    Integer _iv[8];
    Integer _k[64];
    template <typename T>
    T k(int index);
    template <class T>
    T iv(int index);

public:
    sha256();
    void prepare_2pc();
    bool prepared = false;

    template <class T>
    void block(vector<T> &hh, vector<T> in)
    {
        this->_block(hh, in);
    }

    template <class T>
    void block_init(vector<T> &hh, vector<T> in)
    {
        this->_block_init(hh, in);
    }

private:
    template <class T>
    void _block(vector<T> &hh, vector<T> in)
    {

        T w[64];
        for (int i = 0; i < 16; i++)
            w[i] = in[i];

        for (int i = 16; i < 64; i++)
        {
            T v1 = w[i - 2];
            T t1 = rotate_left(v1, 17) ^ rotate_left(v1, 19) ^ (v1 >> 10);
            T v2 = w[i - 15];
            T t2 = rotate_left(v2, 7) ^ rotate_left(v2, 18) ^ (v2 >> 3);
            w[i] = t1 + w[i - 7] + t2 + w[i - 16];
        }

        T a = hh[0], b = hh[1], c = hh[2], d = hh[3];
        T e = hh[4], f = hh[5], g = hh[6], h = hh[7];

        for (int i = 0; i < 64; i++)
        {
            T t1 = h + (rotate_left(e, 6) ^ rotate_left(e, 11) ^ rotate_left(e, 25)) + ((e & f) ^ (inverse(e) & g)) + this->k<T>(i) + w[i];
            T t2 = (rotate_left(a, 2) ^ rotate_left(a, 13) ^ rotate_left(a, 22)) + ((a & b) ^ (a & c) ^ (b & c));
            h = g;
            g = f;
            f = e;
            e = d + t1;
            d = c;
            c = b;
            b = a;
            a = t1 + t2;
        }

        hh[0] = hh[0] + a;
        hh[1] = hh[1] + b;
        hh[2] = hh[2] + c;
        hh[3] = hh[3] + d;
        hh[4] = hh[4] + e;
        hh[5] = hh[5] + f;
        hh[6] = hh[6] + g;
        hh[7] = hh[7] + h;
    };
    template <class T>
    void _block_init(vector<T> &hh, vector<T> in)
    {
        for (int i = 0; i < 8; i++)
            hh[i] = this->iv<T>(i);
        this->_block(hh, in);
    };
};
#endif
