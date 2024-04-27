#include<iostream>
#include<PolygonalMesh.hpp>
#include"Utils.hpp"

using namespace std;
using namespace Mesh;

int main()
{
    PolygonalMesh mesh;
    string filepath = "Mesh";

    //import mesh
    bool done = ImportMesh(filepath,mesh);
    if(!done){
        return 1;
    }
    else{
        cout << "Mesh importata corrrettamente" << "\n" <<endl;
    }

    // Test su mesh
    cout << "Test su mesh:" << endl;

    check_marker(mesh);

    bool side = test_line(mesh);
    if(side){
        cout << "La mesh non ha lati nulli" << endl;
    }

    bool area = test_area(mesh);
    if(area){
        cout << "La mesh non ha aree nulle" << endl;
    }

    return 0;
}
