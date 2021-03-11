#include "tls.h"

wide derive_master_secret(HMAC &hmac, string share, string client_random_hex, string server_random_hex)
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

  return master_secret;
}

wide derive_enc_keys_for_alice(HMAC &hmac, string share, string client_random_hex, string server_random_hex)
{
  wide master_secret = derive_master_secret(hmac, share, client_random_hex, server_random_hex);
  return derive_enc_keys_for_alice(hmac, master_secret, client_random_hex, server_random_hex);
}

wide derive_enc_keys_for_alice(int party, HMAC &hmac, string master_secret_share, string client_random_hex, string server_random_hex)
{
  auto master_secret = hex_to_emp_word(master_secret_share, party);
  return derive_enc_keys_for_alice(hmac, master_secret, client_random_hex, server_random_hex);
}

wide derive_enc_keys_for_alice(HMAC &hmac, wide master_secret, string client_random_hex, string server_random_hex)
{
  hmac.set_secret(master_secret, 12);
  auto seed_key_expansion = key_expansion_seed(client_random_hex, server_random_hex);
  auto key_material_key_expansion = hmac.run(3, seed_key_expansion, key_expansion_seed_len);
  wide enc_keys(8);
  copy(key_material_key_expansion[2].begin(), key_material_key_expansion[2].end(), enc_keys.begin());
  return enc_keys;
}

wide client_finished_message(int party, HMAC &hmac, string message_digest, string master_secret_share)
{

  auto master_secret_share_alice = hex_to_emp_word(master_secret_share, ALICE);
  auto master_secret_share_bob = hex_to_emp_word(master_secret_share, BOB);
  auto master_secret = wide_xor(master_secret_share_alice, master_secret_share_bob);

  auto seed_master_secret = client_finished_seed(message_digest);
  auto seed = client_finished_seed(message_digest);

  hmac.set_secret(master_secret, 12);
  auto material = hmac.run(1, seed, client_finished_seed_len);

  wide mac(3);
  copy(material[0].begin(), material[0].begin() + 3, mac.begin());
  return mac;
}