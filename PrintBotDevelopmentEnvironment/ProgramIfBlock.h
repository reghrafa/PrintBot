#ifndef PROGRAMIFBLOCK_H
#define PROGRAMIFBLOCK_H

#include "Main.h"
#include "ProgramBlock.h"
#include "ProgramSocketBlock.h"
#include "ProgramCommandBlock.h"
#include "EditorWidget.h"

class ProgramBlock;

class ProgramIfBlock : public ProgramSocketBlock
{
public:
    ProgramIfBlock(QString newProgramBlockName,
                   QString newSourcecode,
                   QColor newColor = Qt::red,
                   int newType = TYPE_IF_CUSTOM,
                   int newIndentationDepth = 1,
                   int newNumberOfSockets = 2);
    ProgramIfBlock(int newType,
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
                   int newSuccessorID,
                   int newNumberOfSockets,
                   int socketBlockIDs[]);
//    ProgramIfBlock(QString newProgramBlockName, QString newSourcecode);
    virtual ~ProgramIfBlock();
//    virtual void generateBlockListPixmap();
    virtual void generateSourcecode(bool highlight = false);
    virtual void updateLocation(int socketIndex = -1);
    virtual void updateHitboxes();
    virtual QPoint getAppendPosition();
    virtual QPoint getInsertChildPosition(int socketIndex);
    virtual void createDialog(Dialog *newDialog);

public slots:
    virtual void dialogValueChanged(int newValue);

protected:
    virtual void generatePixmap(bool movePixmap = 0);
    virtual void initializeBlock();

//private:
//    ProgramBlock *ifBlock;
//    ProgramBlock *elseBlock;
//    QRect hitboxTop;
//    QRect hitboxMiddle;
//    QRect hitboxBottom;
};

#endif // PROGRAMIFBLOCK_H
