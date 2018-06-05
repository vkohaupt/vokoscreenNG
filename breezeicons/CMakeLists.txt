cmake_minimum_required(VERSION 3.0)

set(KF5_VERSION "5.45.0") # handled by release scripts
project(BreezeIcons VERSION ${KF5_VERSION})

# ECM setup
include(FeatureSummary)
find_package(ECM 5.45.0  NO_MODULE)
set_package_properties(ECM PROPERTIES TYPE REQUIRED DESCRIPTION "Extra CMake Modules." URL "https://projects.kde.org/projects/kdesupport/extra-cmake-modules")
feature_summary(WHAT REQUIRED_PACKAGES_NOT_FOUND FATAL_ON_MISSING_REQUIRED_PACKAGES)

set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(GtkUpdateIconCache)

# Dependencies
set(REQUIRED_QT_VERSION 5.8.0)

# When cross-compiling, we should compile qrcAlias natively (against a native Qt build)
if (CMAKE_CROSSCOMPILING)
    set(BINARY_ICONS_RESOURCE_OPTION_DEFAULT OFF)
else()
    set(BINARY_ICONS_RESOURCE_OPTION_DEFAULT ON)
endif()

option(BINARY_ICONS_RESOURCE "Install Qt binary resource files containing breeze icons (breeze-icons.rcc, breeze-icons-dark.rcc)" ${BINARY_ICONS_RESOURCE_OPTION_DEFAULT})
option(SKIP_INSTALL_ICONS "Skip installing the icons files" OFF)

if(BINARY_ICONS_RESOURCE)
    find_package(Qt5 NO_MODULE REQUIRED Core)
    add_executable(qrcAlias qrcAlias.cpp)
    target_link_libraries(qrcAlias PUBLIC Qt5::Core)

function(generate_binary_resource target outfile)
    set(RESOURCES_WORKING_DIR ${CMAKE_CURRENT_BINARY_DIR}/res)
    set(RESOURCE_FILE ${RESOURCES_WORKING_DIR}/breeze-${target}.qrc)
    set(BINARY_RESOURCE_FILE ${CMAKE_CURRENT_BINARY_DIR}/breeze-${target}.rcc)

    get_target_property(QT_RCC_EXECUTABLE Qt5::rcc LOCATION)

    add_custom_target(breeze-${target}-mkdir
        COMMAND ${CMAKE_COMMAND} -E make_directory ${RESOURCES_WORKING_DIR}
    )
    add_custom_command(OUTPUT ${RESOURCE_FILE}
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR} ${RESOURCES_WORKING_DIR}
        COMMAND ${CMAKE_COMMAND} -E remove
            ${RESOURCE_FILE}
            ${RESOURCE_FILE}.depends
            ${RESOURCES_WORKING_DIR}/.gitignore
            ${RESOURCES_WORKING_DIR}/CMakeLists.txt
        COMMAND ${QT_RCC_EXECUTABLE} --project -o ${CMAKE_CURRENT_BINARY_DIR}/tmp.qrc
        COMMAND $<TARGET_FILE:qrcAlias> -i ${CMAKE_CURRENT_BINARY_DIR}/tmp.qrc -o ${RESOURCE_FILE}

        WORKING_DIRECTORY ${RESOURCES_WORKING_DIR}
        DEPENDS breeze-${target}-mkdir
    )
    add_custom_command(OUTPUT ${BINARY_RESOURCE_FILE}
        COMMAND ${QT_RCC_EXECUTABLE} --binary
                    -o ${BINARY_RESOURCE_FILE}
                    ${RESOURCE_FILE}
        DEPENDS ${RESOURCE_FILE}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(breeze-${target}-rcc ALL DEPENDS ${BINARY_RESOURCE_FILE})

    set(${outfile} ${BINARY_RESOURCE_FILE} PARENT_SCOPE)
endfunction()
endif()

if(NOT WIN32)
    find_program(SH_EXE sh)
    find_program(XMLLINT_EXE xmllint)
    if(SH_EXE AND XMLLINT_EXE)
        file(GLOB_RECURSE SVGS "*.svg")
        add_custom_target(breeze-validate-svg ALL
            COMMAND ${SH_EXE} ${CMAKE_SOURCE_DIR}/validate_svg.sh
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            SOURCES ${SVGS}
        )
    endif()
endif()

add_subdirectory(autotests)
add_subdirectory(icons)
add_subdirectory(icons-dark)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES INCLUDE_QUIET_PACKAGES)
