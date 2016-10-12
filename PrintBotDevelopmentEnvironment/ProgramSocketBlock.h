#ifndef PROGRAMSOCKETBLOCK_H
#define PROGRAMSOCKETBLOCK_H

#include <QList>
#include <QRect>
#include "ProgramBlock.h"

class ProgramBlock;

class ProgramSocketBlock : public ProgramBlock
{
public:
    ProgramSocketBlock(QString newProgramBlockName,
                       QString newSourcecode,
                       QColor newColor = Qt::red,
                       int newType = TYPE_PORT,
                       int newIndentationDepth = 1,
                       int newNumberOfSockets = 2);
    ProgramSocketBlock(int newType,
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
                       int newSocketBlockIDs[]);
    virtual ~ProgramSocketBlock();
    void insertChildBlock(ProgramBlock* newChildBlock, int socketIndex);
    virtual int getNumberOfSockets();
    virtual void generateGraphics(bool movePixmap = 0);
//    virtual void generateSourcecode() = 0;
    virtual void addBlock(ProgramBlock *newProgramBlock, QPoint mousePosition);
    void addBlock(ProgramBlock *newProgramBlock, int socketIndex);
    virtual QPoint getAddPosition(int socketIndex = -1);
    virtual QPoint getInsertChildPosition(int socketIndex) = 0;
    int getSocketIndex(QPoint position);
    virtual void updateSuccessor(ProgramBlock *thisBlock, ProgramBlock *newSuccessor);
    virtual QString generateSaveData();
    int getSocketBlockID(int index);

public slots:
    virtual void dialogValueChanged(int newValue);

protected:
    virtual void updateHitboxes() = 0;
    virtual void initializeBlock();
    void destroyBlock();

//protected:
//    virtual void generatePixmap(bool movePixmap = 0) = 0;

    QList<ProgramBlock*> blockList;
    QList<QRect*>        hitboxList;
    QList<int> socketBlockIDs;
    int numberOfSockets;
};

#endif // PROGRAMSOCKETBLOCK_H
