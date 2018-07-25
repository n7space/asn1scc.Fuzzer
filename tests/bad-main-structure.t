# Keep no new line at end to make it work on Windows
  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-maltester -m Xyz bad-main-structure.asn1 || echo "Non zero exit code"
  Type "Xyz" from module: "*any*" not found. No cases were generated.
  Non zero exit code* (glob)