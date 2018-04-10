#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
PROJECT_DIR="$( cd "${DIR}"/.. && pwd )"
BUILD_OS_NAME=${TRAVIS_OS_NAME:-linux}
BUILD_DIR_REL="${PROJECT_DIR}/../build"
BUILD_DIR="$( mkdir -p ${BUILD_DIR_REL} && cd ${BUILD_DIR_REL} && pwd )"
DIST_DIR_REL="${PROJECT_DIR}/../dist"
DIST_DIR="$( mkdir -p ${DIST_DIR_REL} && cd ${DIST_DIR_REL} && pwd )"

echo "OS: ${BUILD_OS_NAME}"
echo "Project directory: ${PROJECT_DIR}"
echo "Build directory: ${BUILD_DIR}"
