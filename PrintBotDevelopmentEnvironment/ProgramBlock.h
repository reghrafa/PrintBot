#ifndef PROGRAMBLOCK_H
#define PROGRAMBLOCK_H

#include <QLabel>
#include <QList>
#include <QString>
#include <QDebug>
#include <QPixmap>
#include <QFontMetrics>
#include <QPainter>
#include <QSize>
#include "Main.h"
//#include "Dialog.h"

class Dialog;
//class EditorWidget;
//class ProgramFunction;
//class ProgramBlock;

//union commandParameter
//{
//    int commParInt;
//    QString commParString;
//    float commParFloat;
//};

class ProgramBlock : public QLabel
{
    Q_OBJECT
public:
    ProgramBlock(QString newProgramBlockName,
                 QString newSourcecode,
                 QColor newColor = Qt::red,
                 int newType = TYPE_PORT,
                 int newIndentationDepth = 1);
    ProgramBlock(int newType,
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
                 int newSuccessorID);
//    ProgramBlock(QString newProgramBlockName, QString newSourcecode);
    virtual ~ProgramBlock();
    void setIndentationDepth(int newIndentationDepth);
    void appendTabulator(QTextStream &textStream);
    virtual void generateSourcecode(bool highlight = false) = 0;
    void updateSourcecodeWidget();
    void highlightBegin(QTextStream &sourcecodeStream, bool highlight);
    void highlightEnd(QTextStream &sourcecodeStream, bool highlight);
    void highlightGraphics(QPainter &qPainter);

    void clearPredecessorAndSuccessor();
    void clearPredecessor();
    void appendBlock(ProgramBlock *newProgramBlock);
    virtual void addBlock(ProgramBlock *newProgramBlock, QPoint mousePosition);
    virtual QPoint getAddPosition(int socketIndex = -1);
    virtual QPoint getAppendPosition() = 0;
    virtual void updateLocation(int socketIndex = -1) = 0;
    virtual void generateGraphics(bool movePixmap = 0);
    void generateBlockListPixmap();
    virtual void updateSuccessor(ProgramBlock *thisBlock, ProgramBlock *newSuccessor);
    void deleteThisAndSuccessors();

    virtual void createDialog(Dialog *newDialog = NULL);
    void deleteDialog();
    Dialog *getDialog();

    QString getSourcecode();
    QString getOriginalSourcecode();
    QString getName() const;
    QPixmap &getPixmap(bool movePixmap = 0);
    QPixmap &getBlockListPixmap();
    QColor  getColor() const;
    void    setColor(const QColor &newColor);
    ProgramBlock *getPredecessor() const;
    void setPredecessor(ProgramBlock *newPredecessor);
    ProgramBlock *getSuccessor() const;
    void setSuccessor(ProgramBlock *newSuccessor);
    int getType() const;
    void setType(int newType);
    int getValue(int valueType) const;
    void setValue(int valueType, int newValue);
    QString getQString() const;
    void setQString(const QString &newQString);
    void setFirstBlock(bool firstBlock);
    bool isFirstBlock();
    virtual QString generateSaveData();
    QString getSaveData();
    int getID();
    int getSuccessorID();

public slots:
    virtual void dialogValueChanged(int newValue) = 0;
    void dialogQStringChanged(QString newQString);
    void dialogQDoubleSpinBoxChanged(double newDouble);

protected:
    virtual void generatePixmap(bool movePixmap = 0) = 0;
    virtual void initializeBlock();

    QString name;
    QString sourcecode;
    QString originalSourcecode;
    int     indentationDepth;
    QColor  color;
    QPixmap pixmap;
    QPixmap movePixmap;
    QPixmap blockListPixmap;
    ProgramBlock *predecessor;
    ProgramBlock *successor;
    Dialog *dialog;
    int type;
    int value0;
    int value1;
    int value2;
    double valueDouble;
    QString qString;
    bool firstBlock;  ///Erster Block einer Struktur?
//    QList<commandParameter*> commParList;
    int id;
    int successorID; ///Nur zum Dokument-Öffnen verwenden, nicht immer aktuell

private:
    static int maxID;
};

#endif // PROGRAMBLOCK_H
