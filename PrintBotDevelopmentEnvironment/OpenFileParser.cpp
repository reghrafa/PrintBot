#include "OpenFileParser.h"
#include <QTextStream>
#include <QList>
#include "Main.h"
#include "ProgramBlock.h"
#include "ProgramCommandBlock.h"
#include "ProgramFunction.h"
#include "ProgramIfBlock.h"
#include "ProgramLoopBlock.h"
#include "MainWindow.h"

OpenFileParser::OpenFileParser(MainWindow *newMainWindow, QString fileQString):
    mainWindow(newMainWindow)
{
    try
    {
        processFile(fileQString);
        linkProgramBlocks();
        addBlocksToEditorWidget();
        getEditorWidget()->updateSourcecodeWidget();
    }
    catch (QString* error)
    {
        qDebug() << "ERROR: OpenFileParser::processFile, " << *error;
    }
}

ProgramCode &OpenFileParser::getProgramCode()
{
    return mainWindow->getProgramCode();
}

EditorWidget *OpenFileParser::getEditorWidget()
{
    return mainWindow->getEditorWidget();
}

void OpenFileParser::processFile(QString fileQString)
{
    QTextStream fileStream(&fileQString);
        while (!fileStream.atEnd())
        {
            QString line = fileStream.readLine();
            qDebug() << line;
            if (line.count() > 10)
            {
                processLine(line);
            }
//            else
//            {
//                qDebug() << "WARNUNG: OpenFileParser::processFile, Leerzeile, o.Ä.!";
//            }
        }
}

void OpenFileParser::processLine(QString lineQString)
{
    int commaCounter = -1;
//    QTextStream lineStream(&lineQString);
//    int a = -1;
//    lineStream >> a;
//    qDebug() << "a: " << a;

//    QString name = "";
//    lineStream >> name;
//    qDebug() << "name: " << name;
///Einlesen aller Parameter
    QString typeQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int type = typeQString.toInt();
    QString name = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    QString idQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int id = idQString.toInt();
    QString originalSourcecode = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    QString colorRGBQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    uint colorRGB = colorRGBQString.toUInt();
    QString value0QString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int value0 = value0QString.toInt();
    QString value1QString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int value1 = value1QString.toInt();
    QString value2QString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int value2 = value2QString.toInt();
    QString valueDoubleQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    double valueDouble = valueDoubleQString.toDouble();
    QString qString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    QString firstBlockQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    bool firstBlock = firstBlockQString.toInt();

    QPoint position(-1,-1);
    if (firstBlock)
    {
        QString positionXQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
        position.setX(positionXQString.toInt());
        QString positionYQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
        position.setY(positionYQString.toInt());
    }

    QString successorIDQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
    int successorID = successorIDQString.toInt();

    qDebug() << type << ", "
             << name << ", "
             << id << ", "
             << originalSourcecode << ", "
             << colorRGB << ", "
             << value0 << ", "
             << value1 << ", "
             << value2 << ", "
             << valueDouble << ", "
             << qString << ", "
             << firstBlock << ", "
             << position.x() << ", "
             << position.y() << ", "
             << successorID << ", ";

    ProgramBlock *newBlock = NULL;
    switch(type)
    {
        case TYPE_FUNCTION:
        case TYPE_MAIN_FUNCTION:
        {
            QString returnType = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
            QString parameter = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
            newBlock = new ProgramFunction(type, name, id, originalSourcecode, colorRGB,
                                           value0, value1, value2, valueDouble,
                                           qString, firstBlock, successorID,
                                           returnType, parameter);
            break;
        }
        case TYPE_IF_CUSTOM:
        case TYPE_IF_ADC:
        {
            QString numberOfSocketsQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
            int numberOfSockets = numberOfSocketsQString.toInt();
    
    //        QList<int> socketBlockIDList;
            int socketBlockIDs[numberOfSockets];
            for (int i = 0; i < numberOfSockets; i++)
            {
                QString socketBlockQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
                socketBlockIDs[i] = socketBlockQString.toInt();
               // socketBlockIDList.append(socketBlockQString.toInt());
            }
    
            newBlock = new ProgramIfBlock(type, name, id, originalSourcecode, colorRGB,
                                          value0, value1, value2, valueDouble,
                                          qString, firstBlock, successorID,
                                          numberOfSockets, socketBlockIDs);
            break;
        }
        case TYPE_LOOP_FOR:
        case TYPE_LOOP_ENDLESS:
        case TYPE_LOOP_WHILE:
        {
            QString numberOfSocketsQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
            int numberOfSockets = numberOfSocketsQString.toInt();
    
    //        QList<int> socketBlockIDList;
            int socketBlockIDs[numberOfSockets];
            for (int i = 0; i < numberOfSockets; i++)
            {
                QString socketBlockQString = lineQString.section(',', ++commaCounter, commaCounter).trimmed();
                socketBlockIDs[i] = socketBlockQString.toInt();
               // socketBlockIDList.append(socketBlockQString.toInt());
            }
    
            newBlock = new ProgramLoopBlock(type, name, id, originalSourcecode, colorRGB,
                                            value0, value1, value2, valueDouble,
                                            qString, firstBlock, successorID,
                                            numberOfSockets, socketBlockIDs);
            break;
        }
        default:
        {
            newBlock = new ProgramCommandBlock(type, name, id, originalSourcecode, colorRGB,
                                               value0, value1, value2, valueDouble,
                                               qString, firstBlock, successorID);
            break;
        }
    }

    if (firstBlock)
    {
        newBlock->move(position);
        getProgramCode().addBlock(newBlock);
    }

    programBlockList.append(newBlock);
}

