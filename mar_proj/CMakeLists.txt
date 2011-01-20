########################## Project configuration ##############################
SET( PROJ_NAME      "mar_proj" )
SET( PROJ_PATH      ${CMAKE_SOURCE_DIR} )
SET( PROJ_OUT_PATH  ${CMAKE_BINARY_DIR} )
SET( PROJ_DATA_PATH ${CMAKE_SOURCE_DIR}/resources )
SET( PROJ_DOC_PATH  ${PROJ_OUT_PATH}/doc )
SET( PROJ_SOURCES   "" )
SET( PROJ_HEADERS   "" )
SET( PROJ_LIBRARIES "" )
SET( PROJ_INCLUDES  "" )

####################### Minimum CMake version to run ##########################
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

############################## Project name ###################################
PROJECT( mar_proj )

MESSAGE( "--------------------------------------------------" )
MESSAGE( STATUS "Configuring mar_proj" )

SET( mar_proj_MAJOR_VERSION 0 )
SET( mar_proj_MINOR_VERSION 0 )
SET( mar_proj_PATCH_LEVEL   1 )

########################## Modules configuration ##############################
OPTION( BUILD_DOC    "Build the doxygen documentation" OFF )
OPTION( BUILD_TESTS  "Build the unit tests"            OFF )

MESSAGE( STATUS "Building documentation:   " ${BUILD_DOC}    )
MESSAGE( STATUS "Building tests:           " ${BUILD_TESTS}  )

############################## Sources ########################################
FILE( GLOB_RECURSE PROJ_SOURCES src/*.cpp ) # Scan all source files
FILE( GLOB_RECURSE PROJ_HEADERS inc/mar_proj/*.h ) # Scan all public  header files

SET( PROJ_INCLUDES ${PROJ_INCLUDES} ${PROJ_PATH}/inc )

# Group files in virtual folders under Visual Studio
SOURCE_GROUP( "Headers" FILES ${PROJ_SOURCES} )
SOURCE_GROUP( "Sources" FILES ${PROJ_HEADERS} )

############################ External libraries ###############################
# Add current source directory as module path for Find*.cmake
SET( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJ_PATH} )

# OPENGL
FIND_PACKAGE( OpenGL REQUIRED )
SET( PROJ_INCLUDES  ${PROJ_INCLUDES}  ${OPENGL_INCLUDE_DIR} )
SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${OPENGL_LIBRARIES}   )

# GLUT
FIND_PACKAGE( GLUT REQUIRED )
SET( PROJ_INCLUDES  ${PROJ_INCLUDES}  ${GLUT_INCLUDE_DIR} )
SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${GLUT_LIBRARIES}   )

# GLUi
FIND_PACKAGE( GLUI REQUIRED )
SET( PROJ_INCLUDES  ${PROJ_INCLUDES}  ${GLUI_INCLUDE_DIR} )
SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${GLUI_LIBRARIES}   )

# GLEW
FIND_PACKAGE( GLEW REQUIRED )
SET( PROJ_INCLUDES  ${PROJ_INCLUDES}  ${GLEW_INCLUDE_DIR} )
SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${GLEW_LIBRARIES}   )

# COCOA (needed on OSX)
IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
  FIND_LIBRARY( COCOA_LIBRARIES Cocoa )
  SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${COCOA_LIBRARIES}   )
ENDIF()

# XRANDR (needed on linux)
IF ( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
  FIND_LIBRARY( XRANDR_LIBRARIES Xrandr )
  SET( PROJ_LIBRARIES ${PROJ_LIBRARIES} ${XRANDR_LIBRARIES} )
ENDIF ()

################################ Includes #####################################
INCLUDE_DIRECTORIES( ${PROJ_INCLUDES} ) # Include path

################################ Resources ####################################
FILE( COPY ${PROJ_DATA_PATH} DESTINATION ${PROJ_OUT_PATH} )

############################ Project generation ###############################
#ADD_LIBRARY( mar_proj SHARED ${Sources} )
ADD_EXECUTABLE( mar_proj ${PROJ_SOURCES} )

TARGET_LINK_LIBRARIES( mar_proj ${PROJ_LIBRARIES} )

############################## Documentation ##################################
IF ( BUILD_DOC )
  FIND_PACKAGE( Doxygen )
  SET( DOXYFILE_IN  "${PROJ_PATH}/Doxyfile.in"  )
  SET( DOXYFILE_OUT "${PROJ_OUT_PATH}/Doxyfile" )
  SET_PROPERTY( DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES ${PROJ_DOC_PATH} )
  ADD_CUSTOM_TARGET( doc COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                                 "Generating documentation..."
                                 WORKING_DIRECTORY ${PROJ_PATH} )
  CONFIGURE_FILE( ${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY )
ENDIF( BUILD_DOC )

################################## Tests ######################################
IF ( BUILD_TESTS )
  ENABLE_TESTING()
  INCLUDE( CTest )
  ADD_SUBDIRECTORY( tests )
ENDIF( BUILD_TESTS )

MESSAGE( "--------------------------------------------------" )

############################# Target Properties ###############################
SET_TARGET_PROPERTIES( mar_proj PROPERTIES LINKER_LANGUAGE CXX ) # Language forced to C++

SET_TARGET_PROPERTIES( mar_proj PROPERTIES DEBUG_POSTFIX   "d" ) # Debug mode: a trailing "d"
SET_TARGET_PROPERTIES( mar_proj PROPERTIES RELEASE_POSTFIX ""  ) # Release mode: no postfix

SET_TARGET_PROPERTIES( mar_proj PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for shared libraries
SET_TARGET_PROPERTIES( mar_proj PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for module libraries
SET_TARGET_PROPERTIES( mar_proj PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${PROJ_OUT_PATH} ) # Path for import libraries

SET_TARGET_PROPERTIES( mar_proj PROPERTIES OUTPUT_NAME "mar_proj" ) # Output name

