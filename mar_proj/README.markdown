### Overview
This is mar_proj

### Authors
See AUTHORS

### Installation
Follow these steps to build the project:
    cd mar_proj
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

### Help and more...
Visit [my website](http://www.coderbasement.com)
