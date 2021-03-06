#include <Editor.h>

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QIcon>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPoint>
#include <QSettings>
#include <QSize>
#include <QStatusBar>
#include <QTextStream>
#include <QToolBar>

FoamEditor::FoamEditor(QMainWindow *object )
{
	textEdit = NULL;
	window = object;
}

void FoamEditor::newTab(QTabWidget *tab, QString fileName)
{
	tab->addTab( newEditor(), QFileInfo(fileName).fileName());
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();
}

QsciScintilla *FoamEditor::newEditor()
{
	textEdit = new QsciScintilla;

	//  setup the calltips
	textEdit->setCallTipsStyle(QsciScintilla::CallTipsNoContext);
	textEdit->setCallTipsVisible(0);
	textEdit->setCallTipsPosition(QsciScintilla::CallTipsBelowText);
	textEdit->setCallTipsBackgroundColor(QColor(0xff, 0xff, 0xff, 0xff));
	textEdit->setCallTipsForegroundColor(QColor(0x00, 0x50, 0x00, 0xff));
	textEdit->setCallTipsHighlightColor(QColor(0x00, 0x00, 0xff, 0xff));

	QFont font2("Courier New");
	font2.setPointSize(10);
	textEdit->setFont(font2);
	textEdit->SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);
	textEdit->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

	QFont KeyWordFont("Courier New"); KeyWordFont.setBold(false); KeyWordFont.setPointSize(10);
	QFont DefaultFont("Courier New"); DefaultFont.setBold(false); DefaultFont.setPointSize(10);
	QFont OperatorFont("Courier New"); OperatorFont.setBold(false); OperatorFont.setPointSize(10);
	QsciLexerCPP *textLexer = new QsciLexerCPP;
	textLexer->setColor(QColor(0, 128, 0), QsciLexerCPP::CommentLine);
	textLexer->setColor(QColor(0, 128, 0), QsciLexerCPP::Comment);
	textLexer->setColor(QColor(255, 128, 0), QsciLexerCPP::Number);
	textLexer->setColor(QColor(128, 128, 128), QsciLexerCPP::RawString);
	textLexer->setColor(QColor(128, 0, 255), QsciLexerCPP::Keyword);
	textLexer->setColor(QColor(128, 0, 255), QsciLexerCPP::KeywordSet2);
	textLexer->setColor(QColor(128, 64, 0), QsciLexerCPP::PreProcessor);
	textLexer->setColor(QColor(Qt::black), QsciLexerCPP::Default);
	textLexer->setColor(QColor(0, 0, 128), QsciLexerCPP::Operator);
	textLexer->setColor(QColor(Qt::red), QsciLexerCPP::UnclosedString);
	textLexer->setColor(QColor(0, 128, 128), QsciLexerCPP::GlobalClass);

	textLexer->setColor(QColor(128, 0, 255), QsciLexerCPP::UserLiteral);

	QFont CommantFont("Courier New");
	CommantFont.setPointSize(10);
	CommantFont.setItalic(true);
	textLexer->setFont(CommantFont, QsciLexerCPP::CommentLine);
	textLexer->setFont(CommantFont, QsciLexerCPP::Comment);
	textLexer->setFont(KeyWordFont, QsciLexerCPP::Keyword);
	textLexer->setFont(DefaultFont, QsciLexerCPP::Default);
	textLexer->setFont(OperatorFont, QsciLexerCPP::Operator);

	textEdit->setLexer(textLexer);

	//  current line color
	textEdit->setCaretLineVisible(true);
	textEdit->setCaretLineBackgroundColor(QColor(232,232,255));
	textEdit->setCaretForegroundColor(QColor("grayd"));

	//  marker
	textEdit->markerDefine(QsciScintilla::Minus, QsciScintilla::SC_MARKNUM_FOLDEROPEN);
	textEdit->markerDefine(QsciScintilla::Plus, QsciScintilla::SC_MARKNUM_FOLDER);
	textEdit->markerDefine(QsciScintilla::Minus, QsciScintilla::SC_MARKNUM_FOLDEROPENMID);
	textEdit->markerDefine(QsciScintilla::Plus, QsciScintilla::SC_MARKNUM_FOLDEREND);

	//  whitespace
	textEdit->setWhitespaceVisibility(QsciScintilla::WsInvisible);
	textEdit->setWhitespaceSize(2);

	//  set the margin
	QFont font("Courier New");
	font.setPointSize(10);
	textEdit->setMarginType(0, QsciScintilla::NumberMargin);
	textEdit->setMarginLineNumbers(0, true);
	textEdit->setMarginWidth(0, 20);
	textEdit->setMarginsFont(font);
	textEdit->setMarginSensitivity(0, true);

	//  folder code
	QString fold = "1";
	QVariant variant = fold;
	textLexer->setProperty("fold", variant);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDER, QsciScintilla::SC_MARK_CIRCLEPLUS);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDEROPEN, QsciScintilla::SC_MARK_CIRCLEMINUS);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDEREND, QsciScintilla::SC_MARK_CIRCLEPLUSCONNECTED);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDEROPENMID, QsciScintilla::SC_MARK_CIRCLEMINUSCONNECTED);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDERMIDTAIL, QsciScintilla::SC_MARK_TCORNERCURVE);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDERSUB, QsciScintilla::SC_MARK_VLINE);
	textEdit->markerDefine(QsciScintilla::SC_MARKNUM_FOLDERTAIL, QsciScintilla::SC_MARK_LCORNERCURVE);

	//  annotations
	textEdit->setAnnotationDisplay(QsciScintilla::AnnotationStandard);

	QsciAPIs *apis = new QsciAPIs(textLexer);
	QList<QString> tips;
	QList<int> shifts;
	shifts.append(0);
	char autocompletions[4][100] = {
			"add( int arg1, int arg2 )",
			"subtract(int arg_1, int arg_2)",
			"function_one(int arg_1)",
			"function_two(float arg_1)"
	};
	for( int i=0; i<4; i++ )
	  apis->add(autocompletions[i]);
	if (!apis->load(QString("api.ini")) && apis->load(QString("probes.txt")) )
		QMessageBox::warning(this, QString("Warning"), QString("load the callTips faile"));
	else
		apis->prepare();

	textEdit->setAutoCompletionSource(QsciScintilla::AcsAll);
	textEdit->setAutoCompletionCaseSensitivity(true);
	textEdit->setAutoCompletionThreshold(1);


	return textEdit;
}

