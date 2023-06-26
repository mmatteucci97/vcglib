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
    MeshType section, sectionSurface, underMesh, overMesh, toProjectCopy;

    //  VARIABLES USED FOR CUTTING
    Point3f planeCenter;
    Point3f planeNormal, planeAbsNormal, surfaceAverageNormal;
    Plane3f slicingPlane;
    
    MidPointFunctor slicingFunc(0.0);
    EdgePredicate slicingPred(0.0, 0.0);

    ImportMesh(m, inputFileName.data());
    UpdateMesh(m);

    originBoundingBox = m.bbox;
    planeNormal = params.planeAxis;
    planeAbsNormal = Point3f(fabs(planeNormal.X()), fabs(planeNormal.Y()), fabs(planeNormal.Z()));
    planeNormal.Normalize();

    planeCenter = planeNormal * params.planeOffset;
    slicingPlane.Init(planeCenter, planeNormal);

    tri::IsotropicRemeshing<MeshType>::Params remeshParams;

    float averageEdgeLength = tri::CutAndFill<MeshType>::DetermineAverageEdgeLength(m);
    remeshParams.SetTargetLen(averageEdgeLength * params.averageEdgeLengthMultiplier);
    remeshParams.SetFeatureAngleDeg(params.creaseAngle);
    remeshParams.selectedOnly = true;
    remeshParams.iter = params.iterations;
    remeshParams.surfDistCheck = false;
    remeshParams.adapt = true;
    remeshParams.splitFlag = params.refineStep;
    remeshParams.smoothFlag = params.smoothStep;
    remeshParams.collapseFlag = params.collapseStep;
    remeshParams.swapFlag = params.edgeSwapStep;
    remeshParams.projectFlag = params.reprojectStep;
    //--------------------------------------------------------------------------------

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

    tri::CutAndFill<MeshType>::BoundaryExpand(sectionSurface);
    tri::Append<MeshType, MeshType>::Mesh(toProjectCopy, sectionSurface);
    
    try
    {
        tri::IsotropicRemeshing<MeshType>::Do(sectionSurface, toProjectCopy, remeshParams);
    }
    catch(vcg::MissingPreconditionException& e)
    {
        cout << e.what() << endl;
        return 1;
    }
    tri::UpdateSelection<MeshType>::FaceInvert(sectionSurface);

    for(auto fi = sectionSurface.face.begin(); fi != sectionSurface.face.end(); ++fi)
    {
        if(fi->IsD()) continue;
        if(fi->IsS())
        {
            tri::Allocator<MeshType>::DeleteFace(sectionSurface, *fi);
        }
    }
    tri::Clean<MeshType>::RemoveUnreferencedVertex(sectionSurface);
    tri::Allocator<MeshType>::CompactEveryVector(sectionSurface);
    tri::UpdateBounding<MeshType>::Box(sectionSurface);


    tri::UpdateSelection<MeshType>::VertexFromQualityRange(m, 0.0, std::numeric_limits<float>::max());
    tri::UpdateSelection<MeshType>::FaceFromVertexStrict(m);

    tri::Append<MeshType, MeshType>::Mesh(underMesh, m, true);
    tri::UpdateSelection<MeshType>::FaceInvert(m);
    tri::Append<MeshType, MeshType>::Mesh(overMesh, m, true);

    vcg::tri::CutAndFill<MeshType>::FillMesh(overMesh, underMesh, sectionSurface, m.bbox.Center());

    ExportMesh(sectionSurface, params.sectionSurfaceName.data(), params.createSectionSurface);
    ExportMesh(underMesh, params.underMeshName.data(), params.createUnderMesh);
    ExportMesh(overMesh, params.overMeshName.data(), params.createOverMesh);
    ExportMesh(toProjectCopy, params.toProjectCopyName.data(), true);


    return 0;
}
