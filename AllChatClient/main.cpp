#include "Core/control.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Control c =  Control();
    //todo 数据处理和iu线程分离
    return a.exec();
}
