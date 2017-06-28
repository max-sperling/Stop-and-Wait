cmake_minimum_required (VERSION 3.5)
project (P2P-Transfer)

file (GLOB TRANS_SRC
    Code/Trans/TransP2P/*.hh
    Code/Trans/TransP2P/*.cc)

set (VIEW_SRC
    Code/View/ViewCLI/ViewCLI.hh
    Code/View/ViewCLI/ViewCLI.cc)

file (GLOB CONF_SRC
    Code/Conf/ConfFile/*.hh
    Code/Conf/ConfFile/*.cc)

set (MAIN_SRC
    Code/Main.cc
    Code/Trans/ITrans.hh
    Code/View/IView.hh
    Code/Conf/IConf.hh)

add_library (Trans SHARED ${TRANS_SRC})
add_library (View SHARED ${VIEW_SRC})
add_library (Conf SHARED ${CONF_SRC})
add_executable (${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries (${PROJECT_NAME} Trans View Conf)