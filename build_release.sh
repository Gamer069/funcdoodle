./build.sh debug true true false
./clean.sh
tar -caf debug_build_linux_tiling.tar.xz -C build .
./build.sh debug false true false
./clean.sh
tar -caf debug_build_linux.tar.xz -C build .
./build.sh release true true false
./clean.sh
tar -caf release_build_linux_tiling.tar.xz -C build .
./build.sh release false true false
./clean.sh
tar -caf release_build_linux.tar.xz -C build .
