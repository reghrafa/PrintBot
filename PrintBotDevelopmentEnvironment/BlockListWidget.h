#ifndef BLOCKLISTWIDGET_H
#define BLOCKLISTWIDGET_H

#include <QListWidget>
#include <QtGui>    //notwendig! (für Drag and Drop!?)
#include <QDebug>
#include "Main.h"
#include "EditorWidget.h"
#include "ProgramBlock.h"

class EditorWidget;

///Dieses Widget enthält nur (grobe) Abbildungen aller unterschiedlichen Typen von Blöcken
class BlockListWidget : public QListWidget
{
    Q_OBJECT
public:
    BlockListWidget(EditorWidget *existingEditorWidget);
    void addBlock(ProgramBlock *newProgramBlock, int row = -1);
    void setStartBlockHidden(bool hide);
    void resetAll();
//    void setEditorWidget(EditorWidget *existingEditorWidget);
//    void setFunctionBlock(bool mainFunktion);
//    void changeCaption(QString oldName, QString newName);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);
    void startDrag(Qt::DropActions supportedActions);

private:
    EditorWidget *editorWidget;
//    QList<QListWidgetItem*> qListWidgetItemQList;
};

#endif // BLOCKLISTWIDGET_H
