# FuncDoodle

## building
To build, run build.sh or build.bat depending on your OS, after that if everything worked correctly, the built exec should be in bin/.
BTW for unix a zsh completion script is available: 
```sh
source completion.zsh
```

## NOTE:
I haven't tested this on windows, i mean I have, but don't have the chance to very often.
So if you're trying to build on windows, just know that it MAY or MAY NOT actually work. 

## Cross-compiling
Right now, you can cross compile using mingw32-w64 on macos/linux to windows... 
I haven't really created any build script for it though... yet...
so you'll just have to specify everything manually... sorry.

Just `mkdir bin` if it doesn't exist, cd into `bin`, and 
```sh
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=../mingw.cmake
```
and then
```sh
make
```
and copy over assets:
```sh
cp -r ../assets/ ./assets
```
note that due to how that toolchain file is setup atm, you'll have to `mkdir incl` inside bin and put the portaudio.h inside `incl/` but once you've done that and acquired a portaudio-2.dll ALSO built with mingw, it will work.
Though you'll have to make a symlink portaudio_x64.lib to this dll, or just rename this dll to portaudio_x64.lib...
Because it's using a dll though, you'll obviously have to keep the dll in the same directory as the executable. This cross-compiling setup WILL be improved, it's just very terrible right now
