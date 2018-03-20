/*================================C++========================================*\
Create: 2018/03/20
Creator: Huang Huakun
File Name: Mesh.h
Platform: windows/linux
Version: v1.0

Describe:  software Mesh source file.

Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
CopyRight:
*/

#include "BlockMesh.h"
#include "ui_BlockMesh.h"

BlockMesh::BlockMesh( QWidget *parent ) :QMainWindow(parent),
ui(new Ui::BlockMesh)
{
	ui->setupUi(this);
	
}

BlockMesh::~BlockMesh()
{
	delete ui;
}
void BlockMesh::ShowVTKView()
{

}