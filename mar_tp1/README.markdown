### Overview
This is MAR partical work 1

### Authors
See AUTHORS

### Installation
Follow these steps to build the project:
    cd mar_tp1
    mkdir build
    cd build
    cmake ..
    make

Remember that you can enable the build of tests using the following:
    make .. -DBUILD_TESTS=ON

The tests can be executed using:
    make test

To enable the doxygen documentation, use the following:
    cmake .. -DBUILD_DOC=ON

Then you can generate the documentation using:
    make doc

### Frequently Asked Questions

- **CMake complains because of 2.8 version required.**
  FILE( COPY ) was introduced in CMake 2.8. It allows CMake to copy a directory
  and its content in a platform-independant manner. If you have CMake <2.8 (like
  Debian ships with 2.6), then just delete the first CMAKE_MINIMUM_REQUIRED line
  along with the FILE( COPY ). You will then need to manually copy the data/ dir
  to the binary directory.

- **CMake cannot find GLFW (i.e. GLFW_LIBRARIES NOT FOUND and so on)**
  FindGLFW.cmake is just a hint to help CMake find the GLFW library. If you
  installed it in a non-standard path (or if you're under windows) just manually
  edit the CMakeCache.txt file, and set GLFWDIR/GLFW_INCLUDE_DIR/GLFW_LIBRARIES
  (you can also use the GUI instead of editing CMakeCache.txt).
  An alternative way is to edit the FindGLFW.cmake file to include your preferred
  search directories.

- **Crash & "_Failed to select a GLFWFBConfig from the alternatives ..._" in console**
  Thank you Microsoft for providing outdated drivers. Brand new GC drivers should
  fix this.

- **MVC complains: "_fatal error C1900: Il mismatch between 'P1' version '...' and 'P2' version '...'_"**
  This might happen if GLFW developpers still don't know how to setup MSVC. On some releases they
  activated LTCG (link time code generation). This basically means that the front end process all
  input files before performing any code generation. This allow "code optimization" at link time,
  but when building a library, this means that you are shipping decorated ASTs instead of object
  code. Guess what, the MSVC AST format change from time to time... Just stick with the debug GLFW
  library, it should not have been built with LTCG. Or build GLFW yourself.