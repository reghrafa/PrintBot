#include "ProgramSocketBlock.h"

ProgramSocketBlock::ProgramSocketBlock(QString newProgramBlockName,
                                   QString newSourcecode,
                                   QColor newColor,
                                   int newType,
                                   int newIndentationDepth,
                                   int newNumberOfSockets):
    numberOfSockets(newNumberOfSockets),
    ProgramBlock(newProgramBlockName, newSourcecode, newColor, newType, newIndentationDepth)
{
//    qDebug() << "ProgramSocketBlock::ProgramSocketBlock";
}

ProgramSocketBlock::ProgramSocketBlock(int newType,
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
                               int newSocketBlockIDs[]):
    numberOfSockets(newNumberOfSockets),
    ProgramBlock(newType, newProgramBlockName, newID, newSourcecode, newColor,
                 newValue0, newValue1, newValue2, newValueDouble, newQString,
                 newFirstBlock, newSuccessorID)
{
    qDebug() << "ProgramSocketBlock::ProgramSocketBlock";
    for (int i = 0; i < numberOfSockets; i++)
    {
        socketBlockIDs.append(newSocketBlockIDs[i]);
    }
}

ProgramSocketBlock::~ProgramSocketBlock()
{

}

void ProgramSocketBlock::insertChildBlock(ProgramBlock *newChildBlock, int socketIndex)
{
    if (this->blockList.at(socketIndex))
    {
        newChildBlock->setSuccessor(this->blockList.at(socketIndex));
        this->blockList.at(socketIndex)->setPredecessor(newChildBlock);
    }

    this->blockList.replace(socketIndex, newChildBlock);
    newChildBlock->setPredecessor(this);
    newChildBlock->setIndentationDepth(this->indentationDepth + 1);

    this->generateSourcecode();

    newChildBlock->move(this->getInsertChildPosition(socketIndex));
}

int ProgramSocketBlock::getNumberOfSockets()
{
    return numberOfSockets;
}

void ProgramSocketBlock::generateGraphics(bool movePixmap)
{
    qDebug() << "ProgramSocketBlock::generateGraphics, " << movePixmap;
    this->generatePixmap(movePixmap);
    this->updateHitboxes();
}

void ProgramSocketBlock::addBlock(ProgramBlock *newProgramBlock, QPoint mousePosition)
{
    if (newProgramBlock)
    {
        newProgramBlock->setFirstBlock(false);
        int socketIndex = this->getSocketIndex(mousePosition);
        if (socketIndex == -1)
        {
            this->appendBlock(newProgramBlock);
        }
        else
        {
            this->insertChildBlock(newProgramBlock, socketIndex);
        }
    }
    else qDebug() << "ERROR, ProgramSocketBlock::addBlock, undefined programBlock";
}

void ProgramSocketBlock::addBlock(ProgramBlock *newProgramBlock, int socketIndex)
{
    if (socketIndex == -1)
    {
        this->appendBlock(newProgramBlock);
    }
    else
    {
        this->insertChildBlock(newProgramBlock, socketIndex);
    }
}

QPoint ProgramSocketBlock::getAddPosition(int socketIndex)
{
    if (socketIndex == -1)
    {
        this->getAppendPosition();
    }
    else
    {
        this->getInsertChildPosition(socketIndex);
    }
}

int ProgramSocketBlock::getSocketIndex(QPoint position)
{
//    qDebug() << "ProgramSocketBlock::getSocketIndex, hitboxList.count(): " << hitboxList.count();
    for (int i = 0; i < hitboxList.length(); i++)
    {
        if (hitboxList.at(i)->contains(position))
        {
            return i;
        }
    }

    ///hitboxes wurden nicht getroffen, d.h. Bereich getroffen zum 'appenden' des Blocks
    return -1;
}

void ProgramSocketBlock::initializeBlock()
{
    qDebug() << "ProgramSocketBlock::initializeBlock(), numberOfSockets: " << numberOfSockets;
    for (int i = 0; i < numberOfSockets; i++)
    {
        blockList.append(NULL);
    }
    ProgramBlock::initializeBlock();


//    this->generateGraphics(MOVE_PIXMAP);
//    this->generateSourcecode();
}

void ProgramSocketBlock::destroyBlock()
{
    ///Lösche alle Blöcke innerhalb der Sockets
    for (int i = 0; i < numberOfSockets; i++)
    {
        if (this->blockList.at(i))
        {
            ProgramBlock* programBlock = this->blockList.at(i);
            while (programBlock)
            {
                ProgramBlock* deleteProgramBlock = programBlock;
                programBlock = programBlock->getSuccessor();
                deleteProgramBlock->~ProgramBlock();
            }
        }
    }
}

///Vor dem Löschen des thisBlock aufrufen!?
void ProgramSocketBlock::updateSuccessor(ProgramBlock *thisBlock, ProgramBlock *newSuccessor)
{
    int indexOfThisBlock = blockList.indexOf(thisBlock);

    ///Falls thisBlock in keinem Socket steckt, d.h. am BOTTOM_SOCKET hängt:
    if (indexOfThisBlock == -1)
    {
        this->successor = newSuccessor;
    }
    else
    {
        this->blockList.replace(indexOfThisBlock, newSuccessor);
    }
}

QString ProgramSocketBlock::generateSaveData()
{
    qDebug() << "ProgramSocketBlock::generateSaveData()";
    QString saveData = "";
    QTextStream saveDataStream(&saveData);
    saveDataStream << ProgramBlock::generateSaveData()
                   << numberOfSockets << ", ";

//    for (QList<ProgramBlock*>::iterator it = blockList.begin(); it != blockList.end(); it++)
//    {
//        saveDataStream << (*it)->getSaveData();
//    }

    for (int i = 0; i < numberOfSockets; i++)
    {
        if (blockList.at(i))
        {
            saveDataStream << blockList.at(i)->getID() << ", ";
        }
        else
        {
            saveDataStream << "-2, ";
        }
    }
    saveDataStream << "\n";

    for (int i = 0; i < numberOfSockets; i++)
    {
        if (blockList.at(i))
        {
            saveDataStream << blockList.at(i)->getSaveData();
        }
    }

    return saveData;
}

int ProgramSocketBlock::getSocketBlockID(int index)
{
    return socketBlockIDs.at(index);
}

void ProgramSocketBlock::dialogValueChanged(int newValue)
{

}

