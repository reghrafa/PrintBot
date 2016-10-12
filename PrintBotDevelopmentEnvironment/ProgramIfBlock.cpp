#include "ProgramIfBlock.h"
#include "Dialog.h"

ProgramIfBlock::ProgramIfBlock(QString newProgramBlockName,
                               QString newSourcecode,
                               QColor newColor,
                               int newType,
                               int newIndentationDepth,
                               int newNumberOfSockets):
    ProgramSocketBlock(newProgramBlockName, newSourcecode, newColor, newType, newIndentationDepth, newNumberOfSockets)
{
//    qDebug() << "ProgramIfBlock::ProgramIfBlock";
    this->initializeBlock();
}

ProgramIfBlock::ProgramIfBlock(int newType,
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
    qDebug() << "ProgramIfBlock::ProgramIfBlock wird geladen.";
    this->initializeBlock();
}

ProgramIfBlock::~ProgramIfBlock()
{
    this->destroyBlock();
}

void ProgramIfBlock::generatePixmap(bool movePixmap)
{
//    qDebug() << "ProgramIfBlock::generatePixmap(bool movePixmap): " << movePixmap;
    QPoint socketBottomAppendPosition = getAddPosition();
    QPoint socketElseAppendPosition = getAddPosition(SOCKET_ELSE);

    QPolygon polygon;
    polygon << QPoint(0, 0)
            << QPoint(IFBLOCK_WIDTH, 0)
            << QPoint(IFBLOCK_WIDTH, IFBLOCK_TOP_MARGIN)
            << QPoint(IFBLOCK_LEFT_MARGIN, IFBLOCK_TOP_MARGIN)

            << QPoint(IFBLOCK_LEFT_MARGIN, (socketElseAppendPosition - this->pos()).y() - IFBLOCK_MIDDLE_MARGIN)
            << QPoint(IFBLOCK_WIDTH, (socketElseAppendPosition - this->pos()).y() - IFBLOCK_MIDDLE_MARGIN)
            << QPoint(IFBLOCK_WIDTH, (socketElseAppendPosition - this->pos()).y())
            << QPoint(IFBLOCK_LEFT_MARGIN, (socketElseAppendPosition - this->pos()).y())

            << QPoint(IFBLOCK_LEFT_MARGIN, (socketBottomAppendPosition - this->pos()).y() - IFBLOCK_BOTTOM_MARGIN)
            << QPoint(IFBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y() - IFBLOCK_BOTTOM_MARGIN)
            << QPoint(IFBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y())
            << QPoint(0, (socketBottomAppendPosition - this->pos()).y());
//    qDebug() << polygon;
    QImage image(IFBLOCK_WIDTH, (socketBottomAppendPosition - this->pos()).y(), QImage::Format_ARGB32_Premultiplied);
//    qDebug() << "ProgramIfBlock::generatePixmap, Maße des image: " << image.width() << image.height();
    QFont font;
    font.setPointSize(FONT_SIZE_PIXMAP_HEADING);

    QPainter qPainter;
    qPainter.begin(&image);
    qPainter.setRenderHint(QPainter::Antialiasing);

    ///abgelegte IfBlocks bekommen eine Füllung mit Hintergrundfarbe, um Graphik-Fehler zu verbergen
    if (this->parent())
    {
        ///TODO: +1 sorgt dafür, dass der schwarze Rahmen des rect nicht sichtbar ist
        QRect rect(0,0,IFBLOCK_WIDTH+1, (socketBottomAppendPosition - this->pos()).y());
        qPainter.setBrush(QColor(240,240,240));      //color
        qPainter.drawRect(rect);
    }

    highlightGraphics(qPainter);

    qPainter.setBrush(color);      //color
    qPainter.drawPolygon(polygon);
//    qPainter.setPen(Qt::black);

//    QString labelText = "";
//    QTextStream textStream(&labelText);
//    textStream << "Wenn " << this->name;
    qPainter.setFont(font);
    int middleHitboxHeight = (socketElseAppendPosition - this->pos()).y() - IFBLOCK_MIDDLE_MARGIN;
//    qPainter.drawText(0,7,image.width()-1,image.height()-1, Qt::AlignHCenter, labelText);
    qPainter.drawText(0,middleHitboxHeight + 2,image.width()-1,image.height()-1, Qt::AlignHCenter, "sonst");

    if (movePixmap)
    {
        qPainter.drawText(0,5,image.width()-1,image.height()-1, Qt::AlignHCenter, "Bedingung");
        this->movePixmap = QPixmap::fromImage(image);
    }
    else
    {
        QString caption = "";
        QTextStream captionStream(&caption);
        captionStream << "Wenn ";
        switch(this->type)
        {
        case TYPE_IF_CUSTOM:
            if (qString.length() > 10)
            {
                captionStream << qString.left(8) << "...";   //nur die ersten 10 Zeichen
            }
            else
            {
                captionStream << qString;
            }
            break;

        case TYPE_IF_ADC:
            captionStream << "P" << INT_IF_PORT + 6;

            if (INT_IF_RELATION == IF_RELATION_KLEINER)              captionStream << " < ";
            else if (INT_IF_RELATION == IF_RELATION_KLEINER_GLEICH)  captionStream << " ≤ ";
            else if (INT_IF_RELATION == IF_RELATION_GROESSER)        captionStream << " > ";
            else if (INT_IF_RELATION == IF_RELATION_GROESSER_GLEICH) captionStream << " ≥ ";
            else if (INT_IF_RELATION == IF_RELATION_GLEICH)          captionStream << " = ";
            else qDebug() << "ERROR: ProgramIfBlock::generatePixmap: undefined INT_IF_RELATION";

            captionStream << QString::number(valueDouble, 'f', 1 ) << " V";
            break;

        default:
            qDebug() << "ERROR: ProgramIfBlock::generatePixmap: undefined type";
            break;
        }
        font.setPointSize(12);
        qPainter.setFont(font);
        qPainter.drawText(0,10,image.width()-1,image.height()-1, Qt::AlignHCenter, caption.toLatin1());
        this->pixmap = QPixmap::fromImage(image);
        this->setPixmap(this->pixmap);

        this->resize(pixmap.size());
        this->setFrameRect(this->pixmap.rect());
    }



qPainter.end();
    this->pixmap = QPixmap::fromImage(image);
    this->setPixmap(this->pixmap);

    this->resize(pixmap.size());
    this->setFrameRect(this->pixmap.rect());
}

