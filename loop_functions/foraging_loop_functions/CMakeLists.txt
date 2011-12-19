link_directories(${CMAKE_BINARY_DIR}/controllers/footbot_foraging)
set(foraging_loop_functions_SOURCES foraging_loop_functions.cpp)

set(QT_USE_QTOPENGL true)
find_package(Qt4)
if(QT4_FOUND)
  # QT4 found, is it the minimum required version?
  if(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 1)
    # Is the QTOpenGL module present?
    if(QT_QTOPENGL_FOUND)
      # QT is OK, now check for OpenGL
      find_package(OpenGL)
      if(OPENGL_FOUND)
        # OpenGL is ok, now check for GLUT
        find_package(GLUT)
        if(GLUT_FOUND)
          # GLUT is ok
          
          # All the required libraries are OK
          set(foraging_loop_functions_SOURCES
            ${foraging_loop_functions_SOURCES}
            foraging_qt_user_functions.cpp)
          
          include(${QT_USE_FILE})
	  if( APPLE )
	    include_directories(${OPENGL_INCLUDE_DIR}/Headers)
	  endif( APPLE )

        else(GLUT_FOUND)
          message(STATUS "GLUT not found.")
        endif(GLUT_FOUND)

      else(OPENGL_FOUND)
        message(STATUS "OpenGL not found.")
      endif(OPENGL_FOUND)

    else(QT_QTOPENGL_FOUND)
      message(STATUS "QtOpenGL module not found.")
    endif(QT_QTOPENGL_FOUND)

  else(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 1)
    message(STATUS "Minimum required version for Qt (>= 4.2) not found.")
  endif(QT_VERSION_MAJOR GREATER 3 AND QT_VERSION_MINOR GREATER 1)
  
else(QT4_FOUND)
  message(STATUS "Qt4 not found")
endif(QT4_FOUND)

add_library(foraging_loop_functions MODULE ${foraging_loop_functions_SOURCES})
#target_link_libraries(foraging_loop_functions footbot_foraging)
