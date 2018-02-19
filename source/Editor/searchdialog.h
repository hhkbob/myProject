#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QWidget>
#include "ui_searchdialog.h"
#include "config.h"

class SearchDialog: public QWidget, private Ui::SearchDialog
{
    Q_OBJECT

public:
    SearchDialog(Config *, QWidget * = 0);
    ~SearchDialog();

signals:
    void search(QString, bool, bool, bool); // 查找
    void replace(QString, QString, bool, bool, bool); //替换
    void replaceAll(QString, QString, bool, bool); //全部替换

private slots:
    void search(); //查找
    void replace(); //替换

private:
    void update(QComboBox *); //更新查找/替换历史

    Config *config;
};

#endif // SEARCHDIALOG_H
