project(yalul)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

set(MY_ROOT_DIR ${yalul_SOURCE_DIR}/../root)

find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

include_directories(${MY_ROOT_DIR}/include)
link_directories(${MY_ROOT_DIR})

add_library(yalul SHARED ${SRC_LIST})
set_target_properties(yalul PROPERTIES VERSION 0.0 SOVERSION 1)
target_link_libraries(yalul ${LUA_LIBRARIES})
target_link_libraries(yalul ui)

#INSTALL(FILES yalul.lua 30log.lua DESTINATION ${MY_ROOT_DIR})

INSTALL(TARGETS yalul
#INSTALL(TARGETS yalul yalul_static
    LIBRARY DESTINATION ${MY_ROOT_DIR}
    ARCHIVE DESTINATION ${MY_ROOT_DIR})
