cmake_minimum_required(VERSION 3.13)

project(parametric_cut_and_fill)

find_package(OPENGL REQUIRED)
find_package(GLU REQUIRED)

if(VCG_HEADER_ONLY)

    set(SOURCES
        utility.cpp
        ${VCG_INCLUDE_DIRS}/wrap/ply/plylib.cpp
    )
endif()

add_executable( cut_and_fill
        main_cut_and_fill.cpp
        ${SOURCES})

add_executable( cut_fill_remesh
        main_cut_fill_and_remesh.cpp
        ${SOURCES})


target_include_directories( cut_and_fill
        PRIVATE
                ${VCGLIB_DIR}
                ${OPENGL_INCLUDE_DIRS}
                ${GLU_INCLUDE_DIRS}
)

target_link_libraries( cut_and_fill
        PUBLIC
                vcglib
                ${OPENGL_LIBRARIES}
                ${GLU_LIBRARIES}
        )

        target_include_directories( cut_fill_remesh
        PRIVATE
                ${VCGLIB_DIR}
                ${OPENGL_INCLUDE_DIRS}
                ${GLU_INCLUDE_DIRS}
)

target_link_libraries( cut_fill_remesh
        PUBLIC
                vcglib
                ${OPENGL_LIBRARIES}
                ${GLU_LIBRARIES}
        )