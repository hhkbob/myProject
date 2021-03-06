#include "myui.h"
#include "ui_myui.h"
#include <QFile>
#include <QtPrintSupport/QPrintDialog>
#include <QColorDialog>
#include <QFontDialog>
#include <QtPrintSupport/QPrintEngine>
#include <QTextStream>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QTabWidget>
#include <QTextDocumentWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QKeySequence>
#include <QMenuBar>
#include <QToolBar>
#include <QFont>
#include <QClipboard>
#include <QComboBox>
#include <QFontComboBox>
#include <QFontDatabase>
#include <QActionGroup>
#include <QTextCharFormat>
#include <QMimeData>
#include <QScrollBar>
#include <QWidget>




myUI::myUI(Config *config, QTabWidget *tab, QTreeView *view, QTextBrowser *text, initWindow *init, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myUI)
{

    ui->setupUi(this);
    textBrowser = text ;
    treeView = view ;
    editorToolBar = ui->editorToolBar ;
    mainToolBar = ui->groupBox_3 ;
    newNumber = 0 ;
    initWin = init ;   //付给初始化窗口
    tabWidget = tab ;
    connect(tabWidget,SIGNAL(currentChanged(int)),this,SLOT(currentChanged(int)));
    connect(tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(fileClose(int)));
    

    comboFont = new QFontComboBox ;
    ui->gridLayout->addWidget( comboFont);
    comboFont->setCurrentFont(QFont(config->fontFamily));
    ui->comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        ui->comboSize->addItem(QString::number(size));
    ui->comboSize->setCurrentIndex(
                ui->comboSize->findText(tr("%1").arg(config->fontSize)));
    connect(comboFont, SIGNAL(activated(QString)),
                SLOT(textFontFamily(QString)));
    connect(ui->comboSize, SIGNAL(activated(QString)), SLOT(textSize(QString)));

    BoldOrNor = 0 ;
    italic = 0 ;
    underline = 0 ;
    pathShow = 1 ;

    oldPathShow = initWin->FilePathChange(initWin->workPath ) ;
    logFileOnOROff = 1 ;

    addAct() ;
    addConnect();
    addMenu() ;
}

//添加动作
void myUI::addAct()
{
    ui->copy->setFlat( false );
    ui->paste->setFlat( false ) ;
    ui->close->setFlat( false ) ;
    ui->closeAll->setFlat( false ) ;
    ui->SaveAll->setFlat( false ) ;
}

//添加联系
void myUI::addConnect()
{

}

//添加菜单
void myUI::addMenu()
{
    ui->textBox->setStyleSheet("border:0px,0px,0px,0px;");
    ui->ToolBox->setStyleSheet("border:0px,0px,0px,0px;");
    ui->fontBox->setStyleSheet("border:0px,0px,0px,0px;");
    ui->FileGroup->setStyleSheet("border:10px,10px,10px,10px; border-color: rgb(85, 85, 127);");
    ui->ToolGroup->setStyleSheet("border:10px,10px,10px,10px; border-color: rgb(85, 85, 127);");
    //ui->textBox->widget = ui->FileGroup ;                                //文本菜单
    //ui->ToolBox->widget = ui->ToolGroup ;                                //工具菜单
    //ui->grid->widget=ui->ToolGroup_2 ;

    fontCase = new QMenu ;
    //fontCase->addAction(QString("大写转换"),this, SLOT(UperCase())) ;
    //fontCase->addAction(QString("小写转换"), this, SLOT(LowerCase())) ;

}

myUI::~myUI()
{
    delete ui;
    delete textBoxAct ;
    delete comboFont ;
    delete fontCase ;
    //delete config ;
    //delete textMenu ;
}

