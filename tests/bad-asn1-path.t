# Keep no new line at end to make it work on Windows
  $ asn1scc-maltester -a bad-asn1.exe -m MyInt non-exisiting.asn1 || echo "Non zero exit code"
  ERROR: asn1scc call failed to start
  Non zero exit code* (glob)