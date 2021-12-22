# This file was automatically generated for projects
# without default 'CMakeLists.txt' file.



FILE(GLOB_RECURSE app_sources ${CMAKE_SOURCE_DIR}/src/*.*)


file(GLOB_RECURSE event_sources ${CMAKE_SOURCE_DIR}/src/Events/*.*)
list(REMOVE_ITEM app_sources ${event_sources})

file(GLOB_RECURSE pyro_sources ${CMAKE_SOURCE_DIR}/src/Pyros/*.*)
list(REMOVE_ITEM app_sources ${pyro_sources})


# message(STATUS "${excluded_sources}")

# message(STATUS "Original")
# message(STATUS "${app_sources}")

# message(STATUS "Edited")
# message(STATUS "${app_sources}")

idf_component_register(SRCS ${app_sources})
