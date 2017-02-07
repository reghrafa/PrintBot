#include "ProgramBlock.h"
//#include "ProgramCommandBlock.h"
//#include "ProgramFunction.h"
//#include "ProgramIfBlock.h"
//#include "ProgramLoopBlock.h"
//#include "ProgramSocketBlock.h"
#include "EditorWidget.h"
#include "Dialog.h"

int ProgramBlock::maxID = 0;

ProgramBlock::ProgramBlock(QString newProgramBlockName,
                           QString newSourcecode,
                           QColor newColor,
                           int newType,
                           int newIndentationDepth):
    name(newProgramBlockName),
    id(maxID++),
    originalSourcecode(newSourcecode),
    color(newColor),
    type(newType),
    indentationDepth(newIndentationDepth),
    successor(NULL),
    predecessor(NULL),
    dialog(NULL),
    qString(""),
    valueDouble(2.5),
    firstBlock(false)
{
    qDebug() << "Neuer ProgramBlock: " << name
             << "originalSourcecode: " << originalSourcecode
//             << "newSourcecode: "      << newSourcecode
             << " blockTyp: " << type
             << " Adresse: " << this;
}

ProgramBlock::ProgramBlock(int newType,
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
                           int newSuccessorID):
    type(newType),
    name(newProgramBlockName),
    id(newID),
    originalSourcecode(newSourcecode),
    color(newColor),
    value0(newValue0),
    value1(newValue1),
    value2(newValue2),
    valueDouble(newValueDouble),
    qString(newQString),
    firstBlock(newFirstBlock),
    successorID(newSuccessorID),
    successor(NULL),
    predecessor(NULL),
    dialog(NULL)
{
    qDebug() << "Neuer ProgramBlock " << name << " wird geladen.";
}

ProgramBlock::~ProgramBlock()
{
    qDebug() << "~ProgramBlock, name: " << name << ", id: " << id;
    EditorWidget *editorWidget = dynamic_cast<EditorWidget *> (this->parent());

    this->setFirstBlock(false); ///Lösche Block ggf. aus der ProgramCode-Liste

    if (this->predecessor)
    {
        this->predecessor->updateSuccessor(this, this->successor); ///"setSuccessor"
    }

    if (this->successor)
    {
        this->successor->setPredecessor(this->predecessor);
    }
    if (editorWidget)
    {
        if (this->predecessor)
        {
            editorWidget->updateBlockGroup(this->predecessor);
        }
        else if (this->successor) ///falls Nachfolger aber keine Vorgänger
        {
            this->successor->setFirstBlock(true);
            editorWidget->updateBlockGroup(this->successor);
        }
        this->updateSourcecodeWidget();

        if (dialog)
        {
            dialog->setProgramBlock(NULL); ///Damit nicht this->generateGraphics() aufgerufen wird.
            editorWidget->updateDialog(NULL);
        }

//        qDebug() << "parent: " << this->parent();
    }
    else if (dialog) qDebug() << "ERROR, ~ProgramBlock: Ohne Parent normalerweise kein Dialog!?";

//    qDebug() << "~ProgramBlock, Ende";
}

QString ProgramBlock::getSourcecode()
{
    this->generateSourcecode();
    return sourcecode;
}

QString ProgramBlock::getOriginalSourcecode()
{
    return originalSourcecode;
}

QString ProgramBlock::getName() const
{
    return name;
}

void ProgramBlock::setIndentationDepth(int newIndentationDepth)
{
    indentationDepth = newIndentationDepth;
}

void ProgramBlock::appendTabulator(QTextStream& textStream)
{
    for(int i = 0; i < this->indentationDepth; i++)
    {
        textStream << "<span style=\"background-color: white;\">"
                   << TAB
                   << "</span>";
    }
}

void ProgramBlock::updateSourcecodeWidget()
{
    EditorWidget *editorWidget = dynamic_cast<EditorWidget *> (parent());
    if (editorWidget)
    {
        editorWidget->updateSourcecodeWidget();
    }
    else
    {
        qDebug() << "ERROR: ProgramBlock::updateTextEditorWidget, undefinded parent";
    }
}

void ProgramBlock::highlightBegin(QTextStream &sourcecodeStream, bool highlight)
{
    if (dialog)
    {
        sourcecodeStream << "<span style=\"background-color: yellow;\">";
    }
}

void ProgramBlock::highlightEnd(QTextStream &sourcecodeStream, bool highlight)
{
    if (dialog)
    {
        sourcecodeStream << "</span>";
    }
}

