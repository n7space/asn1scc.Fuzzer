# Keep no new line at end to make it work on Windows
  $ cp ${TESTDIR}/incorrect-asn1/syntax-error.asn1 ${PWD}
  $ asn1scc-maltester -m MyInt syntax-error.asn1 || echo "Non zero exit code"
  ERROR: asn1scc call failed
  syntax-error.asn1:1:6: error: extraneous input '::=' expecting DEFINITIONS
  
  Non zero exit code* (glob)