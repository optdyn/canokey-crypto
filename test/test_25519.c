// SPDX-License-Identifier: Apache-2.0
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>

#include "ed25519.h"
#include "sha.h"
#include <cmocka.h>

static void test_ed25519_public(void **state) {
  (void)state;
  // from rfc8032 test 1
  uint8_t in[64] = {
      0x9d, 0x61, 0xb1, 0x9d, 0xef, 0xfd, 0x5a, 0x60, 0xba, 0x84, 0x4a,
      0xf4, 0x92, 0xec, 0x2c, 0xc4, 0x44, 0x49, 0xc5, 0x69, 0x7b, 0x32,
      0x69, 0x19, 0x70, 0x3b, 0xac, 0x03, 0x1c, 0xae, 0x7f, 0x60,
  };
  uint8_t out[32];
  uint8_t expected[] = {
      0xd7, 0x5a, 0x98, 0x01, 0x82, 0xb1, 0x0a, 0xb7, 0xd5, 0x4b, 0xfe,
      0xd3, 0xc9, 0x64, 0x07, 0x3a, 0x0e, 0xe1, 0x72, 0xf3, 0xda, 0xa6,
      0x23, 0x25, 0xaf, 0x02, 0x1a, 0x68, 0xf7, 0x07, 0x51, 0x1a,
  };

  ed25519_publickey(in, out);

  for (int i = 0; i != 32; ++i) {
    assert_int_equal(out[i], expected[i]);
  }
}

static void test_ed25519_public2(void **state) {
  (void)state;
  // taken from https://blog.dang.fan/zh-Hans/posts/25519
  // also rfc8032 test 2
  uint8_t in[64] = {0x4c, 0xcd, 0x08, 0x9b, 0x28, 0xff, 0x96, 0xda,
                    0x9d, 0xb6, 0xc3, 0x46, 0xec, 0x11, 0x4e, 0x0f,
                    0x5b, 0x8a, 0x31, 0x9f, 0x35, 0xab, 0xa6, 0x24,
                    0xda, 0x8c, 0xf6, 0xed, 0x4f, 0xb8, 0xa6, 0xfb};
  uint8_t out[32];
  uint8_t expected[] = {0x3d, 0x40, 0x17, 0xc3, 0xe8, 0x43, 0x89, 0x5a,
                        0x92, 0xb7, 0x0a, 0xa7, 0x4d, 0x1b, 0x7e, 0xbc,
                        0x9c, 0x98, 0x2c, 0xcf, 0x2e, 0xc4, 0x96, 0x8c,
                        0xc0, 0xcd, 0x55, 0xf1, 0x2a, 0xf4, 0x66, 0x0c};

  ed25519_publickey(in, out);

  for (int i = 0; i != 32; ++i) {
    assert_int_equal(out[i], expected[i]);
  }
}

static void test_ed25519_sign(void **state) {
  (void)state;
  // from rfc8032 test 2
  uint8_t sk[] = {
      0x4c, 0xcd, 0x08, 0x9b, 0x28, 0xff, 0x96, 0xda, 0x9d, 0xb6, 0xc3,
      0x46, 0xec, 0x11, 0x4e, 0x0f, 0x5b, 0x8a, 0x31, 0x9f, 0x35, 0xab,
      0xa6, 0x24, 0xda, 0x8c, 0xf6, 0xed, 0x4f, 0xb8, 0xa6, 0xfb,
  };
  uint8_t pk[] = {
      0x3d, 0x40, 0x17, 0xc3, 0xe8, 0x43, 0x89, 0x5a, 0x92, 0xb7, 0x0a,
      0xa7, 0x4d, 0x1b, 0x7e, 0xbc, 0x9c, 0x98, 0x2c, 0xcf, 0x2e, 0xc4,
      0x96, 0x8c, 0xc0, 0xcd, 0x55, 0xf1, 0x2a, 0xf4, 0x66, 0x0c,
  };
  uint8_t m[] = {0x72};
  uint8_t sig[64];
  uint8_t expected[] = {
      0x92, 0xa0, 0x09, 0xa9, 0xf0, 0xd4, 0xca, 0xb8, 0x72, 0x0e, 0x82,
      0x0b, 0x5f, 0x64, 0x25, 0x40, 0xa2, 0xb2, 0x7b, 0x54, 0x16, 0x50,
      0x3f, 0x8f, 0xb3, 0x76, 0x22, 0x23, 0xeb, 0xdb, 0x69, 0xda, 0x08,
      0x5a, 0xc1, 0xe4, 0x3e, 0x15, 0x99, 0x6e, 0x45, 0x8f, 0x36, 0x13,
      0xd0, 0xf1, 0x1d, 0x8c, 0x38, 0x7b, 0x2e, 0xae, 0xb4, 0x30, 0x2a,
      0xee, 0xb0, 0x0d, 0x29, 0x16, 0x12, 0xbb, 0x0c, 0x00,
  };

  ed25519_sign(m, sizeof(m), sk, pk, sig);

  for (int i = 0; i != 32; ++i) {
    assert_int_equal(sig[i], expected[i]);
  }
}

