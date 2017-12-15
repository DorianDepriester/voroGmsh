# voroGmsh
Class for writing geometry files for Gmsh from voro++

[![https://zenodo.org/badge/89606446.svg](https://zenodo.org/badge/89606446.svg)](https://zenodo.org/badge/latestdoi/89606446)

## Purpose
This class is intended to export the geometry of Voronoï cells computed using the Voro++ library as a Gmsh-readable file.

## Requirements
### Voro++
See [here](http://math.lbl.gov/voro++/download/) for how to install the Voro++ library.

### AdjacencyMatrix
The present class needs the [AdjacencyMatrix](https://github.com/DorianDepriester/adjacencyMatrix) I have developped.

### Gmsh
Since the class is intended to generate files for Gmsh, you may [install it](http://gmsh.info/) on your computer.

## Installation
### Import the required library into your project
Depending on the path to the voroGmsh files, you may need something like this:
```C++
#include "voroGmsh.git/trunk/vorogmsh.h"
```
### Check the path to AdjacencyMatrix
In both vorogmsh.cpp and vorogmsh.h, check the path for the `AdjacencyMatrix` library (see above). E.g.:
```C++
#include "adjacencyMatrix.git/trunk/adjacencyMatrix.h"
```

## How to use it?
### Create an instance of vorogmsh
Once you have defined the Voro++ container and filled it with particles, you can create an object of class `vorogmsh`:
```C++
vorogmsh gmsh(con);
```
where `con` is the name of the container. The object `gmsh` contains all the data needed to create a 3D geometry in Gmsh.

### Dump the geometry into a text file
Then, you can write an ASCII file defining the whole geometry:
```C++
gmsh.saveasgeo("Geometry.geo");
```

The `saveasgeo` method automatically computes the maximum element size at each vertex (based on the edge lengths). You can override this value by passing the nominal size as an optional parameter. E.g.:
```C++
gmsh.saveasgeo("Geometry.geo",0.5);
```
Note that if the nominal size is smaller than the computed one, the latter will be used instead.

## Example
As an example, the [`pack_six_cube_poly` dataset](http://math.lbl.gov/voro++/examples/radical/) (provided in Voro++ examples) has been used. Figure below illustrates the meshes with default mesh size (left) and nominal size of 0.1 (right).

<img src="https://raw.githubusercontent.com/DorianDepriester/voroGmsh/master/Examples/pack_six_cube_poly.png" width="300"><img src="https://raw.githubusercontent.com/DorianDepriester/voroGmsh/master/Examples/pack_six_cube_poly_01.png" width="300">

## Note
I am not a programmer. Thus, this class is probably not optimized in terms of performances or flexibility. Do not hesitate to fork this project in order to improve it.
