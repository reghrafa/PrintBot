#include "EditorWidget.h"
#include "MainWindow.h"
//#include "Dialog.h"

EditorWidget::EditorWidget(MainWindow *newMainWindow):
    dialog(NULL),
    mainWindow(newMainWindow)
{
    programCode.setMainWindow(mainWindow);
//    qDebug() << "EditorWidget: " << this;
    this->setStandardSize();
    this->setAcceptDrops(true);
}

///Aufruf, falls ein Element das BlockEditorWidget betritt
void EditorWidget::dragEnterEvent(QDragEnterEvent *event)
{
//    qDebug() << "EditorWidget::dragEnterEvent";
    if (event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
    {
        if (event->source() == this)
        {
//            qDebug() << "EditorWidget::dragEnterEvent: accept";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else
    {
        qDebug() << "EditorWidget::dragEnterEvent: ignore";
        event->ignore();
    }
}

/////Aufruf, falls ein Element das EditorWidget verlässt
void EditorWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
//    qDebug() << "EditorWidget::dragLeaveEvent: accept";
    event->accept();
}

///Aufruf, falls ein Element innerhalb des EditorWidget bewegt wird
void EditorWidget::dragMoveEvent(QDragMoveEvent *event)
{
//    qDebug() << "EditorWidget::dragMoveEvent";

    if ((event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
//            && findBlock(targetSquare(event->pos())) == -1  //TODO: auskommentieren
            && isFree(event))
    { 
        //qDebug() << "EditorWidget::dragMoveEvent: accept";
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            //qDebug() << "EditorWidget::dragMoveEvent: acceptProposedAction";
            event->acceptProposedAction();
        }
    }
    else
    {
//        qDebug() << "EditorWidget::dragMoveEvent: ignore";
        event->ignore();
    }
}

///Aufruf, falls ein Element im EditorWidget losgelassen wird
void EditorWidget::dropEvent(QDropEvent *event)
{
//    qDebug() << "EditorWidget::dropEvent";
    if (event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
    {
        ProgramBlock *newBlock = NULL;
        ProgramBlock *existingBlock = NULL;
        ProgramBlock *dropBlock = NULL;
        QPixmap pixmap;
        QString blockName;
        QString blockSourcecode = "ERRORtest(42);";
        QColor blockColor;
        QPoint offset = QPoint(0,0);

        int blockType = -1;   //Typ des geDropten Blocks

        if (event->mimeData()->hasFormat("ListBlock"))
        {
            ///Block stammt aus dem BlockListWidget
            QByteArray itemData = event->mimeData()->data("ListBlock");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            dataStream >> pixmap >> blockName >> blockSourcecode >> blockColor >> blockType;
            //TODO: Überarbeiten, sodass auch Funktionen und Ä. übergeben werden können.
            qDebug() << "EditorWidget::dropEvent: dataStream: " << blockName << "Sourcecode: " << blockSourcecode << " " << blockColor << " " << blockType << " ";


            ///Unterscheidung abhängig vom blockType des geDropten Blocks
            switch(blockType)
            {
            case TYPE_WAIT:
            case TYPE_PORT:
            case TYPE_LED:
            case TYPE_MOTOR:
            case TYPE_VEHICLE:
            case TYPE_SERVO:
            case TYPE_UART:
                newBlock = new ProgramCommandBlock(blockName, blockSourcecode, blockColor, blockType);
                break;

            case TYPE_FUNCTION:
            {
                newBlock = new ProgramFunction(blockName, "void",  "", blockColor, blockType);
                break;
            }
            case TYPE_MAIN_FUNCTION:
            {
                newBlock = new ProgramFunction(blockName, blockSourcecode, "", blockColor, blockType);
                break;
            }
            case TYPE_IF_CUSTOM:
            case TYPE_IF_ADC:
                newBlock = new ProgramIfBlock(blockName, blockSourcecode, blockColor, blockType);
                break;

            case TYPE_LOOP_FOR:
            case TYPE_LOOP_ENDLESS:
            case TYPE_LOOP_WHILE:
                newBlock = new ProgramLoopBlock(blockName, blockSourcecode, blockColor);
                break;

            default:
                qDebug() << "ERROR: EditorWidget::dropEvent: falscher blockType";
                break;
            }
            dropBlock = newBlock;
            dropBlock->setParent(this);
        }
        else    ///Block stammt aus dem EditorWidget --> Block wurde nur verschoben
        {
            QByteArray itemData = event->mimeData()->data("EditorBlock");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            u_int intBlockPointer;

            dataStream >> pixmap >> offset >> blockName >> blockColor >> intBlockPointer;
            dropBlock = (ProgramBlock*) intBlockPointer;

//            ///dropBlock wird ohnehin nach dem DragAndDrop gelöscht. TODO
//            if (dropBlock->isFirstBlock())
//            {
//                dropBlock->setFirstBlock(false); ///nötig?
////                programCode.deleteBlock(dropBlock);
//            }
//            qDebug() << "EditorWidget::dropEvent: value0: " << value0 << " value1: " << value1;
//            qDebug() << "EditorWidget::dropEvent: dataStream: "<< offset << " "<< blockName << "sourcecode: " << blockSourcecode << " " << blockColor << " "<< intBlockPointer << " "<< dropBlock << " "<< blockType << " ";
        }

        ///TODO: Problematik: Mausposition bleibt konstant, Blockposition ändert sich durch
        ///clearPredecessorAndSuccessor()...
        existingBlock = dynamic_cast<ProgramBlock*>(childAt(event->pos()));

        dropBlock->clearPredecessorAndSuccessor();

        ///Falls dieser Block auf einen fremden Block gezogen wurde:
        if (existingBlock && childAt(event->pos()) != dropBlock)
        {
            existingBlock->addBlock(dropBlock, event->pos());
        }
        else ///Erster Block einer Struktur
        {
            QPoint blockNewLocation = event->pos() - offset;
            dropBlock->setFirstBlock(true);
            dropBlock->move(blockNewLocation);
        }

        dropBlock->setPixmap(pixmap);
        addBlock(dropBlock);

//        deselectPreviousBlock();
//        updateDialog(dropBlock);
//        this->updateSourcecodeWidget(); //code highlighting
//        newBlock->generateGraphics();

        ///Block stammt aus dem EditorWidget
        if (event->source() == this)
        {
//            qDebug() << "EditorWidget::dropEvent: accept";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else ///Block stammt aus dem BlockListWidget
        {
//            qDebug() << "EditorWidget::dropEvent: acceptProposedAction";
            event->acceptProposedAction();
        }
    }
    else
    {
        qDebug() << "EditorWidget::dropEvent: ignore";
        event->ignore();
    }
}

///Aufruf, falls die Maus im EditorWidget gedrückt wurde
void EditorWidget::mousePressEvent(QMouseEvent *event)
{
//    qDebug() << "EditorWidget::mousePressEvent";
//    ///Positionsanzeige beim Klick des ForwardButton
//    if (event->button() == Qt::ForwardButton)
//    {
//        mainWindow->setQLabelMousePosition(event->pos());
//    }

    //setCursor(Qt::ClosedHandCursor);

    ProgramBlock *clickedBlock = dynamic_cast<ProgramBlock*>(childAt(event->pos()));

    if (!clickedBlock)
    {
//        qDebug() << "EditorWidget::mousePressEvent: nichts angeklickt";
        return;
    }

    ///Mit rechter Maustaste öffne Dialog
    if (event->button() == Qt::RightButton)
    {
        updateDialog(clickedBlock);
        return;
    }

    ///Schließe den Dialog:
    updateDialog(NULL);

    QPixmap blockMovePixmap = clickedBlock->getPixmap(MOVE_PIXMAP);
    QPixmap blockPixmap = clickedBlock->getPixmap();
    QString blockName = clickedBlock->getName();
    QColor blockColor = clickedBlock->getColor();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << blockMovePixmap
               << QPoint(event->pos() - clickedBlock->pos())
               << blockName
               << blockColor
               << (u_int) clickedBlock;
//    qDebug() << "EditorWidget::mousePressEvent: clickedBlock-Address: " << clickedBlock << " "
//             << QPoint(event->pos() - clickedBlock->pos());

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("EditorBlock", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(blockMovePixmap);
    drag->setHotSpot(event->pos() - clickedBlock->pos());

    ///Färbt den Block, der gerade verschoben wird, dunkel
    {
        QPixmap tempPixmap = blockPixmap;

        QPainter painter;
        painter.begin(&tempPixmap);
        painter.fillRect(blockPixmap.rect(), QColor(127, 127, 127, 127));
        painter.end();
        clickedBlock->setColor(blockColor);
        clickedBlock->setPixmap(tempPixmap);
    }
    ///Falls ein Element aus dem EditorWidget auf ein anderes Element
    /// aus dem EditorWidget gezogen wurde: ???stimmt das??
    /// Falls das Element auf einer Stelle losgelassen wurde, auf der loslassen nicht erlaubt ist:
    if (!(drag->exec(Qt::MoveAction) == Qt::MoveAction))
    {
//        qDebug() << "EditorWidget::mousePressEvent: clickedBlock->show()";
        ///Färbe Block wieder hell
        clickedBlock->show();
        clickedBlock->setPixmap(blockPixmap);
    }
    else
    {
//        qDebug() << "EditorWidget::mousePressEvent: clickedBlock->close()";
        //clickedBlock->close(); ///close & delete block
    }
}

QString EditorWidget::getSourcecode()
{
    return this->programCode.getSourcecode();
}

///aktualisiere die Positionen aller Blöcke, die mit dem programBlock zusammenhängen
void EditorWidget::updateBlockGroup(ProgramBlock *programBlock)
{
    if (!programBlock)
    {
        qDebug() << "ERROR, EditorWidget::updateBlockGroup, undefined programBlock";
        return;
    }

//    qDebug() << "EditorWidget::updateBlockGroup: " << programBlock << programBlock->getName();
    ///Finde ersten programBlock der blockGroup
    while (programBlock->getPredecessor())
    {
        programBlock = programBlock->getPredecessor();
    }
//    qDebug() <<  "EditorWidget::updateBlockGroup, erster ProgramBlock: " << programBlock << programBlock->getName();
    programBlock->updateLocation();
}

void EditorWidget::updateSize()
{
//    qDebug() << "EditorWidget::updateSize";

    if (this->childrenRect().bottom() > (EDITORWIDGET_HEIGHT - 2*COMMANDBLOCK_HEIGHT))
    {
        editorSize.setHeight(childrenRect().bottom() + COMMANDBLOCK_HEIGHT);
        this->setMinimumSize(editorSize);
        this->setMaximumSize(editorSize);
    }
    if (this->childrenRect().right() > (EDITORWIDGET_WIDTH - 2*COMMANDBLOCK_WIDTH))
    {
        editorSize.setWidth(childrenRect().right() + COMMANDBLOCK_WIDTH);
        this->setMinimumSize(editorSize);
        this->setMaximumSize(editorSize);
    }

    if (editorSize.width() < this->parentWidget()->width())
    {
        editorSize.setWidth(this->parentWidget()->width());
        this->setMinimumSize(editorSize);
        this->setMaximumSize(editorSize);
    }
    if (editorSize.height() < this->parentWidget()->height())
    {
        editorSize.setHeight(this->parentWidget()->height());
        this->setMinimumSize(editorSize);
        this->setMaximumSize(editorSize);
    }
//    qDebug() << "EditorWidget::updateSize, ENDE";
}

///Aktualisiert das Dialogfenster: Falls programBlock = NULL ist der Dialog leer
void EditorWidget::updateDialog(ProgramBlock *programBlock)
{
//    qDebug() << "EditorWidget::updateDialog, programBlock: " << programBlock;

    if (dialog)
    {
        mainWindow->getQGridLayout().removeWidget(dialog);
//        qDebug() << "EditorWidget::updateDialog, removeWidget";

        ///Falls der Dialog nur aktualisiert werden soll:
        if (dialog->getProgramBlock() == programBlock)
        {
//             qDebug() << "deleteLater dialog: " << dialog;
             dialog->deleteLater();
        }
        else
        {
//            qDebug() << "delete dialog: " << dialog;
//            qDebug() << "vor dialog->getProgramBlock()";
//            qDebug() << "PB: " << dialog->getProgramBlock();
//            qDebug() << "vor delete dialog";
            delete dialog;
        }
//         this->updateSourcecodeWidget(); //code highlighting
//        clickedBlock->generateGraphics();
    }
    dialog = new Dialog(programBlock, mainWindow);
    mainWindow->getQGridLayout().addWidget(dialog,1,1,3,1);

//    qDebug() << "EditorWidget::updateDialog, ENDE";
}

// ///Falls der zugehörige Dialog nicht schon offen ist
//if (programBlock->getDialog() == NULL)

bool EditorWidget::isFree(QDragMoveEvent *event)
{
//    qDebug() << "EditorWidget::isFree";

//    return true; 
    QByteArray itemData = event->mimeData()->data("EditorBlock");
    QDataStream dataStream(&itemData, QIODevice::ReadOnly);

    QRegion blockRegion;
    QPixmap pixmap;
    QString blockName;
    QPoint offset;
    QColor blockColor;
    int blockType;
    ProgramBlock *dropBlock = NULL;

    ///TODO: BLOCK-Typ Unterscheidung fehlt, oder!?
    if (event->mimeData()->hasFormat("EditorBlock"))
    {
        itemData = event->mimeData()->data("EditorBlock");
        u_int intBlockPointer;

        dataStream >> pixmap >> offset >> blockName >> blockColor >> intBlockPointer;
        dropBlock = (ProgramBlock*) intBlockPointer;
        blockType = dropBlock->getType();
    }
    else ///Falls Block aus der BlockList kommt
    {
        itemData = event->mimeData()->data("ListBlock");
        QString blockSourcecode;
        dataStream >> pixmap >> blockName >> blockSourcecode >> blockColor >> blockType;
    }
    ///Falls der Block auf einen anderen gezogen wurde -> erlaube den Drop (appendBlock)
    if (this->childAt(event->pos()) && blockType != TYPE_MAIN_FUNCTION)
    {
        ///Falls der Block auf sich selbst losgelassen wurde
        if (this->childAt(event->pos()) == dropBlock)
        {
            if (/*dropBlock->getSuccessor() || */dropBlock->getPredecessor())
            {
                return false;
            }
        }
//        qDebug() << "true 0";
        return true;
    }

    if (event->mimeData()->hasFormat("EditorBlock"))
    {
        ///Verbiete, dass ein Block zum Teil auf einen anderen Block gezogen wird.
        blockRegion = QRegion(event->pos().x()-offset.x(), event->pos().y()-offset.y(), pixmap.width()/*COMMANDBLOCK_WIDTH*/, pixmap.height()/*COMMANDBLOCK_HEIGHT*/);
        if ((childrenRegion().subtracted(QRegion(dropBlock->rect().adjusted(dropBlock->pos().x(), dropBlock->pos().y(), dropBlock->pos().x(),dropBlock->pos().y())))).intersects(blockRegion))
        {
            return false;
        }
        else
        {
            return true;
        }

    }
    else ///Falls Block aus der BlockList kommt
    {
        blockRegion = QRegion(event->pos().x(), event->pos().y(), pixmap.width()/*COMMANDBLOCK_WIDTH*/, pixmap.height()/*COMMANDBLOCK_HEIGHT*/);

        if (childrenRegion().intersects(blockRegion))
        {
//            qDebug() << "false 2";
            return false;
        }
        else
        {
//            qDebug() << "true 2";
            return true;
        }
    }
}

ProgramCode &EditorWidget::getProgramCode()
{
    return programCode;
}

void EditorWidget::updateSourcecodeWidget()
{
//    qDebug() << "EditorWidget::updateSourcecodeWidget(), mainWindow: " << mainWindow;
    mainWindow->updateSourcecodeWidget();
}

void EditorWidget::addBlock(ProgramBlock *dropBlock, bool boolUpdateSourcecodeWidget)
{
    dropBlock->setParent(this);
    dropBlock->show();

    ///TODO: Beim openFileParser: einmal ausführen reicht!!?
    this->updateBlockGroup(dropBlock);
//    programCode.updateAllBlockGraphics();
    this->updateSize();
    this->updateDialog(dropBlock);
    if (boolUpdateSourcecodeWidget)
    {
        this->updateSourcecodeWidget();
    }
}

///neues Dokument erstellen
void EditorWidget::resetAll()
{
    qDebug() << "EditorWidget::resetAll()";
//    dialog = NULL;
    updateDialog(NULL);
    setStandardSize();
}

///Resetet Fenstergröße abhängig von Bildschirmauflösung
void EditorWidget::setStandardSize()
{
    //    this->resize(this->parentWidget()->size());
    QDesktopWidget qDesktopWidget;
    this->editorSize = qDesktopWidget.screenGeometry().size()
                           - QSize(BLOCKLIST_WIDTH + SOURCECODEWIDGET_WIDTH, DIALOG_HEIGHT);
    //    this->editorSize = QSize(EDITORWIDGET_WIDTH + 150, EDITORWIDGET_HEIGHT + 150);
    this->setMinimumSize(editorSize);
    //    this->setMaximumSize(editorSize);
}

///entfernt den Rahmen des Blocks, der zum Dialog gehört
void EditorWidget::deselectPreviousBlock()
{
    if (dialog)
    {
        dialog->deselectBlock();
    }
    else
    {
        qDebug() << "ERROR: EditorWidget::deselectPreviousBlock(), undefined dialog";
    }
}

Dialog *EditorWidget::getDialog()
{
    return dialog;
}


