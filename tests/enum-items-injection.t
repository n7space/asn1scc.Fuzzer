  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-fuzzer -r EnumTests.MyEnum -o generated myenum.asn1 myenum.acn
  Generated 2 case(s).
  $ ${TESTDIR}/list_files.sh generated
  AllModels.acn
  AllModels.asn1
  test_main.c
  validate.h
  $ cat generated/AllModels.asn1
  EnumTests DEFINITIONS ::= BEGIN
  MyEnum ::= ENUMERATED
  {
      a(1),
      b(3),
      fuzzer-injected-0(0),
      fuzzer-injected-2(2)
  }
  END
  
  $ cat generated/AllModels.acn
  EnumTests DEFINITIONS ::= BEGIN
  MyEnum [size 2, encoding pos-int]
  END
  
  $ cat generated/test_main.c
  #include <stdio.h>
  #include <stdbool.h>
  
  #include "AllModels.h"
  #include "validate.h"
  
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
  
  /// \brief Test 0 validating incorrect message processing.
  /// \details Prepares TMyEnum type with incorrect value (0)
  ///          stored inside.
  static bool test_0(TMyEnum *v, BitStream *stream)
  {
    TMyEnum_Initialize(v);
  
    *v = 0;
  
    TMyEnum_encode(v, stream);
    return validate(stream);
  }
  
  /// \brief Test 2 validating incorrect message processing.
  /// \details Prepares TMyEnum type with incorrect value (2)
  ///          stored inside.
  static bool test_2(TMyEnum *v, BitStream *stream)
  {
    TMyEnum_Initialize(v);
  
    *v = 2;
  
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
  
    result += RUN_TEST(test_0, &v, &stream);
    result += RUN_TEST(test_2, &v, &stream);
  
    if (result == 0)
      printf("OK - all (2) tests passed\n.");
    else
      printf("ERROR - Failed %d out of 2 tests\n.", result);
    return result;
  }
