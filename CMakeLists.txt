cmake_minimum_required(VERSION 3.5) #Проверка версии CMake.

set(PROJECT_NAME Lists) #Создаем обычную локальную переменную с именем проекта
project(${PROJECT_NAME}) # Название проекта


set(application ListsAPP) #Переменная с именем приложения

set(tlistslib TListsLibrary)


set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(NOT CMAKE_BUILD_TYPE) # проверка имеющихся у компилятора вариантов сборки
  set(CMAKE_BUILD_TYPE Release) #Если нет то устанавливаем Release
endif()

if(WIN32)
    add_compile_options(/utf-8)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/TLists)

add_subdirectory(TLists)
add_subdirectory(tests)
add_subdirectory(main)
add_subdirectory(gtest)