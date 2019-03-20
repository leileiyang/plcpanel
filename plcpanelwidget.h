#ifndef PLCPANELWIDGET_H
#define PLCPANELWIDGET_H

#include <QWidget>

#include "plcclient/plcclient.h"
#include "dev/gas.h"

namespace Ui {
class PlcPanelWidget;
}

class PlcPanelWidget : public QWidget
{
  Q_OBJECT

public:
  explicit PlcPanelWidget(QWidget *parent = 0);
  ~PlcPanelWidget();

  int Initialzie();

private slots:
  void onBlow(bool checked);

private:
  Ui::PlcPanelWidget *ui;

  Gas *gas_;
  PlcClient plc_client_;
};

#endif // PLCPANELWIDGET_H
