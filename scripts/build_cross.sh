if [[ $# -ne 8 ]]; then
	echo "Usage: $0 <Debug/Release> <clean?> <run?> <win portaudio.h path> <win portaudio lib path> <mac portaudio.h path> <mac x86_64 portaudio lib/dylib path> <mac arm64 portaudio lib/dylib path>"
	exit -1
fi

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

arg1=$(echo "$1" | tr '[:upper:]' '[:lower:]')
arg2=$(echo "$2" | tr '[:upper:]' '[:lower:]')
arg3=$(echo "$3" | tr '[:upper:]' '[:lower:]')
arg4=$(echo "$4")
arg5=$(echo "$5")
arg6=$(echo "$6")
arg7=$(echo "$7")
arg8=$(echo "$8")

if [[ "$arg1" != "debug" && "$arg1" != "release" ]]; then
	echo "Mode must be either debug or release -- $arg1 is invalid"
	exit -1
fi
if [[ "$arg2" != "true" && "$arg2" != "false" ]]; then
	echo "clean argument must be true or false -- $arg2 is invalid"
	exit -1
fi
if [[ "$arg3" != "true" && "$arg3" != "false" ]]; then
	echo "run argument must be true or false -- $arg3 is invalid"
	exit -1
fi

arg1=$(echo "$arg1" | awk '{print toupper(substr($0,1,1)) substr($0,2)}')

if [[ "$arg2" == "true" ]]; then
	rm -rf "$root_dir/bin/microslop" "$root_dir/bin/macos"
fi

mkdir -p "$root_dir/bin/microslop" || exit -1
pushd "$root_dir/bin/microslop" >/dev/null || exit -1
mkdir -p incl
cp "$arg4" incl/ || exit -1
cp "$arg5" portaudio_x64.lib || exit -1
cmake -DCMAKE_TOOLCHAIN_FILE="$root_dir/mingw.cmake" \
	-DCMAKE_BUILD_TYPE="$arg1" \
	-DPORTAUDIO_INCLDIR="$root_dir/bin/microslop/incl/" \
	-DPORTAUDIO_LIBDIR="$root_dir/bin/microslop/" \
	"$root_dir" || exit -1
mkdir -p CMakeFiles/FuncDoodle.dir/src \
	CMakeFiles/FuncDoodle.dir/lib/glad/src \
	CMakeFiles/FuncDoodle.dir/lib/imgui \
	CMakeFiles/FuncDoodle.dir/lib/imgui/backends \
	CMakeFiles/FuncDoodle.dir/lib/nfd/src || exit -1
jobs=$(( ($(nproc) + 2) / 3 ))
cmake --build . -j"$jobs" || exit -1
cp -r "$root_dir/assets" . || exit -1
cp -r "$root_dir/themes" . || exit -1
if [[ "$arg3" == "true" ]]; then
	wine ./FuncDoodle.exe || exit -1
fi
popd >/dev/null || exit -1

mac_pa_incl="$arg6"
mac_pa_lib_x86="$arg7"
mac_pa_lib_arm="$arg8"

if [[ -f "$mac_pa_incl" ]]; then
	mac_pa_incl="$(dirname "$mac_pa_incl")"
fi
if [[ -f "$mac_pa_lib_x86" ]]; then
	mac_pa_lib_x86="$(dirname "$mac_pa_lib_x86")"
fi
if [[ -f "$mac_pa_lib_arm" ]]; then
	mac_pa_lib_arm="$(dirname "$mac_pa_lib_arm")"
fi
if [[ -n "$mac_pa_lib_x86" ]]; then
	mac_pa_lib_x86="${mac_pa_lib_x86%/}/"
fi
if [[ -n "$mac_pa_lib_arm" ]]; then
	mac_pa_lib_arm="${mac_pa_lib_arm%/}/"
fi

if [[ -n "$mac_pa_incl" && -n "$mac_pa_lib_x86" && -n "$mac_pa_lib_arm" ]]; then
	build_macos() {
		local arch="$1"
		local outdir="$2"
		local libdir="$3"

		mkdir -p "$outdir" || exit -1
		pushd "$outdir" >/dev/null || exit -1
		cmake -DCMAKE_TOOLCHAIN_FILE="$root_dir/macos.cmake" \
			-DCMAKE_BUILD_TYPE="$arg1" \
			-DCMAKE_OSX_ARCHITECTURES="$arch" \
			-DPORTAUDIO_INCLDIR="$mac_pa_incl" \
			-DPORTAUDIO_LIBDIR="$libdir" \
			"$root_dir" || exit -1
		mkdir -p CMakeFiles/FuncDoodle.dir/src \
			CMakeFiles/FuncDoodle.dir/lib/glad/src \
			CMakeFiles/FuncDoodle.dir/lib/imgui \
			CMakeFiles/FuncDoodle.dir/lib/imgui/backends \
			CMakeFiles/FuncDoodle.dir/lib/nfd/src || exit -1
		jobs=$(( ($(nproc) + 2) / 3 ))
		cmake --build . -j"$jobs" || exit -1
		cp -r "$root_dir/assets" . || exit -1
		cp -r "$root_dir/themes" . || exit -1
		if [[ "$arg3" == "true" && "$(uname -s)" == "Darwin" ]]; then
			./FuncDoodle || exit -1
		fi
		popd >/dev/null || exit -1
	}

	build_macos "x86_64" "$root_dir/bin/macos" "$mac_pa_lib_x86"
	build_macos "arm64" "$root_dir/bin/macos-arm64" "$mac_pa_lib_arm"
else
	echo "macOS PortAudio paths not provided. Skipping macOS build."
fi

"$root_dir/scripts/build.sh" "$arg1" true "$arg2" "$arg3" || exit -1
rm -rf "$root_dir/bin/linux-tiling"
cp -r "$root_dir/bin/linux" "$root_dir/bin/linux-tiling"
"$root_dir/scripts/build.sh" "$arg1" false false "$arg3" || exit -1
