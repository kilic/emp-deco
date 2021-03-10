#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-deco/utils.h"
#include "emp-deco/hmac.h"
#include "emp-deco/sha256.h"
#include "emp-deco/tls.h"
#include "emp-deco/debug.h"

using namespace emp;
using namespace std;

int main(int argc, char **argv)
{

  string seed_hex = "6d617374657220736563726574aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";
  string secret_hex = "0000000000000000000000000000000000000000000000000000000000000001";
  int key_size = 2;
  // expect
  // 4ab2c3e73fe7eb35018bd791e9b074cb29b1ed6a15e84a05c55e305a503e35b5
  // 8902f9d73a3f65e8096a33ac36ba00f49e7b28f802d95e93879a45101bd1e27b

  setup_plain_prot(false, "");

  sha256 hasher = sha256();
  hasher.prepare_2pc();

  int seed_bit_len = seed_hex.size() * 4;
  cout << seed_bit_len << endl;
  vector<Integer> seed = hex_to_emp_word(seed_hex, PUBLIC);
  vector<Integer> secret = hex_to_emp_word(secret_hex, PUBLIC);

  HMAC hmac = HMAC(hasher);
  hmac.set_secret(secret, 8);
  auto res = hmac.run(key_size, seed, seed_bit_len);

  // for (int i = 0; i < res.size(); i++)
  //   debug(res[i], "U");

  finalize_plain_prot();
}