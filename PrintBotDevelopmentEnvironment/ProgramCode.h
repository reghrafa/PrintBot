#ifndef PROGRAMCODE_H
#define PROGRAMCODE_H

#include <QDebug>
#include <QObject>
#include <QList>
//#include <QTextStream>
//#include <QFileDialog>
#include <QMessageBox>
#include "Main.h"
#include "ProgramFunction.h"

class MainWindow;

class ProgramCode
{
public:
    ProgramCode();
    void generateSourcecode();
//    void addFunction(ProgramFunction *programFunction);
    void addBlock(ProgramBlock *programBlock);
//    void deleteFunction(ProgramFunction *programFunction);
    void deleteBlock(ProgramBlock *programBlock);
    void updateAllBlockGraphics();
    void resetAll();
    bool existenceOfMain();
    QString getSourcecode();
    QString getSaveData();
    void setMainWindow(MainWindow *newMainWindow);
//    void updateLocation();
//    void newFunction(QString functionName,
//                     QString returnType = "void",
//                     QString parameter = "");
//    ProgramFunction *getFunction(QString functionName,
//                                 QString returnType = "void",
//                                 QString parameter = "");

private:
    //QString programHeader;
    MainWindow *mainWindow;
    QString sourcecode;
//    QList<ProgramFunction *> functionList;
    int isMainInCode;

    ///List, in der alle Funktionen UND loseBlöcke drin sind
    QList<ProgramBlock *> programmBlockList;

};

#endif // PROGRAMCODE_H
