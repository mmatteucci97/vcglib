/****************************************************************************
* VCGLib                                                            o o     *
* Visual and Computer Graphics Library                            o     o   *
*                                                                _   O  _   *
* Copyright(C) 2004-2016                                           \/)\/    *
* Visual Computing Lab                                            /\/|      *
* ISTI - Italian National Research Council                           |      *
*                                                                    \      *
* All rights reserved.                                                      *
*                                                                           *
* This program is free software; you can redistribute it and/or modify      *
* it under the terms of the GNU General Public License as published by      *
* the Free Software Foundation; either version 2 of the License, or         *
* (at your option) any later version.                                       *
*                                                                           *
* This program is distributed in the hope that it will be useful,           *
* but WITHOUT ANY WARRANTY; without even the implied warranty of            *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
* GNU General Public License (http://www.gnu.org/licenses/gpl.txt)          *
* for more details.                                                         *
*                                                                           *
****************************************************************************/

#pragma once

namespace vcg 
{
namespace tri 
{
template < class MeshType> class CutAndFill
{
public:

typedef typename MeshType::VertexType VertexType;
typedef typename MeshType::FaceType FaceType;
typedef typename MeshType::EdgeType EdgeType;
typedef typename MeshType::ScalarType ScalarType;
typedef typename MeshType::CoordType CoordType;
typedef typename MeshType::FaceIterator FaceIterator;
typedef typename MeshType::VertexIterator VertexIterator;
typedef typename MeshType::EdgeIterator EdgeIterator;
typedef typename MeshType::FacePointer FacePointer;
typedef typename MeshType::VertexPointer VertexPointer;
typedef typename MeshType::EdgePointer EdgePointer;

static void CreateSection(MeshType &section, MeshType &m)
{
    // Add in section all vertices with Q==0
    int newVertices=0;
    int newEdges=0;
    float averageEdgeLength=0;
    for(VertexIterator vi=m.vert.begin(); vi!=m.vert.end(); ++vi)
    {
        if((*vi).Q() == 0)
        {
            VertexType newV = *Allocator<MeshType>::AddVertices(section, 1);
            newV = *vi;
            newVertices++;
        }
    }

    // Add in section all edges composed by 2 vertices with Q==0
    std::set<std::pair<VertexType *, VertexType *> > addedEdges;

    for(FaceIterator fi = m.face.begin(); fi!=m.face.end(); ++fi)
    {
        if(!(*fi).IsD())
        {
            for(int j=0; j<3; j++)
            {
                VertexType *v0 = (*fi).V0(j);
                VertexType *v1 = (*fi).V1(j);

                if(v0->Q() == 0 && v1->Q()==0)
                {
                    if(addedEdges.find(std::make_pair(v0, v1)) == addedEdges.end() && addedEdges.find(std::make_pair(v1, v0)) == addedEdges.end())
                    {
                        EdgeType &newE = *Allocator<MeshType>::AddEdges(section, 1);
                        newE.V(0) = v0;
                        newE.V(1) = v1;
                        addedEdges.insert(std::make_pair(v0, v1));
                        newEdges++;

                        float dx = (*fi).V1(j)->P().X() - (*fi).V0(j)->P().X();
                        float dy = (*fi).V1(j)->P().Y() - (*fi).V0(j)->P().Y();
                        float dz = (*fi).V1(j)->P().Z() - (*fi).V0(j)->P().Z();

                        float dist = sqrt(dx*dx + dy*dy + dz*dz);
                        averageEdgeLength += dist;   
                    }
                }
            }
        }
    }
}


static void FillMesh(MeshType &overMesh, MeshType &underMesh, MeshType &section, Point3f planeCenter)
{
    tri::Append<MeshType, MeshType>::Mesh(overMesh, section);
    tri::Clean<MeshType>::FlipMesh(section);
    tri::Append<MeshType, MeshType>::Mesh(underMesh, section);

    tri::Clean<MeshType>::RemoveDuplicateVertex(overMesh);
    tri::Clean<MeshType>::RemoveDuplicateVertex(overMesh);

    tri::Clean<MeshType>::RemoveDuplicateFace(overMesh);

    tri::Clean<MeshType>::RemoveDuplicateVertex(underMesh);
    tri::Clean<MeshType>::RemoveDuplicateVertex(underMesh);

    tri::Clean<MeshType>::RemoveDuplicateFace(underMesh);
}

static void BoundaryExpand(MeshType &m)
{
    tri::RequireFFAdjacency(m);
    tri::UpdateTopology<MeshType>::FaceFace(m);

    tri::UpdateSelection<MeshType>::FaceAll(m);

    std::vector<std::tuple<size_t, size_t, size_t>> newTrianglesVector;
    std::vector<Point3f> newTriangleCoordinates;
    int count = 0;

    for(auto fi = m.face.begin(); fi!=m.face.end(); ++fi)
    {
        for(int i=0; i<3; i++)
        {
            if(face::IsBorder(*fi, i))
            {
                newTrianglesVector.emplace_back(tri::Index(m, fi->V0(i)), tri::Index(m, fi->V1(i)), m.vert.size()+ newTriangleCoordinates.size());
                newTriangleCoordinates.push_back(fi->P0(i) + (fi->P1(i) - fi->P2(i)));
            }
        }
    }

    for(int i=0; i<newTriangleCoordinates.size(); i++)
    {
        tri::Allocator<MeshType>::AddVertex(m, newTriangleCoordinates[i]);
    }

    for(int i=0; i<newTrianglesVector.size(); i++)
    {
        tri:Allocator<MeshType>::AddFace(m, std::get<0>(newTrianglesVector[i]), std::get<2>(newTrianglesVector[i]), std::get<1>(newTrianglesVector[i]));
    }
}

static float DetermineAverageEdgeLength(MeshType &m)
{
    float averageEdgeLength = 0;
    int count = 0;

    for(auto fi = m.face.begin(); fi!=m.face.end(); ++fi)
    {
        for(int i=0; i<3; i++)
        {
            float dx = fi->P1(i).X() - fi->P0(i).X();
            float dy = fi->P1(i).Y() - fi->P0(i).Y();
            float dz = fi->P1(i).Z() - fi->P0(i).Z();

            float dist = sqrt(dx*dx + dy*dy + dz*dz);
            averageEdgeLength += dist;

            count++;
        }
    }

    averageEdgeLength /= count / 2;
    return averageEdgeLength;
}
};
} // end namespace tri
} // end namespace vcg
