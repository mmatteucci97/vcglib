#include "utility.h"

string GetInputMeshName(char * statsFileName)
{
    string defaultInputName = "C:/Users/Marcello/Downloads/tesi_3dprinter_helper/elephant.ply";
    if(statsFileName == nullptr || statsFileName == "")
    {
        return defaultInputName;
    }
    string inputName = "";
    std::ifstream file(statsFileName);
    if( !file.is_open())
    {
        inputName = defaultInputName;
    }
    else
    {
        string line;
        string varName;

        while(getline(file, line))
        {
            istringstream iss(line);
            string token;
            iss >> token;

            if(token == "inputMeshFile")
            {
                iss >> inputName;
            }
        }
    }

    return inputName;
}

void ImportMesh(MeshType &m, char *meshFile)
{
    if(vcg::tri::io::ImporterPLY<MeshType>::Open(m, meshFile) != 0)
    {
        cerr << "Error reading file " << meshFile << endl;
        exit(0);
    }
}

void SetMeshRequirement(MeshType &m)
{
    for(auto fi = m.face.begin(); fi!=m.face.end(); ++fi)
    {
        for(int i=0; i<3; i++)
        {
            MeshType::VertexType *v0 = (*fi).V0(i);
            MeshType::VertexType *v1 = (*fi).V1(i);
            MeshType::VertexType *v2 = (*fi).V2(i);

            if((v0->Q()==0 && v1->Q()==0) || (v1->Q()==0 && v2->Q()==0) || (v2->Q()==0 && v0->Q()==0))
            {
                if(face::IsBorder(*fi, i))
                {
                    cout << "index of the face on the border:" << i << endl;
                    cout << "Mesh ha a border on the plane, cannot apply filter" << endl;
                    exit(1);
                }

                else if(!face::IsManifold(*fi, i))
                {
                    cout << "index of the face non manifold:" << i << endl;
                    cout << "Mesh is not two manifold on the plane, cannot apply filter" << endl;
                    exit(1);
                }
            }
        }
    }
}

void UpdateMesh(MeshType &m)
{
    tri::UpdateNormal<MeshType>::PerVertexNormalized(m);
    tri::UpdateTopology<MeshType>::FaceFace(m);
    tri::UpdateTopology<MeshType>::VertexFace(m);
    tri::UpdateBounding<MeshType>::Box(m);
}

void PrintMesh(MeshType &m, const char *meshName)
{
    // Print mesh information
    cout << "Mesh " << meshName << " has:\n\t " << m.VN() << " vertices,\n\t " << m.EN() << " edges and\n\t " << m.FN() << " faces" << endl;
}

void ExportMesh(MeshType &m, const char *meshName, bool doExport)
{
    if(doExport)
    {
        if(vcg::tri::io::ExporterPLY<MeshType>::Save(m, meshName) != 0)
        {
            cerr << "Error writing file " << meshName << endl;
            exit(0);
        }
    }
}