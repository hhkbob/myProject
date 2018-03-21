/*================================C++========================================*\
Create: 2018/03/20
Creator: Huang Huakun
File Name: Mesh.h
Platform: windows/linux
Version: v1.0

Describe: The header of the software Mesh source file.

Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
CopyRight:
*/

#ifndef  BLOCKMESH_H
#define  BLOCKMESH_H
#include <QMainWindow>

namespace Ui {
	class BlockMesh;
}

class BlockMesh : public QMainWindow
{
	Q_OBJECT
public:
	BlockMesh(QWidget *parent=0);
	~BlockMesh();
	void ShowVTKView();

private:
	Ui::BlockMesh *ui;
};
#endif // ! MESH_H

