cmake_minimum_required(VERSION 3.20)

project(chip8-emulator)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_EXPORT_COMPILE_COMMANDS_COMPATIBILITY ON)

set(SOURCES 
  ${CMAKE_SOURCE_DIR}/lib/cpu.cpp
  ${CMAKE_SOURCE_DIR}/src/main.cpp
  )


include_directories(include)
add_executable(main ${SOURCES})

set(SDL_LIBRARIES
  ${CMAKE_SOURCE_DIR}/lib/libSDL2main.a
  ${CMAKE_SOURCE_DIR}/lib/libSDL2.dll.a
  ${CMAKE_SOURCE_DIR}/lib/libSDL2.a
  )

target_link_options(main PRIVATE "-lmingw32")
target_link_libraries(main PRIVATE
  ${SDL_LIBRARIES}
  )

