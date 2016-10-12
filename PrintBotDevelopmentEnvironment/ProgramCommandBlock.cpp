#include "ProgramCommandBlock.h"
#include "EditorWidget.h"
#include "Dialog.h"

ProgramCommandBlock::ProgramCommandBlock(QString newBlockName,
                                         QString newSourcecode,
                                         QColor newColor,
                                         int newType,
                                         int newIndentationDepth):
    ProgramBlock(newBlockName, newSourcecode, newColor, newType, newIndentationDepth)
{
//    qDebug() << "neuer ProgramCommandBlock: " << newBlockName << "newSourcecode" << newSourcecode;
    this->initializeBlock();
}

ProgramCommandBlock::ProgramCommandBlock(int newType,
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
    ProgramBlock(newType, newProgramBlockName, newID, newSourcecode, newColor,
                 newValue0, newValue1, newValue2, newValueDouble, newQString,
                 newFirstBlock, newSuccessorID)
{

}

ProgramCommandBlock::~ProgramCommandBlock()
{

}

void ProgramCommandBlock::generatePixmap(bool movePixmap)
{
//    qDebug() << "ProgramCommandBlock::generatePixmap(bool movePixmap): " << movePixmap;
    QImage image(COMMANDBLOCK_WIDTH, COMMANDBLOCK_HEIGHT /*+ counter*/, QImage::Format_ARGB32_Premultiplied);
    image.fill(color);

    QFont fontHeading("Arial", FONT_SIZE_PIXMAP_HEADING, QFont::DemiBold);
    QFont fontSubHeading("Arial", FONT_SIZE_PIXMAP_SUBHEADING, QFont::Normal);
    //QFont font(T)
    //font.setPointSize(FONT_SIZE_PIXMAP_HEADING);

    QPainter qPainter;
    qPainter.begin(&image);
    qPainter.setRenderHint(QPainter::Antialiasing);
    //qPainter.setBrush(QBrush(Qt::yellow));

    highlightGraphics(qPainter);

    qPainter.drawRect(QRect(0,0,image.width()-1,image.height()-1));
    //qPainter.setBrush(QBrush(Qt::green));

    qPainter.setPen(Qt::black);
    qPainter.setFont(fontHeading);

    if (movePixmap)
    {
        qPainter.drawText(0,0,image.width()-1,image.height()-1, Qt::AlignCenter, this->name);
        this->movePixmap = QPixmap::fromImage(image);
    }
    else
    {
        QString captionString = "";
        QTextStream captionStream(&captionString);

        switch(this->type)
        {
        case TYPE_WAIT:
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, this->name);
            captionStream << INT_DELAY_VALUE;
            if (INT_DELAY_UNIT == UNIT_s) captionStream << " s";
            else if (INT_DELAY_UNIT == UNIT_ms) captionStream << " ms";
            else if (INT_DELAY_UNIT == UNIT_us) captionStream << " µs";
            else qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, TYPE_WAIT, undefined INT_DELAY_UNIT";

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0,15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString.toLatin1());
            break;

        case TYPE_PORT:
            captionStream << "Port " << INT_PORT_NUMBER;
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString);
            captionString = "";
            if (INT_PORT_OPERATION == PORT_OPERATION_AN) captionStream << " an";
            else if (INT_PORT_OPERATION == PORT_OPERATION_AUS) captionStream << " aus";
            else if (INT_PORT_OPERATION == PORT_OPERATION_TOGGLE) captionStream << " toggeln";
            else qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, undefined INT_PORT_OPERATION";

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0,15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString);
            break;

        case TYPE_LED:
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, "Status LED");
            if (INT_PORT_OPERATION == PORT_OPERATION_AN) captionStream << " an";
            else if (INT_PORT_OPERATION == PORT_OPERATION_AUS) captionStream << " aus";
            else if (INT_PORT_OPERATION == PORT_OPERATION_TOGGLE) captionStream << " toggeln";
            else qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, undefined INT_PORT_OPERATION";

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0,15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString);
            break;

        case TYPE_MOTOR:
            captionStream << "Motor " << (INT_MOTOR_NUMBER + 1) ;
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString);

            captionString = "";
            captionStream << (u_int) (INT_MOTOR_SPEED / 2.55) << " %\n";
            if (INT_MOTOR_OPERATION == MOTOR_OPERATION_FORWARD) captionStream << "vorwärts";
            else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_BACKWARD) captionStream << "rückwärts";
            else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_STOP) captionStream << "bremsen";
            else qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, undefined INT_MOTOR_OPERATION";

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0, 15, image.width()-1, image.height()-1, Qt::AlignCenter, captionString.toLatin1());
            break;

        case TYPE_SERVO:
            captionStream << "Servo " << (INT_MOTOR_NUMBER + 1) ;
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, captionString);

            captionString = "";
            captionStream << "Position: " << INT_SERVO_POSITION;

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0, 15, image.width()-1, image.height()-1, Qt::AlignCenter, captionString);
            break;

        case TYPE_VEHICLE:
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, "Fahrzeug");

            if (INT_MOTOR_OPERATION == MOTOR_OPERATION_FORWARD) captionStream << "vorwärts\n";
            else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_BACKWARD) captionStream << "rückwärts\n";
            else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_STOP) captionStream << "bremsen\n";
            else qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, undefined INT_MOTOR_OPERATION";