void myUI::fileClose(int index)
{
	{
		bool mark = maybeSave(index);
#if _DEBUG
		printf("\nThis a information for developer\n\n");
		printf("In myui.cpp, the fileClose maybeSave function\n");
		printf("The close index = %d\n", index);
		printf("the files count = 1\n");
		if (mark)
			printf("The bool value is 1\n\n");
		else
			printf("The bool value is 0\n\n");
		printf("Debug Exit\n");
#endif
		if (mark)
		{
			if (openedFiles.count() == 1)
			{
#if _DEBUG
				Output.fileCloseMaybeSaveMessage(index);
#endif
				on_newFile_clicked();
				openedFiles.removeAt(0);
				tabWidget->removeTab(0);
			}
			else
			{
#if _DEBUG
				printf("\nThis ia a information for developer\n\n");
				printf("In muui.cpp, the fileClose function\n");
				printf("the close index =%d\n", index);
				printf("the files count =%d\n\n", openedFiles.count());
				printf("Debug Exit\n");
#endif
				openedFiles.removeAt(index);
				tabWidget->removeTab(index);
			}
		}
	}
}

//Tab发生改变时触发的槽
void myUI::currentChanged(int index)
{
    if (index == -1)
    {
        newNumber = 0;
        if (config->showReadme)
            showReadme();
        else
            on_newFile_clicked();
        return;
    }
    updateActions();
}

//创建新的Tab（用于打开文件）
void myUI::newTab(const QString& fileName, QFile& file)
{
    openedFiles << fileName;
    CodeEditor *text = new CodeEditor ;
    tabWidget->addTab(text, QFileInfo(fileName).fileName());
    QByteArray data = file.readAll();
    text->setPlainText( QString::fromLocal8Bit(data) );
    text->setFont(QFont( "Helvetica", 10));
    tabWidget->setCurrentWidget(text);
}

//打开文件
void myUI::openFile(QString fileName)
{
    int index = openedFiles.indexOf(fileName);
    if (index != -1)
        tabWidget->setCurrentIndex(index);
    else
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly))
            newTab(fileName, file);
    }
}
void myUI::on_open_clicked()
{
    QStringList files;
    files = QFileDialog::getOpenFileNames(this, tr("Open files..."), QString(),
                                          tr("All Files(*);;Text Files(*.txt)"));
    QString fileName;
    if (files.count())
    {
        for (int i = 0; i < files.count(); i++)
        {
            fileName = files.at(i);
            if (QFile::exists(fileName))
            {
                QFile file(fileName);
                if (file.open(QFile::ReadOnly))
                {
                    if (openedFiles.contains(fileName))
                        continue;
                    newTab(fileName, file);
                }
            }
        }
    }
}

void myUI::on_newFile_clicked()
{
    QString fileName = tr("New %1").arg(++newNumber);
    openedFiles << fileName;
    tabWidget->setCurrentIndex(
                tabWidget->addTab(new CodeEditor, fileName));
}

void myUI::on_saveFile_clicked()
{
    fileSave(tabWidget->currentIndex());
}

bool myUI::fileSave(int index)
{
    CodeEditor *text = static_cast<CodeEditor*>(tabWidget->widget(index));
    QString fileName = openedFiles.at(index);
    if (!fileName.contains("/") && !fileName.contains("\\"))
        return fileSaveAs(index);

    QFile file( fileName ) ;
    file.open(QIODevice::ReadWrite ) ;
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << text->toPlainText();
    out.flush();
    QApplication::restoreOverrideCursor();
    file.close() ;
    text->document()->setModified(false);
    tabWidget->setCurrentWidget(text);
    setWindowTitle(tr("CFD 2.0(%1)").arg(fileName));
    bool success = 1 ;
    return success;
}

bool myUI::fileSaveAs(int index)
{
    QString fn = QFileDialog::getSaveFileName(this, tr("Save as..."), QString(),
                                              tr("Plain text Files(*.txt);;All Files (*)"));
    if (fn.isEmpty())
        return false;
    if (!fn.endsWith(".txt", Qt::CaseInsensitive))
        fn += ".txt";

    openedFiles.replace(index, fn);
    tabWidget->setTabText(index, QFileInfo(fn).fileName());
    return fileSave(index);
}