//Zeichnet den Rahmen eines Blocks dicker (schwarz)
void ProgramBlock::highlightGraphics(QPainter &qPainter)
{
    if (dialog)
    {
        QPen qPen;
        qPen.setColor(Qt::black); ///TODO: evtl. Komplimentärfarbe?
        qPen.setWidth(5);
//        qPen.setStyle(Qt::NoPen);
        qPainter.setPen(qPen);
    }
}

void ProgramBlock::clearPredecessorAndSuccessor()
{
    if (this->successor)
    {
        this->successor->setPredecessor(this->predecessor);

        if (!this->predecessor)
        {
            this->successor->setFirstBlock(true);
        }
    }
    if (this->predecessor)
    {
        this->predecessor->updateSuccessor(this, this->successor);
//        qDebug() << "this->predecessor->getSuccessor: " << this->predecessor->getSuccessor();
    }
    EditorWidget *editorWidget = dynamic_cast<EditorWidget *> (parent());
    if (editorWidget)
    {
        editorWidget->updateBlockGroup(this);
    }
    this->successor = NULL;
    this->predecessor = NULL;
}

void ProgramBlock::clearPredecessor()
{
    if (this->predecessor)
    {

        this->predecessor->updateSuccessor(this, this->successor);
        this->predecessor = NULL;
    }
}

void ProgramBlock::appendBlock(ProgramBlock *newProgramBlock)
{
//    qDebug() << "ProgramBlock::appendBlock";
    if (this->successor)
    {
        newProgramBlock->setSuccessor(this->successor);
        this->successor->setPredecessor(newProgramBlock);
    }
    this->successor = newProgramBlock;
    newProgramBlock->setPredecessor(this);
    newProgramBlock->setIndentationDepth(this->indentationDepth);

    newProgramBlock->move(this->getAppendPosition());
}

void ProgramBlock::addBlock(ProgramBlock *newProgramBlock, QPoint mousePosition)
{
    if (newProgramBlock)
    {
        newProgramBlock->setFirstBlock(false);
        this->appendBlock(newProgramBlock);
    }
    else qDebug() << "ERROR, ProgramBlock::addBlock, undefined programBlock";
}

QPoint ProgramBlock::getAddPosition(int socketIndex)
{
    getAppendPosition();
}

QPixmap &ProgramBlock::getPixmap(bool movePixmap)
{
    if (movePixmap)
    {
        return this->movePixmap;
    }
    return this->pixmap;
}

QPixmap &ProgramBlock::getBlockListPixmap()
{
    this->generateBlockListPixmap();
    return this->blockListPixmap;
}

QColor ProgramBlock::getColor() const
{
    return color;
}

void ProgramBlock::setColor(const QColor &newColor)
{
    color = newColor;
    this->generateGraphics();
}

ProgramBlock *ProgramBlock::getPredecessor() const
{
    return predecessor;
}

void ProgramBlock::setPredecessor(ProgramBlock *newPredecessor)
{
    predecessor = newPredecessor;
}

ProgramBlock *ProgramBlock::getSuccessor() const
{
    return successor;
}

void ProgramBlock::setSuccessor(ProgramBlock *newSuccessor)
{
    successor = newSuccessor;
}

void ProgramBlock::generateGraphics(bool movePixmap)
{
//    qDebug() << "ProgramBlock::generateGraphics, " << movePixmap;
    this->generatePixmap(movePixmap);
}

void ProgramBlock::generateBlockListPixmap()
{
    QImage image(BLOCK_LIST_ICON_WIDTH, BLOCK_LIST_ICON_HEIGHT, QImage::Format_ARGB32_Premultiplied);
    image.fill(this->color);
    QFont font("Arial", FONT_SIZE_PIXMAP_BLOCK_LIST, QFont::Bold);
    //font.setPointSize(FONT_SIZE_PIXMAP_BLOCK_LIST);

    QPainter painter;
    painter.begin(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawRect(QRect(0,0,image.width()-1,image.height()-1));

    painter.setFont(font);
    painter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignCenter, this->name);
    painter.end();

    this->blockListPixmap = QPixmap::fromImage(image);
}

void ProgramBlock::initializeBlock()
{
    qDebug() << "ProgramBlock::initializeBlock()";
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->generateGraphics(MOVE_PIXMAP);
//    this->generateSourcecode();//gerade auskommentiert23.11.2014
}

