#!/bin/bash

pushd ..
Vendor/Binaries/Premake/Linux/premake5 --cc=clang --file=Build.lua gmake2
Vendor/Binaries/Premake/Linux/premake5 --file=Build.lua ecc
popd
