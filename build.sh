mkdir -p build || exit -1
cd build || exit -1
cmake .. || exit -1
make || exit -1
cp -r ../assets/ . || exit -1
