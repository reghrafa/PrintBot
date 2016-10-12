#include "ProgramLoopBlock.h"
#include "Dialog.h"

ProgramLoopBlock::ProgramLoopBlock(QString newProgramBlockName,
                                   QString newSourcecode,
                                   QColor newColor,
                                   int newType,
                                   int newIndentationDepth):
    ProgramSocketBlock(newProgramBlockName, newSourcecode, newColor, newType, newIndentationDepth, 1)
{
    this->initializeBlock();
}

ProgramLoopBlock::ProgramLoopBlock(int newType,
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
                               int socketBlockIDs[]):
    ProgramSocketBlock(newType, newProgramBlockName, newID, newSourcecode, newColor,
                 newValue0, newValue1, newValue2, newValueDouble, newQString,
                 newFirstBlock, newSuccessorID, newNumberOfSockets, socketBlockIDs)
{
    this->initializeBlock();
}

ProgramLoopBlock::~ProgramLoopBlock()
{
    this->destroyBlock();
}

void ProgramLoopBlock::generatePixmap(bool movePixmap)
{
//    qDebug() << "ProgramLoopBlock::generatePixmap(bool movePixmap):" << movePixmap;
    QPoint socketBottomAppendPosition = getAddPosition();

    if (movePixmap)
    {
        socketBottomAppendPosition = this->pos() + QPoint(0, LOOPBLOCK_TOP_MARGIN + COMMANDBLOCK_HEIGHT + LOOPBLOCK_BOTTOM_MARGIN);
    }

    QPolygon polygon;
    polygon << QPoint(0, 0)
            << QPoint(LOOPBLOCK_WIDTH, 0)
            << QPoint(LOOPBLOCK_WIDTH, LOOPBLOCK_TOP_MARGIN)
            << QPoint(LOOPBLOCK_LEFT_MARGIN, LOOPBLOCK_TOP_MARGIN)
            << QPoint(LOOPBLOCK_LEFT_MARGIN, (socketBottomAppendPosition - this->pos()).y() - LOOPBLOCK_BOTTOM_MARGIN)
            << QPoint(LOOPBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y() - LOOPBLOCK_BOTTOM_MARGIN)
            << QPoint(LOOPBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y())
            << QPoint(0, (socketBottomAppendPosition - this->pos()).y());
//    qDebug() << polygon;
    QImage image(LOOPBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y(), QImage::Format_ARGB32_Premultiplied);
//    qDebug() << "ProgramLoopBlock::generatePixmap, Maße des image: " << image.width() << image.height();

    QFont font;
    font.setPointSize(FONT_SIZE_PIXMAP_HEADING);

    QPainter qPainter;
    qPainter.begin(&image);
    qPainter.setRenderHint(QPainter::Antialiasing);

    ///abgelegte LoopBlocks bekommen eine Füllung mit Hintergrundfarbe, um Graphik-Fehler zu verbergen
    if (this->parent() && !movePixmap)
    {
        ///TODO: +1 sorgt dafür, dass der schwarze Rahmen des rect nicht sichtbar ist
        QRect rect(0,0,LOOPBLOCK_WIDTH + 1, (socketBottomAppendPosition - this->pos()).y());
        qPainter.setBrush(QColor(240,240,240));      //color
        qPainter.drawRect(rect);
    }

    highlightGraphics(qPainter);
    qPainter.setBrush(color);      //color
    qPainter.drawPolygon(polygon);

    qPainter.setFont(font);

    if (movePixmap)
    {
        qPainter.drawText(0,5,image.width()-1,image.height()-1, Qt::AlignHCenter, "Schleife");
        this->movePixmap = QPixmap::fromImage(image);
    }
    else
    {
        QString caption = "";
        QTextStream captionStream(&caption);
        switch(this->type)
        {
        case TYPE_LOOP_FOR:
            captionStream << "Wiederhole\n" << INT_LOOP_REPETITIONS << " mal";
            break;

        case TYPE_LOOP_ENDLESS:
            captionStream << "Wiederhole\n fortlaufend";
            break;

        case TYPE_LOOP_WHILE:
            captionStream << "Wiederhole während\n" << originalSourcecode << " gilt";
            break;

        default:
            qDebug() << "ERROR: ProgramLoopBlock::generatePixmap: undefined type";
            break;
        }
        font.setPointSize(12);
        qPainter.setFont(font);
        qPainter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignHCenter, caption.toLatin1());
        this->pixmap = QPixmap::fromImage(image);
        this->setPixmap(this->pixmap);

        this->resize(pixmap.size());
        this->setFrameRect(this->pixmap.rect());
    }
    qPainter.end();
}

