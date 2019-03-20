#include "plcpanelwidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  PlcPanelWidget w;
  w.show();
  w.Initialzie();

  return a.exec();
}
