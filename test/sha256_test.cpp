
#include <stdint.h>
#include "emp-deco/sha256.h"
#include "emp-deco/utils.h"
#include "emp-deco/debug.h"

using namespace std;

int main(int argc, char **argv)
{

  sha256 h = sha256();
  string in_hex = "52fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c64981855ad8681d0d86d1e91e00167939cb6694d2c422acd208a0072939487f6999";
  // expect c19ccf8b1aa56f290aca5d90acc674bbf4e4fcc22713aeece542cd22c22f1f09
  // string in_hex = "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  // expect da5698be17b9b46962335799779fbeca8ce5d491c0d26243bafef9ea1837a9d8

  vector<uint32_t> in = hex_to_word(in_hex);
  vector<uint32_t> out(8);
  h.block_init(out, in);
  debug(out);

  //circuit
  printf("\ncircuit\n");
  {
    setup_plain_prot(false, "");

    h.prepare_2pc();
    vector<Integer> out(8);
    vector<Integer> in = hex_to_emp_word(in_hex, PUBLIC);
    h.block_init(out, in);
    debug(out);
    finalize_plain_prot();
  }
}
