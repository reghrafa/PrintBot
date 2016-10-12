#ifndef MAIN_H
#define MAIN_H


///MessageBOXES
//QMessageBox::information(this, tr("Empty Search Field"),
//        "The search field is empty. Please enter a word and click Find.");

//#include "programblock.h"
//#include "programfunction.h"
class ProgramBlock;
class ProgramFunction;

#define VERSION 1.04

#define TAB "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;"

#define COMMANDBLOCK_HEIGHT     80
#define COMMANDBLOCK_WIDTH      110
#define FUNCTIONBLOCK_HEIGHT    50
#define FUNCTIONBLOCK_WIDTH     COMMANDBLOCK_WIDTH
#define BLOCK_LIST_ICON_WIDTH   COMMANDBLOCK_WIDTH
#define BLOCK_LIST_ICON_HEIGHT  FUNCTIONBLOCK_HEIGHT

#define LOOPBLOCK_LEFT_MARGIN   30
#define LOOPBLOCK_TOP_MARGIN    40
#define LOOPBLOCK_BOTTOM_MARGIN 30
#define LOOPBLOCK_WIDTH         COMMANDBLOCK_WIDTH + LOOPBLOCK_LEFT_MARGIN

#define IFBLOCK_LEFT_MARGIN   LOOPBLOCK_LEFT_MARGIN
#define IFBLOCK_TOP_MARGIN    LOOPBLOCK_TOP_MARGIN
#define IFBLOCK_BOTTOM_MARGIN LOOPBLOCK_BOTTOM_MARGIN
#define IFBLOCK_MIDDLE_MARGIN LOOPBLOCK_BOTTOM_MARGIN
#define IFBLOCK_WIDTH         COMMANDBLOCK_WIDTH + IFBLOCK_LEFT_MARGIN

#define DIALOG_WIDTH           260
#define DIALOG_HEIGHT           90
#define SOURCECODEWIDGET_WIDTH 275
#define BLOCKLIST_WIDTH        COMMANDBLOCK_WIDTH + 40 //ursprünglich 120
#define BLOCKLIST_SPACING        5 //ursprünglich 10
#define EDITORWIDGET_HEIGHT    450 //ursprünglich 450
#define EDITORWIDGET_WIDTH     450 //ursprünglich 450

#define FONT_SIZE_DIALOG_HEADING    12
#define FONT_SIZE_PIXMAP_HEADING    12
#define FONT_SIZE_PIXMAP_SUBHEADING 11
#define FONT_SIZE_PIXMAP_BLOCK_LIST 12

///Block-Types
#define TYPE_WAIT           0
#define TYPE_FUNCTION       1
#define TYPE_IF_CUSTOM      2
#define TYPE_LOOP_FOR       3
#define TYPE_LOOP_ENDLESS   4
#define TYPE_LOOP_WHILE     5
#define TYPE_PORT           6
#define TYPE_MOTOR          7
#define TYPE_LED            8
#define TYPE_SERVO          9
#define TYPE_UART          10
#define TYPE_VEHICLE       11
#define TYPE_IF_ADC        12
#define TYPE_MAIN_FUNCTION 13

#define SOCKET_IF      0
#define SOCKET_ELSE    1
#define SOCKET_LOOP    0

#define MOVE_PIXMAP true

///DELAY-Konstanten
#define NUMBER_VALUE_DELAY_VALUE 0
#define NUMBER_VALUE_DELAY_UNIT  1
#define NUMBER_QLABEL_NAME     0
#define NUMBER_QLABEL_DURATION 1
#define NUMBER_QCOMBOBOX_DELAY_UNIT    0

#define INT_DELAY_VALUE      value0
#define INT_DELAY_UNIT       value1
#define QLABEL_NAME          label0
#define QLABEL_DURATION      label1
#define QSPINBOX_DELAY_VALUE spinBox0
#define QCOMBOBOX_DELAY_UNIT comboBox0

