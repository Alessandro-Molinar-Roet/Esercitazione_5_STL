#include"Utils.hpp"
#include<iostream>
#include<Eigen/Eigen>
#include<fstream>
#include<math.h>

namespace Mesh {

// Import Function:

bool ImportMesh(const string& filepath, PolygonalMesh& mesh){
    //importo da file Celle1D, se fallisce retorna falso
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",mesh)){
        cout << "error in Cell0D.csv" << endl;
        return false;
    }

    //importo da file Celle1D, se fallisce retorna falso
    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",mesh)){
        cout << "error in Cell1D.csv" << endl;
        return false;
    }

    //importo da file Celle2D, se fallisce retorna falso
    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh)){
        cout << "error in Cell2D.csv" << endl;
        return false;
    }
    else{
        if(!controll_existing(mesh)){
            cout <<"Celle2D con veritici e/o lati non presenti in mesh" << endl;
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------------------------------------

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh)
{
    // apro file
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;

    //leggo file per riga e salvo in lista di stringhe
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    file.close(); // chiudo file
    listLines.pop_front(); // elimino intestazione file

    //prendo info su numero di punti se non c'è ne neache uno ritorno errore
    mesh.numberCell0D = listLines.size();
    if (mesh.numberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }

    //alloco spazio in memoria per Id e cordinate
    mesh.cell0DId.reserve(mesh.numberCell0D);
    mesh.cell0DCoordinates.reserve(mesh.numberCell0D);

    //ciclo sugli elementi stringa salvati nella lista
    for (string& line : listLines)
    {
        replace(line.begin(),line.end(), ';', ' '); // rimpiazzo ";" con " "
        // converto pezzi stirnga in tipo che voglio
        istringstream converter(line);
        unsigned int id = 0;
        unsigned int marker = 0;
        double x = 0;
        double y = 0;
        converter >> id >> marker >> x >> y;
        Vector2d coord(x,y);
        //iserisco in strutta mesh
        mesh.cell0DId.push_back(id);
        mesh.cell0DCoordinates.push_back(coord);
        if( marker != 0)
        {
            //se primo creo chiave altirmenti aggiungo id a lista
            auto ret = mesh.cell0DMarkers.insert({marker, {id}}); //ret.first iteratore a elmento iserito o a chiave, ret.second boleano corretto iserimento
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}
//----------------------------------------------------------------------------------------------------------

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh)
{
    // apro file
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    //salvo le linee del file in una lista di stringhe
    list<string> listLines;
    string line;
    while (getline(file, line))
        listLines.push_back(line);
    listLines.pop_front();

    //salvo quanti lati ho
    mesh.numberCell1D = listLines.size();

    //se file vuoto stampo errore
    if (mesh.numberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    //alloco memoira in vettori sfruttando informazioni lette e salvate in lista
    mesh.cell1DId.reserve(mesh.numberCell1D);
    mesh.cell1DVertices.reserve(mesh.numberCell1D);

    //prendo informazioni da lista e salvo in vettore
    for (string& line : listLines)
    {
        replace(line.begin(),line.end(), ';', ' '); // rimpiazzo ";" con " "
        // converto pezzi stirnga in tipo che voglio
        istringstream converter(line);
        unsigned int id = 0;
        unsigned int marker = 0;
        double start = 0;
        double end = 0;
        converter >> id >> marker >> start >> end;
        Vector2d vertices(start,end);
        //iserisco in strutta mesh
        mesh.cell1DId.push_back(id);
        mesh.cell1DVertices.push_back(vertices);
        if( marker != 0)
        {
            //se primo creo chiave altirmenti aggiungo id a lista
            auto ret = mesh.cell1DMarkers.insert({marker, {id}}); //ret.first iteratore a elmento iserito o a chiave, ret.second boleano corretto iserimento
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}
//----------------------------------------------------------------------------------------------------------

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh){
    ifstream file;
    file.open(filename);

    if(file.fail())
        return false;

    //salvo le linee del file in una lista di stringhe
    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    listLines.pop_front();

    //salvo quante celle ho
    mesh.numberCell2D = listLines.size();
    //se file vuoto stampo errore
    if (mesh.numberCell2D == 0){
        cerr << "There is no cell 1D" << endl;
        return false;
    }

    //alloco memoira in vettori sfruttando informazioni lette e salvate in lista
    mesh.Cell2DVertices.reserve(mesh.numberCell2D);
    mesh.Cell2DEdges.reserve(mesh.numberCell2D);

    //salvo informazioni in mesh
    for (string& line : listLines)
    {
        replace(line.begin(),line.end(), ';', ' '); //rimpiazzo ";" con " "
        //converto parte stirnga in tipo che voglio e poi salvo in mesh
        istringstream converter(line);
        unsigned int id = 0;
        unsigned int marker = 0;
        unsigned int numVertices = 0;
        converter >> id >> marker >> numVertices;

        vector<unsigned int> Verticies(numVertices);
        for(unsigned int i = 0; i < numVertices; i++){
            converter >> Verticies[i];
        }
        mesh.Cell2DVertices.push_back(Verticies);

        unsigned int numEdges = 0;
        converter>>numEdges;
        if(numEdges != numVertices){ //se numero di lati diverso da vertici no poligono quindi stampo errore
            cout << "error in Cell2D.csv" << endl;
            return false;
        }
        vector<unsigned int> Edges(numEdges);
        for(unsigned int i = 0; i < numEdges; i++){
            converter >> Edges[i];
        }
        mesh.Cell2DEdges.push_back(Edges);
    }

    file.close();
    return true;

}
//----------------------------------------------------------------------------------------------------------

bool controll_existing(const PolygonalMesh& mesh){
    //controllo elemento per elemento se esiste anche nell'Id dei vertici
    for(unsigned int i = 0; i < mesh.Cell2DVertices.size(); i++){
        for(unsigned int j = 0; j < mesh.Cell2DVertices[i].size(); j++){
            if(find(mesh.cell0DId.begin(),mesh.cell0DId.end(), mesh.Cell2DVertices[i][j]) == mesh.cell0DId.end())
                return false;
        }
    }
    //controllo elemento per elemento se esiste anche nell'Id dei lati
    for(unsigned int i = 0; i < mesh.Cell2DEdges.size(); i++){
        for(unsigned int j = 0; j < mesh.Cell2DEdges[i].size(); j++){
            if(find(mesh.cell1DId.begin(), mesh.cell1DId.end(), mesh.Cell2DEdges[i][j]) == mesh.cell1DId.end())
                return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------------------------------------

//Test su mesh:

void check_marker(const PolygonalMesh& mesh){
    //stampo tutti gli id dei punti con dato marker
    cout << "Cell0D marker:" << endl;
    for(auto it = mesh.cell0DMarkers.begin(); it != mesh.cell0DMarkers.end(); it++){
        cout << "key:\t" << it -> first << "\t values:";
        for(const unsigned int id : it -> second)
            cout << "\t" << id;
        cout << endl;
    }

    //stampo tutti gli id dei lati con dato marker
    cout << "Cell1D marker:" << endl;
    for(auto it = mesh.cell1DMarkers.begin(); it != mesh.cell1DMarkers.end(); it++)
    {
        cout << "key:\t" << it -> first << "\t values:";
        for(const unsigned int id : it -> second)
            cout << "\t" << id;

        cout << endl;
    }
}
//----------------------------------------------------------------------------------------------------------

bool test_line(const PolygonalMesh& mesh){
    double toll = mesh.tollerance;
    //per ogni lato cerco cerco posizione di id dei vertici per poi prendere cordiate e trovare lunghezza lato
    for(unsigned int i = 0; i < size(mesh.cell1DVertices); ++i){
        auto id_first= find(mesh.cell0DId.begin(), mesh.cell0DId.end(), mesh.cell1DVertices[i][0]);
        auto id_second= find(mesh.cell0DId.begin(), mesh.cell0DId.end(), mesh.cell1DVertices[i][1]);
        if((id_first != mesh.cell0DId.end()) && (id_second != mesh.cell0DId.end())){
            // delcttype restituisce tipo variabile
            decltype(mesh.cell0DCoordinates.size()) place_first = distance(mesh.cell0DId.begin(), id_first); //delcttype per evitare warnign comparison
            decltype(mesh.cell0DCoordinates.size()) place_second = distance(mesh.cell0DId.begin(), id_second); //delcttype per evitare warnign comparison
            if( (place_first < mesh.cell0DCoordinates.size()) && (place_second < mesh.cell0DCoordinates.size()) ){
                //con posizione id accedo a codinate punto (posso farlo perchè salvati sequenzialmente)
                double x_first = mesh.cell0DCoordinates[place_first][0];
                double y_first = mesh.cell0DCoordinates[place_first][1];
                double x_second = mesh.cell0DCoordinates[place_second][0];
                double y_second = mesh.cell0DCoordinates[place_second][1];
                double distance = sqrt( pow(x_first - x_second,2) + pow(y_first - y_second,2));
                if( distance <= toll){
                    cout << "esistono lati nulli" << endl;
                    return false;
                }
            }
        }
    }
    return true;
}
//----------------------------------------------------------------------------------------------------------

bool test_area(const PolygonalMesh& mesh){
    double toll = mesh.tollerance;
    //per ogni cella cerco id vertici per poi caloclare area cella
    for(unsigned int i = 0; i < size(mesh.Cell2DVertices); ++i){
        unsigned int num = mesh.Cell2DVertices[i].size(); //cerco tanti id quanti vertici ha cella
        vector<double> x(num); //vettore con cordianta x dei vertici della cella
        vector<double> y(num); //vettore con cordianta y dei vertici della cella
        for(unsigned int j = 0; j<num; ++j){
            auto id = find(mesh.cell0DId.begin(),mesh.cell0DId.end(), mesh.Cell2DVertices[i][j]);
            if((id != mesh.cell0DId.end())){
                decltype(mesh.cell0DCoordinates.size()) place = distance(mesh.cell0DId.begin(), id); //delcttype per evitare warnign comparison
                if( (place < mesh.cell0DCoordinates.size())){
                    x[j] = mesh.cell0DCoordinates[place][0];
                    y[j] = mesh.cell0DCoordinates[place][1];
                }
            }
        }

        //gauss' shoelace formula per area poligoni
        unsigned int h = num - 1;
        double area = 0.0;
        for (unsigned int k = 0; k < num; k++)
        {
            area += (x[h] + x[k]) * (y[h] - y[k]);
            h = k;
        }

        if(abs(0.5*area) <= toll){
            cout << "esistono aree nulle" << endl;
            return false;
        }
    }
    return true;
}
//----------------------------------------------------------------------------------------------------------



}
