cmake_minimum_required(VERSION 4.1.2)
project(minsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Werror)

add_library(minsh_lib
    src/variables.cpp
    src/tokenizer.cpp
    src/parser.cpp
    src/builtins.cpp
    src/executor.cpp
)

target_include_directories(minsh_lib PUBLIC include)

add_executable(minsh src/main.cpp)
target_link_libraries(minsh PRIVATE minsh_lib readline)