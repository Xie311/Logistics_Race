# Detect ball based on color
Created time: 2024-3-25

Version 2.0 (Want the original verison? Ask SSC)

Editor Liam

## File tree
```
├── build
├── CMakeFiles
├── CMakeLists.txt
├── main.cpp
├── README.md
├── shell.sh
├── test.cpp
└── ximgproc
```

## Usage
```
cd build
cmake ..
make
```

## Known bug

The function `detectEllipse()` will cause the whole program to crash if there is no ball in the video stream. However the stability of this function has been tested when there are always balls being captured by camera.
