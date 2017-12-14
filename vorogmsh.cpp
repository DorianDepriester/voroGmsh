#include "vorogmsh.h"
#include <voro++.hh>
#include <string>
#include <iostream>
using namespace std;
using namespace voro;

#include "adjacencyMatrix.git/trunk/adjacencyMatrix.h"

double distance(double x1,double y1, double z1,double x2,double y2, double z2){
/**
 * Euclidean distance between two points
 */
    return sqrt(pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2));
}

bool vorogmsh::vtxExists(double x,double y,double z,int& id) {
/**
 * Checks if the point with coordinates (x,y,z) already exists in the list of vertices.
 * Returns the index of the first duplicate if any; returns -1 otherwise.
 */
    double eps=1e-6;    // Tolerance for merging two points
    int i=0;
    int nref=vtx_.size();
    bool dupli=false;
    while( (!dupli) & (i<nref)) {
        dupli= distance(x,y,z,vtx_[i],vty_[i],vtz_[i]) < eps;
        i++;
    }
    if(dupli) {
        id=i-1;  // Duplicate found
    }
    return dupli;
}

vector<int> vorogmsh::addVtx(vector<double> v, int& idv) {
/**
 * Adds a new entry to the list of vertices and returns a table giving the global indices depending on local indices (cell-related).
 */
    int n_vtx=v.size()/3;
    vector<int> loc2glo;                // Table giving the global index depending on the local index
    for(int k=0; k<n_vtx; k++) {             // Loop over all the vertices of the considered cell
        double xk=v[3*k];
        double yk=v[3*k+1];
        double zk=v[3*k+2];
        int id_dupli;
        if(vtxExists(xk,yk,zk,id_dupli)) {
            loc2glo.push_back(id_dupli);
        } else {
            vtx_.push_back(xk);
            vty_.push_back(yk);
            vtz_.push_back(zk);
            loc2glo.push_back(idv);
            idv++;
        }
    }
    return loc2glo;
}

void vorogmsh::addFace(vector<int> f_vert,int j,vector<int> loc2glo) {
/**
 *
 */
    int n=f_vert[j];        // Number of vertices defining the considered face
    vector<int> facek;
    for(int k=0; k<n; k++) {// Loop over the edges of the face
        int pt1=f_vert[j+k+1],pt2;  // 1st point of the edge
        if(k+1<n) {                 // 2nd point of the edge
            pt2=f_vert[j+k+2];      // Next index
        } else {
            pt2=f_vert[j+1];        // Closed loop: look back to the 1st vertex
        }
        pt1=loc2glo[pt1];   // Converts local (cell) to global indices
        pt2=loc2glo[pt2];

        int edge_id=adj_mat_vtx_.add(pt1,pt2);  // Pick up the edge ID from the adjacency matrix
        facek.push_back(edge_id);               // Add the edge ID to the list of edges defining the face
    }
    faces_.push_back(facek);           // Add the face ID to the list of faces
}

void vorogmsh::add_boundFace(int nid,int idf) {
/**
 * Adds the face ID to the lists of bounding faces (container boundary).
 */
    if( (nid>=-6) & (nid<=-1) ){
        int bid=-nid-1; // Convert range [-1 -6] to [0 5]
        bounds_[bid].push_back(idf);
    }
}

vorogmsh::vorogmsh(container_poly& con) {
/**
 * Constructor.
 * Compute all the cells and extract:
 *  -the vertices coordinates
 *  -the lists of vertex indices defining the faces
 *  -the vertices' adjacency matrix
 *  -the lists of face indices defining the cells
 *  -the lists of face indices defining the container boundaries
 */

    c_loop_all cls(con);
    voronoicell_neighbor c;
    int idv=0,idc;  // vertex, face and cell indices
    double xp,yp,zp,rp;
    vector<int> f_vert;

    if(cls.start()) do if(con.compute_cell(c,cls)) {
                cls.pos(idc,xp,yp,zp,rp);
                ID_.push_back(idc);
                vector<int> neigh;
                vector<int> voli;
                vector<double> v;
                c.neighbors(neigh);
                c.vertices(xp,yp,zp,v);
                c.face_vertices(f_vert);
                vector<int> loc2glo=addVtx(v,idv);
                for(unsigned int i=0,j=0; i<neigh.size(); i++) {
                    int face_id;                    // Face ID
                    bool face_exists;
                    int neighi=neigh[i];            // ID of the neighbor cell
                    if(neighi<0) {
                        face_exists=false;          // Volume boundary
                    } else {
                        face_exists=adj_mat_cel_.get(neighi,idc,face_id); // Checks in the adjecency matrix if the face already exists
                    }
                    if (face_exists) {
                        voli.push_back(face_id);            // The face already exists
                    } else {
                        int idf=adj_mat_cel_.add(neighi,idc);   // New entry in the cell adjacency matrix
                        addFace(f_vert,j,loc2glo);
                        voli.push_back(idf);
                        if(neighi<0) {
                            add_boundFace(neighi,idf);      // Add the face to the list of faces bounding the considered cell
                        }
                    }
                    j+=f_vert[j]+1;         // Go to the first vertex of the next face
                }
                volumes_.push_back(voli);  // Add the volume to the list of volumes (cells)
            }
        while (cls.inc());
}


