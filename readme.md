REINDEER
======

A 3D Engine.


Building
--------

- Install glew library
- Get the bullet3 and glm source code and extract them, it's recommended to extract them in same directory where the reindeer source dir exists.
- Execute this in a terminal emulator:
```
$ cd <reindeer_path>
$ mkdir build
```
- If the bullet3 and glm source code are in the recommended path:
```
$ cmake ..
```
- If the bullet3 or glm source code in custom path set it with -DBULLET_SRC_DIR or -DGLM_SRC_DIR. Example:
```
$ cmake -DBULLET_SRC_DIR=<bullet_custom_path> ..
```
- Then make it:
```
$ make -j 4
```


Configuration
-------

Some configurations like resolution etc are still currently hard-coded in src/core/config.cpp file. Sorry.


Examples
--------
It's included in examples directory and automatically built with project. Execute it from the build directory with:
```
./reindeer-example
````
[example video](https://youtu.be/WOElID2kxpA)


To do
------
- port direct3d 11
- lighting
- texture
- shadow
- fog
- animation
- editor, cool examples: unity3d, paradox3d


Notes
------
- This is a good sample of game engine goal: [what-are-the-design-goals-of-the-turbulenz-engine](https://github.com/turbulenz/turbulenz_engine#what-are-the-design-goals-of-the-turbulenz-engine)

