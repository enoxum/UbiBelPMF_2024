from conan import ConanFile

import os

from conan.tools.cmake import cmake_layout
from conan.tools.files import copy
from conan.tools.gnu import PkgConfigDeps
from conan.tools.meson import Meson, MesonToolchain

class DaggerRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeToolchain", "CMakeDeps"

    def requirements(self):
        if(self.settings.os == "Linux"):
            self.requires("onetbb/2021.10.0")
        self.requires("fmt/7.1.3")
        self.requires("glad/0.1.34")
        self.requires("glfw/3.4")
        self.requires("glm/cci.20230113")
        self.requires("imgui/1.85")
        self.requires("nlohmann_json/3.11.3")
        self.requires("spdlog/1.8.5")
        self.requires("stb/cci.20230920")
        self.requires("tsl-sparse-map/0.6.2")
        self.requires("simpleini/4.22")
        self.requires("entt/3.13.0")
        self.requires("miniaudio/0.11.21")

    def configure(self):
        self.options['glad'].shared = False
        self.options['glad'].fPIC = True
        self.options['glad'].no_loader = False
        self.options['glad'].spec = 'gl'
        self.options['glad'].gl_profile = 'compatibility'
        self.options['glad'].gl_version = '3.3'

    def imports(self):
        self.copy('*.so*', src='lib', dst='bin')

    def build_requirements(self):
        self.tool_requires("cmake/3.28.1")

    def layout(self):
        cmake_layout(self)

    def generate(self):
        copy(self, "*glfw*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "bindings"))
        copy(self, "*opengl3*", os.path.join(self.dependencies["imgui"].package_folder,
            "res", "bindings"), os.path.join(self.source_folder, "bindings"))

