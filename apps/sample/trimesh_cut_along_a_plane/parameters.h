#ifndef PARAMETERS_H
#define PARAMETERS_H

#include "headers.h"

class Parameters
{
    public:
    // NEW MESH FILE NAMES
    string sectionName;
    bool createSection;

    string sectionSurfaceName;
    bool createSectionSurface;

    string underMeshName;
    bool createUnderMesh;

    string overMeshName;
    bool createOverMesh;

    string underMeshFilledName;
    bool createUnderMeshFilled;

    string overMeshFilledName;
    bool createOverMeshFilled;

    string toProjectCopyName;
    bool createToProjectCopy;

    // PLANE
    float planeOffset;
    Point3f planeAxis;

    //  REMESHING ISOTROPIG
    int iterations;
    bool adaptive;
    bool selectedOnly;
    float targetLenMult;
    float creaseAngle;
    bool checkSurfaceDistance;
    float maxSurfaceDistanceMult;
    bool refineStep;
    bool collapseStep;
    bool edgeSwapStep;
    bool smoothStep;
    bool reprojectStep;

    bool targetLenFromAverageEdgeLength;
    float averageEdgeLengthMultiplier;

    public:
    Parameters():
        sectionName("section.ply"),
        sectionSurfaceName("sectionSurface.ply"),
        underMeshName("underMesh.ply"),
        overMeshName("overMesh.ply"),
        underMeshFilledName("underFilledMesh.ply"),
        overMeshFilledName("overFilledMesh.ply"),
        toProjectCopyName("toProjectCopy.ply"),

        planeOffset(10.0f),
        planeAxis(0.0f, 0.0f, 1.0f),

        createSection(true),
        createSectionSurface(true),
        createUnderMesh(true),
        createOverMesh(true),
        createUnderMeshFilled(true),
        createOverMeshFilled(true),
        createToProjectCopy(true),
    
        iterations(10),
        adaptive(true),
        selectedOnly(true),
        targetLenMult(0.01),
        creaseAngle(60),
        checkSurfaceDistance(false),
        refineStep(true),
        collapseStep(true),
        edgeSwapStep(true),
        smoothStep(true),
        reprojectStep(true),
        averageEdgeLengthMultiplier(0.5f),
        targetLenFromAverageEdgeLength(true)
    {

    }

    Parameters(const string& fileName) : Parameters()
    {
        std::ifstream file(fileName);
        if(!file.is_open())
        {
            throw std::runtime_error("Error opening file " + fileName);
        }

        string line;
        string varName;

        while(getline(file, line))
        {
            istringstream iss(line);
            string token;
            iss >> token;

            if(token == "sectionName")
            {
                iss >> sectionName;
            }
            else if(token == "createSection")
            {
                iss >> createSection;
            }
            else if(token == "sectionSurfaceName")
            {
                iss >> sectionSurfaceName;
            }
            else if(token == "createSectionSurface")
            {
                iss >> createSectionSurface;
            }
            else if(token == "underMeshName")
            {
                iss >> underMeshName;
            }
            else if(token == "createUnderMesh")
            {
                iss >> createUnderMesh;
            }
            else if(token == "overMeshName")
            {
                iss >> overMeshName;
            }
            else if(token == "createOverMesh")
            {
                iss >> createOverMesh;
            }
            else if(token == "underMeshFilledName")
            {
                iss >> underMeshFilledName;
            }
            else if(token == "createUnderMeshFilled")
            {
                iss >> createUnderMeshFilled;
            }
            else if(token == "overMeshFilledName")
            {
                iss >> overMeshFilledName;
            }
            else if(token == "createOverMeshFilled")
            {
                iss >> createOverMeshFilled;
            }
            else if(token == "toProjectCopyName")
            {
                iss >> toProjectCopyName;
            }
            else if(token == "createToProjectCopy")
            {
                iss >> createToProjectCopy;
            }
            else if(token == "planeOffset")
            {
                iss >> planeOffset;
            }
            else if(token == "planeAxis")
            {
                float x, y, z;
                iss >> x >> y >> z;
                planeAxis = Point3f(x, y, z);
            }
            else if(token == "iterations")
            {
                iss >> iterations;
            }
            else if(token == "adaptive")
            {
                iss >> adaptive;
            }
            else if(token == "selectedOnly")
            {
                iss >> selectedOnly;
            }
            else if(token == "targetLenMult")
            {
                iss >> targetLenMult;
            }
            else if(token == "creaseAngle")
            {
                iss >> creaseAngle;
            }
            else if(token == "checkSurfaceDistance")
            {
                iss >> checkSurfaceDistance;
            }
            else if(token == "maxSurfaceDistanceMult")
            {
                iss >> maxSurfaceDistanceMult;
            }
            else if(token == "refineStep")
            {
                iss >> refineStep;
            }
            else if(token == "collapseStep")
            {
                iss >> collapseStep;
            }
            else if(token == "edgeSwapStep")
            {
                iss >> edgeSwapStep;
            }
            else if(token == "smoothStep")
            {
                iss >> smoothStep;
            }
            else if(token == "reprojectStep")
            {
                iss >> reprojectStep;
            }
            else if(token == "targetLenFromAverageEdgeLength")
            {
                iss >> targetLenFromAverageEdgeLength;
            }
            else if(token == "averageEdgeLengthMultiplier")
            {
                iss >> averageEdgeLengthMultiplier;
            }
        }
    }

