/*================================C++========================================*\
Create: 2018/02/09
Creator: Huang Huakun
File Name: TabWidget.h
Platform: windows/linux
Version: v1.0

Describe: define the tab style.

Example:

\*===========================================================================*/

//  The code's length should not exceed 80 characters.
//  Every description for a code line should skip two characters.
//  As you can, do not use the void function. A return code is welcome.

/*
CopyRight:
*/


#include "TabWidget.h"
#include <QEvent>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMouseEvent>
#include <QMultiMap>
#include <QPointer>
#include <QShortcut>
#include <QStyle>
#include <QTabBar>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QtDebug>

//-----------------------------------------------------------------------------
// ******************** FDTabWidget **********************
//-----------------------------------------------------------------------------
TabWidget::FDTabWidget::FDTabWidget(QWidget* parentObject)
	: Superclass(parentObject)
{
}

//-----------------------------------------------------------------------------
TabWidget::FDTabWidget::~FDTabWidget()
{
}

//-----------------------------------------------------------------------------
int TabWidget::FDTabWidget::tabButtonIndex(
	QWidget* wdg, QTabBar::ButtonPosition position) const
{
	for (int cc = 0; cc < this->count(); cc++)
	{
		if (this->tabBar()->tabButton(cc, position) == wdg)
		{
			return cc;
		}
	}
	return -1;
}

//-----------------------------------------------------------------------------
void TabWidget::FDTabWidget::setTabButton(
	int index, QTabBar::ButtonPosition position, QWidget* wdg)
{
	this->tabBar()->setTabButton(index, position, wdg);
}

//-----------------------------------------------------------------------------
const char* TabWidget::FDTabWidget::popoutLabelText(bool popped_out)
{
	return popped_out ? "Bring popped out window back to the frame"
		: "Pop out layout in separate window";
}

//-----------------------------------------------------------------------------
QStyle::StandardPixmap TabWidget::FDTabWidget::popoutLabelPixmap(bool popped_out)
{
	return popped_out ? QStyle::SP_TitleBarNormalButton : QStyle::SP_TitleBarMaxButton;
}

//-----------------------------------------------------------------------------
// ****************     TabWidget   **********************
//-----------------------------------------------------------------------------
class TabWidget::pqInternals
{
public:
	QPointer<FDTabWidget> TabWidget;
	QPointer<QWidget> FullScreenWindow;
	QPointer<QWidget> NewTabWidget;

	void addNewTabWidget()
	{
		if (!this->NewTabWidget)
		{
			this->NewTabWidget = new QWidget(this->TabWidget);
			this->TabWidget->addTab(this->NewTabWidget, "+");
		}
	}
	void removeNewTabWidget()
	{
		if (this->NewTabWidget)
		{
			this->TabWidget->removeTab(this->TabWidget->indexOf(this->NewTabWidget));
			delete this->NewTabWidget;
		}
	}
};

//-----------------------------------------------------------------------------
TabWidget::TabWidget(QWidget* parentObject)
	: Superclass(parentObject)
	, Internals(new pqInternals())
{
	this->Internals->TabWidget = new FDTabWidget(this);
	this->Internals->TabWidget->setObjectName("CoreWidget");
	EditorWidget = this->Internals->TabWidget;

	QVBoxLayout* vbox = new QVBoxLayout();
	this->setLayout(vbox);
	vbox->setMargin(0);
	vbox->setSpacing(0);
	vbox->addWidget(this->Internals->TabWidget);
}

//-----------------------------------------------------------------------------
TabWidget::~TabWidget()
{
	delete this->Internals;
}

//-----------------------------------------------------------------------------
void TabWidget::toggleFullScreen()
{
	if (this->Internals->FullScreenWindow)
	{
		this->Internals->FullScreenWindow->layout()->removeWidget(this->Internals->TabWidget);
		this->layout()->addWidget(this->Internals->TabWidget);
		delete this->Internals->FullScreenWindow;
	}
	else
	{
		QWidget* fullScreenWindow = new QWidget(this, Qt::Window);
		this->Internals->FullScreenWindow = fullScreenWindow;
		fullScreenWindow->setObjectName("FullScreenWindow");
		this->layout()->removeWidget(this->Internals->TabWidget);

		QVBoxLayout* vbox = new QVBoxLayout(fullScreenWindow);
		vbox->setSpacing(0);
		vbox->setMargin(0);

		vbox->addWidget(this->Internals->TabWidget);
		fullScreenWindow->showFullScreen();
		fullScreenWindow->show();

		QShortcut* esc = new QShortcut(Qt::Key_Escape, fullScreenWindow);
		QObject::connect(esc, SIGNAL(activated()), this, SLOT(toggleFullScreen()));
		QShortcut* f11 = new QShortcut(Qt::Key_F11, fullScreenWindow);
		QObject::connect(f11, SIGNAL(activated()), this, SLOT(toggleFullScreen()));
	}
}
//-----------------------------------------------------------------------------
bool TabWidget::eventFilter(QObject* obj, QEvent* evt)
{
	// filtering events on the QLabel added as the tabButton to the tabbar to
	// close the tabs. If clicked, we close the tab.
	if (evt->type() == QEvent::MouseButtonRelease && qobject_cast<QLabel*>(obj))
	{
		QMouseEvent* mouseEvent = dynamic_cast<QMouseEvent*>(evt);
		if (mouseEvent->button() == Qt::LeftButton)
		{
			int index =
				this->Internals->TabWidget->tabButtonIndex(qobject_cast<QWidget*>(obj), QTabBar::RightSide);
			if (index != -1)
			{
				return true;
			}

			// user clicked on the popout label. We pop the frame out (or back in).
			index =
				this->Internals->TabWidget->tabButtonIndex(qobject_cast<QWidget*>(obj), QTabBar::LeftSide);
			if (index != -1)
			{
				return true;
			}
		}
	}

	return this->Superclass::eventFilter(obj, evt);
}

//-----------------------------------------------------------------------------
QSize TabWidget::clientSize() const
{
	if (this->Internals->TabWidget->currentWidget())
	{
		return this->Internals->TabWidget->currentWidget()->size();
	}

	return this->size();
}

void TabWidget::FDTabWidget::setReadOnly(bool val)
{
	if (this->ReadOnly == val)
	{
		return;
	}

	this->ReadOnly = val;
	QList<QLabel*> labels = this->findChildren<QLabel*>("close");
	foreach(QLabel* label, labels)
	{
		label->setVisible(!val);
	}
}
void TabWidget::setReadOnly(bool val)
{
	if (val != this->readOnly())
	{
		this->Internals->TabWidget->setReadOnly(val);
		if (val)
		{
			this->Internals->removeNewTabWidget();
		}
		else
		{
			this->Internals->addNewTabWidget();
		}
	}
}

//-----------------------------------------------------------------------------
bool TabWidget::readOnly() const
{
	return this->Internals->TabWidget->readOnly();
}