#include "emp-sh2pc/emp-sh2pc.h"
#include "utils.h"
#include "tls.h"
#include <iostream>
using namespace std;
using namespace emp;

Integer master_secret_seed(string client_random_hex, string server_random_hex)
{
  Integer client_random_int = hex_to_integer(256, client_random_hex, ALICE);
  Integer server_random_int = hex_to_integer(256, server_random_hex, ALICE);
  Integer label_master_secret = hex_to_integer(104, "6d617374657220736563726574", PUBLIC); // "master secret"
  Integer seed(256 + 256 + 104, 0, PUBLIC);
  copy_int(seed, server_random_int, 0, 0, 256);
  copy_int(seed, client_random_int, 256, 0, 256);
  copy_int(seed, label_master_secret, 512, 0, 104);
  return seed;
}

Integer key_expansion_seed(string client_random_hex, string server_random_hex)
{
  Integer client_random_int = hex_to_integer(256, client_random_hex, ALICE);
  Integer server_random_int = hex_to_integer(256, server_random_hex, ALICE);
  Integer label_master_secret = hex_to_integer(104, "6b657920657870616e73696f6e", PUBLIC); // "key expansion"
  Integer seed(256 + 256 + 104, 0, PUBLIC);
  copy_int(seed, client_random_int, 0, 0, 256);
  copy_int(seed, server_random_int, 256, 0, 256);
  copy_int(seed, label_master_secret, 512, 0, 104);
  return seed;
}

vector<Integer> derive_enc_keys_for_alice(HMAC &hmac, string share, string client_random_hex, string server_random_hex)
{

  Integer seed_master_secret = master_secret_seed(client_random_hex, server_random_hex);
  Integer premaster_share_alice = hex_to_integer(256, share, ALICE);
  Integer premaster_share_bob = hex_to_integer(256, share, BOB);
  Integer premaster_secret(256, 0, XOR);
  premaster_secret = premaster_share_alice + premaster_share_bob;
  hmac.set_secret(premaster_secret);

  vector<Integer>
      key_material_master_secret = hmac.run(2, seed_master_secret);

  Integer master_secret(384, 0, XOR);
  copy_int(master_secret, key_material_master_secret[0], 128, 0, 256);
  copy_int(master_secret, key_material_master_secret[1], 0, 128, 128);

  Integer seed_expansion = key_expansion_seed(client_random_hex, server_random_hex);
  hmac.set_secret(master_secret);
  vector<Integer> key_material_expansion = hmac.run(3, seed_expansion);

  Integer client_enc(128, 0, ALICE);
  Integer server_enc(128, 0, ALICE);
  copy_int(client_enc, key_material_expansion[2], 0, 128, 128);
  copy_int(server_enc, key_material_expansion[2], 0, 0, 128);
  vector<Integer> enc_keys;
  enc_keys.push_back(client_enc);
  enc_keys.push_back(server_enc);

  return enc_keys;
}