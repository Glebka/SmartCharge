SET PATH=%~dp0\tools\ninja;%PATH%
SET BOARD=%1

RMDIR build /S /Q
MKDIR build

CD build

cmake -D CMAKE_TOOLCHAIN_FILE=cmake/Arduino-toolchain.cmake -DARDUINO_BOARD_OPTIONS_FILE=boards/%BOARD%.cmake -G"Ninja" ..
cmake --build .