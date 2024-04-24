#include "PolygonalMesh.hpp"
#include "Utils.hpp"
using namespace PolygonalLibrary;

int main()
{
    PolygonalMesh mesh;
    string filepath = "PolygonalMesh";
    if(!ImportMesh(filepath,mesh))
    {
        return 1;
    }

    return 0;
}
