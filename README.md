# d3d9_chainload

Proxy `d3d9.dll` for auto-loading libraries during program startup.

## Usage

1. Copy `d3d9.dll` and `chainload.txt` to the directory containing the program executable.
2. Add a line to the `chainload.txt` file containing a path to a library.
3. Launch the game.

## Requirements

* [Microsoft Visual C++ Redistributable for Visual Studio 2019 (64-bit)](https://aka.ms/vs/16/release/vc_redist.x64.exe)

# More information

* [Dynamic-Link Library Search Order](https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order)

*Forked from https://github.com/emskye96/infinitas_chainload*