QString ProgramBlock::getQString() const
{
    return this->qString;
}

void ProgramBlock::setQString(const QString &newQString)
{
    this->qString = newQString;
}

void ProgramBlock::setFirstBlock(bool firstBlock)
{
    qDebug () << "ProgramBlock::setFirstBlock, " << firstBlock;
    if (this->firstBlock != firstBlock)
    {
        this->firstBlock = firstBlock;
        EditorWidget *editorWidget = dynamic_cast<EditorWidget *> (this->parent());
        if (editorWidget)
        {
            if (this->firstBlock)
            {
                editorWidget->getProgramCode().addBlock(this);
            }
            else
            {
                editorWidget->getProgramCode().deleteBlock(this);
            }
        }
        else
        {
            qDebug() << "ERROR?, undefined parent";
        }
    }
}

bool ProgramBlock::isFirstBlock()
{
    return firstBlock;
}

QString ProgramBlock::generateSaveData()
{
    qDebug() << "ProgramBlock::generateSaveData()";
    QString saveData = "";
    QTextStream saveDataStream(&saveData);
    saveDataStream << type << ", "
                   << name << ", "
                   << id << ", "
                   << originalSourcecode << ", "
                   << color.rgba() << ", "
                   << value0 << ", "
                   << value1 << ", "
                   << value2 << ", "
                   << valueDouble << ", "
                   << qString << ", "
                   << firstBlock << ", ";

    if (firstBlock)
    {
        saveDataStream << this->pos().x() << ", "
                       << this->pos().y() << ", ";
    }

    if (successor)
    {
        saveDataStream << successor->getID() << ", ";
    }
    else
    {
        saveDataStream << "-1, ";
    }
    return saveData;
}

QString ProgramBlock::getSaveData()
{
    QString saveData = "";
    QTextStream saveDataStream(&saveData);
    saveDataStream  << this->generateSaveData()
                    << "\n";

    if (successor)
    {
        saveDataStream << successor->getSaveData();
    }
    return saveData;
}

int ProgramBlock::getID()
{
    return id;
}

int ProgramBlock::getSuccessorID()
{
    return successorID;
}

void ProgramBlock::dialogQStringChanged(QString newQString)
{
    this->qString = newQString;
    this->generateGraphics();
    //this->generateSourcecode();
    this->updateSourcecodeWidget();
}

void ProgramBlock::dialogQDoubleSpinBoxChanged(double newDouble)
{
    this->valueDouble = newDouble;
    this->generateGraphics();
    this->updateSourcecodeWidget();
}


void ProgramBlock::updateSuccessor(ProgramBlock *thisBlock, ProgramBlock *newSuccessor)
{
    this->successor = newSuccessor;
}

void ProgramBlock::deleteThisAndSuccessors()
{
    if (this->successor)
    {
        successor->deleteThisAndSuccessors();
    }
    delete this;
}

void ProgramBlock::createDialog(Dialog *newDialog)
{
    if (newDialog)
    {
        this->dialog = newDialog;
    }
    else
    {
        if (!dialog)
        {
            qDebug() << "ERROR: ProgramBlock::createDialog, undefined dialog & newDialog";
            return;
        }
    }
}

void ProgramBlock::deleteDialog()
{
//    qDebug() << "ProgramBlock::deleteDialog";
    this->dialog = NULL;
//    qDebug() << "ProgramBlock::deleteDialog, Ende";
}

Dialog *ProgramBlock::getDialog()
{
    return this->dialog;
}

int ProgramBlock::getType() const
{
    return this->type;
}

void ProgramBlock::setType(int newType)
{
    this->type = newType;
}

int ProgramBlock::getValue(int valueType) const
{
//    qDebug() << "ProgramBlock::getValue" << this;
    if     (valueType == 0) return value0;
    else if (valueType == 1) return value1;
    else if (valueType == 2) return value2;
    else qDebug() << "ERROR: ProgramBlock::getValue, undefined valueType";
}

void ProgramBlock::setValue(int valueType, int newValue)
{
    qDebug() << "ProgramBlock::setValue: " << valueType << newValue;
    if     (valueType == 0) value0 = newValue;
    else if (valueType == 1) value1 = newValue;
    else if (valueType == 2) value2 = newValue;
    else qDebug() << "ERROR: ProgramBlock::setValue, undefined valueType";
    this->generateGraphics();
    ///TODO: this->updateDialog() für Schleifen-Dialog-Änderungen
}
