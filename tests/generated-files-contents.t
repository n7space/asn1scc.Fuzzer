  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-fuzzer -r Tests.MyInt -o generated a.asn1 b.asn1 a.acn b.acn
  Generated 1 case(s).
  $ ${TESTDIR}/list_files.sh generated
  AllModels.acn
  AllModels.asn1
  test_main.c
  validate.h
  $ cat generated/AllModels.asn1
  Tests DEFINITIONS ::= BEGIN
  MyInt ::= INTEGER(0 .. 255)
  END
  
  Tests2 DEFINITIONS ::= BEGIN
  MyInt2 ::= INTEGER(0 .. 15)
  END
  
  $ cat generated/AllModels.acn
  Tests DEFINITIONS ::= BEGIN
  MyInt [size 8, encoding pos-int]
  END
  
  Tests2 DEFINITIONS ::= BEGIN
  MyInt2 []
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
  
  static void TMyInt_encode(const TMyInt *v, BitStream *stream)
  {
    BitStream_Init(stream, stream->buf, stream->count);
    int errCode = 0;
    TMyInt_ACN_Encode(v, stream, &errCode, FALSE);
  }
  
  /// \brief Test 255 validating incorrect message processing.
  /// \details Prepares TMyInt type with incorrect value (255)
  ///          stored inside.
  static bool test_255(TMyInt *v, BitStream *stream)
  {
    TMyInt_Initialize(v);
  
    *v = 255;
  
    TMyInt_encode(v, stream);
    return validate(stream);
  }
  
  int main()
  {
    TMyInt v;
    byte buf[TMyInt_REQUIRED_BYTES_FOR_ACN_ENCODING];
    BitStream stream;
    BitStream_Init(&stream, buf, sizeof(buf));
    int result = 0;
  
    result += RUN_TEST(test_255, &v, &stream);
  
    if (result == 0)
      printf("OK - all (1) tests passed\n.");
    else
      printf("ERROR - Failed %d out of 1 tests\n.", result);
    return result;
  }
  $ cat generated/validate.h
  #ifndef ASN1SCC_FUZZER_VALIDATE_H
  #define ASN1SCC_FUZZER_VALIDATE_H
  
  #include <stdbool.h>
  
  #include "asn1crt.h"
  
  bool validate(const BitStream *stream);
  
  #endif
