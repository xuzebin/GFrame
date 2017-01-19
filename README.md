# GFrame
A tiny C++ OpenGL framework.

## Compile & Run
### Compile all the examples and the framework code.

```
cd GFrame
mkdir build
cd build
cmake ..
make
```

### Run
Run the MainExample:

```
./build/examples/MainExample
```

Run the TexturedModel example:

```        
./build/examples/TexturedModel/TexturedModel
```

## Build a Xcode project

1. Generate Xcode project files:

```
cd GFrame
mkdir build
cd build
cmake .. -G Xcode
```

2. Copy `assets/` and `shaders/` into the executable directory (build/examples/Debug/).

3. Build and run the project in Xcode.