void ProgramLoopBlock::initializeBlock()
{
//    qDebug() << "ProgramLoopBlock::initializeBlock()";

    switch(this->type)
    {
    case TYPE_LOOP_FOR:
        INT_LOOP_REPETITIONS = 10;
        INT_LOOP_TYPE = LOOP_TYPE_FOR;
        break;
    case TYPE_LOOP_WHILE:
        INT_LOOP_TYPE = LOOP_TYPE_WHILE;
        break;
    case TYPE_LOOP_ENDLESS:
        INT_LOOP_TYPE = LOOP_TYPE_ENDLESS;
        break;
    default:
        qDebug() << "ProgramLoopBlock::initializeBlock(): undefined type";
        break;
    }

    ProgramSocketBlock::initializeBlock();
}

///generiere das Pixmap, welches in der BlockList zu sehen ist.
//void ProgramLoopBlock::generateBlockListPixmap()
//{
//    QImage image(FUNCTIONBLOCK_WIDTH, FUNCTIONBLOCK_HEIGHT, QImage::Format_ARGB32_Premultiplied);
//    image.fill(color);
//    QFont font;
//    font.setPointSize(14);

//    QPainter painter;
//    qPainter.begin(&image);
//    qPainter.setRenderHint(QPainter::Antialiasing);
//    qPainter.drawRect(QRect(0,0,image.width()-1,image.height()-1));

//    qPainter.setFont(font);
//    qPainter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignCenter, "Schleife");
//    qPainter.end();

//    this->blockListPixmap = QPixmap::fromImage(image);
//}

void ProgramLoopBlock::generateSourcecode(bool highlight)
{
//    qDebug() << "ProgramLoopBlock::generateSourcecode, highlight: " << highlight;
    this->sourcecode = "";
    QTextStream sourcecodeStream(&sourcecode);
    this->appendTabulator(sourcecodeStream);

    highlightBegin(sourcecodeStream, highlight);

    switch(this->type)
    {
    case TYPE_LOOP_FOR:
        sourcecodeStream << "<span style=\"color:rgb(0,0,255)\">for</span> (<span style=\"color:rgb(128,0,255)\">int</span> i = 0; i &lt; " << INT_LOOP_REPETITIONS << "; i++)<br>";
        break;

    case TYPE_LOOP_ENDLESS:
        sourcecodeStream << "<span style=\"color:rgb(0,0,255)\">while</span> (1)<br>";
        break;

    case TYPE_LOOP_WHILE:
        sourcecodeStream << "<span style=\"color:rgb(0,0,255)\">while</span> (" << originalSourcecode << ")<br>";
        break;

    default:
        sourcecodeStream << originalSourcecode << "<br>";
        break;
    }

    this->appendTabulator(sourcecodeStream);
    sourcecodeStream << "{<br>";
    highlightEnd(sourcecodeStream, highlight);

    ///Generiere Code für alle Blöcke innerhalb der Schleife:
    if (this->blockList.at(SOCKET_LOOP))
    {
        sourcecodeStream << this->blockList.at(SOCKET_LOOP)->getSourcecode();
    }
    highlightBegin(sourcecodeStream, highlight);
    this->appendTabulator(sourcecodeStream);
    sourcecodeStream << "}<br>";
    highlightEnd(sourcecodeStream, highlight);

    ///Generiere Code für alle Blöcke nach dem loopBlock:
    if (this->successor)
    {
        sourcecodeStream << this->successor->getSourcecode();
    }

//    this->updateTextEditorWidget();
//    qDebug() << "ProgramIfBlock::generateSourcecode: " << programBlockSourcecode;
}

