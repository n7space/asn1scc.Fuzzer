#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

OS_TAR_SUFFIX="linux-x64"

pushd ${DIST_DIR}
mkdir asn1scc-Fuzzer
cd asn1scc-Fuzzer

cp ${PROJECT_DIR}/LICENSE .
cp ${PROJECT_DIR}/README.md .
cp ${BUILD_DIR}/src/app/asn1scc-fuzzer .

mkdir lib
cp /opt/qt510/lib/libQt5Core.so.5 lib/
cp /usr/lib/x86_64-linux-gnu/libicui18n.so.52 lib/
cp /usr/lib/x86_64-linux-gnu/libicuuc.so.52 lib/
cp /usr/lib/x86_64-linux-gnu/libicudata.so.52 lib/

cd ..

tar czf ${PROJECT_DIR}/asn1scc-Fuzzer-${TRAVIS_TAG}-${OS_TAR_SUFFIX}.tar.gz asn1scc-Fuzzer

popd
