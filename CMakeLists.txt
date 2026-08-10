cmake_minimum_required(VERSION 3.30)
project(BookDB VERSION 1.0.0 LANGUAGES CXX)

if(MSVC)
    # У MSVC нет отдельного ключа /std:c++26, и CMake не знает, чем включить
    # диалект CXX26 для cl.exe, — генерация обрывается с "does not support this".
    # Значение 23 CMake разворачивает в /std:c++latest, то есть ровно в тот же
    # режим "самый свежий стандарт", который нужен <print> и <flat_map>.
    set(CMAKE_CXX_STANDARD 23)
else()
    set(CMAKE_CXX_STANDARD 26)
endif()
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Все предупреждения - это ошибки
    add_compile_options(-Wfatal-errors -Wall -Werror)

    # Не больше одной ошибки за раз
    add_compile_options(-fmax-errors=1)
endif()

if(MSVC)
    # Исходники лежат в UTF-8 без BOM. Без /utf-8 cl.exe читает их в системной
    # ANSI-кодировке, и результат сборки начинает зависеть от локали машины:
    # на неподходящей кодовой странице появляется C4819, а литералы вроде
    # "Charlotte Brontë" и "рейтинг ≥ 4.5" молча портятся. Ключ фиксирует и
    # исходную, и исполняемую кодировку как UTF-8. На GCC это уже поведение
    # по умолчанию, поэтому Linux-сборка не меняется.
    add_compile_options(/utf-8)
endif()

# Ищем необходимые библиотеки
find_package(GTest REQUIRED)

file(GLOB HEADER_FILES "${CMAKE_SOURCE_DIR}/include/*.hpp")

# Создаём статическую библиотеку
add_library(${PROJECT_NAME}_imp STATIC "${CMAKE_SOURCE_DIR}/src/main.cpp" ${HEADER_FILES})

# Добавляем в проект используемые сторонние библиотеки
target_include_directories(${PROJECT_NAME}_imp PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${OPENSSL_INCLUDE_DIR}
    ${Boost_INCLUDE_DIRS}
)
target_link_libraries(${PROJECT_NAME}_imp PRIVATE ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES})

# Создаём исполняемый таргет и линкуем к нему статическую библиотеку
add_executable(${PROJECT_NAME} "${CMAKE_SOURCE_DIR}/src/main.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_imp)

#
# Тесты
#

# Рекурсивно ищем все используемые в тестах .cpp файлы
file(GLOB TEST_SRC_FILES "${CMAKE_SOURCE_DIR}/tests/*.cpp")

add_executable(${PROJECT_NAME}_tests "${TEST_SRC_FILES}")
target_link_libraries(${PROJECT_NAME}_tests PRIVATE ${PROJECT_NAME}_imp GTest::GTest GTest::Main)
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Включаем тестирование
enable_testing()
add_test(NAME BookDB_Tests COMMAND ${PROJECT_NAME}_tests)
