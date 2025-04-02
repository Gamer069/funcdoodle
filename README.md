# FuncDoodle

## building
To build, run scripts/build.sh or scripts/build.bat depending on your OS, after that if everything worked correctly, the built exec should be in bin/. (for scripts/build.sh, usage is shown if ran with no arguments)
BTW for unix a zsh completion script is available: 
```sh
source scripts/completion/completion.zsh
```

## NOTE:
I haven't tested this on windows, i mean I have, but don't have the chance to very often.
So if you're trying to build on windows, just know that it MAY or MAY NOT actually work. 

## Cross-compiling
You can cross compile using mingw32-w64 on macos/linux to windows... 
use 
```sh
source scripts/completion/completion_cross.zsh
```
for completion, and run scrippts/build_cross.sh. (usage is shown if ran with no arguments)
