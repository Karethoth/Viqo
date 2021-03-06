cmake_minimum_required(VERSION 2.6)
project(Viqo)

if(WIN32)
        set(CMAKE_MODULE_PATH "$ENV{OGRE_HOME}/CMake/;${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}/")
        set(OGRE_SAMPLES_INCLUDEPATH
                "$ENV{OGRE_HOME}/Samples/include"
        )
endif(WIN32)

if(UNIX)
        if(EXISTS "/usr/local/lib/OGRE/cmake")

          set(CMAKE_MODULE_PATH "/usr/local/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
          set(OGRE_SAMPLES_INCLUDEPATH "/usr/local/share/OGRE/samples/Common/include/") # We could just *assume* that developers uses this basepath : /usr/local

        elseif(EXISTS "/usr/lib/OGRE/cmake")

          set(CMAKE_MODULE_PATH "/usr/lib/OGRE/cmake/;${CMAKE_MODULE_PATH}")
          set(OGRE_SAMPLES_INCLUDEPATH "/usr/share/OGRE/samples/Common/include/") # Otherwise, this one

        else ()
          message(SEND_ERROR "Failed to find module path.")
        endif()


endif(UNIX)

if (NOT CMAKE_BUILD_TYPE)
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
 SET(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
           "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
                 FORCE)
endif ()

set(CMAKE_DEBUG_POSTFIX "_d")

set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/dist")


set(CMAGE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAGE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -g")
set(CMAGE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
set(CMAGE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -O3")

find_package(OGRE REQUIRED)

#if(NOT "${OGRE_VERSION_NAME}" STREQUAL "Cthugha")
#  message(SEND_ERROR "You need Ogre 1.7 Cthugha to build this.")
#endif()

find_package(OIS REQUIRED)

if(NOT OIS_FOUND)
  message(SEND_ERROR "Failed to find OIS.")
endif()

if(OGRE_Terrain_FOUND)
  set( OGRE_LIBRARIES ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARIES})
else(OGRE_Terrain_FOUND)
  message(SEND_ERROR "OgreTerrain Library not found.")
endif(OGRE_Terrain_FOUND)

# Find Boost
if (NOT OGRE_BUILD_PLATFORM_IPHONE)
        if (WIN32 OR APPLE)
                set(Boost_USE_STATIC_LIBS TRUE)
        else ()
                # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64bit
                set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
        endif ()
        if (MINGW)
                # this is probably a bug in CMake: the boost find module tries to look for
                # boost libraries with name libboost_*, but CMake already prefixes library
                # search names with "lib". This is the workaround.
                set(CMAKE_FIND_LIBRARY_PREFIXES ${CMAKE_FIND_LIBRARY_PREFIXES} "")
        endif ()
        set(Boost_ADDITIONAL_VERSIONS "1.44" "1.44.0" "1.42" "1.42.0" "1.41.0" "1.41" "1.40.0" "1.40" "1.39.0" "1.39" "1.38.0" "1.38" "1.37.0" "1.37" )
        # Components that need linking (NB does not include header-only components like bind)
        set(OGRE_BOOST_COMPONENTS thread date_time)
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
        if (NOT Boost_FOUND)
                # Try again with the other type of libs
                set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
                find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
        endif()
        find_package(Boost QUIET)

        # Set up referencing of Boost
        include_directories(${Boost_INCLUDE_DIR})
        add_definitions(-DBOOST_ALL_NO_LIB)
        set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${Boost_LIBRARIES})
endif()


find_package(CEGUI REQUIRED)
if(NOT CEGUI_FOUND)
 message(SEND_ERROR "Failed to find CEGUI.")
endif()
if(NOT CEGUI_OgreRenderer_FOUND)
 message(SEND_ERROR "Failed to find CEGUI_OgreRenderer.")
endif()



set(HDRS
        ./src/viqo/managers/TemplateManager.hpp
        ./src/viqo/managers/SceneManager.hpp
        ./src/viqo/managers/GameObjectManager.hpp
        ./src/viqo/singletons/SinSceneManager.hpp
        ./src/viqo/stacks/TemplateStack.hpp
        ./src/viqo/singletons/SinSceneStack.hpp
        ./src/viqo/gameworld/MovableGameObject.hpp
        ./src/viqo/gameworld/GameObject.hpp
        ./src/viqo/gameworld/GameRoadGraph.hpp
        ./src/viqo/gameworld/GameRoadSystem.hpp
        ./src/viqo/gameworld/GameBuildingSystem.hpp
        ./src/viqo/gameworld/GameTerrain.hpp
        ./src/viqo/GameCamera.hpp
        ./src/viqo/Scene.hpp
        ./src/viqo/scenes/TestScene.hpp
        ./src/viqo/scenes/GameScene.hpp
        ./src/viqo/scenes/IntroScene.hpp
)

set(SRCS
        ./src/viqo/managers/SceneManager.cpp
        ./src/viqo/managers/GameObjectManager.cpp
        ./src/viqo/gameworld/MovableGameObject.cpp
        ./src/viqo/gameworld/GameObject.cpp
        ./src/viqo/gameworld/GameRoadGraph.cpp
        ./src/viqo/gameworld/GameRoadSystem.cpp
        ./src/viqo/gameworld/GameBuildingSystem.cpp
        ./src/viqo/gameworld/GameTerrain.cpp
        ./src/viqo/GameCamera.cpp
        ./src/viqo/Scene.cpp
        ./src/viqo/scenes/TestScene.cpp
        ./src/viqo/scenes/GameScene.cpp
        ./src/viqo/scenes/IntroScene.cpp
        ./src/viqo/main.cpp
)

include_directories( ${OIS_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIRS}
        ${CEGUI_INCLUDE_DIRS}
        ${CEGUI_OgreRenderer_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}/include
)

add_executable(Viqo WIN32 ${HDRS} ${SRCS})


set_target_properties(Viqo PROPERTIES DEBUG_POSTFIX _d)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
add_custom_command(
        OUTPUT ${CMAKE_SOURCE_DIR}/lib/libopensteer.a
        COMMAND make &&cp objs_optimized/libopensteer.a ${CMAKE_SOURCE_DIR}/lib/libopensteer.a
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/src/opensteer/linux
)
add_custom_target(
        libopensteer
        DEPENDS ${CMAKE_SOURCE_DIR}/lib/libopensteer.a
)

set(OGRE_LIBRARIES ${OGRE_LIBRARIES} ${CMAKE_SOURCE_DIR}/lib/libopensteer.a)

add_dependencies(Viqo libopensteer)

target_link_libraries(Viqo ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${CEGUI_LIBRARIES} ${CEGUI_OgreRenderer_LIBRARIES})


file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/dist/bin)

# post-build copy for win32
if(WIN32 AND NOT MINGW)
        add_custom_command( TARGET Viqo PRE_BUILD
                COMMAND if not exist .\\dist\\bin mkdir .\\dist\\bin )
        add_custom_command( TARGET Viqo POST_BUILD
                COMMAND copy \"$(TargetPath)\" .\\dist\\bin )
endif(WIN32 AND NOT MINGW)

if(MINGW OR UNIX)
        set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/dist/bin)
endif(MINGW OR UNIX)

if(WIN32)

        install(TARGETS Viqo
                RUNTIME DESTINATION bin
                CONFIGURATIONS All)

        install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
                DESTINATION .
                CONFIGURATIONS Release RelWithDebInfo Debug
        )

