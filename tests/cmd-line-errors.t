  $ asn1scc-maltester || printf "Non zero exit code
  > "
  ERROR: No files provided
  Non zero exit code

  $ asn1scc-maltester non-exisiting.asn1 || printf "Non zero exit code
  > "
  ERROR: Main structure name must be set
  Non zero exit code
