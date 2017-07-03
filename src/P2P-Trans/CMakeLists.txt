cmake_minimum_required (VERSION 3.5)
project (P2P-Transfer)

find_package (Qt5 COMPONENTS Network Widgets)

file (GLOB TRANS_SRC
    Code/Trans/TransP2P/*.hh
    Code/Trans/TransP2P/*.cc)

set (QT_TRANS
    Code/Trans/TransP2P/Server.hh
    Code/Trans/TransP2P/Income.hh
    Code/Trans/TransP2P/Client.hh)
qt5_wrap_cpp (QT_TRANS_MOC ${QT_TRANS})

set (VIEW_SRC
    Code/View/ViewQt/ViewQt.hh
    Code/View/ViewQt/ViewQt.cc)

set (QT_VIEW
    Code/View/ViewQt/ViewQt.hh)
qt5_wrap_cpp (QT_VIEW_MOC ${QT_VIEW})

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
include_directories (Trans ${Qt5Network_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries (Trans ${Qt5Network_LIBRARIES} ${Qt5Widgets_LIBRARIES})

add_library (View SHARED ${VIEW_SRC} ${QT_VIEW_MOC})
include_directories (View ${Qt5Widgets_INCLUDE_DIRS})
target_link_libraries (View ${Qt5Widgets_LIBRARIES})

add_library (Conf SHARED ${CONF_SRC})

add_executable (${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries (${PROJECT_NAME} Trans View Conf)

file (COPY Code/Conf/ConfJson/config.json
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file (COPY ${QT_PATH}/bin/Qt5Core.dll ${QT_PATH}/bin/Qt5Network.dll
    ${QT_PATH}/bin/Qt5Widgets.dll ${QT_PATH}/bin/Qt5Gui.dll
    DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
