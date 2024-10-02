# PP Snake

## Linux
1. Make sure to install all build dependencies:
```shell
sudo apt install build-essential cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev -y
```
2. Run CMake: `cmake -B build .`
3. Change to the build directory: `cd build`
4. Run Make: `make`
5. Run the Game: `./pp_snake`

## Windows (MSVC)
1. Make sure to install all build dependencies (if not using WSL):
```shell
winget install cmake 
wget https://aka.ms/vs/17/release/vs_buildtools.exe -O vs_buildtools.exe 
./vs_buildtools.exe --wait --passive --norestart --nocache --add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --includeOptional
```
2. Run CMake: `cmake --build build .`
3. Build: `cmake --build build --config Debug` or `cmake --build build --config Release`
4. Run the Game: `./build/Debug/pp_snake.exe` or `./build/Release/pp_snake.exe`