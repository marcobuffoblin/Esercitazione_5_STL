#include "Utils.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<cmath>
#include<iomanip>

namespace PolygonalLibrary {

bool ImportMesh(const string& filepath,PolygonalMesh& mesh)
{
    if(!ImportCell0Ds(filepath + "/Cell0Ds.csv",mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell0D marker:" << endl;
        for(auto itor = mesh.Cell0DMarkers.begin(); itor != mesh.Cell0DMarkers.end(); itor++)
        {
            cout << "key:\t" << itor -> first << "\t values:";
            for(const unsigned int id : itor -> second)
                cout << "\t" << id;
            cout << endl;
        }
        cout<<endl;
    }

    if(!ImportCell1Ds(filepath + "/Cell1Ds.csv",mesh))
    {
        return false;
    }
    else
    {
        cout << "Cell1D marker:" << endl;
        for(auto itor = mesh.Cell1DMarkers.begin(); itor != mesh.Cell1DMarkers.end(); itor++)
        {
            cout << "key:\t" << itor -> first << "\t values:";
            for(const unsigned int id : itor -> second)
                cout << "\t" << id;
            cout << endl;
        }
    }

    if(!ImportCell2Ds(filepath + "/Cell2Ds.csv",mesh))
    {
        return false;
    }
    return true;
}


// ***************************************************************************
bool ImportCell0Ds(const string &filename,PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    file.close();
    listLines.pop_front();
    mesh.NumberCell0D = listLines.size();
    if (mesh.NumberCell0D == 0)
    {
        cerr << "There is no cell 0D" << endl;
        return false;
    }
    mesh.Cell0DId.reserve(mesh.NumberCell0D);
    mesh.Cell0DCoordinates.reserve(mesh.NumberCell0D);
    for (const string& line : listLines)
    {
        istringstream converter(line);
        Vector2d coord;
        string val_da_prend;
        getline(converter,val_da_prend,';');
        unsigned int id = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        unsigned int marker = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        coord(0) = stod(val_da_prend);
        getline(converter,val_da_prend,';');
        coord(1) = stod(val_da_prend);
        mesh.Cell0DId.push_back(id);
        mesh.Cell0DCoordinates.push_back(coord);
        if( marker != 0)
        {
            auto ret = mesh.Cell0DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();
    return true;
}

// ***************************************************************************
bool ImportCell1Ds(const string &filename,PolygonalMesh& mesh)
{
    ifstream file;
    file.open(filename);
    if(file.fail())
        return false;
    list<string> listLines;
    string line;
    while (getline(file, line)){
        listLines.push_back(line);
    }
    listLines.pop_front();
    mesh.NumberCell1D = listLines.size();
    if (mesh.NumberCell1D == 0)
    {
        cerr << "There is no cell 1D" << endl;
        return false;
    }
    mesh.Cell1DId.reserve(mesh.NumberCell1D);
    mesh.Cell1DVertices.reserve(mesh.NumberCell1D);
    for (const string& line : listLines)
    {
        istringstream converter(line);
        string val_da_prend;
        Vector2i vertices;
        getline(converter,val_da_prend,';');
        unsigned int id = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        unsigned int marker = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        vertices(0) = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        vertices(1) = stoi(val_da_prend);
        mesh.Cell1DId.push_back(id);
        mesh.Cell1DVertices.push_back(vertices);
        if( marker != 0)
        {
            auto ret = mesh.Cell1DMarkers.insert({marker, {id}});
            if(!ret.second)
                (ret.first)->second.push_back(id);
        }
    }
    file.close();


    // Calcolo e verifico se due punti sono lo stesso fissata una tolleranza
    // Ovvero verifico che: the edges of the polygons have non-zero length,
    cout<<endl<<"Fissata la tolleranza a "<<setprecision(16)<<mesh.tol<<endl;
    unsigned int num_latonullo=0;
    for(unsigned int c = 0; c < mesh.NumberCell1D; c++){
            double distanza=sqrt((mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][0]][0]-mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][1]][0])*
                                 (mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][0]][0]-mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][1]][0])+
                                 (mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][0]][1]-mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][1]][1])*
                                 (mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][0]][1]-mesh.Cell0DCoordinates[mesh.Cell1DVertices[c][1]][1]));
            if(distanza<mesh.tol){
                cout<<"Il lato " <<c<<" che collega i punti "<<mesh.Cell1DVertices[c][0]<<"->"
                     <<mesh.Cell1DVertices[c][1]<<"  "<<fixed<<setprecision(16)<<"ha lunghezza inferiore e pari a:"<<distanza<<endl;
                num_latonullo++;
            }
    }
    if(num_latonullo==0)
        cout<<"Nessun lato ha distanza inferiore alla tolleranza fissata"<<endl;
    cout<<endl;


    return true;
}

