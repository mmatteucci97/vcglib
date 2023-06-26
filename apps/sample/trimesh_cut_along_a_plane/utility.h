#ifndef UTILITY_H
    #define UTILITY_H
    
    # include "headers.h"
    # include "meshType.h"

    void ImportMesh(MeshType &m, char *meshFile);
    
    string GetInputMeshName(char * statsFileName);
    void SetMeshRequirement(MeshType &m);
    void UpdateMesh(MeshType &m);
    void PrintMesh(MeshType &m, const char *meshName);

    void ExportMesh(MeshType &m, const char *meshName, bool doExport);
#endif // !UTILITY_H