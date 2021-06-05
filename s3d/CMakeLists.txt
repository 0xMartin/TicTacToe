cmake_minimum_required(VERSION 3.5)

file(GLOB S3D_sources "*.c" "ui/*.c")
file(GLOB S3D_headers "*.h" "ui/*.h")

add_library(s3d
    ${S3D_headers}
    ${S3D_sources}
    )

if (UNIX)
    target_link_libraries(s3d m)
endif (UNIX)

#OpenGL + GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(Threads)

include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})
target_link_libraries(s3d ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${CMAKE_THREAD_LIBS_INIT})

