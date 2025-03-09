#!/usr/bin/env bash

echo "Building program..."
if [[ ! $OSTYPE == 'darwin'* ]]; then
    source /etc/profile.d/devkit-env.sh
fi
make clean && make


echo
echo "Creating release..."
if [ -d "release" ]; then
    rm -rf "release"
fi
mkdir "release"


echo
echo "'apps' folder..."
mkdir -p "./release/apps/WiiLikeToParty"
cp ./WiiLikeToParty.dol ./release/apps/WiiLikeToParty/boot.dol
cp ./WiiLikeToParty.elf ./release/apps/WiiLikeToParty/alt_for_nerds.elf
cp ./assets/hb/* ./release/apps/WiiLikeToParty/


echo
echo "'wad' folder..."
mkdir -p "./release/wad"
cp ./wad/ios/*.wad ./release/wad/
cp ./wad/forwarder_WL2P.wad ./release/wad/


echo
echo "Compressing release..."
GIT_VERSION=$(git describe --tags --always)
ZIP_FILE="WiiLikeToParty_$GIT_VERSION.zip"
if [ -f "./$ZIP_FILE" ]; then
    rm -f "./$ZIP_FILE"
fi
# sudo apt install zip
cd ./release; zip -r9 "../$ZIP_FILE" *

echo
echo Done!