void OpenFileParser::linkProgramBlocks()
{
    qDebug() << "OpenFileParser::linkProgramBlocks";
    for (ProgramBlockIterator it = programBlockList.begin(); it != programBlockList.end(); it++)
    {
        qDebug() << "OpenFileParser::linkProgramBlocks, name: " << (*it)->getName();
        ///Verknüpfe direkte Nachfolger mit Vorgänger
        int successorID = (*it)->getSuccessorID();
        if(successorID >= 0)
        {
            ProgramBlock *successor = findProgramBlock(successorID);
            (*it)->setSuccessor(successor);
            successor->setPredecessor(*it);
        }

        ///Falls SocketBlock: Verknüpfe Sockets und Vorgänger
        ProgramSocketBlock *programSocketBlock = dynamic_cast<ProgramSocketBlock *>(*it);
        if (programSocketBlock)
        {
 //           qDebug() << "OpenFileParser::linkProgramBlocks: Socket Block!";
            for (int i = 0; i < programSocketBlock->getNumberOfSockets(); i++)
            {
                int socketBlockID = programSocketBlock->getSocketBlockID(i);
                if (socketBlockID >= 0)
                {
                    ProgramBlock *programBlockInSocket = findProgramBlock(socketBlockID);
                    programSocketBlock->addBlock(programBlockInSocket, i);
                    programBlockInSocket->setPredecessor(programSocketBlock);
                }

            }
        }
        else
        {
//            qDebug() << OpenFileParser::linkProgramBlocks: KEIN Socket Block!";
        }
    }
}

ProgramBlock *OpenFileParser::findProgramBlock(int id)
{
    for (ProgramBlockIterator it = programBlockList.begin(); it != programBlockList.end(); it++)
    {
        if ((*it)->getID() == id)
        {
            return (*it);
        }
    }
    qDebug() << "OpenFileParser::findProgramBlock, id: " << id << ", return NULL!";
    return NULL;
}

void OpenFileParser::addBlocksToEditorWidget()
{
    for (ProgramBlockIterator it = programBlockList.begin(); it != programBlockList.end(); it++)
    {
        getEditorWidget()->addBlock(*it, false);
    }
}

