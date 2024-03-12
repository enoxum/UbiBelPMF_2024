# Setting up development environment on Ubuntu 20.04

### Install needed system commands and dependencies:

```sh
sudo apt install git python3 python3-pip python3-setuptools python3-wheel ninja-build libglfw3-dev cmake libfmt-dev libtbb-dev clang
```

### Clone repository
```sh
git clone --recursive https://github.com/enoxum/UbiBelPMF_2022.git
```

### Lets grab ourselves the latest version of meson:

```sh
sudo pip3 install meson
```

### Grab an IDE:

```
sudo snap install --classic code
```

### Install C++ and Meson extensions for our IDE:

```sh
code --install-extension ms-vscode.cpptools
code --install-extension asabil.meson
```

### Open dagger folder in VS Code, press F5, and profit!