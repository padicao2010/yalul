project(yalul)
cmake_minimum_required(VERSION 2.8)

set(MY_ROOT_DIR ${yalul_SOURCE_DIR}/../root)

ADD_SUBDIRECTORY(src)

INSTALL(DIRECTORY test/ DESTINATION ${MY_ROOT_DIR})
