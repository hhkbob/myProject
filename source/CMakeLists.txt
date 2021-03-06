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
pv_find_package_qt(qt_targets
  QT4_COMPONENTS QtGui
  QT5_COMPONENTS Widgets)

find_path( QScintilla_DIR  QScintilla /../QScintilla )
if( NOT QScintilla_DIR )
   message(FATAL_ERROR "QScintilla_DIR not found")
endif( NOT QScintilla_DIR )

# include the QScintilla_INCLUDE dir
include_directories( ${QScintilla_DIR}/Qt4Qt5  )

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
	   #Editor/codeeditor.h
	   E#ditor/config.h
	   #Editor/gotolinedialog.h
	   #Editor/notepad.h
	   #Editor/perferencedialog.h
	   #Editor/searchdialog.h
	   #Editor/textview.h
	   Editor/EditorAction.h
	   Editor/Editor.h
       #Core/myui.h
       #Core/mytoolbar.h
       #Core/qmenuButton.h
	   Core/RunCommand.h
	   ToolBar/mainToolBar.h
	   Mesh/BlockMesh.h
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
	   #Editor/codeedit.cpp
	   #Editor/config.cpp
	   #Editor/gotolinedialog.cpp
	   #Editor/notepad.cpp
	   #Editor/perferencedialog.cpp
	   #Editor/searchdialog.cpp
	   #Editor/textview.cpp
	   Editor/EditorAction.cpp
	   Editor/Editor.cpp
       #Core/myui.cpp
       #Core/mytoolbar.cpp
       #Core/qmenuButton.cpp
	   Core/RunCommand.cpp
	   ToolBar/mainToolBar.cpp
	   Mesh/BlockMesh.cpp
           
    )
FILE( GLOB UI_FILES
       CFDbeta.ui
	   CFDMainWindow/initwindow/configuration.ui
	   Mesh/BlockMesh.ui
	   #Editor/gotolinedialog.ui
	   #Editor/perferencedialog.ui
	   #Editor/searchdialog.ui
       #Core/myui.ui
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
       #Editor/codeeditor.cpp
	   #Editor/config.cpp
	   #Editor/gotolinedialog.cpp
	   #Editor/notepad.cpp
	   #Editor/perferencedialog.cpp
	   #Editor/searchdialog.cpp
	   #Editor/textview.cpp
	   #Editor/codeeditor.h
	   #Editor/config.h
	   #Editor/gotolinedialog.h
	   #Editor/notepad.h
	   #Editor/perferencedialog.h
	   #Editor/searchdialog.h
	   #Editor/textview.h
	   Editor/EditorAction.h
	   Editor/EditorAction.cpp
	   Editor/Editor.cpp
	   Editor/Editor.h
    )
FILE( GLOB Core
       #Core/myui.cpp
       #Core/myui.h
       #Core/mytoolbar.cpp
       #Core/mytoolbar.h
       #Core/qmenuButton.h
       #Core/qmenuButton.cpp
	   Core/RunCommand.h
	   Core/RunCommand.cpp
     )
FILE( GLOB ToolBar
       ToolBar/mainToolBar.cpp
	   ToolBar/mainToolBar.h
	 )
if (PARAVIEW_QT_VERSION VERSION_GREATER "4")
  qt5_wrap_cpp(MOC_BUILT_SOURCES
    ${SOURCE_FILES})
  qt5_wrap_ui(UI_BUILT_SOURCES
    ${UI_FILES})
  qt5_add_resources( res images.qrc )
else()
  qt4_wrap_cpp(MOC_BUILT_SOURCES
    ${SOURCE_FILES})
  qt4_wrap_ui(UI_BUILT_SOURCES
    ${UI_FILES})
  qt4_add_resources( res images.qrc )
endif()


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

if( WIN32 )
  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")
endif()

target_link_libraries(FlowCD LINK_PRIVATE ${qt_targets})
#target_link_libraries( FlowCD Qt5::Gui Qt5::Core Qt5::Widgets)
target_link_libraries( FlowCD ${VTK_LIBRARIES} )  
if(WIN32)
  if (PARAVIEW_QT_VERSION VERSION_GREATER "4")
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/qscintilla2_qt5d.lib )  
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/qscintilla2_qt5.lib ) 
  else()
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/qscintilla2_qt4d.lib )  
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/qscintilla2_qt4.lib ) 
  endif()
else()
  if (PARAVIEW_QT_VERSION VERSION_GREATER "4")
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/libqscintilla2_qt5.so)  
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/Release/libqscintilla2_qt5.so ) 
  else()
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/libqscintilla2_qt4.so)  
    target_link_libraries( FlowCD ${QScintilla_DIR}/lib/Release/libqscintilla2_qt4.so )  
  endif()
endif() 


                                            