void ProgramIfBlock::initializeBlock()
{
//    qDebug() << "ProgramIfBlock::initializeBlock()";

    switch(this->type)
    {
    case TYPE_IF_CUSTOM:
        INT_IF_PORT = IF_PORT_8;
        INT_IF_RELATION = IF_RELATION_KLEINER;
        INT_IF_TYPE = IF_TYPE_CUSTOM;
        break;
    case TYPE_IF_ADC:
        INT_IF_PORT = IF_PORT_8;
        INT_IF_RELATION = IF_RELATION_KLEINER;
        INT_IF_TYPE = IF_TYPE_ADC;
        break;
    default:
        qDebug() << "ProgramIfBlock::initializeBlock(): undefined type";
        break;
    }

    ProgramSocketBlock::initializeBlock();
}

//void ProgramIfBlock::generateBlockListPixmap()
//{
//    QImage image(FUNCTIONBLOCK_WIDTH, FUNCTIONBLOCK_HEIGHT, QImage::Format_ARGB32_Premultiplied);
//    image.fill(color);
//    QFont font;
//    font.setPointSize(14);

//    QPainter qPainter;
//    qPainter.begin(&image);
//    qPainter.setRenderHint(QPainter::Antialiasing);
//    qPainter.drawRect(QRect(0,0,image.width()-1,image.height()-1));

//    qPainter.setFont(font);
//    qPainter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignCenter, this->name);
//    qPainter.end();

//    this->blockListPixmap = QPixmap::fromImage(image);
//}

