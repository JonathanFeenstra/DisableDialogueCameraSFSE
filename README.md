# FirstPersonDialogueSFSE

SFSE plugin that automatically switches to first person when the dialogue menu opens (work in progress).

The project structure is copied and modified from [SmartAimingSFSE by powerofthree](https://github.com/powerof3/SmartAimingSFSE), which was released under the [MIT license](LICENSES/SmartAimingSFSE).

## Requirements
* [CMake](https://cmake.org/)
	* Add this to your `PATH`
* [PowerShell](https://github.com/PowerShell/PowerShell/releases/latest)
* [Vcpkg](https://github.com/microsoft/vcpkg)
	* Add the environment variable `VCPKG_ROOT` with the value as the path to the folder containing vcpkg
* [Visual Studio Community 2022](https://visualstudio.microsoft.com/)
	* Desktop development with C++
* [CommonLibSF](https://github.com/Starfield-Reverse-Engineering/CommonLibSF)
	* Add this as as an environment variable `CommonLibSFPath`

## Register Visual Studio as a Generator
* Open `x64 Native Tools Command Prompt`
* Run `cmake`
* Close the cmd window

## Building
```
git clone https://github.com/JonathanFeenstra/FirstPersonDialogueSFSE.git
cd FirstPersonDialogueSFSE
cmake --preset vs2022-windows-vcpkg-sf
cmake --build build --config Release
```

## License
[GPL-3.0-or-later](COPYING) WITH [Modding Exception AND GPL-3.0 Linking Exception (with Corresponding Source)](EXCEPTIONS).
