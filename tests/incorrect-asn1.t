# Keep no new line at end to make it work on Windows
  $ ${TESTDIR}/copy_test_resources.sh
  $ asn1scc-fuzzer -r MyInt syntax-error.asn1 || echo "Non zero exit code"
  ERROR: asn1scc call failed
  syntax-error.asn1:1:6: error: extraneous input '::=' expecting DEFINITIONS 
  
  Non zero exit code* (glob)