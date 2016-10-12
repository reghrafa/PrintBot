#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "EditorWidget.h"

#include <QMainWindow>
#include <QDebug>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QPlainTextEdit>
#include <QFileDialog>
#include <QMenuBar>
#include <QProcess>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QSplitter>
#include <QString>

#include "SourcecodeWidget.h"

#include "Main.h"
#include "ProgramBlock.h"
#include "ProgramIfBlock.h"
#include "ProgramLoopBlock.h"
#include "ProgramCommandBlock.h"

#include "BlockListWidget.h"
#include "Dialog.h"
//#include "TerminalWidget.h"
#include "AboutDialog.h"

class EditorWidget;
class BlockListWidget;
class TerminalWidget;
class ProgramCode;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = NULL);
    ~MainWindow();
//    void setQLabelMousePosition(QPoint mousePosition);
    QGridLayout &getQGridLayout();
    QGroupBox &getQGridGroupBox();
    void resetTerminalWidgetPointer();
    void addBlockToBlockListWidget(ProgramBlock *newProgramBlock, int row = -1);
    ProgramCode &getProgramCode();
    EditorWidget *getEditorWidget();
    BlockListWidget *getBlockListWidget();

public slots:
    void updateSourcecodeWidget();

private slots:
    void newDokument();
    void saveSourcecode();
    void saveAs();
    void openSourcecode();
    void compileSourcecode();
    void uploadHexfile();
    void readQProcessOutput();
    void openTerminal();
    void on_actionAbout_triggered();

private:
    void setupWidgets();
    void setupMenus();
    void createTestProgram();

//    bool alreadyMaked;
    QString openFile;
    BlockListWidget *blockListWidget;       ///Widget, dass jede Art von Block einmal enthält
    EditorWidget *editorWidget;
    SourcecodeWidget *sourcecodeWidget;
    QPlainTextEdit qPlainTextEdit;
//    QLabel qLabelMousePosition;
    QGroupBox qGridGroupBox;
    QGridLayout qGridLayout;
    QProcess *qProcess;
    TerminalWidget *terminalWidget;
    //    QPlainTextEdit qPlainTextEdit;
};

#endif // MAINWINDOW_H