//            0-127
//            128-254
//            255-256
//            257-383
//            384-511
            if (INT_VEHICLE_STEERING <= 127)      captionStream << "stark links";
            else if (INT_VEHICLE_STEERING <= 254) captionStream << "leicht links";
            else if (INT_VEHICLE_STEERING <= 256) captionStream << "geradeaus";
            else if (INT_VEHICLE_STEERING <= 383) captionStream << "leicht rechts";
            else if (INT_VEHICLE_STEERING <= 511) captionStream << "stark rechts";

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0, 15, image.width()-1, image.height()-1, Qt::AlignCenter, captionString.toLatin1());
            break;

        case TYPE_UART:
            qDebug() << "+++++UART draw";
            qPainter.drawText(0,-15,image.width()-1,image.height()-1, Qt::AlignCenter, "Terminal\nAusgabe");

            if (this->qString.length() > 10)
            {
                captionStream << this->qString.left(8) << "...";   //nur die ersten 10 Zeichen
            }
            else
            {
                captionStream << this->qString;
            }

            qPainter.setFont(fontSubHeading);
            qPainter.drawText(0, 20, image.width()-1, image.height()-1, Qt::AlignCenter, captionString);
            qDebug() << "+++++UART draw: " << captionString;
            break;

        default:
            qDebug() << "ERROR: ProgramCommandBlock::generatePixmap, undefined blockType";
        }
    }
    qPainter.end();

    ///TODO: Pixmap als Attribut löschen und das Pixmap der Basisklasse QLabel nutzen!!?
    setPixmap(QPixmap::fromImage(image));
    this->pixmap = QPixmap::fromImage(image);
//    this->setFrameRect(this->pixmap.rect());
    //        qDebug() << "ProgramCommandBlock::generatePixmap: " << color;
}

void ProgramCommandBlock::initializeBlock()
{
//    qDebug() << "ProgramCommandBlock::initializeBlock()";

    switch(this->type)
    {
    case TYPE_WAIT:
        INT_DELAY_VALUE = 1;
        INT_DELAY_UNIT = UNIT_s;
        break;

    case TYPE_PORT:
    case TYPE_LED:
        INT_PORT_NUMBER = 1;
        INT_PORT_OPERATION = PORT_OPERATION_AN;
        break;

    case TYPE_MOTOR:
    case TYPE_SERVO:
        INT_MOTOR_OPERATION = MOTOR_OPERATION_FORWARD;
        INT_MOTOR_NUMBER = 0;
        INT_MOTOR_SPEED = 255;
        break;

    case TYPE_VEHICLE:
        INT_VEHICLE_OPERATION = VEHICLE_OPERATION_FORWARD;
        INT_VEHICLE_STEERING = 256;
        //INT_VEHICLE_SPEED = 255;
        break;

    case TYPE_UART:
        this->qString = "";
        break;

    default:
        qDebug() << "ProgramCommandBlock::initializeBlock(): undefined type";
        break;
    }
    ProgramBlock::initializeBlock();
}

