if [[ $# -ne 6 ]]; then
	echo "Usage: $0 <Debug/Release> <clean?> <run?> <portaudio.h path> <portaudio lib path>"
	exit -1
fi

arg1=$(echo "$1" | tr '[:upper:]' '[:lower:]')
arg2=$(echo "$2" | tr '[:upper:]' '[:lower:]')
arg3=$(echo "$3" | tr '[:upper:]' '[:lower:]')
arg4=$(echo "$4")
arg5=$(echo "$5")

if [[ "$arg1" != "debug" && "$arg1" != "release" ]]; then
	echo "Mode must be either debug or release -- $arg1 is invalid"
	exit -1
fi
if [[ "$arg2" != "true" && "$arg3" != "false" ]]; then
	echo "clean argument must be true or false -- $arg2 is invalid"
	exit -1
fi
if [[ "$arg3" != "true" && "$arg4" != "false" ]]; then
	echo "run argument must be true or false -- $arg3 is invalid"
	exit -1
fi

arg1=$(echo "$arg1" | awk '{print toupper(substr($0,1,1)) substr($0,2)}')

if (( arg2 == true )); then
	rm -rf bin
fi

mkdir -p bin || exit -1 
cd bin || exit -1 
mkdir incl
cp $arg4 incl/
cp $arg5 portaudio_x64.lib
cmake -DCMAKE_TOOLCHAIN_FILE=../mingw.cmake -DCMAKE_BUILD_TYPE=$arg1 .. || exit -1
make || exit -1
cp -r ../assets . || exit -1
cp ../assets/keys.txt ./assets/ || exit -1
cp ../assets/icon.png ./assets/ || exit -1
if [[ "$arg3" == "true" ]]; then
	wine ./FuncDoodle.exe || exit -1
fi
