#pragma once

#include "PolygonalMesh.hpp"

using namespace std;

namespace Mesh {

// importa function: leggi da file inofmrazioni e salva su mesh poliedrale, return true se fatto correttamente altrimenti false
bool ImportMesh(const string &filepath, PolygonalMesh& mesh);

bool ImportCell0Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell1Ds(const string &filename, PolygonalMesh& mesh);

bool ImportCell2Ds(const string &filename, PolygonalMesh& mesh);

bool controll_existing(const PolygonalMesh& mesh);

//------------------------------------------------------------------------------------------------------------

//test function:
//stampa marker non nulli per confrontarli con immagine
void check_marker(const PolygonalMesh& mesh);

//controlla se ci sono lati nulli
bool test_line(const PolygonalMesh& mesh);

//controlla se ci sono celle nulle
bool test_area(const PolygonalMesh& mesh);
}
