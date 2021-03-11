
#include <stdint.h>
#include "emp-deco/debug.h"
#include "emp-deco/tls.h"

using namespace std;

void test_master_secret(int port, int party, string client_random, string server_random)
{

  NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

  setup_semi_honest(io, party);

  sha256 hasher = sha256();
  hasher.prepare_2pc();
  HMAC hmac = HMAC(hasher);
  string secret;

  string share = party == ALICE ? "1" : "0";

  auto master_secret_xor = derive_master_secret(hmac, share, client_random, server_random);
  cout << "hi" << endl;

  debug(master_secret_xor, "xor", XOR);
  debug(master_secret_xor, "master secret");
  // expect
  // 8902f9d73a3f65e8096a33ac36ba00f49e7b28f802d95e93879a45101bd1e27b4ab2c3e73fe7eb35018bd791e9b074cb

  cout << CircuitExecution::circ_exec->num_and() << endl;
  finalize_semi_honest();
  delete io;
}

void test_enc_keys(int port, int party, string client_random, string server_random)
{

  NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

  setup_semi_honest(io, party);

  sha256 hasher = sha256();
  hasher.prepare_2pc();
  HMAC hmac = HMAC(hasher);

  string share = party == ALICE ? "1" : "0";
  wide enc_keys = derive_enc_keys_for_alice(hmac, share, client_random, server_random);

  debug(enc_keys, "encryption keys");
  // expect
  // client enc: bb0941a9c66263c9106bab97169183a4
  // server enc: 30e6464281fd14092b1a30af241007cb

  cout << CircuitExecution::circ_exec->num_and() << endl;
  finalize_semi_honest();
  delete io;
}

void test_client_finished(int port, int party)
{

  NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

  setup_semi_honest(io, party);

  sha256 hasher = sha256();
  hasher.prepare_2pc();
  HMAC hmac = HMAC(hasher);

  string digest = "75c23158bf5e419ee13b3a17e9e83c1833e976187d6b00816bd342960f4d6ba3";

  string xor_master_secret = party == ALICE
                                 ? "58d0dbbc748cad73e4573494fe2793afeb17dd4b310b97bc356b806a31b8864671587bcd6c51eb9d98954bb38ac62fe6"
                                 : "d1d2226b4eb3c89bed3d0738c89d935b756cf5b333d2c92fb2f1c57a2a69643d3beab82a53b600a8991e9c2263765b2d";

  auto msg = client_finished_message(party, hmac, digest, xor_master_secret);

  debug(msg, "client finished");
  // expect
  // e6ab0f4859b9e27520392d31

  cout << CircuitExecution::circ_exec->num_and() << endl;
  finalize_semi_honest();
  delete io;
}

// void test_xor(int port, int party)
// {
//   NetIO *io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);
//   setup_semi_honest(io, party);

//   uint32_t number = party == ALICE ? 12 : 25;
//   Integer a(32, number, ALICE);
//   Integer b(32, number, BOB);
//   Integer c = a ^ b;

//   cout << "a " << c.reveal<uint32_t>(XOR) << endl;

//   cout << CircuitExecution::circ_exec->num_and() << endl;
//   finalize_semi_honest();
//   delete io;
// }

int main(int argc, char **argv)
{
  string client_random = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";
  string server_random = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb";

  int port, party;
  parse_party_and_port(argv, &party, &port);

  // test_master_secret(port, party, client_random, server_random);
  // test_enc_keys(port, party, client_random, server_random);
  // test_client_finished(port, party);
}