void ProgramIfBlock::generateSourcecode(bool highlight)
{
//    qDebug() << "ProgramIfBlock::generateSourcecode, highlight: " << highlight;
//    highlight = true;
    sourcecode = "";
    QTextStream sourcecodeStream(&sourcecode);

    highlightBegin(sourcecodeStream, highlight);

    this->appendTabulator(sourcecodeStream);
    sourcecodeStream << "<span style=\"color:rgb(0,0,255)\">if</span> (";

    switch(this->type)
    {
    case TYPE_IF_CUSTOM:
            sourcecodeStream << qString << ")<br>";
        break;

    case TYPE_IF_ADC:
        sourcecodeStream << "ADC_Value(" << INT_IF_PORT << ")";

        if (INT_IF_RELATION == IF_RELATION_KLEINER)              sourcecodeStream << " &lt; ";
        else if (INT_IF_RELATION == IF_RELATION_KLEINER_GLEICH)  sourcecodeStream << " &lt;= ";
        else if (INT_IF_RELATION == IF_RELATION_GROESSER)        sourcecodeStream << " &gt; ";
        else if (INT_IF_RELATION == IF_RELATION_GROESSER_GLEICH) sourcecodeStream << " &gt;= ";
        else if (INT_IF_RELATION == IF_RELATION_GLEICH)          sourcecodeStream << " == ";
        else qDebug() << "ERROR: ProgramIfBlock::generateSourcecode: undefined INT_IF_RELATION";

//        double input_min = 0;
//        double input_max = 5.0;
//        int output_min = 0;
//        int output_max = 255;

//        double faktor = (output_max - output_min) / (input_max - input_min);
//        int valuePWM = round(output_min + valueDouble * faktor);

        sourcecodeStream << round(valueDouble/5 * 255) << ")<br>"; //QString::number(valueDouble, 'f', 2 ) << " V";
        break;

    default:
        qDebug() << "ERROR: ProgramIfBlock::generatePixmap: undefined type";
        break;
    }

    this->appendTabulator(sourcecodeStream);
    sourcecodeStream << "{<br>";

    highlightEnd(sourcecodeStream, highlight);

    ///Generiere Code für alle Blöcke innerhalb des if-Bereichs:
    if (this->blockList.at(SOCKET_IF))
    {
        qDebug() << "ProgramIfBlock::generateSourcecode, blocklost.at(SOCKET_IF): " << blockList.at(SOCKET_IF);
        sourcecodeStream << blockList.at(SOCKET_IF)->getSourcecode();
    }

    ///Generiere Code für alle Blöcke innerhalb des Else-Bereichs:
    if (this->blockList.at(SOCKET_ELSE))
    {
        highlightBegin(sourcecodeStream, highlight);
        this->appendTabulator(sourcecodeStream);
        sourcecodeStream << "}<br>";
        this->appendTabulator(sourcecodeStream);
        sourcecodeStream << "<span style=\"color:rgb(0,0,255)\">else</span><br>";
        this->appendTabulator(sourcecodeStream);
        sourcecodeStream << "{<br>";
        highlightEnd(sourcecodeStream, highlight);
        sourcecodeStream << this->blockList.at(SOCKET_ELSE)->getSourcecode();
    }
    highlightBegin(sourcecodeStream, highlight);
    this->appendTabulator(sourcecodeStream);
    sourcecodeStream << "}<br>";
    highlightEnd(sourcecodeStream, highlight);

    ///Generiere Code für alle Blöcke nach dem ifBlock:
    if (this->successor)
    {
        sourcecodeStream << this->successor->getSourcecode();
    }
//    this->updateTextEditorWidget();
//    qDebug() << "ProgramIfBlock::generateSourcecode: " << programBlockSourcecode;
}

void ProgramIfBlock::updateLocation(int socketIndex)
{
    ///Falls Vorgängerblock existiert, ändert sich ggf. die eigene Position
    if (this->predecessor)
    {
        this->move(this->predecessor->getAddPosition(socketIndex));
    }

    if (this->blockList.at(SOCKET_IF))
    {
        this->blockList.at(SOCKET_IF)->updateLocation(SOCKET_IF);
    }

    if (this->blockList.at(SOCKET_ELSE))
    {
        this->blockList.at(SOCKET_ELSE)->updateLocation(SOCKET_ELSE);
    }

    ///Wenn man alle Blöcke innerhalb des BedingungsBlock kennt, kann man das Pixmap eindeutig erstellen und positionieren, oder!?
    this->generateGraphics();

    if (this->successor)
    {
        this->successor->updateLocation();
    }
}