#define UNIT_s  0
#define UNIT_ms 1
#define UNIT_us 2

///PORT-Konstanten
#define NUMBER_VALUE_PORT_NUMBER        0
#define NUMBER_VALUE_PORT_OPERATION     1
#define NUMBER_QCOMBOBOX_PORT_OPERATION 0
//#define NUMBER_QLABEL_NAME     0
#define NUMBER_QLABEL_PORT_NUMBER       1


#define INT_PORT_NUMBER          value0
#define INT_PORT_OPERATION       value1
#define QLABEL_PORT_NUMBER       label0
#define QSPINBOX_PORT_NUMBER     spinBox0
#define QCOMBOBOX_PORT_OPERATION comboBox0

#define PORT_OPERATION_AN     0
#define PORT_OPERATION_AUS    1
#define PORT_OPERATION_TOGGLE 2

///UART-Konstanten
//#define NUMBER_VALUE_UART_     0
#define NUMBER_QLABEL_UART_TEXT 1

///MOTOR-Konstanten
#define NUMBER_VALUE_MOTOR_SPEED     0
#define NUMBER_VALUE_MOTOR_OPERATION 1
#define NUMBER_VALUE_MOTOR_NUMBER    2

#define NUMBER_QCOMBOBOX_MOTOR_OPERATION 0
#define NUMBER_QCOMBOBOX_MOTOR_NUMBER    1
#define NUMBER_QLABEL_MOTOR_SPEED        0
#define NUMBER_QLABEL_SERVO_POSITION     NUMBER_QLABEL_MOTOR_SPEED

#define INT_MOTOR_SPEED           value0
#define INT_MOTOR_OPERATION       value1
#define INT_MOTOR_NUMBER          value2
#define INT_SERVO_POSITION        INT_MOTOR_SPEED

#define QLABEL_SERVO_POSITION     label0
#define QLABEL_MOTOR_SPEED        label0
//#define QSPINBOX_PORT_NUMBER      spinBox0
#define QCOMBOBOX_MOTOR_OPERATION comboBox0
#define QCOMBOBOX_MOTOR_NUMBER    comboBox1
#define QSLIDER_MOTOR_SPEED       qSlider0

#define MOTOR_OPERATION_FORWARD  0
#define MOTOR_OPERATION_BACKWARD 1
#define MOTOR_OPERATION_STOP     2

///vehicle-Konstanten
#define NUMBER_VALUE_VEHICLE_STEERING  0
#define NUMBER_VALUE_VEHICLE_OPERATION 1
//#define NUMBER_VALUE_VEHICLE_SPEED     2

#define NUMBER_QCOMBOBOX_VEHICLE_OPERATION 0
#define NUMBER_QLABEL_VEHICLE_STEERING     1
//#define NUMBER_QLABEL_VEHICLE_SPEED        1

#define INT_VEHICLE_STEERING   value0
#define INT_VEHICLE_OPERATION  value1
//#define INT_VEHICLE_SPEED      value2

#define QLABEL_VEHICLE_STEERING     label0
#define QLABEL_VEHICLE_SPEED        label1
#define QCOMBOBOX_VEHICLE_OPERATION comboBox0
#define QSLIDER_VEHICLE_STEERING    qSlider0
//#define QSLIDER_VEHICLE_SPEED       qSlider1

#define VEHICLE_OPERATION_FORWARD  MOTOR_OPERATION_FORWARD
#define VEHICLE_OPERATION_BACKWARD MOTOR_OPERATION_BACKWARD
#define VEHICLE_OPERATION_STOP     MOTOR_OPERATION_STOP

///IF-Konstanten
#define NUMBER_VALUE_IF_TYPE           0
#define NUMBER_VALUE_IF_PORT           1
#define NUMBER_VALUE_IF_RELATION       2
#define NUMBER_VALUE_IF_VALUE          3
//#define NUMBER_QLABEL_LOOP_REPETITIONS 0
#define NUMBER_QCOMBOBOX_IF_TYPE       0
#define NUMBER_QCOMBOBOX_IF_PORT       1
#define NUMBER_QCOMBOBOX_IF_RELATION   2
#define NUMBER_QLABEL_BEDINGUNG        0
#define NUMBER_QLABEL_VOLT             1
//#define NUMBER_QSPINBOX_IF_VALUE       3

