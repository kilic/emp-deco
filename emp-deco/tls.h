#ifndef EMP_DECO_TLS_H_
#define EMP_DECO_TLS_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include "hmac.h"
#include "utils.h"
using namespace std;
using namespace emp;

vector<Integer> derive_enc_keys_for_alice(HMAC &hmac, string share, string client_random_hex, string server_random_hex);

#endif
