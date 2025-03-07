#!/bin/bash

if [ ! -d src ]; then
  echo "ERROR: Please run build_deb.sh from project root. Aborting"
  exit 1
fi

source ./release/functions.sh

release=false
source_only=false
snapshot=true
revision=1
ppa=false

while getopts "k:n:rsxp" opt
do
  case $opt in
    r) release=true
       echo "INFO: Making release package"
       ;;
    s) source_only=true
       echo "INFO: Building source package only"
       ;;
    k) key_id="$OPTARG"
       ;;
    x) snapshot=false
       echo "INFO: Building non-snapshot package"
       ;;
    n) revision="$OPTARG"
       ;;
    p) ppa=true
       echo "INFO: Building package for PPA"
       ;;
  esac
done

if [ -z $key_id ]; then
  echo "ERROR: Missing required option -k key_id"
  exit 1
fi

prep_debian_dir_for_ppa() {
  mv ./debian/control ./debian/control_
  mv ./debian/rules ./debian/rules_
  cp ./debian/ppa/control ./debian/control
  cp ./debian/ppa/rules ./debian/rules
}

revert_prep_debian_dir_for_ppa() {
  rm ./debian/control
  rm ./debian/rules
  mv ./debian/control_ ./debian/control
  mv ./debian/rules_ ./debian/rules
}

export DEBEMAIL="jinmanr@gmail.com"
export DEBFULLNAME="Rob Jinman"

changelog_path="$(pwd)/debian/changelog"

if [ $snapshot == true ]; then
  ./release/create_tarball.sh -s -v "$version"
else
  ./release/create_tarball.sh -v "$version"
fi

tarball_path="$(get_tarball_path ./build)"
tarball_name="$(get_tarball_name ./build)"
version="$(get_tarball_version ./build)"
deb_version="${version}-0ubuntu$revision"

mv "$tarball_path" ../
cd ..
tar -xf "$tarball_name"
cd "procalc-${version}"

if $ppa; then
  prep_debian_dir_for_ppa
fi

dch --newversion "$deb_version"
if $release; then
  dch --release
fi

# Replace the old changelog with the new one
if [ "./debian/changelog" -nt "$changelog_path" ]; then
  cp ./debian/changelog "$changelog_path"
fi

export CMAKE_BUILD_TYPE=Release

if $source_only; then
  debuild -S -sa -k$key_id
else
  debuild -k$key_id
fi

if $ppa; then
  revert_prep_debian_dir_for_ppa
fi
