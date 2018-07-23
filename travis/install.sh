#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip qt5
  brew link --force qt5

  curl https://bootstrap.pypa.io/get-pip.py | sudo python

elif [[ $"$BUILD_OS_NAME" == "linux" ]]; then
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 --slave /usr/bin/g++ g++ /usr/bin/g++-5
fi

sudo pip install cram
