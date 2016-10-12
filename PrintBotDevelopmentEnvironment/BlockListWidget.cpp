#include "BlockListWidget.h"
#include <QItemSelectionModel>
#include <QAbstractItemView>
#include <QListWidgetItem>
#include <QScrollBar>

BlockListWidget::BlockListWidget(EditorWidget *existingEditorWidget):
    editorWidget(existingEditorWidget)
{
    setDragEnabled(true);
    setViewMode(QListView::IconMode);
    setIconSize(QSize(BLOCK_LIST_ICON_WIDTH, BLOCK_LIST_ICON_HEIGHT));
    setSpacing(BLOCKLIST_SPACING);
    setAcceptDrops(true);
    setDragEnabled(true);
    setDropIndicatorShown(true);
    setMinimumWidth(BLOCKLIST_WIDTH);
    setMaximumWidth(BLOCKLIST_WIDTH);
    ///TODO: Falls verticalScrollBar->isVisible() --> Breite anpassen.
//    qDebug() << "QQQQQQQQQQQQQQQQhScrollBar(): " << horizontalScrollBar()->isVisible();
//    qDebug() << "QQQQQQQQQQQQQQQQhScrollBar(): " << verticalScrollBar()->isVisible();
//    setSelectionMode(QAbstractItemView::NoSelection);
}

///Fügt Block hinzu, ersetzt ggf. vorhandenen Block
void BlockListWidget::addBlock(ProgramBlock *newProgramBlock, int row)//(QPixmap &pixmap, QString blockName)
{
//    qDebug() << "BlockListWidget::addBlock, count vorher: " << this->count();
    QListWidgetItem *blockItem = new QListWidgetItem(/*this*/);
    if (row == -1)
    {
        this->addItem(blockItem);
    }
    else
    {
        this->takeItem(row);
        this->insertItem(row, blockItem);
    }

    blockItem->setIcon(QIcon(newProgramBlock->getBlockListPixmap()));
    blockItem->setData(Qt::UserRole, QVariant(newProgramBlock->getPixmap(MOVE_PIXMAP)));
    //blockItem->setData(Qt::UserRole+1, this->pos());
    blockItem->setData(Qt::UserRole+1, newProgramBlock->getName());
    blockItem->setData(Qt::UserRole+2, newProgramBlock->getOriginalSourcecode());
    blockItem->setData(Qt::UserRole+3, QVariant(newProgramBlock->getColor()));
    blockItem->setData(Qt::UserRole+4, QVariant(newProgramBlock->getType()));
//    blockItem->setData(Qt::UserRole+5, (u_int64) newProgramBlock); ///GEHT GAR NICHT!
    blockItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);

//    qDebug() << "BlockListWidget::addBlock: "
//             << newProgramBlock->getName() << " "
//             << newProgramBlock->getColor() << " "
    //             << newProgramBlock->getBlockListPixmap();
}

///verbirgt bzw. zeigt den ersten Block
void BlockListWidget::setStartBlockHidden(bool hide)
{
    qDebug() << "BlockListWidget::setStartBlockHidden, " << hide;
    this->item(0)->setHidden(hide);
    //qDebug() << "qListWidgetItemQList.count(): " << qListWidgetItemQList.count();
//    QListWidgetItem *startBlock = NULL;
//    this->findItems()

//    for (QList<QListWidgetItem*>::iterator it = qListWidgetItemQList.begin(); it != qListWidgetItemQList.end(); it++)
//    {
//        qDebug() << "Name: " << (*it)->data(Qt::UserRole+1).toString();
//        if ((*it)->data(Qt::UserRole+1).toString().compare("Start") == 0)
//        {
//            qDebug() << "drinnen";
//            startBlock = (*it);
//            break;
//        }
//    }
//    if (startBlock)
//    {
//        qDebug() << "startBlock->setHidden(hide);";
//        startBlock->setHidden(hide);
//    }
}

void BlockListWidget::resetAll()
{
//    ProgramFunction *mainFunction = new ProgramFunction("Start", "", "", Qt::green, TYPE_MAIN_FUNCTION);
//    addBlock(mainFunction, 0);
}

//void BlockListWidget::setEditorWidget(EditorWidget *existingEditorWidget)
//{
//    this->editorWidget = existingEditorWidget;
//}

