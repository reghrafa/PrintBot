#include "Dialog.h"
#include "ProgramBlock.h"

Dialog::Dialog(ProgramBlock *newProgramBlock, QWidget *parent) :
    programBlock(newProgramBlock),
    QWidget(parent)
{    
//    qDebug() << "neuer Dialog, this: " << this;
    qGridGroupBox.setFixedSize(DIALOG_WIDTH,DIALOG_HEIGHT);

    if (programBlock)
    {
//        qDebug() << "neuer Dialog mit newProgramBlock: " << programBlock;
        this->programBlock->createDialog(this);
        this->programBlock->updateSourcecodeWidget();
        this->programBlock->generateGraphics();
//        this->setupWidgets();
    }
    else ///Leerer Dialog
    {
//        qDebug() << "neuer Dialog ohne newProgramBlock";
    }

    this->qGridLayout.setColumnStretch(3,1);
    this->qGridLayout.setRowStretch(0,100);
    this->qGridLayout.setRowStretch(1,50);

    this->qGridGroupBox.setLayout(&qGridLayout);
    this->qGridGroupBox.setParent(this);
}

Dialog::~Dialog()
{
//    qDebug() << "~Dialog";
//    qDebug() << "~Dialog, this: " << this;
//    qDebug() << "~Dialog, programBlock: " << programBlock;
//    qDebug() << "~Dialog Mitte";
    if (this->programBlock)
    {
        if (this->programBlock->getDialog() == this)
        {
            this->programBlock->deleteDialog();
        }
        else
        {
            qDebug() << "ERROR?, ~Dialog: es existiert noch ein anderer Dialog: " << this->programBlock->getDialog();
        }
        ///TODO: Evtl. Problem, dass Destruktor von programBlock gerade beim Durchlaufen ist!?
        programBlock->generateGraphics();   //deselectBlock
        programBlock->updateSourcecodeWidget();
    }
}

///entfernt den Rahmen des Blocks, der zu diesem Dialog gehört
void Dialog::deselectBlock()
{
    qDebug() << "Dialog::deselectBlock()";
    if (programBlock)
    {
        programBlock->deleteDialog();
//        qDebug() << "Dialog::deselectBlock(), Mitte";
        programBlock->generateGraphics();
    }
//    qDebug() << "Dialog::deselectBlock(), Ende";
}

ProgramBlock *Dialog::getProgramBlock()
{
//    qDebug() << "Dialog::getProgramBlock()";
    return this->programBlock;
}

void Dialog::setProgramBlock(ProgramBlock *newProgramBlock)
{
    this->programBlock = newProgramBlock;
}

QGridLayout &Dialog::getQGridLayout()
{
    return this->qGridLayout;
}

QLabel &Dialog::getQLabel(int labelNumber)
{
    if (labelNumber == 0) return this->qLabel0;
    else if (labelNumber == 1) return this->qLabel1;
    else qDebug() << "ERROR: Dialog::getLabel, undefined labelNumber";
}

QSpinBox &Dialog::getQSpinBox()
{
    return this->qSpinBox0;
}

QComboBox &Dialog::getQComboBox(int comboBoxNumber)
{
    if (comboBoxNumber == 0) return this->qComboBox0;
    else if (comboBoxNumber == 1) return this->qComboBox1;
    else if (comboBoxNumber == 2) return this->qComboBox2;
    else qDebug() << "ERROR: Dialog::getQComboBox, undefined comboBoxNumber";
}

QSlider &Dialog::getQSlider()
{
    return qSlider0;
}

QLineEdit &Dialog::getQLineEdit()
{
    return qLineEdit0;
}

QDoubleSpinBox &Dialog::getQDoubleSpinBox()
{
    return qDoubleSpinBox;
}

QDial &Dialog::getQDial()
{
    return qDial;
}

