#include "SourcecodeWidget.h"

SourcecodeWidget::SourcecodeWidget(QWidget *parent) :
    QTextEdit(parent)
{
    this->setParent(parent);
    this->setMinimumWidth(SOURCECODEWIDGET_WIDTH);
    this->setCurrentFont(QFont("Consolas"));
    this->setFontPointSize(10);
    this->setTabStopWidth(this->TAB_WIDTH);
    this->setReadOnly(true);
    this->setUndoRedoEnabled(false);
}