static void test_ed25519_sign2(void **state) {
  (void)state;
  // from rfc8032 test 1
  uint8_t sk[64] = {
      0x9d, 0x61, 0xb1, 0x9d, 0xef, 0xfd, 0x5a, 0x60, 0xba, 0x84, 0x4a,
      0xf4, 0x92, 0xec, 0x2c, 0xc4, 0x44, 0x49, 0xc5, 0x69, 0x7b, 0x32,
      0x69, 0x19, 0x70, 0x3b, 0xac, 0x03, 0x1c, 0xae, 0x7f, 0x60,
  };
  uint8_t pk[] = {
      0xd7, 0x5a, 0x98, 0x01, 0x82, 0xb1, 0x0a, 0xb7, 0xd5, 0x4b, 0xfe,
      0xd3, 0xc9, 0x64, 0x07, 0x3a, 0x0e, 0xe1, 0x72, 0xf3, 0xda, 0xa6,
      0x23, 0x25, 0xaf, 0x02, 0x1a, 0x68, 0xf7, 0x07, 0x51, 0x1a,
  };
  uint8_t m[] = {};
  uint8_t sig[64];
  uint8_t expected[] = {
      0xe5, 0x56, 0x43, 0x00, 0xc3, 0x60, 0xac, 0x72, 0x90, 0x86, 0xe2,
      0xcc, 0x80, 0x6e, 0x82, 0x8a, 0x84, 0x87, 0x7f, 0x1e, 0xb8, 0xe5,
      0xd9, 0x74, 0xd8, 0x73, 0xe0, 0x65, 0x22, 0x49, 0x01, 0x55, 0x5f,
      0xb8, 0x82, 0x15, 0x90, 0xa3, 0x3b, 0xac, 0xc6, 0x1e, 0x39, 0x70,
      0x1c, 0xf9, 0xb4, 0x6b, 0xd2, 0x5b, 0xf5, 0xf0, 0x59, 0x5b, 0xbe,
      0x24, 0x65, 0x51, 0x41, 0x43, 0x8e, 0x7a, 0x10, 0x0b,
  };

  ed25519_sign(m, sizeof(m), sk, pk, sig);

  for (int i = 0; i != 32; ++i) {
    assert_int_equal(sig[i], expected[i]);
  }
}

static void test_cv25519_decrypt(void **state) {
  (void)state;
  // taken from rfc7748 section 6.1
  // swapped endianness
  uint8_t input[] = {
      0x6a, 0x4e, 0x9b, 0xaa, 0x8e, 0xa9, 0xa4, 0xeb, 0xf4, 0x1a, 0x38,
      0x26, 0x0d, 0x3a, 0xbf, 0x0d, 0x5a, 0xf7, 0x3e, 0xb4, 0xdc, 0x7d,
      0x8b, 0x74, 0x54, 0xa7, 0x30, 0x89, 0x09, 0xf0, 0x20, 0x85,
  };
  uint8_t sk[] = {0xeb, 0xe0, 0x88, 0xff, 0x27, 0x8b, 0x2f, 0x1c,
                  0xfd, 0xb6, 0x18, 0x26, 0x29, 0xb1, 0x3b, 0x6f,
                  0xe6, 0x0e, 0x80, 0x83, 0x8b, 0x7f, 0xe1, 0x79,
                  0x4b, 0x8a, 0x4a, 0x62, 0x7e, 0x08, 0xab, 0x5d};
  uint8_t out[32];
  uint8_t expected[] = {0x42, 0x17, 0x16, 0x1e, 0x3c, 0x9b, 0xf0, 0x76,
                        0x33, 0x9e, 0xd1, 0x47, 0xc9, 0x21, 0x7e, 0xe0,
                        0x25, 0x0f, 0x35, 0x80, 0xf4, 0x3b, 0x8e, 0x72,
                        0xe1, 0x2d, 0xce, 0xa4, 0x5b, 0x9d, 0x5d, 0x4a};

  curve25519_key_from_random(sk);
  x25519(out, sk, input);

  for (int i = 0; i != 32; ++i) {
    assert_int_equal(out[i], expected[i]);
  }
}

int main() {
  const struct CMUnitTest tests[] = {
      cmocka_unit_test(test_ed25519_public),
      cmocka_unit_test(test_ed25519_public2),
      cmocka_unit_test(test_ed25519_sign),
      cmocka_unit_test(test_ed25519_sign2),
      cmocka_unit_test(test_cv25519_decrypt),
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
