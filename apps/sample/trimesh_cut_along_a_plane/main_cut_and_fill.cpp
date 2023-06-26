#include "headers.h"
#include "utility.h"
#include "meshType.h"
#include "cut_and_fill.h"
#include "parameters.h"

int main(int argc, char *argv[])
{
    cout << argv[0] << endl;
    string inputFileName;
    Parameters params;

    if(argc > 1)
    {
        Parameters inputStats(argv[1]);
        params = inputStats;
    }
    inputFileName = GetInputMeshName(argv[1]);

    MeshType m;
    Box3f originBoundingBox;

    //  OUTPUT MESHES
    MeshType section, sectionSurface, underMesh, overMesh;

    //  VARIABLES USED FOR CUTTING
    Point3f planeCenter;
    Point3f planeNormal, surfaceAverageNormal;
    Plane3f slicingPlane;
    
    MidPointFunctor slicingFunc(0.0);
    EdgePredicate slicingPred(0.0, 0.0);

    ImportMesh(m, inputFileName.data());
    UpdateMesh(m);

    originBoundingBox = m.bbox;
    planeNormal = params.planeAxis;

    planeNormal.Normalize();

    planeCenter = planeNormal * params.planeOffset;
    slicingPlane.Init(planeCenter, planeNormal);

    tri::UpdateQuality<MeshType>::VertexFromPlane(m, slicingPlane);
    tri::UpdateTopology<MeshType>::FaceFace(m);
    tri::RefineE<MeshType, MidPointFunctor, EdgePredicate>(m, slicingFunc, slicingPred);

    SetMeshRequirement(m);

    vcg::tri::CutAndFill<MeshType>::CreateSection(section, m);

    CapEdgeMesh(section, sectionSurface);


    UpdateNormal<MeshType>::PerFaceNormalized(sectionSurface);
    UpdateNormal<MeshType>::PerVertexAngleWeighted(sectionSurface);

    Point3f sectionAverageNormal = Point3f(0.0, 0.0, 0.0);
    for(auto fi = sectionSurface.face.begin(); fi != sectionSurface.face.end(); ++fi)
    {
        if(fi->IsD()) continue;
        sectionAverageNormal += fi->N();
    }
    sectionAverageNormal /= sectionSurface.FN();

    if(sectionAverageNormal.dot(planeNormal) < 0.0)
    {
        tri::Clean<MeshType>::FlipMesh(sectionSurface);
    }


    tri::UpdateSelection<MeshType>::VertexFromQualityRange(m, 0.0, std::numeric_limits<float>::max());
    tri::UpdateSelection<MeshType>::FaceFromVertexStrict(m);

    tri::Append<MeshType, MeshType>::Mesh(underMesh, m, true);
    tri::UpdateSelection<MeshType>::FaceInvert(m);
    tri::Append<MeshType, MeshType>::Mesh(overMesh, m, true);

    vcg::tri::CutAndFill<MeshType>::FillMesh(overMesh, underMesh, sectionSurface, m.bbox.Center());

    ExportMesh(sectionSurface, params.sectionSurfaceName.data(), params.createSectionSurface);
    ExportMesh(underMesh, params.underMeshName.data(), params.createUnderMesh);
    ExportMesh(overMesh, params.overMeshName.data(), params.createOverMesh);


    return 0;
}
