#pragma once

#include<Eigen/Eigen>

using namespace std;
using namespace Eigen;

namespace Mesh{

struct PolygonalMesh
{
    double tollerance =  pow(10,-10); //tolleranza per check lati e aree nulle;

    unsigned int numberCell0D = 0; //numero di vertici totali
    vector<unsigned int> cell0DId = {}; //ID per vertici
    vector<Vector2d> cell0DCoordinates = {}; //cordinate punti (x,y)
    map<unsigned int, list<unsigned int>> cell0DMarkers = {}; //key:maker object: list_id

    unsigned int numberCell1D = 0; //numero di lati
    vector<unsigned int> cell1DId = {}; //ID per lati
    vector<Vector2d> cell1DVertices = {}; //ID dei vertici del lato (fromID, toID)
    map<unsigned int, list<unsigned int>> cell1DMarkers = {}; //key:maker object: list_id

    unsigned int numberCell2D = 0; //numero di celle
    vector<unsigned int> cell2DId = {}; //ID per celle
    vector<vector<unsigned int>> Cell2DVertices = {}; //ID dei vertici della cella
    vector<vector<unsigned int>> Cell2DEdges = {};//Id dei lati della cella
};

}
