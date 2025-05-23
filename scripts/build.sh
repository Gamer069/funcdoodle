if [[ $# -ne 4 ]]; then
	echo "Usage: $0 <Debug/Release> <tiling?> <clean?> <run?>"
	exit -1
fi
arg1=$(echo "$1" | tr '[:upper:]' '[:lower:]')
arg2=$(echo "$2" | tr '[:upper:]' '[:lower:]')
arg3=$(echo "$3" | tr '[:upper:]' '[:lower:]')
arg4=$(echo "$4" | tr '[:upper:]' '[:lower:]')

if [[ "$arg1" != "debug" && "$arg1" != "release" ]]; then
	echo "Mode must be either debug or release -- $arg1 is invalid"
	exit -1
fi
if [[ "$arg2" != "true" && "$arg2" != "false" ]]; then
	echo "istiling argument must be true or false -- $arg2 is invalid"
	exit -1
fi
if [[ "$arg3" != "true" && "$arg3" != "false" ]]; then
	echo "clean argument must be true or false -- $arg3 is invalid"
	exit -1
fi
if [[ "$arg4" != "true" && "$arg4" != "false" ]]; then
	echo "run argument must be true or false -- $arg4 is invalid"
	exit -1
fi

arg1=$(echo "$arg1" | awk '{print toupper(substr($0,1,1)) substr($0,2)}')

if [ "$arg3" == "true" ]; then
	rm -rf bin
fi

mkdir -p bin || exit -1 
cd bin || exit -1 
cmake -DCMAKE_BUILD_TYPE=$arg1 -DISTILING=$( (( arg2 == "true" )) && echo "ON" || echo "OFF" ) .. || exit -1
make || exit -1
cp -r ../assets . || exit -1
cp -r ../themes . || exit -1
cd ..
if [[ "$arg4" == "true" ]]; then
	./bin/FuncDoodle || exit -1
fi
