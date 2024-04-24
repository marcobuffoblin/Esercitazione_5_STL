#pragma once
#include <iostream>
#include "Eigen/Eigen"
using namespace std;
using namespace Eigen;


namespace PolygonalLibrary {

struct PolygonalMesh
{
    unsigned int NumberCell0D = 0;
    std::vector<unsigned int> Cell0DId = {};
    std::vector<Vector2d> Cell0DCoordinates = {};
    std::map<unsigned int, list<unsigned int>> Cell0DMarkers = {};

    unsigned int NumberCell1D = 0;
    std::vector<unsigned int> Cell1DId = {};
    std::vector<Vector2i> Cell1DVertices = {};
    std::map<unsigned int, list<unsigned int>> Cell1DMarkers = {};

    unsigned int NumberCell2D = 0;
    std::vector<unsigned int> Cell2DId = {};
    std::vector<unsigned int> NumberVertices2D = {};
    std::vector<std::vector<unsigned int>> Cell2DVertices = {};
    std::vector<std::vector<unsigned int>> Cell2DEdges = {};

    double tol=10e-7;//numeric_limits<double>::epsilon(); //usando 0.01 si vede che funzionano i calcoli
    double tol_area=(tol*tol)/2; //usando 0.01 si vede che funzionano i calcoli
};

}
