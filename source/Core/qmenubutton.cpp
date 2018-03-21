#include "qmenubutton.h"
#include<QRect>

qmenubutton::qmenubutton(QWidget *parent) :
    QToolButton(parent)
{
    menu = new QMenu(this);

    connect(this,SIGNAL(clicked()),this,SLOT(popupmenu()));//点击按钮就弹出菜单

}
QMenu *qmenubutton::getmenu()
{
     return menu;
}
void qmenubutton:: popupmenu()
{   QPoint pos; //获取按键菜单的坐标

   // int x = pos.x();

    int y = pos.y();
   // pos.setX(x + this->geometry().width()/2);//也可以改变出现菜单的窗口的x位置

    pos.setY(y-this->geometry().height()+25);


    //返回菜单下面的action不管有没被选中，可以算一个小事件循环

    //里面参数的意思是在哪个坐标弹出菜单，这里将自定义的pop按键的坐标作参考，并在其下面弹出菜单
    widget->show( ) ;
    widget->move( this->mapToGlobal(pos) );

   // menu->exec(this->mapToGlobal(pos));

}

