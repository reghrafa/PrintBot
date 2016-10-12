#include "ProgramFunction.h"
#include "Dialog.h"

ProgramFunction::ProgramFunction(QString FunctionName,
                                 QString newReturnType,
                                 QString newParameter,
                                 QColor newColor,
                                 int newType):
    returnType(newReturnType),
    parameter(newParameter),
    ProgramBlock(FunctionName, "kein Sourcecode!", newColor, newType)
{
//    qDebug() << "Neue ProgramFunction: " << FunctionName << ", returnType: " << returnType << ", parameter: " << parameter;

    this->initializeBlock();
//    ProgramCommandBlock* endBlock = new ProgramCommandBlock("end", "end(2);", Qt::darkCyan, ENDBLOCK);
//    this->appendBlock(endBlock);

}

ProgramFunction::ProgramFunction(int newType,
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
                                 QString newReturnType,
                                 QString newParameter):
    returnType(newReturnType),
    parameter(newParameter),
    ProgramBlock(newType, newProgramBlockName, newID, newSourcecode, newColor,
                 newValue0, newValue1, newValue2, newValueDouble, newQString,
                 newFirstBlock, newSuccessorID)
{

}

ProgramFunction::~ProgramFunction()
{
    qDebug() << "~ProgramFunction()";
}

void ProgramFunction::generateSourcecode(bool highlight)
{
//    qDebug() << "ProgramFunction::generateSourcecode, highlight: " << highlight;
    sourcecode = "";
    QTextStream sourcecodeStream(&sourcecode);

    if (this->name.compare("Start") == 0)
    {
        sourcecodeStream << "main";
    }
    else
    {
//        qDebug() << "RRRRRRRRRRRRRR predecessor: " << predecessor;
        if (this->predecessor) ///Falls dies ein Funktionsaufruf ist
        {
//            qDebug() << "PPPREEE this: " << this;
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << name;
        }
        else
        {
//            qDebug() << "NICHT PPPREEE this: " << this;
            sourcecodeStream << returnType << " " << name;
        }
    }
    if (this->predecessor)
    {
        sourcecodeStream << "(" << parameter << ");<br>";
    }
    else
    {
        sourcecodeStream << "(" << parameter << ")<br>{<br>";
    }


    if (this->name.compare("Start") == 0)
    {
        sourcecodeStream << TAB << "PrintBot_Library_Initialization();<br><br>";
    }

    if (this->successor)
    {
        sourcecodeStream << this->successor->getSourcecode();
    }
    if (this->name.compare("Start") == 0)
    {
        sourcecodeStream << "<br>" << TAB << "<span style=\"color:rgb(0,0,255)\">while</span> (1);<br>";
    }
    if (!predecessor)
    {
        sourcecodeStream << "}<br>";
    }

//    qDebug() << "ProgramFunction::generateQString: " << programFunctionSourcecode;
}

void ProgramFunction::generatePixmap(bool movePixmap)
{
//    qDebug() << "ProgramFunction::generatePixmap(bool movePixmap): " << movePixmap;
    QImage image(FUNCTIONBLOCK_WIDTH, FUNCTIONBLOCK_HEIGHT, QImage::Format_ARGB32_Premultiplied);
    image.fill(color);

    QFont font;
    font.setPointSize(14);

    QPainter qPainter;
    qPainter.begin(&image);
    qPainter.setRenderHint(QPainter::Antialiasing);
    //highlightGraphics(qPainter);
    qPainter.drawRect(QRect(0,0,image.width()-1,image.height()-1));

    qPainter.setFont(font);
    qPainter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignCenter, this->name);
    qPainter.end();

    if (movePixmap)
    {
        this->movePixmap = QPixmap::fromImage(image);
    }

    this->pixmap = QPixmap::fromImage(image);
}

void ProgramFunction::initializeBlock()
{
//    qDebug() << "ProgramFunction::initializeBlock()";
    ProgramBlock::initializeBlock();

    switch(this->type)
    {
    case TYPE_FUNCTION:
    case TYPE_MAIN_FUNCTION:
        break;
    default:
        qDebug() << "ERROR, ProgramFunction::setInitalConditions: undefined type";
        break;
    }
}

void ProgramFunction::generateBlockListPixmap()
{
    this->generatePixmap();
    blockListPixmap = pixmap;
}

void ProgramFunction::updateLocation(int socketIndex)
{
    ///Falls Vorgängerblock existiert, ändert sich ggf. die eigene Position
    if (this->predecessor)
    {
        this->move(this->predecessor->getAddPosition(socketIndex));
    }

    this->generateGraphics();

    if (this->successor)
    {
        this->successor->updateLocation();
    }
}

QString ProgramFunction::getReturnType() const
{
    return this->returnType;
}

QString ProgramFunction::getParameter() const
{
    return this->parameter;
}

QPoint ProgramFunction::getAppendPosition()
{
    return this->pos() + QPoint(0,FUNCTIONBLOCK_HEIGHT);
}

void ProgramFunction::createDialog(Dialog *newDialog)
{
    ProgramBlock::createDialog(newDialog);
}

QString ProgramFunction::getSourcecodeHeader()
{
    if (this->name.compare("Start") == 0)
    {
        return "";
    }

    QString sourcecodeHeader = "";
    QTextStream sourcecodeStream(&sourcecodeHeader);

    sourcecodeStream << returnType << " "
                     << name
                     << "(" << parameter << ");<br>";
    return sourcecodeHeader;
}

QString ProgramFunction::generateSaveData()
{
//    qDebug() << "ProgramFunction::generateSaveData";
    QString saveData = "";
    QTextStream saveDataStream(&saveData);
    saveDataStream << ProgramBlock::generateSaveData()
                   << returnType << ", "
                   << parameter << ", ";
    return saveData;
}

void ProgramFunction::dialogValueChanged(int newValue)
{
    ///TODO: ...

    this->generateGraphics();
    this->updateSourcecodeWidget();
}
