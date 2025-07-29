#include "XWing.h"

#include <cstdio>

void show_shared_secret(const XWing::XWingSharedSecret& xwing_ss) {
  for (int i = 0; i < XWing::XWING_SS_BYTES; i++) {
    printf("%02x", xwing_ss.b.data()[i]);
  }
  puts("");
}

int main(void) {
  XWing::XWingSecretKey x_sk;
  XWing::XWingPublicKey x_pk;
  XWing::XWingCipherText x_ct;

  XWing::XWingSharedSecret x_ss_a;
  XWing::XWingSharedSecret x_ss_b;

  bool passed = true;

  for (int i = 0; i < 10000; i++) {
    std::tie(x_sk, x_pk) = XWing::generate_key_pair();
    std::tie(x_ss_a, x_ct) = XWing::encapsulate(x_pk);
    x_ss_b = XWing::decapsulate(x_ct, x_sk);

    if (x_ss_a.b != x_ss_b.b) {
      passed = false;
      show_shared_secret(x_ss_a);
      show_shared_secret(x_ss_b);
      break;
    }
  }

  if (passed) {
    puts("PASSED!");
  } else{
    puts("FAILED!");
  }
}
