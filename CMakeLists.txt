cmake_minimum_required( VERSION 2.6 )

project( ITK_graphicslib )

find_package( SDL2 REQUIRED )


link_libraries (
  SDL2
  SDL2_ttf
)

add_library( graphics graphics.cpp )
add_executable( test_graphics main.cpp )



target_link_libraries( test_graphics graphics )

