mkdir -p build || exit -1
cd build || exit -1
cmake -DCMAKE_BUILD_TYPE=Debug .. || exit -1
make || exit -1
cp -r ../assets . || exit -1
