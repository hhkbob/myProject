#ifndef CONFIG_H
#define CONFIG_H
#include <QObject>
#include <QStringList>
#include <QString>
#include <QByteArray>
#include <QSettings>

struct Config: public QObject
{
    Q_OBJECT

public:
    enum Receiver
    {
        Init = 1, //0000001
        Editor = 2, //0000010
        Highlighter = 4 //0000100
    };

signals:
    void reread(int);
private:
    void createIniFile(QString iniFile);    //创建settings.ini文件

public slots:
    void reconfig(int receiver);    //重新配置

public:
    Config();
    ~Config();

    QString iniFile;    //settings.ini文件路径
    //General
    QByteArray mainWindowsGeometry; //窗口大小
    QByteArray mainWindowState; //窗口状态

    int maxRecentFiles; //最大文件数（最近的文档）
    QStringList recentFiles; //最近的文档
    bool showReadme; //是否显示readme文件

    //Editor
    QString fontFamily; //字体设置
    int fontSize; //字体大小
    QString fontStyle; //字体风格（粗体/斜体）
    bool showLineNumber; //是否显示行号

    bool tabIndents; //是否使用Tab缩进
    bool autoIndent; //是否自动缩进
    bool backUnindent; //是否使用退格取消缩进
    bool spaceTabs; //是否显示空格和Tab

    int indentSize; //缩进大小
    int tabSize; //Tab所占字符大小

    bool whitespaces; //是否使用空格代替Tab

    //Search
    int maxHistory; //查找和替换的最大记录数
    QStringList findHistory; //查找历史记录
    QStringList replaceHistory; //替换历史记录

    bool matchCase; //是否匹配大小写
    bool regExp; //是否采用正则表达式
};

#endif//CONFIG_H
