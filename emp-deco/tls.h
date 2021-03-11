#ifndef EMP_DECO_TLS_H_
#define EMP_DECO_TLS_H_
#include "emp-sh2pc/emp-sh2pc.h"
#include "hmac.h"
#include "utils.h"
using namespace std;
using namespace emp;

const string master_secret_label = "6d617374657220736563726574";
const string key_expansion_label = "6b657920657870616e73696f6e";
const string client_finished_label = "636c69656e742066696e6973686564";
const int master_secret_seed_len = 256 + 256 + 104;
const int key_expansion_seed_len = 256 + 256 + 104;
const int client_finished_seed_len = 256 + 120;

inline wide to_word(Integer a, int party)
{
  int n = a.size() / 32;
  if (a.size() % 32 != 0)
    n += 1;

  wide out(n);
  for (int i = 0; i < n; i++)
  {
    out[n - i - 1] = Integer(32, 0, party);
    cpy_emp_int(out[n - i - 1], a, 0, i * 32, 32);
  }
  return out;
}

inline wide master_secret_seed(string client_random_hex, string server_random_hex)
{
  auto seed_str = master_secret_label + client_random_hex + server_random_hex;
  return hex_to_emp_word(seed_str, PUBLIC);
}

inline wide key_expansion_seed(string client_random_hex, string server_random_hex)
{
  auto seed_str = key_expansion_label + server_random_hex + client_random_hex;
  return hex_to_emp_word(seed_str, PUBLIC);
}

inline wide client_finished_seed(string message_digest)
{
  auto seed_str = client_finished_label + message_digest;
  return hex_to_emp_word(seed_str, PUBLIC);
}

wide derive_master_secret(HMAC &hmac, string share, string client_random_hex, string server_random_hex);
wide derive_enc_keys_for_alice(HMAC &hmac, string share, string client_random_hex, string server_random_hex);
wide derive_enc_keys_for_alice(int party, HMAC &hmac, string master_secret_share, string client_random_hex, string server_random_hex);
wide derive_enc_keys_for_alice(HMAC &hmac, wide master_secret, string client_random_hex, string server_random_hex);
wide client_finished_message(int party, HMAC &hmac, string message_digest, string master_secret_str);

#endif