///aktualisiere die eigene Position und lasse alle daranhängenden Blöcke auch ihre Position ändern.
void ProgramLoopBlock::updateLocation(int socketIndex)
{
    ///Falls Vorgängerblock existiert, ändert sich ggf. die eigene Position
    if (this->predecessor)
    {
        this->move(this->predecessor->getAddPosition(socketIndex));
    }

    if (this->blockList.at(SOCKET_LOOP))
    {
        this->blockList.at(SOCKET_LOOP)->updateLocation(SOCKET_LOOP);
    }
    ///Wenn man alle Blöcke innerhalb der Schleife kennt, kann man das Pixmap eindeutig erstellen und positionieren, oder!?
    this->generateGraphics();

    if (this->successor)
    {
        this->successor->updateLocation();
    }
}

void ProgramLoopBlock::updateHitboxes()
{
//    QPoint socketBottomAppendPosition = getAppendPosition();
    QRect *hitboxTop = new QRect(this->pos(), QSize(LOOPBLOCK_WIDTH, LOOPBLOCK_TOP_MARGIN + COMMANDBLOCK_HEIGHT));
//    QRect *hitboxBottom = new QRect(socketBottomAppendPosition - QPoint(0, LOOPBLOCK_BOTTOM_MARGIN), socketBottomAppendPosition + QPoint(LOOPBLOCK_WIDTH-1, -1));
    if (hitboxList.count() < 1)
    {
        hitboxList.insert(0, hitboxTop);
    }
    else
    {
        hitboxList.replace(0, hitboxTop);
    }

//    hitboxList.insert(0, hitboxTop);
//    hitboxList.insert(1, hitboxBottom);

//    qDebug() << hitboxTop << hitboxBottom;
//    qDebug() << "ProgramLoopBlock::updateHitboxes, hitboxList.count(): " << hitboxList.count();
}

void ProgramLoopBlock::createDialog(Dialog *newDialog)
{
//    qDebug() << "ProgramLoopBlock::createDialog, type: " << type;
    ProgramBlock::createDialog(newDialog);

    QFont font;
    font.setPointSize(FONT_SIZE_DIALOG_HEADING);

    switch(this->type)
    {
    case TYPE_LOOP_FOR:
        dialog->getQLabel(NUMBER_QLABEL_LOOP_REPETITIONS).setText("Anzahl an Wiederholungen: ");

        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Zählschleife");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Endlosschleife");
//        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Bedingungsschleife");
        qDebug() << "ProgramLoopBlock::createDialog, getQComboBox.setCurrentIndex: " << INT_LOOP_TYPE;
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setCurrentIndex(INT_LOOP_TYPE);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setFixedSize(185,30);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setFont(font);

        dialog->getQSpinBox().setMinimum(0);
        dialog->getQSpinBox().setMaximum(9999);
        dialog->getQSpinBox().setFixedSize(55,20);
        dialog->getQSpinBox().setValue(INT_LOOP_REPETITIONS);
        qDebug() << "ProgramLoopBlock::createDialog, getQSpinBox.setValue: " << INT_LOOP_REPETITIONS;

        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE), 0,0,1,2);
        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_LOOP_REPETITIONS), 1,0);
        dialog->getQGridLayout().addWidget(&dialog->getQSpinBox(), 1,1);//,2,1);

        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE), SIGNAL(currentIndexChanged(int)),
                this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQSpinBox(), SIGNAL(valueChanged(int)),
                this, SLOT(dialogValueChanged(int)));
        break;

    case TYPE_LOOP_ENDLESS:
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Zählschleife");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Endlosschleife");
//        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).addItem("Bedingungsschleife");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setCurrentIndex(INT_LOOP_TYPE);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setFixedSize(185,30);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE).setFont(font);

        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE), 0,0,1,2);
