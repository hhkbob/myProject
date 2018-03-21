#ifndef EDITOR_H
#define EDITOR_H

#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qsciapis.h>



class FoamEditor : public QMainWindow
{
    Q_OBJECT

public:
	FoamEditor(QMainWindow *object);
	QsciScintilla *textEdit;
public:
    void newTab(QTabWidget *tab, QString fileName );
	QMainWindow *window;
	QsciScintilla *newEditor();

protected:
	void closeEvent(QCloseEvent *event);

private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void documentWasModified();
private:

	void readSettings();
	void writeSettings();
	bool maybeSave();
	void loadFile(const QString &fileName);
	bool saveFile(const QString &fileName);
	void setCurrentFile(const QString &fileName);
	QString strippedName(const QString &fullFileName);
	QString curFile;
};

#endif
