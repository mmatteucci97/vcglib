#ifndef HEADERS_H_
#define HEADERS_H_

    // This filter requires gl.
    #ifndef GLU_VERSIONS
        #ifdef __APPLE__
            #include <OpenGL/gl.h>
        #else
            #ifdef _WIN32
                #define NOMINMAX
                #include <windows.h>
            #endif
            #include <GL/gl.h>
            #include <GL/glu.h>
        #endif
    #endif

    #include <list>
    #include <vector>
    #include <iostream>
    #include <fstream>
    #include <sstream>
    #include <string>

    #include <vcg/complex/allocate.h>
    #include <vcg/complex/complex.h>
    #include <vcg/complex/used_types.h>

    #include <vcg/complex/algorithms/attribute_seam.h>
    #include <vcg/complex/algorithms/bitquad_support.h>
    #include <vcg/complex/algorithms/bitquad_creation.h>
    #include <vcg/complex/algorithms/clean.h>
    #include <vcg/complex/algorithms/clustering.h>
    #include <vcg/complex/algorithms/hole.h>
    #include <vcg/complex/algorithms/intersection.h>
    #include <vcg/complex/algorithms/isotropic_remeshing.h>
    #include <vcg/complex/algorithms/local_optimization.h>
    #include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric.h>
    #include <vcg/complex/algorithms/local_optimization/tri_edge_collapse_quadric_tex.h>
    #include <vcg/complex/algorithms/pointcloud_normal.h>
    #include <vcg/complex/algorithms/refine.h>
    #include <vcg/complex/algorithms/refine_loop.h>
    #include <vcg/complex/algorithms/smooth.h>
    #include <vcg/complex/algorithms/stat.h>

    #include <vcg/complex/algorithms/create/platonic.h>

    #include <vcg/complex/algorithms/update/bounding.h>
    #include <vcg/complex/algorithms/update/curvature.h>
    #include <vcg/complex/algorithms/update/curvature_fitting.h>
    #include <vcg/complex/algorithms/update/position.h>
    #include <vcg/complex/algorithms/update/quality.h>
    #include <vcg/complex/algorithms/update/topology.h>

    #include <vcg/container/simple_temporary_data.h>


    #include <vcg/space/color4.h>
    #include <vcg/space/plane3.h>
    #include <vcg/space/point3.h>
    #include <vcg/space/fitting3.h>

    #include <vcg/simplex/vertex/base.h>

    #include<wrap/gl/glu_tessellator_cap.h>
    #include <wrap/io_trimesh/import_ply.h>
    #include <wrap/io_trimesh/export_ply.h>

    using namespace std;
    using namespace vcg;
    using namespace vcg::tri;

#endif // HEADERS_H_