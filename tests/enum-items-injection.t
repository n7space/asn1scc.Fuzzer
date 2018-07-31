# Keep no new line at end to make it work on Windows
  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-maltester -r EnumTests.MyEnum -o generated myenum.asn1 myenum.acn
  Generated 1 case(s).
  $ ${TESTDIR}/list_files.sh generated
  AllModels.acn
  AllModels.asn1
  test_main.c
  $ cat generated/AllModels.asn1
  EnumTests DEFINITIONS ::= BEGIN
  MyEnum ::= ENUMERATED
  {
      a(0),
      b(1),
      c(2),
      maltester-injected-3(3)
  }
  END
  
  $ cat generated/AllModels.acn
  EnumTests DEFINITIONS ::= BEGIN
  MyEnum []
  END
  
  $ cat generated/test_main.c
  #include <stdio.h>
  #include <stdbool.h>
  
  #include "AllModels.h"
  
  #define RUN_TEST(T, ...) \
    (printf("Executing " #T " ... "), \
     (T(__VA_ARGS__) \
        ? (printf("PASSED\n"), 0) \
        : (printf("FAILED\n"), 1)))
  
  static void TMyEnum_encode(const TMyEnum *v, BitStream *stream)
  {
    BitStream_Init(stream, stream->buf, stream->count);
    int errCode = 0;
    TMyEnum_ACN_Encode(v, stream, &errCode, FALSE);
  }
  
  static bool validate(BitStream *stream)
  {
    // TODO - fill to perform tests on desired target
    return false;
  }
  
  static bool test_3(TMyEnum *v, BitStream *stream)
  {
    TMyEnum_Initialize(v);
  
    *v = 3;
  
    TMyEnum_encode(v, stream);
    return validate(stream);
  }
  
  int main()
  {
    TMyEnum v;
    byte buf[TMyEnum_REQUIRED_BYTES_FOR_ACN_ENCODING];
    BitStream stream;
    BitStream_Init(&stream, buf, sizeof(buf));
    int result = 0;
  
    result += RUN_TEST(test_3, &v, &stream);
  
    if (result == 0)
      printf("OK - all (1) tests passed\n.");
    else
      printf("ERROR - Failed %d out of 1 tests\n.", result);
    return result;
  }
