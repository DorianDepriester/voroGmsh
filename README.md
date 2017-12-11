# voroGmsh
Class for writing geometry files for Gmsh from voro++

## Purpose
This project aims to convert data from Voro++ to Gmsh-readable files (.geo). Then, the volumes can be meshed thanks to Gmsh.

## Requirements
### Voro++
See [here](http://math.lbl.gov/voro++/download/) for how to install the Voro++ library.

### AdjacencyMatrix
The present class needs the [AdjacencyMatrix](https://github.com/DorianDepriester/adjacencyMatrix) I have developped.

### Gmsh
Since the class is intended to generate files for Gmsh, you may [install it](http://gmsh.info/) on your computer.

## Installation
### Import the required library into your project
Depending on the path to the the voroGmsh files, you may need something like this:
```C++
#include "voroGmsh.git/trunk/vorogmsh.h"
```
### Check the path to AdjacencyMatrix
In both vorogmsh.cpp and vorogmsh.h, check the path for the AdjacencyMatrix library (see above). E.g.:
```C++
#include "adjacencyMatrix.git/trunk/adjacencyMatrix.h"
```

## How to use it?
### Create an instance of vorogmsh
Once you have defined the Voro++ container and filled it with particles, you can create an object of class vorogmsh:
````C++
vorogmsh gmsh(con);
```
where con is the name of the container. This object gmsh contains all data needed to create a 3D geometry in Gmsh.

### Dump the geometry into a text file
Then, you can write an ASCII file defining the whole geometry:
```C++
gmsh.saveasgeo("Geometry.geo");
```

The saveasgeo method automatically computes the maximum element size at each vertex. You can override this value by passing the nominal size as an optional parameter. E.g.:
````C++
gmsh.saveasgeo("Regis_cemen.geo",0.5);
```
Note that if the nominal value is smaller than the computed size, the latter will be used instead.