void FoamEditor::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		writeSettings();
		event->accept();
	}
	else {
		event->ignore();
	}
}

void FoamEditor::newFile()
{
	if (maybeSave()) {
		textEdit->clear();
		setCurrentFile("");
	}
}

void FoamEditor::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
			loadFile(fileName);
	}
}

bool FoamEditor::save()
{
	if (curFile.isEmpty()) {
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool FoamEditor::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this);
	if (fileName.isEmpty())
		return false;

	return saveFile(fileName);
}

void FoamEditor::documentWasModified()
{
	setWindowModified(textEdit->isModified());
}

void FoamEditor::readSettings()
{
	QSettings settings("Trolltech", "Application Example");
	QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
	QSize size = settings.value("size", QSize(400, 400)).toSize();
	resize(size);
	move(pos);
}

void FoamEditor::writeSettings()
{
	QSettings settings("Trolltech", "Application Example");
	settings.setValue("pos", pos());
	settings.setValue("size", size());
}

bool FoamEditor::maybeSave()
{
	if (textEdit->isModified()) {
		int ret = QMessageBox::warning(this, tr("Application"),
			tr("The document has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Yes | QMessageBox::Default,
			QMessageBox::No,
			QMessageBox::Cancel | QMessageBox::Escape);
		if (ret == QMessageBox::Yes)
			return save();
		else if (ret == QMessageBox::Cancel)
			return false;
	}
	return true;
}

void FoamEditor::loadFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::ReadOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot read file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return;
	}

	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setText(in.readAll());
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File loaded"), 2000);
}

bool FoamEditor::saveFile(const QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QFile::WriteOnly)) {
		QMessageBox::warning(this, tr("Application"),
			tr("Cannot write file %1:\n%2.")
			.arg(fileName)
			.arg(file.errorString()));
		return false;
	}

	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << textEdit->text();
	QApplication::restoreOverrideCursor();

	setCurrentFile(fileName);
	statusBar()->showMessage(tr("File saved"), 2000);
	return true;
}

void FoamEditor::setCurrentFile(const QString &fileName)
{
	curFile = fileName;
	textEdit->setModified(false);
	setWindowModified(false);

	QString shownName;
	if (curFile.isEmpty())
		shownName = "untitled.txt";
	else
		shownName = strippedName(curFile);

	setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("Application")));
}

QString FoamEditor::strippedName(const QString &fullFileName)
{
	return QFileInfo(fullFileName).fileName();
}























