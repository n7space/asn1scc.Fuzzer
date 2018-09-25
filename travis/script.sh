#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ $BUILD_OS_NAME == 'linux' ]]; then
    . /opt/qt510/bin/qt510-env.sh
    QMAKE_PARAMS="-spec linux-g++"
fi

echo "Building..."
cd "${BUILD_DIR}"
$CXX --version
echo "Executing qmake..."
set -x
qmake "${PROJECT_DIR}"/fuzzer.pro -r ${QMAKE_PARAMS} \
      CONFIG+=release
set +x
echo "Executing make..."
make || exit 1

echo "Running Unit Tests"
src/tests/tests || exit 1

if [[ $BUILD_OS_NAME == 'linux' ]]; then
    echo "Running Integration Tests"
    PATH=${BUILD_DIR}/src/app:$HOME/asn1scc:$PATH cram "${PROJECT_DIR}"/tests/*.t || exit 1
fi
