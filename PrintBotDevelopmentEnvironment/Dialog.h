#ifndef DIALOG_H
#define DIALOG_H

#include <QWidget>
#include <QGridLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QLabel>
#include <QSpacerItem>
#include <QSlider>
#include <QLineEdit>
#include <QDial>
#include "Main.h"
//#include "ProgramBlock.h"

class ProgramBlock;

class Dialog : public QWidget
{
    Q_OBJECT
public:
    explicit Dialog(ProgramBlock *newProgramBlock, QWidget *parent = 0);
    ~Dialog();
    void deselectBlock();

    ProgramBlock *getProgramBlock();
    void setProgramBlock(ProgramBlock *newProgramBlock);
    QGridLayout &getQGridLayout();
    QLabel &getQLabel(int labelNumber);
    QSpinBox &getQSpinBox();
    QComboBox &getQComboBox(int comboBoxNumber);
    QSlider &getQSlider();
    QLineEdit &getQLineEdit();
    QDoubleSpinBox &getQDoubleSpinBox();
    QDial &getQDial();

private:
    ProgramBlock *programBlock;
    QGroupBox qGridGroupBox;
    QGridLayout qGridLayout;
    QLabel qLabel0;
    QLabel qLabel1;
    QSpinBox qSpinBox0;
    QDoubleSpinBox qDoubleSpinBox;
    QComboBox qComboBox0;
    QComboBox qComboBox1;
    QComboBox qComboBox2;
    QSlider qSlider0;
    QLineEdit qLineEdit0;
    QDial qDial;
};

#endif // DIALOG_H