    void print() const
    {
        cout << "\tsectionName: " << sectionName << endl;
        cout << "\tcreateSection: " << createSection << endl;
        cout << "\tsectionSurfaceName: " << sectionSurfaceName << endl;
        cout << "\tcreateSectionSurface: " << createSectionSurface << endl;
        cout << "\tunderMeshName: " << underMeshName << endl;
        cout << "\tcreateUnderMesh: " << createUnderMesh << endl;
        cout << "\toverMeshName: " << overMeshName << endl;
        cout << "\tcreateOverMesh: " << createOverMesh << endl;
        cout << "\tunderMeshFilledName: " << underMeshFilledName << endl;
        cout << "\tcreateUnderMeshFilled: " << createUnderMeshFilled << endl;
        cout << "\toverMeshFilledName: " << overMeshFilledName << endl;
        cout << "\tcreateOverMeshFilled: " << createOverMeshFilled << endl;
        cout << "\ttoProjectCopyName: " << toProjectCopyName << endl;
        cout << "\tcreateToProjectCopy: " << createToProjectCopy << endl;
        cout << "\tplaneOffset: " << planeOffset << endl;
        cout << "\tplane axis " << planeAxis.X() << " " << planeAxis.Y() << " " << planeAxis.Z() << std::endl;

        cout << "\titerations: " << iterations << endl;
        cout << "\tadaptive: " << adaptive << endl;
        cout << "\tselectedOnly: " << selectedOnly << endl;
        cout << "\ttargetLenMult: " << targetLenMult << endl;
        cout << "\tcreaseAngle: " << creaseAngle << endl;
        cout << "\tcheckSurfaceDistance: " << checkSurfaceDistance << endl;
        cout << "\tmaxSurfaceDistanceMult: " << maxSurfaceDistanceMult << endl;
        cout << "\trefineStep: " << refineStep << endl;
        cout << "\tcollapseStep: " << collapseStep << endl;
        cout << "\tedgeSwapStep: " << edgeSwapStep << endl;
        cout << "\tsmoothStep: " << smoothStep << endl;
        cout << "\treprojectStep: " << reprojectStep << endl;
        cout << "\ttargetLenFromAverageEdgeLength: " << targetLenFromAverageEdgeLength << endl;
        cout << "\taverageEdgeLengthMultiplier: " << averageEdgeLengthMultiplier << endl;
    }
};



#endif // !PARAMETERS_H
