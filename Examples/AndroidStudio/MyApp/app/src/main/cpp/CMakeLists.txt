cmake_minimum_required(VERSION 3.22.1)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible with ${CMAKE_PROJECT_NAME}
project("myapp")


## ADDED everything from here to  add_library()


set (SHARED_INCLUDE_DIRS
    "/home/user/XMLFoundation/Libraries/XMLFoundation/inc"
    ${EXT_SOURCE_DIR}
)

include_directories(${SHARED_INCLUDE_DIRS})

# supposing you needed to link to a static shared library or add cppBuild flags this is where to do it
if (0)
#	add_definitions("-fPIC ")
#	link_directories("/home/user/XMLFoundation/Libraries")
endif()

#    
#${WINDOWS__SYSTEM_INCLUDES}
# "/home/user/XMLFoundation/Libraries/WinResKit/10.0.22621.0/ucrt"
# "/home/user/XMLFoundation/Libraries/MinGW-w64-11.0.0/include"

#	include_directories(
#			"/home/user/XMLFoundation/Libraries/MinGW-w64-11.0.0/include"
#	)




# In this application-top-level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define the target library name;
# in the sub-module's CMakeLists.txt, ${PROJECT_NAME} is preferred for the same purpose.
add_library(${CMAKE_PROJECT_NAME} 
              SHARED 
              native-lib.cpp
	    )

target_link_libraries( ${CMAKE_PROJECT_NAME}  android  log)
