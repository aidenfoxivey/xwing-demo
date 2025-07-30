#include "XWing.h"

#include <cstdio>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/* Testing harness written by claude-3-5-haiku for sake of focusing on XWING. */

void show_shared_secret(const XWing::XWingSharedSecret& xwing_ss) {
  for (int i = 0; i < XWing::XWING_SS_BYTES; i++) {
    printf("%02x", xwing_ss.b.data()[i]);
  }
  puts("");
}

struct TestVector {
  std::string seed;
  std::string sk;
  std::string pk;
  std::string eseed;
  std::string ct;
  std::string ss;
};

std::vector<uint8_t> hex_to_bytes(const std::string& hex) {
  std::vector<uint8_t> bytes;
  for (size_t i = 0; i < hex.length(); i += 2) {
    std::string byte_string = hex.substr(i, 2);
    uint8_t byte = static_cast<uint8_t>(strtol(byte_string.c_str(), nullptr, 16));
    bytes.push_back(byte);
  }
  return bytes;
}

std::vector<TestVector> parse_test_vectors(const std::string& filename) {
  std::vector<TestVector> vectors;
  std::ifstream file(filename);
  std::string line;
  TestVector current_vector;
  std::string current_field;
  std::string current_value;

  while (std::getline(file, line)) {
    if (line.empty() || line.find("extracted from") != std::string::npos) {
      continue;
    }

    if (line.find("seed") == 0) {
      if (!current_vector.seed.empty()) {
        vectors.push_back(current_vector);
        current_vector = TestVector();
      }
      current_field = "seed";
      current_value = line.substr(line.find_first_not_of("seed "));
    } else if (line.find("sk") == 0) {
      current_field = "sk";
      current_value = line.substr(line.find_first_not_of("sk "));
    } else if (line.find("pk") == 0) {
      current_field = "pk";
      current_value = "";
    } else if (line.find("eseed") == 0) {
      current_field = "eseed";
      current_value = "";
    } else if (line.find("ct") == 0) {
      current_field = "ct";
      current_value = "";
    } else if (line.find("ss") == 0) {
      current_field = "ss";
      current_value = line.substr(line.find_first_not_of("ss "));
    } else if (line.find("  ") == 0) {
      std::string hex_part = line.substr(2);
      current_value += hex_part;
    } else {
      continue;
    }

    if (current_field == "seed") current_vector.seed = current_value;
    else if (current_field == "sk") current_vector.sk = current_value;
    else if (current_field == "pk") current_vector.pk = current_value;
    else if (current_field == "eseed") current_vector.eseed = current_value;
    else if (current_field == "ct") current_vector.ct = current_value;
    else if (current_field == "ss") current_vector.ss = current_value;
  }

  if (!current_vector.seed.empty()) {
    vectors.push_back(current_vector);
  }

  return vectors;
}

bool test_vector(const TestVector& tv, int vector_num) {
  printf("Testing vector %d:\n", vector_num);

  XWing::XWingSecretKey sk_bytes;
  auto sk_vec = hex_to_bytes(tv.sk);
  std::copy(sk_vec.begin(), sk_vec.end(), sk_bytes.b.begin());

  XWing::XWingSecretKey xwing_sk;
  XWing::XWingPublicKey xwing_pk;
  std::tie(xwing_sk, xwing_pk) = XWing::generate_key_pair_derand(sk_bytes);

  auto expected_pk_vec = hex_to_bytes(tv.pk);
  XWing::XWingPublicKey expected_pk;
  std::copy(expected_pk_vec.begin(), expected_pk_vec.end(), expected_pk.b.begin());

  bool pk_match = xwing_pk.b == expected_pk.b;
  printf("  Key generation: %s\n", pk_match ? "PASS" : "FAIL");

  auto eseed_vec = hex_to_bytes(tv.eseed);
  std::array<uint8_t, 64> eseed_array;
  std::copy(eseed_vec.begin(), eseed_vec.end(), eseed_array.begin());

  XWing::XWingCipherText xwing_ct;
  XWing::XWingSharedSecret xwing_ss;
  std::tie(xwing_ss, xwing_ct) = XWing::encapsulate_derand(xwing_pk, eseed_array);

  auto expected_ct_vec = hex_to_bytes(tv.ct);
  XWing::XWingCipherText expected_ct;
  std::copy(expected_ct_vec.begin(), expected_ct_vec.end(), expected_ct.b.begin());

  auto expected_ss_vec = hex_to_bytes(tv.ss);
  XWing::XWingSharedSecret expected_ss;
  std::copy(expected_ss_vec.begin(), expected_ss_vec.end(), expected_ss.b.begin());

  bool ct_match = xwing_ct.b == expected_ct.b;
  bool ss_match = xwing_ss.b == expected_ss.b;
  printf("  Encapsulation: %s\n", (ct_match && ss_match) ? "PASS" : "FAIL");

  XWing::XWingSharedSecret decap_ss = XWing::decapsulate(expected_ct, xwing_sk);
  bool decap_match = decap_ss.b == expected_ss.b;
  printf("  Decapsulation: %s\n", decap_match ? "PASS" : "FAIL");

  return pk_match && ct_match && ss_match && decap_match;
}


int main(void) {
  printf("=== XWing Test Vector Validation ===\n\n");

  auto test_vectors = parse_test_vectors("raw_test_vectors.txt");
  printf("Loaded %zu test vectors\n\n", test_vectors.size());

  int passed = 0;
  int total = test_vectors.size();

  for (size_t i = 0; i < test_vectors.size(); i++) {
    if (test_vector(test_vectors[i], i + 1)) {
      passed++;
    }
    printf("\n");
  }

  printf("=== Final Result ===\n");
  printf("Passed: %d/%d\n", passed, total);

  if (passed == total) {
    printf("ALL TESTS PASSED!\n");
    return 0;
  } else {
    printf("SOME TESTS FAILED!\n");
    return 1;
  }
}
