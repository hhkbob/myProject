#ifndef QMENUBUTTON_H
#define QMENUBUTTON_H
#include <QMenu>

#include <QToolButton>
#include <QPushButton>
class QMenu;
class qmenubutton : public QToolButton
{
    Q_OBJECT
public:
    explicit qmenubutton(QWidget *parent = 0);
    QMenu* getmenu();
    QMenu* menu;
    QWidget *widget ;


signals:

public slots:
    void popupmenu();

};

#endif // QMENUBUTTON_H
