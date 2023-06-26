#ifndef MESHTYPE_H
#define MESHTYPE_H

    #include "headers.h"

    class MeshVertex;
    class MeshEdge;
    class MeshFace;

    struct MyUsedTypes: public vcg::UsedTypes
    <
        vcg::Use<MeshVertex>   ::AsVertexType,
        vcg::Use<MeshEdge>     ::AsEdgeType,
        vcg::Use<MeshFace>     ::AsFaceType
    >{};

    class MeshVertex: public vcg::Vertex
    <
            MyUsedTypes,
            vertex::Coord3f,
            vertex::Color4b,
            vertex::BitFlags,
            vertex::Normal3f,
            vertex::Qualityf,
            vertex::VFAdj,
            vertex::Mark,
            vertex::TexCoord2f,
            vertex::CurvatureDirf,
            vertex::Radiusf
    >{};

    class MeshEdge : public vcg::Edge
    <
        MyUsedTypes,
        edge::BitFlags,
        edge::Mark,
        edge::Qualityf,
        edge::Color4b,
        edge::EVAdj,
        edge::EEAdj,
        edge::VertexRef
    > {};

    class MeshFace : public vcg::Face
    <
        MyUsedTypes,
        face::VertexRef,
        face::BitFlags,
        face::Normal3f,
        face::Qualityf,
        face::Mark,
        face::Color4b,
        face::FFAdj,
        face::VFAdj,
        face::CurvatureDirf,
        face::WedgeTexCoord2f
    > {};

    class MeshType: public vcg::tri::TriMesh
    <
            std::vector<MeshVertex>,
            std::vector<MeshFace> ,
            std::vector<MeshEdge>
    > {};

    typedef vcg::tri::QualityEdgePredicate<MeshType> EdgePredicate;
    typedef vcg::tri::QualityMidPointFunctor<MeshType> MidPointFunctor;
    typedef vcg::tri::IsotropicRemeshing<MeshType>::Params RemeshingParams;

#endif // MESHTYPE_H