void ProgramIfBlock::updateHitboxes()
{
//    QPoint socketBottomAppendPosition = getAddPosition();
    QPoint socketElseAppendPosition = getAddPosition(SOCKET_ELSE);
    QRect *hitboxTop = new QRect(this->pos(), QSize(IFBLOCK_WIDTH, IFBLOCK_TOP_MARGIN + COMMANDBLOCK_HEIGHT));
    QRect *hitboxMiddle = new QRect(socketElseAppendPosition + QPoint(-IFBLOCK_LEFT_MARGIN, -IFBLOCK_MIDDLE_MARGIN), socketElseAppendPosition + QPoint(IFBLOCK_WIDTH-IFBLOCK_LEFT_MARGIN-1, COMMANDBLOCK_HEIGHT-1));
//    QRect *hitboxBottom = new QRect(socketBottomAppendPosition + QPoint(0, -IFBLOCK_BOTTOM_MARGIN), socketBottomAppendPosition + QPoint(IFBLOCK_WIDTH-1, -1));

    if (hitboxList.count() < 2)
    {
        hitboxList.insert(0, hitboxTop);
        hitboxList.insert(1, hitboxMiddle);
    }
    else
    {
        hitboxList.replace(0, hitboxTop);
        hitboxList.replace(1, hitboxMiddle);
    }


//    qDebug() << "ProgramIfBlock::updateHitboxes: " << *hitboxTop << *hitboxMiddle;
//    hitboxList.insert(2, hitboxBottom);
}


QPoint ProgramIfBlock::getAppendPosition()
{
    ///Falls im Else-Zweig kein Block ist, lasse etwas freien Platz
    if (blockList.at(SOCKET_ELSE) == NULL)
    {
        return this->getInsertChildPosition(SOCKET_ELSE) + QPoint(-IFBLOCK_LEFT_MARGIN, COMMANDBLOCK_HEIGHT + IFBLOCK_BOTTOM_MARGIN);
    }
    else
    {
        ///Finde den letzten Block innerhalb des Else-Zweigs
        ProgramBlock *programBlock = blockList.at(SOCKET_ELSE);
        while (programBlock->getSuccessor())
        {
            programBlock = programBlock->getSuccessor();
        }

        return programBlock->getAppendPosition() + QPoint(-IFBLOCK_LEFT_MARGIN, IFBLOCK_BOTTOM_MARGIN);
    }
}

QPoint ProgramIfBlock::getInsertChildPosition(int socketIndex)
{
    if (socketIndex == SOCKET_IF)
    {
        return this->pos() + QPoint(IFBLOCK_LEFT_MARGIN, IFBLOCK_TOP_MARGIN);
    }
    else if (socketIndex == SOCKET_ELSE)
    {
        if (blockList.at(SOCKET_IF) == NULL)
        {
            return this->pos() + QPoint(IFBLOCK_LEFT_MARGIN, IFBLOCK_TOP_MARGIN + COMMANDBLOCK_HEIGHT + IFBLOCK_MIDDLE_MARGIN);
        }
        else
        {
            ///Finde den letzten Block im If-Zweig
            ProgramBlock *programBlock = blockList.at(SOCKET_IF);
            while (programBlock->getSuccessor())
            {
                programBlock = programBlock->getSuccessor();
            }
            return programBlock->getAppendPosition() + QPoint(0, IFBLOCK_MIDDLE_MARGIN);
        }
    }
    else
    {
        qDebug() << "ProgramIfBlock::getAppendPosition: ERROR, socketTyp: " << socketIndex;
    }
}