//void ProgramCommandBlock::generateBlockListPixmap()
//{
//    this->generatePixmap(MOVE_PIXMAP);
//    blockListPixmap = pixmap;
//}

void ProgramCommandBlock::generateSourcecode(bool highlight)
{
//    qDebug() << "ProgramCommandBlock::generateSourcecode, highlight: " << highlight;
//    highlight = true;
    sourcecode = "";
    QTextStream sourcecodeStream(&sourcecode);
    this->appendTabulator(sourcecodeStream);

    highlightBegin(sourcecodeStream, highlight);

    switch(type)
    {
    case TYPE_WAIT:
        if (INT_DELAY_UNIT == UNIT_s)
        {
            sourcecodeStream << "_delay_ms(" << INT_DELAY_VALUE << "000);<br>";
        }
        else if (INT_DELAY_UNIT == UNIT_ms)
        {
            sourcecodeStream << "_delay_ms(" << INT_DELAY_VALUE << ");<br>";
        }
        else if (INT_DELAY_UNIT == UNIT_us)
        {
            sourcecodeStream << "_delay_us(" << INT_DELAY_VALUE << ");<br>";
        }
        else
        {
            qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: TYPE_WAIT, undefined INT_DELAY_UNIT";
        }
        break;

    case TYPE_PORT:
    {
        QChar portLetterQChar = 0;
        int portNumberInt = 0;
        if (INT_PORT_NUMBER <= 2)       portLetterQChar = 'D';
        else if (INT_PORT_NUMBER <= 5)  portLetterQChar = 'B';
        else if (INT_PORT_NUMBER <= 8)  portLetterQChar = 'A';
        else if (INT_PORT_NUMBER <= 16) portLetterQChar = 'C';

        switch(INT_PORT_NUMBER)
        {
        case 1:  portNumberInt = 3; break;
        case 2:  portNumberInt = 2; break;
        case 3:  portNumberInt = 2; break;
        case 4:  portNumberInt = 1; break;
        case 5:  portNumberInt = 0; break;
        case 6:  portNumberInt = 0; break;
        case 7:  portNumberInt = 1; break;
        case 8:  portNumberInt = 2; break;
        case 9:  portNumberInt = 7; break;
        case 10: portNumberInt = 6; break;
        case 11: portNumberInt = 5; break;
        case 12: portNumberInt = 4; break;
        case 13: portNumberInt = 3; break;
        case 14: portNumberInt = 2; break;
        case 15: portNumberInt = 1; break;
        case 16: portNumberInt = 0; break;
        default: qDebug() << "ERROR, ProgramCommandBlock::generateSourcecode, TYPE_PORT, undefined INT_PORT_NUMBER: " << INT_PORT_NUMBER;
            break;
        }

        QString portOperationQString = "";
        if (INT_PORT_OPERATION == PORT_OPERATION_AN)          portOperationQString = " |= ";
        else if (INT_PORT_OPERATION == PORT_OPERATION_AUS)    portOperationQString = " &= ~";
        else if (INT_PORT_OPERATION == PORT_OPERATION_TOGGLE) portOperationQString = " ^= ";
        else qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: TYPE_PORT, undefined INT_PORT_OPERATION";

        sourcecodeStream << "DDR" << portLetterQChar << " |= (1&lt;&lt;P" << portLetterQChar << portNumberInt << ");<br>";
        this->appendTabulator(sourcecodeStream);
        sourcecodeStream << "PORT" << portLetterQChar << portOperationQString << "(1&lt;&lt;P" << portLetterQChar << portNumberInt << ");<br>";

        break;
    }
    case TYPE_LED:
        if (INT_PORT_OPERATION == PORT_OPERATION_AN)
        {
            sourcecodeStream << "DDRA |= (1&lt;&lt;PA3);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA3);<br>";
        }
        else if (INT_PORT_OPERATION == PORT_OPERATION_AUS)
        {
            sourcecodeStream << "DDRA |= (1&lt;&lt;PA3);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA3);<br>";
        }
        else if (INT_PORT_OPERATION == PORT_OPERATION_TOGGLE)
        {
            sourcecodeStream << "DDRA |= (1&lt;&lt;PA3);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA ^= (1&lt;&lt;PA3);<br>";
        }
        else
        {
            qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: TYPE_LED, undefined INT_PORT_OPERATION";
        }
        break;

    case TYPE_MOTOR:
        if (INT_MOTOR_NUMBER == 0)      sourcecodeStream << "OCR2A = " << INT_MOTOR_SPEED << ";<br>";
        else if (INT_MOTOR_NUMBER == 1) sourcecodeStream << "OCR2B = " << INT_MOTOR_SPEED << ";<br>";
        else qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: undefined INT_MOTOR_NUMBER";

        this->appendTabulator(sourcecodeStream);

        if (INT_MOTOR_OPERATION == MOTOR_OPERATION_FORWARD)
        {
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 4) << ");<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 5) << ");<br>";
        }
        else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_BACKWARD)
        {
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 4) << ");<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 5) << ");<br>";
        }
        else if (INT_MOTOR_OPERATION == MOTOR_OPERATION_STOP)
        {
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 4) << ") & ~(1&lt;&lt;PA" << (INT_MOTOR_NUMBER * 2 + 5) << ");<br>";
        }
        else qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: undefined INT_MOTOR_OPERATION";
        break;

    case TYPE_SERVO:
        {
            const double input_min = 0;
            const double input_max = 255;
            const int output_min_8bit = 9;
            const int output_max_8bit = 25;
            const int output_min_16bit = 77;
            const int output_max_16bit = 203;

            if (INT_MOTOR_NUMBER == 0)
            {
                double faktor = (output_max_8bit - output_min_8bit) / (input_max - input_min);
                int valuePWM = round(output_min_8bit + INT_SERVO_POSITION * faktor);
                ///TODO: folgende Zeile löschen:
                valuePWM = INT_SERVO_POSITION;
                sourcecodeStream << "OCR0A = " << valuePWM << ";<br>";
            }
            else if (INT_MOTOR_NUMBER == 1)
            {
                double faktor = (output_max_8bit - output_min_8bit) / (input_max - input_min);
                int valuePWM = round(output_min_8bit + INT_SERVO_POSITION * faktor);
                ///TODO: folgende Zeile löschen:
                valuePWM = INT_SERVO_POSITION;
                sourcecodeStream << "OCR0B = " << valuePWM << ";<br>";
            }
            else if (INT_MOTOR_NUMBER == 2)
            {
                double faktor = (output_max_16bit - output_min_16bit) / (input_max - input_min);
                int valuePWM = round(output_min_16bit + INT_SERVO_POSITION * faktor);
                sourcecodeStream << "OCR1A = " << valuePWM << ";<br>";
            }
            else if (INT_MOTOR_NUMBER == 3)
            {
                double faktor = (output_max_16bit - output_min_16bit) / (input_max - input_min);
                int valuePWM = round(output_min_16bit + INT_SERVO_POSITION * faktor);
                sourcecodeStream << "OCR1B = " << valuePWM << ";<br>";
            }
            else qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: undefined INT_MOTOR_NUMBER";
            break;
        }
    case TYPE_VEHICLE:
        if (INT_VEHICLE_STEERING <= 255)
        {
            sourcecodeStream << "OCR2A = " << INT_VEHICLE_STEERING << ";<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "OCR2B = 255;<br>";
        }
        else
        {
            sourcecodeStream << "OCR2A = 255;<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "OCR2B = " << 511 - INT_VEHICLE_STEERING << ";<br>";
        }

        this->appendTabulator(sourcecodeStream);

        if (INT_VEHICLE_OPERATION == VEHICLE_OPERATION_FORWARD)
        {
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA4);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA5);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA6);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA7);<br>";
        }
        else if (INT_VEHICLE_OPERATION == VEHICLE_OPERATION_BACKWARD)
        {
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA4);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA5);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA |= (1&lt;&lt;PA6);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA7);<br>";
        }
        else if (INT_VEHICLE_OPERATION == VEHICLE_OPERATION_STOP)
        {
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA4) & ~(1&lt;&lt;PA5);<br>";
            this->appendTabulator(sourcecodeStream);
            sourcecodeStream << "PORTA &= ~(1&lt;&lt;PA6) & ~(1&lt;&lt;PA7);<br>";
        }
        else qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: undefined INT_VEHICLE_OPERATION";
        break;

    case TYPE_UART:
        sourcecodeStream << "UART_Transmit_String(\"" << this->qString << "\");<br>";
        break;

    default:
        sourcecodeStream << originalSourcecode << "<br>";
        qDebug() << "ERROR: ProgramCommandBlock::generateSourcecode: undefined type";
        break;
    }

    highlightEnd(sourcecodeStream, highlight);

    if (this->successor)
    {
        sourcecodeStream << this->successor->getSourcecode();
    }
