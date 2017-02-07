#ifndef SOURCECODEWIDGET_H
#define SOURCECODEWIDGET_H

#include <QTextEdit>
#include "Main.h"


class SourcecodeWidget : public QTextEdit
{
    Q_OBJECT
public:
    explicit SourcecodeWidget(QWidget *parent = 0);

private:
    static const int TAB_WIDTH = 25;    //lovis: ohne static
};

#endif // SOURCECODEWIDGET_H
