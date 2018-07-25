# Keep no new line at end to make it work on Windows
  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-maltester -m MyInt -o generated file-list.asn1
  Generated 3 cases.
  $ ${TESTDIR}/list_files.sh generated
  AllModels.acn
  AllModels.asn1
  test_main.c
