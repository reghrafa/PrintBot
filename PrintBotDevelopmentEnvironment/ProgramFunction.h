#ifndef FUNCTION_H
#define FUNCTION_H

#include <QList>
#include <QDebug>
#include "Main.h"
#include "ProgramBlock.h"
#include "ProgramCommandBlock.h"


class ProgramBlock;

class ProgramFunction : public ProgramBlock
{
public:
    ProgramFunction(QString FunctionName,
                    QString newReturnType = "void",
                    QString newParameter = "",
                    QColor newColor = Qt::darkGreen,
                    int newType = TYPE_FUNCTION);
    ProgramFunction(int newType,
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
                    QString newParameter);
    virtual ~ProgramFunction();
    virtual void generateSourcecode(bool highlight = false);
    virtual void generateBlockListPixmap();
    virtual void updateLocation(int socketIndex = -1);
    QString getReturnType() const;
    QString getParameter() const;
    virtual QPoint getAppendPosition();
    virtual void createDialog(Dialog *newDialog);
    QString getSourcecodeHeader();
    virtual QString generateSaveData();

   public slots:
    virtual void dialogValueChanged(int newValue);

protected:
    virtual void generatePixmap(bool movePixmap = 0);
    virtual void initializeBlock();

    QString returnType;
    QString parameter;
};

#endif // FUNCTION_H
