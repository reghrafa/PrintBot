#ifndef PROGRAMCOMMANDBLOCK_H
#define PROGRAMCOMMANDBLOCK_H

#include <QMessageBox>
#include "Main.h"
#include "ProgramBlock.h"

class ProgramBlock;

class ProgramCommandBlock : public ProgramBlock
{
public:
    ProgramCommandBlock(QString newBlockName,
                        QString newSourcecode,
                        QColor newColor = Qt::red,
                        int newType = TYPE_PORT,
                        int newIndentationDepth = 1);
    ProgramCommandBlock(int newType,
                        QString newProgramBlockName,
                        int newID,
                        QString newSourcecode,
                        QColor newColor,
                        int newValue0,
                        int newValue1,
                        int newValue2,
                        double newValueDouble,
                        QString newQString,
                        bool newFirstBlock,
                        int newSuccessorID);
//    ProgramCommandBlock(QString newProgramBlockName, QString newSourcecode);
    virtual ~ProgramCommandBlock();
//    virtual void generateBlockListPixmap();
    virtual void generateSourcecode(bool highlight = false);
    virtual void updateLocation(int socketIndex = -1);
    virtual QPoint getAppendPosition();
    virtual void createDialog(Dialog *newDialog);

public slots:
    virtual void dialogValueChanged(int newValue);

protected:
    virtual void generatePixmap(bool movePixmap = 0);
    virtual void initializeBlock();
};

#endif // PROGRAMCOMMANDBLOCK_H