//    this->updateTextEditorWidget();
//    qDebug() << "ProgramCommandBlock::generateSourcecode: " << programBlockSourcecode;
}

///aktualisiere die eigene Position und lasse alle daranhängenden Blöcke auch ihre Position ändern.
void ProgramCommandBlock::updateLocation(int socketIndex)
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

QPoint ProgramCommandBlock::getAppendPosition()
{
    return this->pos() + QPoint(0,COMMANDBLOCK_HEIGHT);
}

void ProgramCommandBlock::createDialog(Dialog *newDialog)
{
//    qDebug() << "ProgramCommandBlock::createDialog";
    ProgramBlock::createDialog(newDialog);

    QFont font;
    font.setPointSize(FONT_SIZE_DIALOG_HEADING);

    switch(this->type)
    {
    case TYPE_WAIT:
        qDebug() << "ProgramCommandBlock::createDialog TYPE_WAIT";

        dialog->getQLabel(NUMBER_QLABEL_NAME).setText("Warten");
        dialog->getQLabel(NUMBER_QLABEL_NAME).setFont(font);
        dialog->getQLabel(NUMBER_QLABEL_NAME).setFixedHeight(30);

        dialog->getQLabel(NUMBER_QLABEL_DURATION).setText("Dauer: ");

        dialog->getQSpinBox().setMinimum(0);
        dialog->getQSpinBox().setMaximum(9999);
        dialog->getQSpinBox().setFixedSize(55,20);
        dialog->getQSpinBox().setValue(INT_DELAY_VALUE);
        qDebug() << "dialog->getQSpinBox().setValue(INT_DELAY_VALUE): " << INT_DELAY_VALUE;
        qDebug() << "dialog->getQSpinBox().value(): " << dialog->getQSpinBox().value();



        dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT).addItem("s");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT).addItem("ms");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT).addItem("µs");
        dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT).setCurrentIndex(INT_DELAY_UNIT);
        dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT).setFixedSize(40,20);

        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_NAME), 0,0,1,2);
        dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_DURATION), 1,0);
        dialog->getQGridLayout().addWidget(&dialog->getQSpinBox(), 1,1);//,2,1);
        dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT), 1,2);

        connect(&dialog->getQSpinBox(), SIGNAL(valueChanged(int)), this, SLOT(dialogValueChanged(int)));
        connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_DELAY_UNIT), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
        break;

    case TYPE_PORT:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_PORT";

            dialog->getQLabel(NUMBER_QLABEL_NAME).setText("PORT");
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFixedHeight(30);

            dialog->getQLabel(NUMBER_QLABEL_PORT_NUMBER).setText("Anschluss: ");

            dialog->getQSpinBox().setMinimum(1);
            dialog->getQSpinBox().setMaximum(16);
            dialog->getQSpinBox().setFixedSize(40,20);
            dialog->getQSpinBox().setValue(INT_PORT_NUMBER);

            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("an");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("aus");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("togglen");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).setCurrentIndex(INT_PORT_OPERATION);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).setFixedSize(60,20);

            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_NAME), 0,0,1,2);
            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_PORT_NUMBER), 1,0);
            dialog->getQGridLayout().addWidget(&dialog->getQSpinBox(), 1,1);//,2,1);
            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION), 1,2);

            connect(&dialog->getQSpinBox(), SIGNAL(valueChanged(int)), this, SLOT(dialogValueChanged(int)));
            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            break;
        }

    case TYPE_LED:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_LED";

            dialog->getQLabel(NUMBER_QLABEL_NAME).setText("Status LED");
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFixedHeight(30);   ///TODO: Warum nötig

            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("an");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("aus");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).addItem("togglen");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).setCurrentIndex(INT_PORT_OPERATION);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION).setFixedSize(60,20);

            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_NAME), 0,0);
            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION), 1,0);

            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_PORT_OPERATION), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            break;
        }

    case TYPE_MOTOR:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_MOTOR";

            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Motor 1");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Motor 2");
            qDebug() << "ProgramLoopBlock::createDialog, getQComboBox.setCurrentIndex: " << INT_MOTOR_NUMBER;
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setCurrentIndex(INT_MOTOR_NUMBER);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setFixedSize(80,30);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setFont(font);

            //font.setPointSize(12);
            dialog->getQLabel(NUMBER_QLABEL_MOTOR_SPEED).setText("Leistung: ");
            //dialog->getQLabel(NUMBER_QLABEL_MOTOR_SPEED).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_MOTOR_SPEED).setFixedHeight(30);

            dialog->getQSlider().setOrientation(Qt::Horizontal);
            dialog->getQSlider().setMinimum(0);
            dialog->getQSlider().setMaximum(255);
            dialog->getQSlider().setFixedSize(140,20);
            dialog->getQSlider().setValue(INT_MOTOR_SPEED);


            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION).addItem("Vorwärts");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION).addItem("Rückwärts");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION).addItem("Bremsen");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION).setCurrentIndex(INT_MOTOR_OPERATION);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION).setFixedSize(80,20);

            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER), 0,0);
            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION), 0,1, Qt::AlignRight);
            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_MOTOR_SPEED), 1,0);
            dialog->getQGridLayout().addWidget(&dialog->getQSlider(), 1,1);//,2,1);


            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_OPERATION), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            connect(&dialog->getQSlider(), SIGNAL(sliderMoved(int)), this, SLOT(dialogValueChanged(int)));
            break;
        }

    case TYPE_SERVO:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_SERVO";

            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Servo 1");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Servo 2");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Servo 3");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).addItem("Servo 4");
            qDebug() << "ProgramLoopBlock::createDialog, getQComboBox.setCurrentIndex: " << INT_MOTOR_NUMBER;
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setCurrentIndex(INT_MOTOR_NUMBER);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setFixedSize(80,30);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER).setFont(font);

            //font.setPointSize(12);
            dialog->getQLabel(NUMBER_QLABEL_SERVO_POSITION).setText("Position: ");
            //dialog->getQLabel(NUMBER_QLABEL_MOTOR_SPEED).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_SERVO_POSITION).setFixedHeight(30);

            //dialog->getQDial().setOrientation(Qt::Horizontal);
