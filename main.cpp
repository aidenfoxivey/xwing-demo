#include "XWing.h"

#include <cstdio>


int main(void) {
  XWing::XWingSecretKey x_sk;
  XWing::XWingPublicKey x_pk;
  XWing::XWingCipherText x_ct;

  XWing::XWingSharedSecret x_ss_a;
  XWing::XWingSharedSecret x_ss_b;

  std::tie(x_sk, x_pk) = XWing::generate_key_pair();

  printf("XWING SK BYTES is %zu == %zu\n", x_sk.b.size(), XWing::XWING_SK_BYTES);
  printf("XWING PK BYTES is %zu == %zu\n", x_pk.b.size(), XWing::XWING_PK_BYTES);
  puts("\n");

  for (int i = 0; i < XWing::XWING_SK_BYTES; i++) {
    printf("%x", x_sk.b.data()[i]);
  }
  puts("\n");
  
  for (int i = 0; i < XWing::XWING_PK_BYTES; i++) {
    printf("%x", x_pk.b.data()[i]);
  }
  puts("\n");


  std::tie(x_ss_a, x_ct) = XWing::encapsulate(x_pk);


  x_ss_b = XWing::decapsulate(x_ct, x_sk);

  
  for (int i = 0; i < XWing::XWING_SS_BYTES; i++) {
    printf("%x", x_ss_a.b.data()[i]);
  }
  puts("\n");

  for (int i = 0; i < XWing::XWING_SS_BYTES; i++) {
    printf("%x", x_ss_b.b.data()[i]);
  }
  puts("\n");

  return 0;  
}
