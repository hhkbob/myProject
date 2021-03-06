#include "gotolinedialog.h"
#include "ui_gotolinedialog.h"

GoToLineDialog::GoToLineDialog(QWidget *parent) :
    QWidget(parent), ui(new Ui::GoToLineDialog)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(tr(":images/notepad.png")));
    setWindowTitle(tr("Goto Line"));
    connect(ui->pushButton, SIGNAL(clicked()), SLOT(gotoLine()));
}
/*GoToLineDialog::~GoToLineDialog( )
{
    delete ui ;
}*/

void GoToLineDialog::setMaxLineNumber(int lineNumber)
{
    ui->spinBox->setMaximum(lineNumber);
}
void GoToLineDialog::gotoLine()
{
    int lineNumber = ui->spinBox->value();
    emit gotoLine(lineNumber);
}