void myUI::on_saveAs_clicked()
{
    fileSaveAs(tabWidget->currentIndex());
}

//baocun all
void myUI::on_SaveAll_clicked()
{
    bool success = true;
    for (int i = 0; i < tabWidget->count(); i++)
    {
        tabWidget->setCurrentIndex(i);
        success = fileSave(i);
    }
}

//判断指定文件是否需要保存
bool myUI::maybeSave(int index)
{
    CodeEditor *text = static_cast<CodeEditor*>(tabWidget->widget(index)) ;
    QString fileName = tabWidget->tabText(tabWidget->currentIndex());
    //QString fileName = openedFiles.at(index);
#if _DEBUG
        printf("\nThis a information for developer\n\n" );
        printf("In myui.cpp, the maybeSave function\n" ) ;
        printf("The close index = %d\n", index ) ;
        //printf("The file name is %s\n", fileName.toLocal8Bit().data() ) ;
        printf("Debug Exit\n" ) ;
#endif
    if (!text->document()->isModified())
        return true;
    if (fileName.startsWith(QLatin1String(":/")))
        return true;
    return true;
}

//关闭当前文件
void myUI::on_close_clicked()
{
    fileClose(tabWidget->currentIndex());
}

//关闭
void myUI::on_closeAll_clicked()
{
    while (tabWidget->count() >= 1)
    {
        if (maybeSave(tabWidget->currentIndex()))
        {
            if (openedFiles.count() == 1)
            {
                on_newFile_clicked();
                openedFiles.removeAt(0);
                tabWidget->removeTab(0);
                break;
            }
            else
            {
                openedFiles.removeAt(tabWidget->currentIndex());
                tabWidget->removeTab(tabWidget->currentIndex());
            }
        }
        else
            break;
    }
}

void myUI::on_PDF_clicked()
{

}

void myUI::on_print_clicked()
{

}

void myUI::on_printView_clicked()
{

}

//打印预览子函数
void myUI::printPreview(QPrinter *printer)
{

}

void myUI::on_color_clicked()
{
    QColor col = QColorDialog::getColor(Qt::black, this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);
}

//设置文本字体
void myUI::textFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, EDITOR->font(),this);
    if (!ok)
        return;
    QTextCharFormat fmt;
    fmt.setFont(font);
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(font);
}

//设置字体大小
void myUI::textSize(const QString &size)
{
    qreal pointSize = size.toFloat();
    if (size.toFloat() > 0)
    {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        ui->comboSize->setCurrentIndex(ui->comboSize->findText(size));
        mergeFormatOnWordOrSelection(fmt);
    }
}

//字体发生改变
void myUI::fontChanged(const QFont &font)
{
    comboFont->setCurrentIndex(comboFont->findText(QFontInfo(font).family()));
    ui->comboSize->setCurrentIndex(
                ui->comboSize->findText(QString::number(font.pointSize())));
}

//设置文本格式（包括字体、颜色、风格）
void myUI::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = EDITOR->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
    EDITOR->mergeCurrentCharFormat(format);
}

//设置文本字体（通过字体组合框）
void myUI::textFontFamily(const QString& font)
{
    QTextCharFormat fmt;
    fmt.setFontFamily(font);
    currentCharFormatChanged(fmt);
    mergeFormatOnWordOrSelection(fmt);
}

//文本格式发生改变
void myUI::currentCharFormatChanged(const QTextCharFormat &format)
{
    fontChanged(format.font());
}

