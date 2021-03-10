#include "tls.h"

const string master_secret_label = "6d617374657220736563726574";
const string key_expansion_label = "6b657920657870616e73696f6e";
const int master_secret_seed_len = 616;
const int key_expansion_seed_len = 616;

wide to_word(Integer a, int party)
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

wide master_secret_seed(string client_random_hex, string server_random_hex)
{
  auto seed_str = master_secret_label + client_random_hex + server_random_hex;
  return hex_to_emp_word(seed_str, PUBLIC);
}

wide key_expansion_seed(string client_random_hex, string server_random_hex)
{
  auto seed_str = key_expansion_label + server_random_hex + client_random_hex;
  return hex_to_emp_word(seed_str, PUBLIC);
}

wide derive_enc_keys_for_alice(HMAC &hmac, string share, string client_random_hex, string server_random_hex)
{

  auto seed_master_secret = master_secret_seed(client_random_hex, server_random_hex);

  auto premaster_share_alice = hex_to_emp_int(256, share, ALICE);
  auto premaster_share_bob = hex_to_emp_int(256, share, BOB);
  auto premaster_secret = to_word(premaster_share_alice + premaster_share_bob, XOR);

  hmac.set_secret(premaster_secret, 8);

  auto key_material_master_secret = hmac.run(2, seed_master_secret, master_secret_seed_len);

  wide master_secret(12);
  copy(key_material_master_secret[0].begin(), key_material_master_secret[0].end(), master_secret.begin());
  copy(key_material_master_secret[1].begin(), key_material_master_secret[1].begin() + 4, master_secret.begin() + 8);

  hmac.set_secret(master_secret, 12);
  auto seed_key_expansion = key_expansion_seed(client_random_hex, server_random_hex);
  auto key_material_key_expansion = hmac.run(3, seed_key_expansion, key_expansion_seed_len);

  wide enc_keys(8);
  copy(key_material_key_expansion[2].begin(), key_material_key_expansion[2].end(), enc_keys.begin());

  return enc_keys;
}