#install(FILES ${CMAKE_SOURCE_DIR}/plugins.cfg
#${CMAKE_SOURCE_DIR}/resources.cfg
#DESTINATION bin
#CONFIGURATIONS Release RelWithDebInfo
#)

        install(FILES ${CMAKE_SOURCE_DIR}/plugins_d.cfg
                ${CMAKE_SOURCE_DIR}/resources_d.cfg
                DESTINATION bin
                CONFIGURATIONS Debug
        )

        # NOTE: for the 1.7.1 sdk the OIS dll is called OIS.dll instead of libOIS.dll
        # so you'll have to change that to make it work with 1.7.1
        install(FILES ${OGRE_PLUGIN_DIR_REL}/OgreMain.dll
                ${OGRE_PLUGIN_DIR_REL}/RenderSystem_Direct3D9.dll
                ${OGRE_PLUGIN_DIR_REL}/RenderSystem_GL.dll
                ${OGRE_PLUGIN_DIR_REL}/libOIS.dll
                DESTINATION bin
                CONFIGURATIONS Release RelWithDebInfo
        )

        install(FILES ${OGRE_PLUGIN_DIR_DBG}/OgreMain_d.dll
                ${OGRE_PLUGIN_DIR_DBG}/RenderSystem_Direct3D9_d.dll
                ${OGRE_PLUGIN_DIR_DBG}/RenderSystem_GL_d.dll
                ${OGRE_PLUGIN_DIR_DBG}/libOIS_d.dll
                DESTINATION bin
                CONFIGURATIONS Debug
        )

   # as of sdk 1.7.2 we need to copy the boost dll's as well
   # because they're not linked statically (it worked with 1.7.1 though)
   install(FILES ${Boost_DATE_TIME_LIBRARY_RELEASE}
      ${Boost_THREAD_LIBRARY_RELEASE}
      DESTINATION bin
      CONFIGURATIONS Release RelWithDebInfo
   )

   install(FILES ${Boost_DATE_TIME_LIBRARY_DEBUG}
      ${Boost_THREAD_LIBRARY_DEBUG}
      DESTINATION bin
      CONFIGURATIONS Debug
   )
endif(WIN32)

if(UNIX)

        install(TARGETS Viqo
                RUNTIME DESTINATION bin
                CONFIGURATIONS All)

        install(DIRECTORY ${CMAKE_SOURCE_DIR}/assets
                DESTINATION .
                CONFIGURATIONS Release RelWithDebInfo Debug
        )

        install(DIRECTORY ${CMAKE_SOURCE_DIR}/lib
                DESTINATION .
                CONFIGURATIONS Release RelWithDebInfo Debug
        )


#install(FILES ${CMAKE_SOURCE_DIR}/plugins.cfg
#${CMAKE_SOURCE_DIR}/resources.cfg
#DESTINATION bin
#CONFIGURATIONS Release RelWithDebInfo Debug
#)

endif(UNIX)
