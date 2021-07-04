project(flatpak-portal-test-kde)

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
set(QT_MIN_VERSION "5.4.0")

################# set KDE specific information #################

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(ECMPackageConfigHelpers)
include(ECMOptionalAddSubdirectory)
include(FeatureSummary)

include(FindPkgConfig)

find_package(GLIB2 REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED COMPONENTS
    Core
    DBus
    Widgets
)

find_package(KF5 REQUIRED
    I18n
    KIO
    Notifications
)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0 -DQT_NO_KEYWORDS)
add_definitions(-DQT_USE_FAST_CONCATENATION -DQT_USE_FAST_OPERATOR_PLUS)
remove_definitions(-DQT_NO_CAST_FROM_ASCII -DQT_STRICT_ITERATORS -DQT_NO_CAST_FROM_BYTEARRAY)

add_subdirectory(src)

feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)
