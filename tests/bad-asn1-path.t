# Keep no new line at end to make it work on Windows
  $ asn1scc-fuzzer -a bad-asn1.exe -r MyInt non-exisiting.asn1 || echo "Non zero exit code"
  ERROR: asn1scc call failed to start
  Non zero exit code* (glob)