//关闭已经重复打开的文件
void myUI::closeDuplicate(int index)
{
    QString fileName = openedFiles.at(index);
    for (int i = 0; i < openedFiles.count(); i++)
    {
        if (openedFiles.at(i) == fileName && i != index)
        {
            openedFiles.removeAt(i);
            tabWidget->removeTab(i);
        }
    }
    int currIndex = openedFiles.indexOf(fileName);
    if( initWin->StringCompare( fileName.toLocal8Bit().data(), "model"));
    else
    {
        tabWidget->setCurrentIndex(currIndex);
        setWindowTitle(tr("CFD 2.0(%1)").arg(fileName));
    }
}

void myUI::updateActions()
{
    connect(EDITOR, SIGNAL(modificationChanged(bool)), SLOT(modificationChanged(bool)), Qt::UniqueConnection);
    connect(EDITOR,SIGNAL(selectionChanged()),SLOT(selectionChanged()));
    connect(EDITOR,SIGNAL(blockCountChanged(int)),SLOT(blockCountChanged(int)));
    //refreshActions();
   // updateRecentFilesList();
}

//显示readme文件
void myUI::showReadme()
{
    QString readmeFile = QApplication::applicationDirPath();
    readmeFile += readmeFile.endsWith('/') ? "readme.txt" : "/readme.txt";
    if (!QFile::exists(readmeFile))
    {
        QFile file(readmeFile);
        file.open(QIODevice::ReadWrite | QIODevice::Text | QIODevice::Truncate);
        QTextStream stream(&file);
        stream
                << "/*===========================================================================\n"
                << "CFD_beta, editor for Common Use.\n"
                << "Copyright (C) 2017 Huanghuakun(DLUT)\n" << "\n"
                << "This program is free software: you can redistribute it and/or modify\n"
                << "it under the terms of the GNU General Public License as published by\n"
                << "the Free Software Foundation, either version 3 of the License, or\n"
                << "(at your option) any later version.\n" << "\n"
                << "This program is distributed in the hope that it will be useful,\n"
                << "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                << "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                << "GNU General Public License for more details.\n" << "\n"
                << "You should have received a copy of the GNU General Public License\n"
                << "along with this program.  If not, see <http://www.gnu.org/licenses/>\n"
                << "===========================================================================*/";
        file.close();
    }
    QFile rfile(readmeFile);
    rfile.open(QFile::ReadOnly);
    newTab(readmeFile, rfile);
}

//文档发生改变
void myUI::modificationChanged(bool changed)
{
    QString str = tabWidget->tabText(tabWidget->currentIndex());
    if( initWin->StringCompare( str.toLocal8Bit().data(), "model"))
        ;
    else{
    if (str[str.length() - 1] == '*')
    {
        if (!changed)
            str.resize(str.length() - 1);
    }
    else if (changed)
    {
        str += '*';
    }
    tabWidget->setTabText(tabWidget->currentIndex(), str);}
    //refreshActions();
    //setupEditActions();
}

//文档选中状态发生改变
void myUI::selectionChanged()
{
   // ui->uupperCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
   // lowerCaseAct->setEnabled(EDITOR->textCursor().hasSelection());
    QTextCharFormat fmt=EDITOR->currentCharFormat();
    currentCharFormatChanged(fmt);
}

//文档段落发生改变
void myUI::blockCountChanged(int blockCount)
{
    //gotoLineDialog->setMaxLineNumber(blockCount);
}

