# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.



FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)

# file(GLOB_RECURSE engine_sources ${CMAKE_SOURCE_DIR}/src/Engine/*.*)
# list(REMOVE_ITEM app_sources ${engine_sources})


# message(STATUS "${excluded_sources}")

# message(STATUS "Original")
# message(STATUS "${app_sources}")

# message(STATUS "Edited")
# message(STATUS "${app_sources}")

idf_component_register(SRCS ${app_sources})
