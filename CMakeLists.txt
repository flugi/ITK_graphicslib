cmake_minimum_required( VERSION 2.6 )

project( ITK_graphicslib )

find_package( SDL REQUIRED )
find_package( SDL_ttf REQUIRED )

if (NOT SDLTTF_FOUND)
  message ( FATAL_ERROR "SDL_ttf not found!" )
endif ()



link_libraries (
  ${SDL_LIBRARY}
  ${SDLTTF_LIBRARY}
  SDLmain
)

add_library( graphics graphics.cpp )
add_executable( test_graphics main.cpp )



target_link_libraries( test_graphics graphics )

