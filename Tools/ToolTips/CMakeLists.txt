#*================================C++========================================*\
#      Create: 2018/03/19
#      Creator: Huang Huakun
#      File Name: CMakeLists.txt
#      Platform: windows/linux
#      Version: v1.0
#
#      Describe: The CmakeLists file.
#                add the tooltips for current code. the tips is the OpenFoam code
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

cmake_minimum_required( VERSION 2.8)

INCLUDE_DIRECTORIES(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR})

add_executable( ToolTips ToolTipsForCode.cpp )