void myUI::on_bold_clicked()
{
    QTextCharFormat fmt;
    if( BoldOrNor != 1 )
    {
       fmt.setFontWeight( QFont::Bold );
       BoldOrNor = 1 ;
    }
    else
    {
        fmt.setFontWeight( QFont::Normal );
        BoldOrNor = 0 ;
    }
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

void myUI::on_italic_clicked()
{
    QTextCharFormat fmt;
    if( italic != 1 )
    {
       fmt.setFontItalic( 1 );
       italic = 1 ;
    }
    else
    {
        fmt.setFontItalic( 0 );
        italic = 0 ;
    }
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

void myUI::on_underline_clicked()
{
    QTextCharFormat fmt;
    if( underline != 1 )
    {
       fmt.setFontUnderline( 1 );
       underline = 1 ;
    }
    else
    {
        fmt.setFontUnderline( 0 );
        underline = 0 ;
    }
    mergeFormatOnWordOrSelection(fmt);
    fontChanged(fmt.font());
}

void myUI::on_paste_clicked()
{
    //QClipboard *clipboard = QApplication::clipboard();   //获取系统剪贴板指针
   // QString originalText = clipboard->text();         //获取剪贴板上文本信息


}

void myUI::on_copy_clicked()
{

}

void myUI::on_cutFile_clicked()
{

}

//开启命令行
void myUI::on_command_line_clicked()
{

}

void myUI::on_blockMesh_clicked()
{

}


void myUI::on_fluentMeshToFoam_clicked()
{
 /*   QString logCFD = initWin->workPath + "\\logCFD.txt" ;
    remove( logCFD.toLocal8Bit().data() ) ;
    initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
    command = "fluentMeshToFoam fluent.msh" ;
    textBrowser->textCursor().insertText( command );
    textBrowser->textCursor().insertText( "\n" ) ;
    textBrowser->moveCursor( QTextCursor::End );
    if( initWin->hwnd ==NULL )
        printf("no\n");
    else
    {
        initWin->InputFromKeyBoard( initWin->hwnd, command, logFileOnOROff );            //输入blockMesh
        readCMD( pathShow, oldPathShow, textBrowser ) ;
    }*/
}



void myUI::on_pushButton_23_clicked()
{
  /*  QString logCFD = initWin->workPath + "\\logCFD.txt" ;
    remove( logCFD.toLocal8Bit().data() ) ;
    initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
    command = "checkMesh" ;                                    //结果重定向到文本文件
    textBrowser->textCursor().insertText( command );
    textBrowser->textCursor().insertText( "\n" ) ;
    textBrowser->moveCursor( QTextCursor::End );
    if( initWin->hwnd ==NULL )
        printf("no\n");
    else
    {
        //initWin->InputFromKeyBoard( initWin->hwnd, command, logFileOnOROff );                  //输入blockMesh
        readCMD( pathShow, oldPathShow, textBrowser ) ;
    }*/
}

void myUI::on_wmake_clicked()
{
   /* QString logCFD = initWin->workPath + "\\logCFD.txt" ;
    remove( logCFD.toLocal8Bit().data() ) ;
    initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
    command = "wmake" ;
    textBrowser->textCursor().insertText( command );
    textBrowser->textCursor().insertText( "\n" ) ;
    textBrowser->moveCursor( QTextCursor::End );
    if( initWin->hwnd ==NULL )
        printf("no\n");
    else
    {
        initWin->InputFromKeyBoard( initWin->hwnd, command, logFileOnOROff );                  //输入blockMesh
        readCMD( pathShow, oldPathShow, textBrowser ) ;
    }*/
}

void myUI::runApplication( )
{

    //initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
    //textBrowser->textCursor().insertText( application );
    //textBrowser->textCursor().insertText( "\n" ) ;
    //textBrowser->moveCursor( QTextCursor::End );
    //if( initWin->hwnd ==NULL )
        //printf("no\n");
   // else ;
        //initWin->InputFromKeyBoard( initWin->hwnd, QString( application ), logFileOnOROff );                  //输入blockMesh
    //readCMD( pathShow, oldPathShow, textBrowser ) ;
}

//------------------------------后处理





void myUI::on_paraview_clicked()
{
    QString paraFile ;                                   //创建paraview文件
    paraFile = initWin->workPath + "\\cfdFoam.foam" ;
    FILE *para ;
    para = fopen( paraFile.toLocal8Bit().data(), "w" ) ;
    fclose( para ) ;
    system( paraFile.toLocal8Bit().data() ) ;
}



void myUI::on_tecplot360_clicked()
{
    /*QString logCFD = initWin->workPath + "\\logCFD.txt" ;
    remove( logCFD.toLocal8Bit().data() ) ;
    initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
    command = "cd " + initWin->FilePathChange( initWin->workPath ) ;                     //返回工作目录位置
    QString fileName ;                                                                 //获取文件夹名称
    int first = initWin->workPath.lastIndexOf("\\") ;
    fileName = initWin->workPath.right( initWin->workPath.length()-first-1 ) ;
    QString old = initWin->windowTitle ;
    QString oldWorkPath = initWin->workPath ;
    if( initWin->hwnd ==NULL )
        printf("no\n");
    else
    {
        initWin->InputFromKeyBoard( initWin->hwnd, command, 0 );
        initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
        initWin->InputFromKeyBoard( initWin->hwnd, "cd ..", 0 );                             //返回瞩目
        initWin->windowTitle = initWin->userName + ": " + initWin->FilePathChange( initWin->workPath.left( first ) ) ;
        initWin->workPath =  initWin->workPath.left( first ) ;
        initWin->hwnd = initWin->CaptureWindow( initWin->windowTitle.toLocal8Bit().data() ) ; //获取新的窗口句柄
        command = "foamToTecplot360 -case " +fileName ;
        if( initWin->hwnd !=NULL  )
        {
            //command += " >>" + initWin->FilePathChange( initWin->workPath ) + "/logCFD 2>&1" ;

            initWin->InputFromKeyBoard( initWin->hwnd, command, logFileOnOROff );
            pathShow = 1 ;
            oldPathShow = initWin->FilePathChange( oldWorkPath ) ;
            readCMD( pathShow, oldPathShow, textBrowser ) ;
        }
        else
            printf( "zhaobudao\n") ;
        command = "cd " + initWin->FilePathChange( oldWorkPath ) ;            //返回工作目录位置
        initWin->InputFromKeyBoard( initWin->hwnd, command, 0 );
        initWin->workPath = oldWorkPath ;
        initWin->windowTitle = old ;                                          //获取新的窗口
        pathShow = 0 ;
        Sleep(100) ;                                                         //滞后一段时间
    }
    //on_actionBash_triggered() ;*/
}

void myUI::readCMD( int mark, QString str, QTextBrowser *textBrowser )
{
    /*thread.text = textBrowser ;
    thread.currentPathWindow = initWin->workPath ;
    if( mark == 100 ) //use pyFoam
    {
        thread.pyFoam = 1 ;
        thread.caseName = str ;
    }
    thread.run();*/
}

void myUI::on_dirChoose_clicked( )
{

    //寻找文件路径
    QString path ;
    QFileDialog *fileDialog = new QFileDialog ;
    path = fileDialog->getExistingDirectory(this,"请选择路径","./") ;
    if( path.isEmpty() )
    {
        //QMessageBox::warning(NULL, "warning", "路径不存在", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        path = initWin->workPath ;
    }
	else {
		path = initWin->QtPathToCPath(path);
	}
    //cout<< path.toLocal8Bit().data() << endl ;
  /*  int success ;
    success = initWin->ChangeDir( path ) ;
    if( success )
    {
        textBrowser->textCursor().insertText("\n") ;
        textBrowser->textCursor().insertText( initWin->FilePathChange(initWin->workPath) + ">>");
        dirRefresh();
    }
    }
    //lineEdit->setFont(  textBrowser->font()  );*/
}



/*void myUI::on_FontChoose_clicked(  )
{
    textBrowser->setFont( QFontDialog::getFont( 0, textBrowser->font() ) );
}*/

/*void myUI::dirRefresh()
{
        treeView->reset();
        model = new QDirModel;
        treeView->setModel( model );
        treeView->setRootIndex( model->index( initWin->workPath ) );
}*/

/*void myUI::on_controlDict_clicked()
{
    control controlDict( initWin ) ;
    controlDict.exec() ;
}*/
