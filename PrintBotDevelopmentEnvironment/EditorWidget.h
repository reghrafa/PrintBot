#ifndef EDITORWIDGET_H
#define EDITORWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QDebug>
#include <QList>
#include <QSize>
#include <QPoint>
#include <QPixmap>
#include <QWidget>
#include <QFileDialog>
#include <QTextStream>
#include <QDesktopWidget>
//#include "MainWindow.h"
#include "Main.h"
#include "ProgramCode.h"
//#include "ProgramIfBlock.h"
//#include "ProgramLoopBlock.h"
//#include "ProgramCommandBlock.h"

class QDragEnterEvent;
class QDropEvent;
class QMouseEvent;
class ProgramBlock;
class ProgramCode;
//class ProgramIfBlock;
//class ProgramLoopBlock;
//class ProgramCommandBlock;
class MainWindow;
class Dialog;

class EditorWidget : public QWidget
{
    Q_OBJECT

public:
    EditorWidget(MainWindow *newMainWindow);
    QString getSourcecode();
    void updateBlockGroup(ProgramBlock *programBlock);
    ProgramCode &getProgramCode();
    void updateSourcecodeWidget();
    void addBlock(ProgramBlock *dropBlock, bool boolUpdateSourcecodeWidget = true);
    void resetAll();
    void setStandardSize();
    Dialog *getDialog();

public slots:
    void updateSize();
    void updateDialog(ProgramBlock *programBlock);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void mousePressEvent(QMouseEvent *event);
//    void paintEvent(QPaintEvent *event);

private:
    bool isFree(QDragMoveEvent *event);
    void deselectPreviousBlock();

    MainWindow *mainWindow;
    ProgramCode programCode;
    Dialog *dialog;
    QSize editorSize;
//    QPushButton qPushButton;
};

#endif // EDITORWIDGET_H
