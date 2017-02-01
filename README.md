# GFrame [![Build Status](https://travis-ci.org/xuzebin/GFrame.svg?branch=master)](https://travis-ci.org/xuzebin/GFrame)
A tiny C++ OpenGL framework.

## Build a standalone project
#### Compile all the examples and the framework code.

```
mkdir -p build
cd build
cmake ..
make
```

or directly run the build script:

```
source buildStandalone.sh
```

#### Run
Run the MainExample:

```
./build/examples/MainExample
```

Run the TexturedModel example:

```        
./build/examples/TexturedModel/TexturedModel
```

## Build an Xcode project 

```
mkdir -p build
cd build
cmake .. -G Xcode
xcodebuild -target ALL_BUILD -configuration Debug
xcodebuild -target install -configuration Debug    
```

or directly run the build script:

```
source buildXcode.sh
```        

## TODO
1. Add support for different platforms. Currently only support Mac OSX.