//void BlockListWidget::changeCaption(QString oldName, QString newName)
//{
//    for (int i = 0; i < this->count(); i++)
//    {
//        if (this->item(i)->data(Qt::UserRole + 1).compare(oldName) == 0)
//        {
//            this->takeItem()
//        }
//    }
//}

///Aufruf, falls ein Element das BlockListWidget betritt
void BlockListWidget::dragEnterEvent(QDragEnterEvent *event)
{
//    qDebug() << "BlockListWidget::dragEnterEvent";
    if (event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
    {
        if (event->source() == this)
        {
//            qDebug() << "BlockListWidget::dragEnterEvent: accept";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
//            qDebug() << "BlockListWidget::dragEnterEvent: acceptProposedAction";
            event->acceptProposedAction();
        }
    }
    else
    {
        qDebug() << "BlockListWidget::dragEnterEvent: ignore";
        event->ignore();
    }
}

///Aufruf, falls ein Element innerhalb BlockListWidget bewegt wird
void BlockListWidget::dragMoveEvent(QDragMoveEvent *event)
{
    //qDebug() << "BlockList::dragMoveEvent";
    if (event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
    {
        if (event->source() == this)
        {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else
        {
            //qDebug() << "BlockListWidget::dragMoveEvent: acceptProposedAction";
            event->acceptProposedAction();
        }

    }
    else
    {
        qDebug() << "BlockListWidget::dragMoveEvent: ignore";
        event->ignore();
    }
}

///Aufruf, falls ein Element innerhalb BlockListWidget losgelassen wird
void BlockListWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("EditorBlock") || event->mimeData()->hasFormat("ListBlock"))
    {     
        if (event->source() == this) ///Falls ListBlock
        {
//            qDebug() << "BlockListWidget::dropEvent: accept";
            event->setDropAction(Qt::MoveAction);
            event->accept();
        }
        else ///Falls EditorBlock
        {
//            qDebug() << "BlockListWidget::dropEvent: acceptProposedAction";
            event->acceptProposedAction();


            ///TODO?: Abfrage, ob es sich um eine Funktion handelt

            /// falls EditorBlock --> Lösche Block
            if (event->mimeData()->hasFormat("EditorBlock"))
            {
                ProgramBlock *dropBlock = NULL;
                QPixmap pixmap;
                QString blockName;
                QColor blockColor;
                QPoint offset = QPoint(0,0);
                QByteArray itemData = event->mimeData()->data("EditorBlock");
                QDataStream dataStream(&itemData, QIODevice::ReadOnly);
                u_int intBlockPointer;
                dataStream >> pixmap >> offset >> blockName >> blockColor >> intBlockPointer;
                dropBlock = (ProgramBlock*) intBlockPointer;

                this->editorWidget->updateSize();
                this->editorWidget->updateBlockGroup(dropBlock);
                ///Schließe den Dialog
                this->editorWidget->updateDialog(NULL);
                dropBlock->close();
            }
        }
    }
    else
    {
        qDebug() << "BlockListWidget::dropEvent: ignore";
        event->ignore();
    }
}

///(einmaliger) Aufruf, falls ein Element innerhalb BlockListWidget angeklickt und gezogen wird
void BlockListWidget::startDrag(Qt::DropActions /*supportedActions*/)
{
//    qDebug() << "BlockListWidget::startDrag";
    QListWidgetItem *item = currentItem();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    QPixmap movePixmap = qvariant_cast<QPixmap>(item->data(Qt::UserRole));
    QString blockName = item->data(Qt::UserRole+1).toString();
    QString blockSourcecode = item->data(Qt::UserRole+2).toString();
    QColor blockColor = qvariant_cast<QColor>(item->data(Qt::UserRole+3));
    int blockTyp = qvariant_cast<int>(item->data(Qt::UserRole+4));

    dataStream << movePixmap << blockName << blockSourcecode << blockColor << blockTyp;

//    qDebug() << "BlockListWidget::startDrag: DataStream: "
//             << blockName << " "
//             << blockSourcecode << " "
//             << blockColor << " "
//             << item->data(Qt::UserRole+3) << " "
//             << blockTyp << " ";

    QMimeData *mimeData = new QMimeData;
    mimeData->setData("ListBlock", itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setHotSpot(QPoint(0, 0));//QPoint(pixmap.width()/2, pixmap.height())
    drag->setPixmap(movePixmap);
    drag->exec(Qt::MoveAction);
}
//target() source()
