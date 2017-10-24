cmake_minimum_required(VERSION 2.8.12)
project(RunnerMycroft)

find_package(ECM 5.12.0 REQUIRED NO_MODULE)
set (CMAKE_MODULE_PATH
     ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${CMAKE_MODULE_PATH}
)


# Find the required Libaries
find_package (Qt5 ${QT_MIN_VERSION} REQUIRED CONFIG COMPONENTS Widgets Core Network Quick QuickWidgets WebSockets)
find_package (KF5 ${KF5_MIN_VERSION} REQUIRED COMPONENTS I18n Service Runner TextWidgets ConfigWidgets PlasmaQuick)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

set(krunner_mycroft_SRCS
    mycroft.cpp
)

# Now make sure all files get to the right place
add_library(krunner_mycroft MODULE ${krunner_mycroft_SRCS})
target_link_libraries(krunner_mycroft KF5::Runner Qt5::Widgets Qt5::WebSockets
                      KF5::I18n
                      KF5::Service
                      KF5::ConfigWidgets
                      KF5::Plasma)
                      
#add_dependencies(krunner_mycroft)
                      
install(TARGETS krunner_mycroft DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES plasma-runner-mycroft.desktop DESTINATION ${SERVICES_INSTALL_DIR})
install( DIRECTORY image/breeze DESTINATION ${ICON_INSTALL_DIR})
install( DIRECTORY image/breeze-dark DESTINATION ${ICON_INSTALL_DIR})
