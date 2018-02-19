#*================================C++========================================*\
#      Create: 2018/02/09
#      Creator: Huang Huakun
#      File Name: CmakeLists.txt
#      Platform: windows
#      Version: v1.0
#
#      Describe: The software main CmakeLists file.
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

cmake_minimum_required( VERSION 2.8 )
CMAKE_POLICY(SET CMP0020 NEW)

include_directories( 
         source/CFDMainWindow/initwindow
	 soucr/CFDMainWindow
	 source/
	 source/Editor
	 source/OutputMessage
         source/Core
	)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)	 
link_directories( ${PROJECT_BINARY_DIR}/libs )
set_directory_properties(PROPERTIES COMPILE_DEFINITIONS_DEBUG "_DEBUG")
set( LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/libs )
set( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )

# add the subdirectory. Every directory should have the CmakeLists.txt
add_subdirectory( source )
