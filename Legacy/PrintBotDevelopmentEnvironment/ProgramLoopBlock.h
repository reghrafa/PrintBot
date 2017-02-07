#ifndef PROGRAMLOOPBLOCK_H
#define PROGRAMLOOPBLOCK_H

#include "Main.h"
#include "ProgramSocketBlock.h"
#include "EditorWidget.h"
#include "ProgramBlock.h"

class ProgramBlock;

class ProgramLoopBlock : public ProgramSocketBlock
{
public:
    ProgramLoopBlock(QString newProgramBlockName,
                     QString newSourcecode,
                     QColor newColor = Qt::red,
                     int newType = TYPE_LOOP_FOR,
                     int newIndentationDepth = 1);
    ProgramLoopBlock(int newType,
                     QString newProgramBlockName,
                     int newID, QString newSourcecode,
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
//    ProgramLoopBlock(QString newProgramBlockName, QString newSourcecode);
    virtual ~ProgramLoopBlock();
//    virtual void generateBlockListPixmap();
    virtual void generateSourcecode(bool highlight = false);
    virtual void updateLocation(int socketIndex = -1);
    virtual void updateHitboxes();
    virtual void createDialog(Dialog *newDialog = NULL);
    virtual QPoint getAppendPosition();
    virtual QPoint getInsertChildPosition(int socketIndex);
//    virtual int getSocketIndex(QPoint position);
    ProgramBlock *getLoopBlock() const;
    void setLoopBlock(ProgramBlock *newLoopBlock);

public slots:
    virtual void dialogValueChanged(int newValue);

protected:
    virtual void generatePixmap(bool movePixmap = 0);
    virtual void initializeBlock();
};

#endif // PROGRAMLOOPBLOCK_H
