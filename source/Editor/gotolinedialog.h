#ifndef GOTOLINEDIALOG_H
#define GOTOLINEDIALOG_H

#include <QWidget>

namespace Ui
{
class GoToLineDialog;
}

class GoToLineDialog: public QWidget
{
    Q_OBJECT

public:
    explicit GoToLineDialog(QWidget *parent = 0);
    void setMaxLineNumber(int lineNumber);  //设置行号最大范围
    //~GoToLineDialog();

signals:
    void gotoLine(int lineNumber);  //转到行

private slots:
    void gotoLine();    //用于emit gotoLine

private:
    Ui::GoToLineDialog *ui;
};

#endif // GOTOLINEDIALOG_H