vector<double> vorogmsh::computeElmtSize() {
    vector<double> minSize(vtx_.size());
    vector<double> lengths=this->edgeLengths(); // List of edge lengths
    for(int i=0; i<adj_mat_vtx_.size(); i++) {
        for(int j=0;j<i;j++){
            int edge_id;
            if(adj_mat_vtx_.get(i,j,edge_id)){
                edge_id=abs(edge_id);   // ID of the corresponding edge
                double dist=lengths.at(edge_id-1);
                if(minSize.at(i)==0){   // Local size not computed yet
                    minSize.at(i)=dist;
                } else {                // Keep the lowest value
                    minSize.at(i)=min(dist,minSize.at(i));
                }
                if(minSize.at(j)==0){   // Local size not computed yet
                    minSize.at(j)=dist;
                } else {                // Keep the lowest value
                    minSize.at(j)=min(dist,minSize.at(j));
                }
            }
        }
    }
    return minSize;
}

void formatSequence(FILE *fp,const char *seq_name,vector<int> vect,const char* entity_name) {
/**
 * Formats a named sequence and dumps it as follows:
 *      seq_name(entity_name)={0,1,5,7};
 */
    int n=vect.size();
    fprintf(fp,"%s(%s)={",seq_name,entity_name);
    for(int i=0; i<n; i++) {
        fprintf(fp,"%d",vect[i]);
        if (i<n-1) {
            fprintf(fp,",");
        }
    }
    fprintf(fp,"};\n");
}

void writeSequence(FILE *fp,const char *seq_name,vector<int> vect,int i) {
/**
 * Dumps a named sequence of IDs, separated by a comma.
 */
    char id_str[10];
    sprintf(id_str,"%d",i);
    formatSequence(fp,seq_name,vect,id_str);
}


void vorogmsh::saveasgeo(const char * filename,double nomiSize) {
/**
 * Exports the geometry as a gmsh file (.geo).
 * \param[in] Path to the file to write to
 * \param[in] Nominal element size (optional)
 */
    FILE *fp=fopen(filename,"w");
    vector<double> minSize=this->computeElmtSize();

    // Vertices
    for(unsigned int i=0; i<vtx_.size(); i++) {
        double lSize;
        if(nomiSize<=0){
            lSize=minSize[i];
        } else {
            lSize=min(minSize[i],nomiSize);
        }
        fprintf(fp,"Point(%d)={%g,%g,%g,%g};\n",i,vtx_[i],vty_[i],vtz_[i],lSize);
    }

    // Edges
    for(int i=0; i<adj_mat_vtx_.size(); i++) {
        for(int j=0;j<i;j++){
            int edge_id;
            if(adj_mat_vtx_.get(i,j,edge_id)){
                if(edge_id<0){   // Avoid negative index
                    fprintf(fp,"Line(%d)={%d,%d};\n",-edge_id,j,i);
                } else {
                    fprintf(fp,"Line(%d)={%d,%d};\n",edge_id,i,j);
                }
            }
        }
    }

   // Faces
    for(unsigned int i=0; i<faces_.size(); i++) {
        writeSequence(fp,"Line Loop",faces_[i],i+1);
        fprintf(fp,"Plane Surface(%d)={%d};\n",i+1,i+1);
    }

   // Volumes
    for(unsigned int i=0; i<volumes_.size(); i++) {
        writeSequence(fp,"Surface Loop",volumes_[i],i);
        fprintf(fp,"Volume(%d)={%d};\n",i,i);
        fprintf(fp,"Physical Volume(\"Cell %d\")={%d};\n",ID_[i],i);
    }

    // Bounding faces
    string bname[6]= {"\"xmin\"","\"xmax\"","\"ymin\"","\"ymax\"","\"zmin\"","\"zmax\""};
    for(int i=0;i<6;i++){
        const char* entityName=bname[i].c_str();
        formatSequence(fp,"Physical Surface",bounds_[i],entityName);
    }
}

vector<double> vorogmsh::edgeLengths() {
/**
 * Returns a vector containing the length of each edge.
 */
    vector<double> lengths(adj_mat_vtx_.nEdges());
    unsigned int i,j;
    double x1,y1,z1,x2,y2,z2;
    for(i=0; i<(unsigned int)adj_mat_vtx_.size(); i++) {
        x1=vtx_[i];
        y1=vty_[i];
        z1=vtz_[i];
        int ide;
        for(j=0; j<i; j++) {
            if(adj_mat_vtx_.get(i,j,ide)){
                ide=abs(ide);
                x2=vtx_[j];
                y2=vty_[j];
                z2=vtz_[j];
                lengths.at(ide-1)=distance(x1,y1,z1,x2,y2,z2);
            }
        }
    }
    return lengths;
}


vorogmsh::~vorogmsh() {
/**
 * Destructor
 */
}
