#ifndef OPENFILEPARSER_H
#define OPENFILEPARSER_H

#include <QString>
#include <QDebug>

class ProgramBlock;
class MainWindow;
class ProgramCode;
class EditorWidget;

class OpenFileParser
{
public:
    OpenFileParser(MainWindow *newMainWindow, QString fileQString);
    ProgramCode &getProgramCode();
    EditorWidget *getEditorWidget();

private:
    void processFile(QString fileQString);
    void processLine(QString lineQString);
    void linkProgramBlocks();
    ProgramBlock *findProgramBlock(int id);
    void addBlocksToEditorWidget();

    QList<ProgramBlock *> programBlockList;
    MainWindow *mainWindow;
};

#endif // OPENFILEPARSER_H
