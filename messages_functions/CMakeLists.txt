cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
set(PROJECT messages_functions)
project(${PROJECT} LANGUAGES CXX)

#################################################################################
# Sources
#################################################################################
set(SOURCES
  src/messages_functions.cpp
)

set(PUBLIC_HEADERS
  src/messages_functions.h
)

add_library(${PROJECT} STATIC ${SOURCES})

#################################################################################
# Properties
#################################################################################

set_property(TARGET ${PROJECT} PROPERTY CXX_STANDARD 11)

target_compile_options(${PROJECT} PRIVATE
  -Wall -Werror
  -fno-strict-aliasing
)

#if(UNIX AND (X86_64 OR X86)) need detect X86
  target_compile_options(${PROJECT} PUBLIC -m32)
  set_target_properties(${PROJECT} PROPERTIES LINK_FLAGS -m32)
#endif()

set_target_properties(${PROJECT} PROPERTIES DEBUG_POSTFIX "_debug")

#################################################################################
# Libraries
#################################################################################

###############################
## Boost
###############################
if (NOT BOOST_ROOT)
  set(BOOST_ROOT "../../../../libraries/3rdparty/boost")
endif()

if (NOT BOOST_ROOT STREQUAL BOOST_ROOT_INTERNAL)
  unset(Boost_FOUND)
  unset(Boost_INCLUDE_DIRS)
  unset(Boost_LIBRARIES)
endif()
set(BOOST_ROOT_INTERNAL ${BOOST_ROOT} CACHE INTERNAL "" FORCE)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_NO_BOOST_CMAKE ON)

set(BOOST_COMPONENTS)
list(APPEND BOOST_COMPONENTS system thread)

find_package(Boost 1.60.0 COMPONENTS ${BOOST_COMPONENTS})
if(Boost_FOUND)
  target_include_directories(${PROJECT} PRIVATE ${Boost_INCLUDE_DIRS})
  target_link_libraries(${PROJECT} PUBLIC ${Boost_LIBRARIES})
  if(WIN32)
    target_compile_definitions(${PROJECT}
      PRIVATE _WIN32_WINNT=0x0501
      PRIVATE WINVER=0x0501
    )
  endif(WIN32)
else()
  message(SEND_ERROR "Please set or correct BOOST_ROOT, current value: ${BOOST_ROOT}")
endif()

#################################################################################
# Install
#################################################################################

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
target_include_directories(${PROJECT} INTERFACE $<INSTALL_INTERFACE:include>)

install(TARGETS ${PROJECT} EXPORT ${PROJECT} DESTINATION lib)
install(FILES ${PUBLIC_HEADERS} DESTINATION include)
install(EXPORT ${PROJECT} DESTINATION config)
