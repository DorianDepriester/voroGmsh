#ifndef VOROGMSH_H
#define VOROGMSH_H

#include <vector>
#include <voro++.hh>
#include "adjacencyMatrix.git/trunk/adjacencyMatrix.h"


class vorogmsh {
public:
    vorogmsh(voro::container_poly&);
    /**
    *   Constructor.
    *   Computes the Voronoi cells and store the data (vertices, edges, faces and cells).
    */
    virtual ~vorogmsh();
    std::vector<double> Getvtx() {
    /**
    *   Returns the x coordinates of the vertices.
    */
        return vtx_;
    }
    std::vector<double> Getvty() {
    /**
    *   Returns the y coordinates of the vertices.
    */
        return vty_;
    }
    std::vector<double> Getvtz() {
    /**
    *   Returns the z coordinates of the vertices.
    */
        return vtz_;
    }
    adjacencyMatrix Getedges() {
    /**
    *   Returns the adjacency matrix related to the vertices (edges)
    */
        return adj_mat_vtx_;
    }
    adjacencyMatrix Getfaces() {
    /**
    *   Returns the adjacency matrix related to the cells (faces)
    */
        return adj_mat_cel_;
    }
    adjacencyMatrix GetfacesAdjacency() {
    /**
    *   Returns a vector containing the lists of edges defining each face
    */
        return adj_mat_vtx_;
    }
    std::vector<std::vector<int> > Getvolumes() {
    /**
    *   Returns a vector containing the lists of faces defining each cell.
    */
        return volumes_;
    }
    void saveasgeo(const char *,double eSize=0);
    /**
    *   Saves the geometry as a gmsh file (.geo).
    */
    vector<double> edgeLengths();
    /**
    *   Returns the edge lenghts.
    */

    vector<double> computeElmtSize();
    /**
    *   Computes the local element size
    */

private:
    std::vector<double> vtx_;
    std::vector<double> vty_;
    std::vector<double> vtz_;
    std::vector<std::vector<int> > faces_;
    std::vector<std::vector<int> > volumes_;
    std::vector<int> bounds_[6];
    adjacencyMatrix adj_mat_vtx_;
    adjacencyMatrix adj_mat_cel_;

    bool vtxExists(double,double,double,int&);
    std::vector<int> addVtx(std::vector<double>,int&);
    void add_boundFace(int,int);
    void addFace(vector<int>,int,vector<int>);
};

#endif // VOROGMSH_H
