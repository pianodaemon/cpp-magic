PROJECT( tries )
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.0)


INCLUDE_DIRECTORIES(
    includes
)

SET( CMAKE_CXX_FLAGS "-g -pedantic-errors -Wall -std=c++11" )

SET( CPP
    main.cpp
    trie.cpp
)


ADD_EXECUTABLE(
    ${PROJECT_NAME}
    ${HPP}
    ${CPP}
)

TARGET_LINK_LIBRARIES(
    ${PROJECT_NAME}
)