//combobox type auch benutzerdefiniert.
//combobox anschluss
//combobox ><= sonderzeichen für größergleich

//spinbox wert 0-255

#define INT_IF_TYPE               value0
#define INT_IF_PORT               value1
#define INT_IF_RELATION           value2
//#define QLABEL_LOOP_REPETITIONS   label0
#define QSPINBOX_IF_VALUE         spinBox0
#define QCOMBOBOX_IF_TYPE         comboBox0
#define QCOMBOBOX_IF_PORT         comboBox1
#define QCOMBOBOX_IF_RELATION     comboBox2

#define IF_TYPE_CUSTOM 0
#define IF_TYPE_ADC    1

#define IF_PORT_6 0
#define IF_PORT_7 1
#define IF_PORT_8 2

#define IF_RELATION_KLEINER         0
#define IF_RELATION_KLEINER_GLEICH  1
#define IF_RELATION_GROESSER        2
#define IF_RELATION_GROESSER_GLEICH 3
#define IF_RELATION_GLEICH          4

///For-Schleifen-Konstanten
#define NUMBER_VALUE_LOOP_REPETITIONS  0
#define NUMBER_VALUE_LOOP_TYPE         1
#define NUMBER_QLABEL_LOOP_REPETITIONS 0
#define NUMBER_QCOMBOBOX_LOOP_TYPE     1

#define INT_LOOP_REPETITIONS      value0
#define INT_LOOP_TYPE             value1
#define QLABEL_LOOP_REPETITIONS   label0
#define QSPINBOX_LOOP_REPETITIONS spinBox0
#define QCOMBOBOX_LOOP_TYPE       comboBox0

#define LOOP_TYPE_FOR     0
#define LOOP_TYPE_ENDLESS 1
#define LOOP_TYPE_WHILE   2

#define USB_CFG_VENDOR_ID  0xc0, 0x16  /* 5824 in dec, stands for VOTI */
#define USB_CFG_DEVICE_ID   0xdc, 0x05  /* 1500 in dec, obdev's free PID */
#define	USB_CFG_VENDOR_NAME     'w', 'w', 'w', '.', 'f', 'i', 's', 'c', 'h', 'l', '.', 'd', 'e'
#define	USB_CFG_DEVICE_NAME		'U', 'S', 'B', 'a', 's', 'p'
#define USBASP_FUNC_SET_UART1        17
#define USBASP_FUNC_SET_UART2        18
#define USBASP_FUNC_TXRX_UART        19

///Iteratoren
#include <QList>
typedef QList<ProgramFunction*>::iterator ProgramFunctionIterator;
typedef QList<ProgramBlock*>::iterator ProgramBlockIterator;
//typedef QList<QRect*>::iterator HitboxIterator;


//#define VALUE(number) {if (number == 0) value0 else if (number == 1) value1}
//#define TAUSCHE(a, b, typ) { typ temp; temp=b; b=a; a=temp; }


//class MainWindow;
//class EditorWidget;
//class SourcecodeWidget;
//class BlockListWidget;
//class ProgramCode;
//class ProgramBlock;
//class ProgramIfBlock;
//class ProgramLoopBlock;
//class ProgramCommandBlock;
//class ProgramFunction;


//#include "ProgramBlock.h"
//#include "ProgramIfBlock.h"
//#include "ProgramLoopBlock.h"
//#include "ProgramCommandBlock.h"
//#include "ProgramFunction.h"
//#include "ProgramCode.h"
//#include "EditorWidget.h"
//#include "SourcecodeWidget.h"
//#include "BlockListWidget.h"
//#include "mainwindow.h"

#endif // MAIN_H