// ***************************************************************************
bool ImportCell2Ds(const string& path, PolygonalMesh& mesh)
{

    ifstream file;
    file.open(path);
    if(file.fail())
        return false;
    string header;
    getline(file,header);  // ignoro la prima riga
    list<string> listLines;
    string line;
    while(getline(file,line)){
        listLines.push_back(line);
    }
    mesh.NumberCell2D = listLines.size();
    mesh.Cell2DId.reserve(mesh.NumberCell2D);
    mesh.NumberVertices2D.reserve(mesh.NumberCell2D);
    mesh.Cell2DVertices.reserve(mesh.NumberCell2D);
    mesh.Cell2DEdges.reserve(mesh.NumberCell2D);
    for (const string& line :listLines)
    {
        istringstream converter(line);
        string val_da_prend;
        getline(converter,val_da_prend,';');
        unsigned int id = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        //unsigned int marker = stoi(val_da_prend);
        getline(converter,val_da_prend,';');
        unsigned int NumVertices = stoi(val_da_prend);
        vector<unsigned int> vertices = {};
        for(unsigned int i = 0;i<NumVertices;i++){
            getline(converter,val_da_prend,';');
            vertices.push_back(stoi(val_da_prend));
        }
        getline(converter,val_da_prend,';');
        unsigned int NumEdges = stoi(val_da_prend);
        vector<unsigned int> edges;
        for(unsigned int i = 0;i<NumEdges;i++){
            getline(converter,val_da_prend,';');
            edges.push_back(stoi(val_da_prend));
        }
        mesh.Cell2DId.push_back(id);
        mesh.NumberVertices2D.push_back(NumVertices);
        mesh.Cell2DVertices.push_back(vertices);
        mesh.Cell2DEdges.push_back(edges);
    }
    file.close();


    // Calcolo e verifico che: the area of the triangles is non-zero,
    // formula di Erone A=sqrt(p/2*(p/2-l1)*(p/2-l2)*(p/2-l3))
    cout<<endl<<"Fissata la tolleranza a "<<setprecision(16)<<mesh.tol_area<<endl;
    unsigned int num_areanulla=0;
    for(unsigned int c = 0; c < mesh.NumberCell2D; c++){
        if(mesh.NumberVertices2D[c]==3){
            double latouno=sqrt((mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][0])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][0])+
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][1])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][1]));
            double latodue=sqrt((mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][0])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][0])+
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][1])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][1]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][1]));
            double latotre=sqrt((mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][0])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][0]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][0])+
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][1])*
                                (mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][2]][1]-mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][1]));
            double semiperim=(latouno+latodue+latotre)/2;
            double area=sqrt(semiperim*(semiperim-latouno)*(semiperim-latodue)*(semiperim-latotre));
            if(area<mesh.tol_area){
                cout<<"L'area del triangolo "<<c<<" di vertici "<<mesh.Cell2DVertices[c][0]<<" "<<mesh.Cell2DVertices[c][1]
                     <<" "<<mesh.Cell2DVertices[c][2]<<"  "<<fixed<<setprecision(16)<<"ha area inferiore e pari a:"<<area<<endl;
                num_areanulla++;
            }
        }
    }
    if(num_areanulla==0)
        cout<<"Nessun triangolo ha area inferiore alla tolleranza fissata"<<endl;


    // Calcolo e verifico che: the area of the polygons is non-zero
    // Utilizzo la formula 0.5*|sum(x(i)*y(i+1)-sum(x(i+1)*y(i)|
    // sommatoria per i=1:n   tale che x(n+1)=x(1) e y(n+1)=y(1)
    cout<<endl<<"Fissata la tolleranza a "<<setprecision(16)<<mesh.tol_area<<endl;
    unsigned int num_areanullaP=0;
    for(unsigned int c = 0; c < mesh.NumberCell2D; c++){
        double sommatoria1=0;
        double sommatoria2=0;
        for(unsigned int i = 0; i < mesh.NumberVertices2D[c]; i++){
            // gestisco il caso particolare
            if(i==(mesh.NumberVertices2D[c]-1)){
                sommatoria1=sommatoria1+(mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i]][0]*mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][1]);
                sommatoria2=sommatoria2+(mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][0]][0]*mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i]][1]);
            }
            //gestisco il caso generale
            else{
                sommatoria1=sommatoria1+(mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i]][0]*mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i+1]][1]);
                sommatoria2=sommatoria2+(mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i+1]][0]*mesh.Cell0DCoordinates[mesh.Cell2DVertices[c][i]][1]);
            }
        }
        double areaP=0.5*abs(sommatoria1-sommatoria2);
            if(areaP<mesh.tol_area){
                cout<<"L'area del poligono "<<c<<" di vertici ";
                for(unsigned int i=0; i<mesh.NumberVertices2D[c];i++){
                    cout<<mesh.Cell2DVertices[c][i]<<" ";
                }
                cout<<fixed<<setprecision(16)<<"ha area inferiore e pari a:"<<areaP<<endl;
                num_areanullaP++;
            }
    }
    if(num_areanullaP==0)
        cout<<"Nessun poligono ha area inferiore alla tolleranza fissata"<<endl;
    cout<<endl;

    return true;
}

}

