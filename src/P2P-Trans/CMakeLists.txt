cmake_minimum_required (VERSION 3.5)
project (P2P-Transfer)

find_package (Qt5 COMPONENTS Network)

file (GLOB TRANS_SRC
    Code/Trans/TransP2P/*.hh
    Code/Trans/TransP2P/*.cc)

set (QT_TRANS
    Code/Trans/TransP2P/TransP2P.hh
    Code/Trans/TransP2P/Server.hh
    Code/Trans/TransP2P/Income.hh
    Code/Trans/TransP2P/Client.hh)
qt5_wrap_cpp (QT_TRANS_MOC ${QT_TRANS})

set (VIEW_SRC
    Code/View/ViewCLI/ViewCLI.hh
    Code/View/ViewCLI/ViewCLI.cc)

file (GLOB CONF_SRC
    Code/Conf/ConfJson/*.hh
    Code/Conf/ConfJson/*.cc)

set (MAIN_SRC
    Code/Main.cc
    Code/Trans/ITrans.hh
    Code/View/IView.hh
    Code/Conf/IConf.hh)

set (QT_PATH ${CMAKE_PREFIX_PATH})
add_library (Trans SHARED ${TRANS_SRC} ${QT_TRANS_MOC})
include_directories (Trans ${Qt5Network_INCLUDE_DIRS})
target_link_libraries (Trans ${Qt5Network_LIBRARIES})
file (COPY ${QT_PATH}/bin/Qt5Core.dll ${QT_PATH}/bin/Qt5Network.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library (View SHARED ${VIEW_SRC})

add_library (Conf SHARED ${CONF_SRC})
file (COPY Code/Conf/ConfJson/config.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable (${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries (${PROJECT_NAME} Trans View Conf)
