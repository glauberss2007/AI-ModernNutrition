cmake_minimum_required(VERSION 3.17)

project(modern-nutrition
        DESCRIPTION "Optimal Nutrition"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(cmake/CPM.cmake)

CPMAddPackage(
        NAME matplot
        GITHUB_REPOSITORY alandefreitas/matplotplusplus
        GIT_TAG origin/master # or whatever tag you want
)

CPMAddPackage(
        NAME cereal
        VERSION 1.2.2
        GITHUB_REPOSITORY USCiLab/cereal
        OPTIONS
        "SKIP_PORTABILITY_TEST ON"
        "JUST_INSTALL_CEREAL ON"
        set(CMAKE_CXX_STANDARD 17)

)

option(BUILD_WITH_PEDANTIC_WARNINGS "Use pedantic warnings. Developers should leave this ON." ON)

set(LOCAL_PATH_EXPORTS "${CMAKE_CURRENT_SOURCE_DIR}/exports/")
set(LOCAL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/nutrition_facts.txt")
add_definitions(-DLOCAL_PATH=\"${LOCAL_PATH}\")
add_definitions(-DLOCAL_PATH_EXPORTS=\"${LOCAL_PATH_EXPORTS}\")
#add_subdirectory(include)
add_subdirectory(source)
add_subdirectory(test)