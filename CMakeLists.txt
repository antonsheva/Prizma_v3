cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD 17)
set(EXTRA_COMPONENT_DIRS 
                          main   
                         
                          ../arduino55

)


include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(Prizma_v3)
