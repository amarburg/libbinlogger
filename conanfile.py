from conans import ConanFile, CMake

class LibActiveObjectConan(ConanFile):
  name = "liblogger"
  version = "0.1"
  settings = "os", "compiler", "build_type", "arch"
  generators = "cmake"
  requires = "TCLAP/master@jmmut/testing", \
              "snappy/1.1.3@hoxnox/testing", \
              "zlib/1.2.8@lasote/stable", \
              "libactive_object/0.1@amarburg/testing", \
              "g3log/0.1@amarburg/testing"

  def config(self):
    if self.scope.dev and self.scope.build_tests:
      self.requires( "gtest/1.8.0@lasote/stable" )
      self.options["gtest"].shared = False

  def imports(self):
    self.copy("*.dll", dst="bin", src="bin") # From bin to bin
    self.copy("*.dylib*", dst="bin", src="lib") # From lib to bin

  def build(self):
    cmake = CMake(self.settings)
    flag_build_tests = "-DBUILD_UNIT_TESTS=1" if self.scope.dev and self.scope.build_tests else ""
    self.run('cmake "%s" %s %s' % (self.conanfile_directory, cmake.command_line, flag_build_tests))
    self.run('cmake --build . %s' % cmake.build_config)
    if self.scope.dev and self.scope.build_tests:
      self.run('make unit_test')
