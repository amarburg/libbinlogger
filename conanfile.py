from conans import ConanFile, CMake

class LibLoggerConan(ConanFile):
  name = "liblogger"
  version = "0.1"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake"
  exports = "*"
  options = {"opencv_dir": "ANY","shared": [True, False], "build_parallel": [True, False]}
  default_options = "opencv_dir=''", "shared=True", "build_parallel=True"

  requires = "TCLAP/master@jmmut/testing", \
              "snappy/1.1.3@hoxnox/testing", \
              "zlib/1.2.8@lasote/stable", \
              "libactive_object/master@amarburg/testing", \
              "g3log/master@amarburg/testing"

  def config(self):
    if self.scope.dev and self.scope.build_tests:
      self.requires( "gtest/1.8.0@lasote/stable" )
      self.options["gtest"].shared = False

  def imports(self):
    self.copy("*.dll", dst="bin", src="bin") # From bin to bin
    self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

  def build(self):
    cmake = CMake(self.settings)
    cmake_opts = ""

    cmake_opts += "-DOpenCV_DIR=%s " % (self.options.opencv_dir) if self.options.opencv_dir else ""
    cmake_opts += "-DBUILD_SHARED_LIBS=True " if self.options.shared else ""
    cmake_opts += "-DBUILD_UNIT_TESTS=1" if self.scope.dev and self.scope.build_tests else ""

    build_opts = "-j" if self.options.build_parallel else ""

    self.run('cmake "%s" %s %s' % (self.conanfile_directory, cmake.command_line, cmake_opts))
    self.run('cmake --build . %s -- %s' % (cmake.build_config, build_opts))
    if self.scope.dev and self.scope.build_tests:
      self.run('make unit_test')

  def package(self):
    self.copy("*.h", dst="")
    if self.options.shared:
      if self.settings.os == "Macos":
          self.copy(pattern="*.dylib", dst="lib", keep_path=False)
      else:
          self.copy(pattern="*.so*", dst="lib", keep_path=False)
    else:
       self.copy(pattern="*.a", dst="lib", src="lib", keep_path=False)

  def package_info(self):
      self.cpp_info.libs = ["logger"]
