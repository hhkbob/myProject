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

#ifndef TABWIDGET_H
#define TABWIDGET_H

//  here, I use the paraview's method
#include "pqComponentsModule.h"
#include "vtkType.h"  // needed for vtkIdType
#include <QStyle>     // needed for QStyle:StandardPixmap
#include <QTabBar>    // needed for QTabBar::ButtonPosition
#include <QTabWidget> // needed for QTabWidget.



/**
* TabWidget is used to to enable adding of multiple
* pqMultiViewWidget instances in tabs. This class directly listens to the
* server-manager to automatically create pqMultiViewWidget instances for every
* vtkSMViewLayoutProxy registered.
*/
class _declspec(dllexport) TabWidget : public QWidget
{
	Q_OBJECT
		typedef QWidget Superclass;
	Q_PROPERTY(bool readOnly READ readOnly WRITE setReadOnly)
public:
	TabWidget(QWidget* parent = 0);
	virtual ~TabWidget();

	/**
	* Returns the size for the tabs in the widget.
	*/
	virtual QSize clientSize() const;

signals:
	/**
	* fired when lockViewSize() is called.
	*/
	void viewSizeLocked(bool);

public slots:

	/**
	* toggles fullscreen state.
	*/
	virtual void toggleFullScreen();

	void setReadOnly(bool val);
	bool readOnly() const;

protected:
	virtual bool eventFilter(QObject* obj, QEvent* event);

	/**
	* Internal class used as the TabWidget.
	*/
	class FDTabWidget : public QTabWidget
	{
		typedef QTabWidget Superclass;

	public:
		FDTabWidget(QWidget* parentWdg = NULL);
		virtual ~FDTabWidget();

		/**
		* Set a button to use on the tab bar.
		*/
		virtual void setTabButton(int index, QTabBar::ButtonPosition position, QWidget* wdg);

		/**
		* Given the QWidget pointer that points to the buttons (popout or close)
		* in the tabbar, this returns the index of that that the button corresponds
		* to.
		*/
		virtual int tabButtonIndex(QWidget* wdg, QTabBar::ButtonPosition position) const;

		/**
		* Returns the label/tooltip to use for the popout button given the
		* popped_out state.
		*/
		static const char* popoutLabelText(bool popped_out);

		/**
		* Returns the icon to use for the popout button given the popped_out state.
		*/
		static QStyle::StandardPixmap popoutLabelPixmap(bool popped_out);

		bool readOnly() const { return this->ReadOnly; }
		void setReadOnly(bool val);

	private:
		Q_DISABLE_COPY(FDTabWidget)
			bool ReadOnly;
		friend class TabWidget;
	};
	public:
		FDTabWidget *EditorWidget;

private:
	Q_DISABLE_COPY(TabWidget)

	class pqInternals;
	pqInternals* Internals;
	friend class pqInternals;
};

#endif