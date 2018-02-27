#*================================C++========================================*\
#      Create: 2018/02/09
#      Creator: Huang Huakun
#      File Name: CmakeLists.txt
#      Platform: windows
#      Version: v1.0
#
#      Describe: The CFDbeta main CmakeLists file.
#                using vtk7.1 Qt5.6 paraview-5.3.0
#
#      Example:
#
#*===========================================================================*/

#  The code's length should not exceed 80 characters.
#  Every description for a code line should skip two characters.
#  As you can, do not use the void function. A return code is welcome.

#
#    CopyRight:
#

project( FlowCD )

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTOUIC ON )
set( CMAKE_AUTORCC ON )

find_package( ParaView REQUIRED )
include( ${PARAVIEW_USE_FILE} )
find_package( Qt5Widgets )

# include rules for creating custom applications
include(ParaViewBranding)

# Set a consistent MACOSX_RPATH default across all CMake versions.
# When CMake 2.8.12 is required, change this default to 1.
# When CMake 3.0.0 is required, remove this block (see CMP0042).
if(NOT DEFINED CMAKE_MACOSX_RPATH)
  set(CMAKE_MACOSX_RPATH 0)
endif()

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

FILE( GLOB HEADER_FILES
       CFDbeta.h
	   QVTKView.h
	   TabWidget.h
       CFDMainWindow/dir.h
	   CFDMainWindow/FileOpen.h
	   CFDMainWindow/MeshFunction.h
	   CFDMainWindow/initwindow/initwindow.h
	   OutputMessage/OutputMessage.h
	   CFDMainWindow/initwindow/configuration.h
	   Editor/codeeditor.h
	   Editor/config.h
	   Editor/gotolinedialog.h
	   Editor/notepad.h
	   Editor/perferencedialog.h
	   Editor/searchdialog.h
	   Editor/textview.h
	   Editor/EditorAction.h
       Core/myui.h
       Core/mytoolbar.h
       Core/qmenuButton.h
	   Core/RunCommand.h
	   ToolBar/mainToolBar.h
	)
FILE( GLOB SOURCE_FILES
       CFDbeta.cpp
	   QVTKView.cpp
	   TabWidget.cpp
	   CFDMainWindow/FileOpen.cpp
	   CFDMainWindow/MeshFuction.cpp
	   CFDMainWindow/initwindow/initwindow.cpp
	   CFDMainWindow/initwindow/stringToChange.cpp
	   CFDMainWindow/initwindow/initConsole.cpp
	   CFDMainWindow/initwindow/configuration.cpp
       CFDMainWindow/initwindow/initMainWindowAction.cpp
	   OutputMessage/OutputMessage.cpp
	   Editor/codeedit.cpp
	   Editor/config.cpp
	   Editor/gotolinedialog.cpp
	   Editor/notepad.cpp
	   Editor/perferencedialog.cpp
	   Editor/searchdialog.cpp
	   Editor/textview.cpp
	   Editor/EditorAction.cpp
       Core/myui.cpp
       Core/mytoolbar.cpp
       Core/qmenuButton.cpp
	   Core/RunCommand.cpp
	   ToolBar/mainToolBar.cpp
           
    )
FILE( GLOB UI_FILES
       CFDbeta.ui
	   CFDMainWindow/initwindow/configuration.ui
	   Editor/gotolinedialog.ui
	   Editor/perferencedialog.ui
	   Editor/searchdialog.ui
       Core/myui.ui
	 )
	 
FILE( GLOB initwindow
       CFDMainWindow/initwindow/initwindow.h 
       CFDMainWindow/initwindow/initwindow.cpp
	   CFDMainWindow/initwindow/stringToChange.cpp
	   CFDMainWindow/initwindow/initConsole.cpp
	   CFDMainWindow/initwindow/configuration.cpp
	   CFDMainWindow/initwindow/configuration.h
       CFDMainWindow/initwindow/initMainWindowAction.cpp
	)
FILE( GLOB OutputMessage
       OutputMessage/OutputMessage.h 
       OutputMessage/OutputMessage.cpp
	)
FILE( GLOB Editor
       Editor/codeeditor.cpp
	   Editor/config.cpp
	   Editor/gotolinedialog.cpp
	   Editor/notepad.cpp
	   Editor/perferencedialog.cpp
	   Editor/searchdialog.cpp
	   Editor/textview.cpp
	   Editor/codeeditor.h
	   Editor/config.h
	   Editor/gotolinedialog.h
	   Editor/notepad.h
	   Editor/perferencedialog.h
	   Editor/searchdialog.h
	   Editor/textview.h
	   Editor/EditorAction.h
	   Editor/EditorAction.cpp
    )
FILE( GLOB Core
       Core/myui.cpp
       Core/myui.h
       Core/mytoolbar.cpp
       Core/mytoolbar.h
       Core/qmenuButton.h
       Core/qmenuButton.cpp
	   Core/RunCommand.h
	   Core/RunCommand.cpp
     )
FILE( GLOB ToolBar
       ToolBar/mainToolBar.cpp
	   ToolBar/mainToolBar.h
	 )

qt5_wrap_ui( uic ${UI_FILES} )	
qt5_wrap_cpp( moc ${SOURCE_FILES} )
qt5_add_resources( res images.qrc )

source_group( "initwindow" FILES ${initwindow} )	
source_group( "OutputMessage" FILES ${OutputMessage} )
source_group( "UI" FILES ${UI_FILES} )
source_group( "Editor" FILES ${Editor} )
source_group( "Core" FILES ${Core} )
source_group( "ToolBar" FILES ${ToolBar} )

build_paraview_client( FlowCD 
      TITLE "FlowCD"
	  ORGANIZATION "Test"
	  VERSION_MAJOR 1
	  VERSION_MINOR 1
	  VERSION_PATCH 1
	  PVMAIN_WINDOW FlowCD
	  PVMAIN_WINDOW_INCLUDE CFDbeta.h
	  GUI_CONFIGURATION_XMLS
	    ${CMAKE_CURRENT_SOURCE_DIR}/ParaViewSources.xml
	    ${CMAKE_CURRENT_SOURCE_DIR}/ParaViewFilters.xml
	  SOURCES ${SOURCE_FILES} ${HEADER_FILES} ${UI_FILES} images.qrc 
)
# set the target properties for the project. Do not edit this line
# use the console to launch our soft
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE") 
target_link_libraries( FlowCD Qt5::Widgets Qt5::Core Qt5::Gui )  
target_link_libraries( FlowCD ${VTK_LIBRARIES} )    
                                                 
