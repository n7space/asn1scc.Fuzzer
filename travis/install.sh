#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
source "${DIR}/setup-env.sh"

if [[ "$BUILD_OS_NAME" == "osx" ]]; then
  brew update
  brew install p7zip qt5
  brew link --force qt5

elif [[ $"$BUILD_OS_NAME" == "linux" ]]; then
    sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 100 --slave /usr/bin/g++ g++ /usr/bin/g++-5

    sudo pip install cram

    pushd $HOME
    wget "https://n7space-my.sharepoint.com/:u:/p/kgrochowski/EZSwzkHV-C1Mn1nppkUqKIcBjFpzsn67G_N_1RMW2Yr7dQ?download=1" -O asn1scc.7z
    7z x asn1scc.7z
    popd

fi