//        dialog->getQGridLayout().addWidget(NULL, 1,0,1,2);

        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE), SIGNAL(currentIndexChanged(int)),
                this, SLOT(dialogValueChanged(int)));
//        connect(comboBoxLoopType, SIGNAL(currentIndexChanged(int)), this, SLOT(changeType(int)));
        break;

    case TYPE_LOOP_WHILE:
        break;
    default:
        qDebug() << "ERROR: ProgramLoopBlock::createDialog, undefined type: " << type;
        break;
    }
    qDebug() << "ProgramLoopBlock::createDialog, ENDE, this->dialog: " << this->dialog;
}

///Gibt die absolute Anhängposition zurück
QPoint ProgramLoopBlock::getAppendPosition()
{
    ///Falls in der Schleife kein Block ist, lasse etwas freien Platz
    if (blockList.at(SOCKET_LOOP) == NULL)
    {
        return this->pos() + QPoint(0, LOOPBLOCK_TOP_MARGIN + COMMANDBLOCK_HEIGHT + LOOPBLOCK_BOTTOM_MARGIN);
    }
    else
    {
        ///Finde den letzten Block innerhalb der Schleife
        ProgramBlock *programBlock = blockList.at(SOCKET_LOOP);
        while (programBlock->getSuccessor())
        {
            programBlock = programBlock->getSuccessor();
        }

        return programBlock->getAppendPosition() + QPoint(-LOOPBLOCK_LEFT_MARGIN, LOOPBLOCK_BOTTOM_MARGIN);
    }
}

QPoint ProgramLoopBlock::getInsertChildPosition(int socketIndex)
{
    if (socketIndex == SOCKET_LOOP)
    {
        return this->pos() + QPoint(LOOPBLOCK_LEFT_MARGIN, LOOPBLOCK_TOP_MARGIN);
    }
    else
    {
        qDebug() << "ProgramLoopBlock::getAppendPosition: ERROR";
    }
}

void ProgramLoopBlock::dialogValueChanged(int newValue)
{
    if (this->sender() ==  &dialog->getQSpinBox())
    {
        this->value0 = newValue;
        qDebug() << "ProgramLoopBlock::dialogValueChanged, newValue0: " << value0;
    }
    else if (this->sender() == &dialog->getQComboBox(NUMBER_QCOMBOBOX_LOOP_TYPE))
    {
        this->value1 = newValue;
        this->type = newValue + 3;
        if (type == TYPE_LOOP_FOR) name = "Zählschleife";
        else if (type == TYPE_LOOP_ENDLESS) name = "Endlosschleife";
        else if (type == TYPE_LOOP_WHILE) name = "Bedingungsschleife";
        else qDebug() << "ERROR: ProgramLoopBlock::dialogValueChanged, undefined newType: " << type;

        qDebug() << "ProgramLoopBlock::dialogValueChanged, newType: " << type;
//        qDebug() << "ProgramLoopBlock::dialogValueChanged, newValue1: " << value1;
//        qDebug() << "ProgramLoopBlock::dialogValueChanged, newName: " << name;
        ((EditorWidget*)this->parent())->updateDialog(this);
    }
    else
    {
        qDebug() << "ERROR: ProgramLoopBlock::WidgetValueChanged, undefined this->sender()";
//        qDebug() << "ERROR: ProgramLoopBlock::WidgetValueChanged, sender(): " << this->sender();
//        qDebug() << "ERROR: ProgramLoopBlock::WidgetValueChanged, dialog: " << this->dialog;
    }

    this->generateGraphics();
    //this->generateSourcecode();
    this->updateSourcecodeWidget();
//    qDebug() << "ProgramLoopBlock::dialogValueChanged, ENDE";
}
