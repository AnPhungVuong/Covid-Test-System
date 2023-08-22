# CMakeList.txt : CMake project for An_Vuong_FinalProject, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("An_Vuong_FinalProject")

# Add source to this project's executable.
add_executable (An_Vuong_FinalProject "An_Vuong_FinalProject.cpp" )
target_include_directories(An_Vuong_FinalProject PUBLIC "./include")
target_link_directories(An_Vuong_FinalProject PUBLIC "./lib")
target_link_libraries(An_Vuong_FinalProject sfml-main-d sfml-graphics-d sfml-system-d sfml-window-d tgui-d)


# TODO: Add tests and install targets if needed.
