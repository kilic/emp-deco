
#include <stdint.h>
#include "emp-deco/sha256.h"
#include <iostream>
#include <boost/algorithm/hex.hpp>

using namespace std;

int main(int argc, char **argv)
{

  uint8_t in[64] = {0};
  // expect da5698be17b9b46962335799779fbeca8ce5d491c0d26243bafef9ea1837a9d8

  string in_hex = "52fdfc072182654f163f5f0f9a621d729566c74d10037c4d7bbb0407d1e2c64981855ad8681d0d86d1e91e00167939cb6694d2c422acd208a0072939487f6999";
  // expect c19ccf8b1aa56f29aca5d90acc674bbf4e4fcc22713aeece542cd22c22f1f09

  string unhexed = boost::algorithm::unhex(string(in_hex));
  copy(unhexed.begin(), unhexed.end(), in);

  uint32_t r[8];
  sha256_block(r, IV, in);

  for (int i = 0; i < 8; i++)
  {
    cout << hex << r[i] << endl;
  }
}
