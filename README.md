# Dagger

A fully-featured, modern game engine made for educational purposes.

## Features

- Dagger is data-driven and event-based.
- Dagger is extremely modular.
- Dagger is clear and clearly educational.

## Installation

You can clone and fully set up this project by simply running:

```git
  git clone https://github.com/igorkandic/UbiBelPMF_2024.git
```

### Conan 
Conan is used for dependecies
```shell
sudo pip3 install conan
```
### Conan simpleini
Official conan repository doesn't have [simpleini](https://github.com/brofield/simpleini).
##### Building simpleini
```shell
cd deps
conan create .
```

### Build from terminal
```shell
  mkdir build
  cd build
  conan install .. --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True
  cd ..
  cmake --preset conan-release
  cd build/Release
  make
```

### Using CLion
#### CLion conan extension:
[Download extension](https://www.jetbrains.com/help/clion/conan-plugin.html)

## Resources

Any kind of data used by engine that is not source code is considered a resource. Root directory for resources is `data\`
and so all references to resoruces begin there. For example, if you want to get a texture (from `data\textures`) you would
use `textures\mytexture.png` instead `data\textures\mytexture.png`.

## Rights and Reservations

Dagger is made as a part of Ubisoft Belgrade's game development course. All rights reserved.
