#include "ProgramCode.h"
#include "MainWindow.h"

ProgramCode::ProgramCode():
    isMainInCode(0)
{

}

void ProgramCode::generateSourcecode()
{
//    qDebug() << "ProgramCode::generateSourcecode()";
    sourcecode = "";
    QTextStream SourcecodeStream(&sourcecode);
    SourcecodeStream << "<span style=\"color:rgb(128,64,0)\">#define</span> F_CPU 12000000UL<br>"
                     << "<span style=\"color:rgb(128,64,0)\">#include</span> &lt;avr/io.h&gt;<br>"
                     << "<span style=\"color:rgb(128,64,0)\">#include</span> &lt;avr/interrupt.h&gt;<br>"
                     << "<span style=\"color:rgb(128,64,0)\">#include</span> &lt;util/delay.h&gt;<br>"
                     << "<span style=\"color:rgb(128,64,0)\">#include</span> \"PrintBot_Library.h\"<br><br>";

    ///Liste aller Funktions-Header:
    for(ProgramBlockIterator it = programmBlockList.begin(); it != programmBlockList.end(); it++)
    {
        if ((*it)->getType() == TYPE_FUNCTION || (*it)->getType() == TYPE_MAIN_FUNCTION)
        {
            SourcecodeStream << ((ProgramFunction*) (*it))->getSourcecodeHeader();
        }
    }

    for(ProgramBlockIterator it = programmBlockList.begin(); it != programmBlockList.end(); it++)
    {
        if ((*it)->getType() == TYPE_FUNCTION || (*it)->getType() == TYPE_MAIN_FUNCTION)
        {
            SourcecodeStream << "<br>"
                             << (*it)->getSourcecode();
        }
    }
}

//            QMessageBox::warning(this, tr("Doppelte Main-Funktion"),
//                    tr("Es gibt schon einen Startblock!"));


void ProgramCode::addBlock(ProgramBlock *programBlock)
{
    qDebug() << "ProgramCode::addBlock, " << programBlock;
    if (programBlock)
    {
        this->programmBlockList.append(programBlock);
        if (programBlock->getType() == TYPE_MAIN_FUNCTION)
        {
            isMainInCode++;
            mainWindow->getBlockListWidget()->setStartBlockHidden(true);
    //        qDebug() << "000ProgramCode::addBlock, isMainInCode = " << isMainInCode;
        }
    }
    else qDebug() << "ProgramCode::addBlock, programBlock existiert nicht: " << programBlock;
}

void ProgramCode::deleteBlock(ProgramBlock *programBlock)
{
    qDebug() << "ProgramCode::deleteBlock, " << programBlock;
    if (programBlock)
    {
        if (programmBlockList.contains(programBlock))
        {
            if (programBlock->getType() == TYPE_MAIN_FUNCTION)
            {
                isMainInCode--;
    //            qDebug() << "111ProgramCode::deleteBlock, isMainInCode = " << isMainInCode;
                if (isMainInCode == 0)
                {
                     mainWindow->getBlockListWidget()->setStartBlockHidden(false);
//                        ProgramFunction *mainFunction = new ProgramFunction("Start", "", "", Qt::green, TYPE_MAIN_FUNCTION);
//                        this->mainWindow->addBlockToBlockListWidget(mainFunction, 0);
                }
            }
            this->programmBlockList.removeOne(programBlock);
            qDebug() << "ProgramCode::deleteBlock: erfolgreich!";
        }
        else qDebug() << "ERROR: ProgramCode::deleteBlock: Block ist nicht in programmBlockList!";
    }
    else qDebug() << "ERROR: ProgramCode::deleteBlock: programBlock existiert nicht: " << programBlock;
}

///aktualisiere die Pixmaps & Position aller Blöcke
void ProgramCode::updateAllBlockGraphics()
{
    for(ProgramBlockIterator it = programmBlockList.begin(); it != programmBlockList.end(); it++)
    {
        (*it)->updateLocation();
    }
}

///neues Dokument erstellen
void ProgramCode::resetAll()
{
    qDebug() << "ProgramCode::resetAll()";
    for (ProgramBlockIterator it = programmBlockList.begin(); it != programmBlockList.end(); it++)
    {
        (*it)->deleteThisAndSuccessors();
    }
    programmBlockList.clear();
    isMainInCode = 0;
    sourcecode = "";
}

bool ProgramCode::existenceOfMain()
{
    if (isMainInCode >= 0)
    {
        return isMainInCode;
    }
    qDebug() << "ERROR: ProgramCode::existenceOfMain(), isMainInCode: " << isMainInCode;
}

QString ProgramCode::getSourcecode()
{
    this->generateSourcecode();
    return this->sourcecode;
}

QString ProgramCode::getSaveData()
{
    qDebug() << "ProgramCode::getSaveData()";
    QString saveData = "";
    QTextStream saveDataStream(&saveData);

    ///Liste aller Funktions-Header:
    for(ProgramBlockIterator it = programmBlockList.begin(); it != programmBlockList.end(); it++)
    {
        saveDataStream << (*it)->getSaveData();
    }
    return saveData;
}

void ProgramCode::setMainWindow(MainWindow *newMainWindow)
{
    this->mainWindow = newMainWindow;
}

//void ProgramCode::updateLocation()
//{
//    for(ProgramFunctionIterator it = functionList.begin(); it != functionList.end(); it++)
//    {
//        (*it)->updateLocation();
//    }
//}

//void ProgramCode::newFunction(QString functionName, QString returnType, QString parameter)
//{
//    ProgramFunction *newFunctionPointer = new ProgramFunction(functionName, returnType, parameter);
//    this->functionList.append(newFunctionPointer);
//}

//ProgramFunction *ProgramCode::getFunction(QString functionName,
//                                          QString returnType,
//                                          QString parameter)
//{
//    for(ProgramFunctionIterator it = functionList.begin(); it != functionList.end(); it++)
//    {
//        if (((*it)->getName().compare(functionName)) == 0
//                && (*it)->getReturnType().compare(returnType) == 0
//                && (*it)->getParameter().compare(parameter) == 0)
//        {
//            return *it;
//        }
//    }
//    qDebug() << "ProgramCode::getProgramFunction - Funktion existiert nicht!";
//}
