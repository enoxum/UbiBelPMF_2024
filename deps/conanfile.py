from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
from conan.tools.scm import Git
from conan.tools.files import copy

import os


class simpleiniRecipe(ConanFile):
    name = "simpleini"
    version = "4.22"

    # Optional metadata
    license = "MIT"
    author = "Brodie Thiesfield"
    url = "https://github.com/brofield/simpleini"
    description = "Cross-platform C++ library providing a simple API to read and write INI-style configuration files"
    topics = ("conan", "ini", "c")

    exports_sources = "include/*"
    no_copy_source = True

    def source(self):
        git = Git(self)
        git.clone(url="https://github.com/brofield/simpleini.git", target=".")

    def package(self):
        copy(self, "*.h", self.source_folder, os.path.join(self.package_folder, "include"))

    def package_info(self):
        self.cpp_info.bindirs = []
        self.cpp_info.libdirs = []
        