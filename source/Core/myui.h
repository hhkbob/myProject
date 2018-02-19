#ifndef MYUI_H
#define MYUI_H

#include <QWidget>
#include <QGroupBox>
#include <QTextEdit>
#ifdef WIN32
#include <windows.h>
#endif
#include <iostream>
#include <QTabWidget>
#include <QAction>
#include <QMenu>
#include <QToolButton>
#include "codeeditor.h"
#include "config.h"
#include "initwindow.h"
#include <QTextBrowser>
#include <QTreeView>
#include <QDirModel>
#include "mytoolbar.h"
#include "codeeditor.h"
#include "OutputMessage.h"

QT_BEGIN_NAMESPACE
QT_FORWARD_DECLARE_CLASS(QTabWidget)
QT_FORWARD_DECLARE_CLASS (QMenuBar)
QT_FORWARD_DECLARE_CLASS (QToolBar)
QT_FORWARD_DECLARE_CLASS (QFont)
QT_FORWARD_DECLARE_CLASS (QComboBox)
QT_FORWARD_DECLARE_CLASS (QFontComboBox)
QT_FORWARD_DECLARE_CLASS (QActionGroup)
QT_FORWARD_DECLARE_CLASS (QTextCharFormat)
QT_FORWARD_DECLARE_CLASS (QPrinter)
QT_END_NAMESPACE

#define EDITOR   static_cast<CodeEditor *>(tabWidget->currentWidget())

using namespace std ;

namespace Ui {
class myUI;
}

class myUI : public QWidget
{
    Q_OBJECT

public:
    explicit myUI(Config *config,QTabWidget *tabWidget, QTreeView *view, QTextBrowser *text, initWindow *init,QWidget *parent = 0);
    ~myUI();
    QTextEdit *text ;                                      //文本框
    void readCMD( int pathShow, QString oldPathShow, QTextBrowser *textBrowser ) ; //读取内容
    void runApplication( ) ;
    int pathShow ;
    QString oldPathShow ;
    QString command ;                                     //命令
    int logFileOnOROff ;                                  //打开日志

private:
    Ui::myUI *ui;
    QAction  *textBoxAct ;                                 //弹出文本菜单
    QMenu *textMenu ;                                      //设计菜单
    Config *config ;

    initWindow *initWin ;                                   //初始化窗口
    CodeEditor Code ;
    OutputMessage Output;

public:
    QGroupBox *editorToolBar ;
    QGroupBox *mainToolBar ;
    QTabWidget *tabWidget ;                               //标签页
    bool maybeSave(int index) ;                           //判断
    QTextBrowser *textBrowser ;                           //显示
    QLineEdit  *lineEdit ;                                //
    QTreeView *treeView ;                                 //显示目录
    QDirModel *model ;                                    //目录类型
    QMenu   *fontCase ;                                   //大小写



private slots:
   void currentChanged(int) ;
   void fileClose(int) ;
   void printPreview(QPrinter *printer) ;                //打印预览
   void textFont() ;                                     //shezhi ziti
   void textSize(const QString &size) ;                  //she zhi da xiao
   void fontChanged(const QFont &font) ;                 //size
   void textFontFamily(const QString& font) ;             //shezhi ziti
   void modificationChanged(bool changed) ;              //
   void selectionChanged() ;
   void blockCountChanged(int blockCount) ;



   void on_open_clicked();

   void on_newFile_clicked();

   void on_saveFile_clicked();

   void on_saveAs_clicked();

   void on_SaveAll_clicked();

   void on_close_clicked();

   void on_closeAll_clicked();

   void on_PDF_clicked();

   void on_print_clicked();

   void on_printView_clicked();

   void on_color_clicked();

   void on_bold_clicked();

   void on_italic_clicked();

   void on_underline_clicked();

   void on_paste_clicked();

   void on_copy_clicked();

   void on_cutFile_clicked();

   void on_blockMesh_clicked();

   void on_fluentMeshToFoam_clicked();

   void on_pushButton_23_clicked();

   void on_wmake_clicked();

   void on_paraview_clicked();

   void on_tecplot360_clicked();

public slots:

   void on_dirChoose_clicked(  );

   void on_command_line_clicked();

private:
    QStringList openedFiles;                              //打开的文件
    int newNumber ;                                       //tab数
    QFontComboBox *comboFont ;                            //newcombox
    int BoldOrNor ;                                       //粗体
    int italic ;                                          //斜体
    int underline ;                                       //下划线


private:
    void addAct( ) ;                                      //添加动作
    void addConnect( ) ;                                  //添加槽函数
    void addMenu( ) ;                                     //添加菜单
    void newTab(const QString& fileName, QFile& file) ;    //创建tab
    bool fileSave(int index) ;                            //保存
    bool fileSaveAs(int index) ;                          //另存为
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format) ; //wenben fengge
    void currentCharFormatChanged(const QTextCharFormat &format) ; //zziti bian hua
    void updateActions() ;
    void showReadme() ;
    void closeDuplicate( int ) ;                          //
public:
    void openFile( QString filename ) ;


};

#endif // MYUI_H
