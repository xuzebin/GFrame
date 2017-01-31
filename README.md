# GFrame [![Build Status](https://travis-ci.org/xuzebin/GFrame.svg?branch=master)](https://travis-ci.org/xuzebin/GFrame)
A tiny C++ OpenGL framework.

## Build
#### Compile all the examples and the framework code.

```
cd GFrame
mkdir build
cd build
cmake ..
make
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

## Build as an Xcode project

#### Generate an Xcode project:

```
cd GFrame
mkdir build
cd build
cmake .. -G Xcode
```

#### Build the project in Xcode.
1. Run `ALL_BUILD`
2. Run `install`
3. Run examples

## TODO
1. Add support for different platforms. Currently only support Mac OSX.
