mkdir -p build || exit -1 
cd build || exit -1 
cmake -DCMAKE_BUILD_TYPE=Debug -DISTILING=ON .. || exit -1
make || exit -1
cp -r ../assets . || exit -1
cp ../keys.txt ./assets/ || exit -1
cp ../icon.png ./assets/ || exit -1
