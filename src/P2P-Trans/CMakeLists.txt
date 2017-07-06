cmake_minimum_required (VERSION 3.5)
project (P2P-Transfer)

# ***** Setup **************************************************************************************
find_package (Qt5 COMPONENTS Network Widgets)
set (QT_PATH ${CMAKE_PREFIX_PATH})

file (GLOB TRANS_SRC
    Code/Trans/TransP2P/*.hh
    Code/Trans/TransP2P/*.cc)

set (QT_TRANS
    Code/Trans/TransP2P/Server.hh
    Code/Trans/TransP2P/Income.hh
    Code/Trans/TransP2P/Client.hh
    Code/Trans/TransP2P/Outcome.hh)
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
# **************************************************************************************************

# ***** Building ***********************************************************************************
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
    DESTINATION ${CMAKE_BINARY_DIR})
# **************************************************************************************************

# ***** Post-Build *********************************************************************************
set (QT_LIBS_VIEW Qt5Widgets Qt5Gui)
set (QT_LIBS_TRANS Qt5Core Qt5Network)

foreach (FILE ${QT_LIBS_VIEW})
add_custom_command(TARGET View
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${QT_PATH}/bin/${FILE}d.dll> 
    $<$<CONFIG:Release>:${QT_PATH}/bin/${FILE}.dll> 
    $<TARGET_FILE_DIR:View>)
endforeach (FILE)

foreach (FILE ${QT_LIBS_TRANS})
add_custom_command(TARGET Trans
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<$<CONFIG:Debug>:${QT_PATH}/bin/${FILE}d.dll> 
    $<$<CONFIG:Release>:${QT_PATH}/bin/${FILE}.dll> 
    $<TARGET_FILE_DIR:Trans>)
endforeach (FILE)
# **************************************************************************************************
