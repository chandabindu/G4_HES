#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
project(HES)

#----------------------------------------------------------------------------
# Compile with option
#
option(G4GEOMETRY_VOXELDEBUG "Debug option of G4SmartVoxelHeader class" ON)
#option(GEANT4_USE_OPENGL_X11 ON)
#option(GEANT4_USE_QT ON)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available UI and Vis drivers by default
# You can set WITH_GEANT4_UIVIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------


#set(LIBS )
#file(GLOB SRCS *.cpp)

#add_executable(a.out ${SRCS})
#target_link_libraries(a.out ${LIBS})

message(STATUS "G4GEOMETRY_VOXELDEBUG=${G4GEOMETRY_VOXELDEBUG}")

#----------------------------------------------------------------------------
# Find Geant4 package, no UI and Vis drivers activated
#
find_package(Geant4 REQUIRED)

#----------------------------------------------------------------------------
# Find ROOT (required package)
#
#find_package(ROOT REQUIRED)

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})
#set(Geant4_INCLUDE_DIR2 /home/gogami/g4install/geant4.10.03-install2/include/Geant4)
#set(Geant4_LIBRARIES2 -L/home/gogami/root/root-5.34.36-install/lib/root -lCore -lCint -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lTree -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -pthread -lm -ldl -rdynamic)

execute_process(
	COMMAND root-config --incdir
	OUTPUT_VARIABLE ROOT_INCLUDE_DIR	
)
execute_process(
	COMMAND root-config --libs
	OUTPUT_VARIABLE "ROOT_LIBRARIES"
	OUTPUT_STRIP_TRAILING_WHITESPACE
)
#suzuki
#set(ROOT_INCLUDE_DIR /usr/local/root-6.18.00/obj_gdml/include)
#dragon

execute_process(
  COMMAND root-config --incdir
  OUTPUT_VARIABLE ROOT_INCLUDE_DIR
  )
execute_process(
  COMMAND root-config --libs
  OUTPUT_VARIABLE ROOT_LIBRARIES
  OUTPUT_STRIP_TRAILING_WHITESPACE
  )
set(CMAKE_CXX_STANDARD 11)
#execute_process(
#  COMMAND root-config --cflags
#  OUTPUT_VARIABLE CMAKE_CXX_FLAGS
#  )


#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include 
                    ${Geant4_INCLUDE_DIR}
                    ${Geant4_INCLUDE_DIR2}							${ROOT_INCLUDE_DIR})
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

#----------------------------------------------------------------------------
# Add the executables, and link it to the Geant4 libraries
add_executable(HES HES.cc ${sources} ${headers})
#target_link_libraries(HES ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})
target_link_libraries(HES ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------
#set(CMAKE_INSTALL_PREFIX /home/gogami/11bgogami/svnroot_JLab/E12-15-008/simulation/HES)
install(TARGETS HES RUNTIME DESTINATION bin)