//            dialog->getQDial().setMinimum(0);
//            dialog->getQDial().setMaximum(255);
//            dialog->getQDial().setFixedSize(70,70);
//            dialog->getQDial().setValue(INT_SERVO_POSITION);

            dialog->getQSlider().setOrientation(Qt::Horizontal);
            dialog->getQSlider().setMinimum(0);
            dialog->getQSlider().setMaximum(255);
            dialog->getQSlider().setFixedSize(140,20);
            dialog->getQSlider().setValue(INT_SERVO_POSITION);

            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER), 0,0);
            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_SERVO_POSITION), 1,0);
//            dialog->getQGridLayout().addWidget(&dialog->getQDial(), 0,1);//,2,1);
            dialog->getQGridLayout().addWidget(&dialog->getQSlider(), 1,1);//,2,1);

            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_MOTOR_NUMBER), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            connect(&dialog->getQSlider(), SIGNAL(sliderMoved(int)), this, SLOT(dialogValueChanged(int)));
//            connect(&dialog->getQDial(), SIGNAL(sliderMoved(int)), this, SLOT(dialogValueChanged(int)));
            break;
        }

    case TYPE_VEHICLE:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_VEHICLE";

            dialog->getQLabel(NUMBER_QLABEL_NAME).setText("Fahrzeug");
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFixedHeight(30);

            dialog->getQLabel(NUMBER_QLABEL_VEHICLE_STEERING).setText("Lenkung: ");
            dialog->getQLabel(NUMBER_QLABEL_VEHICLE_STEERING).setFixedHeight(30);

            dialog->getQSlider().setOrientation(Qt::Horizontal);
            dialog->getQSlider().setMinimum(0);
            dialog->getQSlider().setMaximum(511);
            dialog->getQSlider().setFixedSize(140,20);
            dialog->getQSlider().setValue(INT_VEHICLE_STEERING);

            dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION).addItem("Vorwärts");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION).addItem("Rückwärts");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION).addItem("Bremsen");
            dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION).setCurrentIndex(INT_MOTOR_OPERATION);
            dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION).setFixedSize(80,20);

            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_NAME), 0,0);
            dialog->getQGridLayout().addWidget(&dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION), 0,1, Qt::AlignRight);
            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_VEHICLE_STEERING), 1,0);
            dialog->getQGridLayout().addWidget(&dialog->getQSlider(), 1,1);//,2,1);

            connect(&dialog->getQComboBox(NUMBER_QCOMBOBOX_VEHICLE_OPERATION), SIGNAL(currentIndexChanged(int)), this, SLOT(dialogValueChanged(int)));
            connect(&dialog->getQSlider(), SIGNAL(sliderMoved(int)), this, SLOT(dialogValueChanged(int)));
            break;
        }

    case TYPE_UART:
        {
            qDebug() << "ProgramCommandBlock::createDialog: TYPE_UART";

            dialog->getQLabel(NUMBER_QLABEL_NAME).setText("Terminal-Ausgabe");
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_NAME).setFixedHeight(30);

            //font.setPointSize(12);
            dialog->getQLabel(NUMBER_QLABEL_UART_TEXT).setText("Text: ");
            //dialog->getQLabel(NUMBER_QLABEL_UART_TEXT).setFont(font);
            dialog->getQLabel(NUMBER_QLABEL_UART_TEXT).setFixedHeight(30);

            dialog->getQLineEdit().setText(this->qString);
            dialog->getQLineEdit().setFixedSize(140,20);

            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_NAME), 0,0,1,2);
            dialog->getQGridLayout().addWidget(&dialog->getQLabel(NUMBER_QLABEL_UART_TEXT), 1,0);
            dialog->getQGridLayout().addWidget(&dialog->getQLineEdit(), 1,1);//,2,1);

            connect(&dialog->getQLineEdit(), SIGNAL(textChanged(QString)), this, SLOT(dialogQStringChanged(QString)));
            break;
        }

    default:
        qDebug() << "ERROR: ProgramCommandBlock::createDialog, undefined type";
        break;
    }

//    qDebug() << "ProgramCommandBlock::createDialog ENDE";
}

void ProgramCommandBlock::dialogValueChanged(int newValue)
{
    if (this->sender() ==  &dialog->getQSpinBox())       value0 = newValue;
    else if (this->sender() == &dialog->getQComboBox(0)) value1 = newValue;
    else if (this->sender() == &dialog->getQComboBox(1)) value2 = newValue;
    else if (this->sender() == &dialog->getQSlider())    value0 = newValue;
    else if (this->sender() == &dialog->getQDial())      value0 = newValue;
    else qDebug() << "ERROR: ProgramCommandBlock::WidgetValueChanged, undefined this->sender()";

    this->generateGraphics();
    //this->generateSourcecode();
    this->updateSourcecodeWidget();
}