void ProgramIfBlock::createDialog(Dialog *newDialog)
{
    qDebug() << "ProgramIfBlock::createDialog, type: " << type;
    ProgramBlock::createDialog(newDialog);

    QFont font;
    font.setPointSize(FONT_SIZE_DIALOG_HEADING);

    switch(this->type)
    {
    case TYPE_IF_CUSTOM:
        dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG).setText("Bedingung");
        dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG).setFont(font);

        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).addItem("Benutzerdefiniert");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).addItem("Analog-Digital-Wandler");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).setCurrentIndex(INT_IF_TYPE);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).setFixedSize(200,20);

        dialog->getQLineEdit().setFixedSize(200, 20);
        dialog->getQLineEdit().setText(qString);

        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG), 0, 0);
        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE), 1, 0, 1, 3);
        dialog->getQGridLayout().addWidget(&dialog->getQLineEdit(), 2, 0, 1, 3);

        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE), SIGNAL(currentIndexChanged(int)),
                this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQLineEdit(), SIGNAL(textChanged(QString)),
                this, SLOT(dialogQStringChanged(QString)));
        break;

    case TYPE_IF_ADC:
        dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG).setText("Bedingung");
        dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG).setFont(font);

        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).addItem("Benutzerdefiniert");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).addItem("Analog-Digital-Wandler");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).setCurrentIndex(INT_IF_TYPE);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).setFixedSize(200,20);
        //dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE).setFont(font);

        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).addItem("Spannung an Port 6");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).addItem("Spannung an Port 7");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).addItem("Spannung an Port 8");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).setCurrentIndex(INT_IF_PORT);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).setFixedSize(120,20);
        //dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT).setFont(font);

        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).addItem("<");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).addItem("≤");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).addItem(">");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).addItem("≥");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).addItem("=");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).setCurrentIndex(INT_IF_RELATION);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).setFixedSize(35,20);
        //dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION).setFont(font);

        ///Eingabefeld evtl. Spannung in Volt    
        dialog->getQDoubleSpinBox().setMinimum(0.0);
        dialog->getQDoubleSpinBox().setMaximum(5.0);
        dialog->getQDoubleSpinBox().setFixedSize(50,20);
        dialog->getQDoubleSpinBox().setValue(valueDouble);

        dialog->getQLabel(NUMBER_QLABEL_VOLT).setText("Volt");

        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_BEDINGUNG), 0, 0);
        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE), 1, 0, 1, 3);
        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT), 2, 0);
        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION), 2, 1);
        dialog->getQGridLayout().addWidget(&dialog->getQDoubleSpinBox(), 2, 2);
        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_VOLT), 2, 3);

        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_PORT), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_RELATION), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQDoubleSpinBox(), SIGNAL(valueChanged(double)), this, SLOT(dialogQDoubleSpinBoxChanged(double)));
        break;

    default:
        qDebug() << "ERROR: ProgramIfBlock::createDialog, undefined type: " << type;
        break;
    }
    qDebug() << "ProgramIfBlock::createDialog, ENDE, this->dialog: " << this->dialog;
}

void ProgramIfBlock::dialogValueChanged(int newValue)
{
    qDebug() << "ProgramIfBlock::dialogValueChanged";
    if (this->sender() == &dialog->getQComboBox(NUMBER_QCOMBOBOX_IF_TYPE))
    {
        value0 = newValue;
        if (newValue == IF_TYPE_CUSTOM)   type = TYPE_IF_CUSTOM;
        else if (newValue == IF_TYPE_ADC) type = TYPE_IF_ADC;
        else qDebug() << "ERROR: ProgramIfBlock::WidgetValueChanged, undefined newValue";
        qDebug() << "ProgramIfBlock::dialogValueChanged, type: " << type << " " << newValue;

        ((EditorWidget*)this->parent())->updateDialog(this);
    }
    else if (this->sender() == &dialog->getQComboBox(1)) value1 = newValue;
    else if (this->sender() == &dialog->getQComboBox(2)) value2 = newValue;
    else qDebug() << "ERROR: ProgramIfBlock::WidgetValueChanged, undefined this->sender()";

    this->generateGraphics();
    this->updateSourcecodeWidget();
}
