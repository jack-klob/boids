# Flocking Simulator
Flocking simulation based on the [boids](https://www.red3d.com/cwr/boids/) algorithm by Craig Reynolds. Completed for SENG 475 - Advanced Programming Techniques
for Robust Efficient Computing at the University of Victoria.

## Installation
Let $INSTALL_DIR denote the directory into
which the software is to be installed.
The following are the commands to compile and install the application. They must be executed within the top level project directory
```
cmake -H. -Btmp_cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
```
```
cmake --build tmp_cmake --clean-first --target install
```

## Running application
The following command is used to run the application
```
$INSTALL_DIR/bin/flock
```

A help screen can be displayed with
```
$INSTALL_DIR/bin/flock --help
```

A demo script can be run with
```
$INSTALL_DIR/bin/demo
```
