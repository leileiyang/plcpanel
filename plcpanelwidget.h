#ifndef PLCPANELWIDGET_H
#define PLCPANELWIDGET_H

#include <QWidget>
#include <QTimer>

#include "plcclient/plcclient.h"
#include "dev/gas.h"

#include "plcmsg/plc_nml.h"

#include <QTime>

template <typename T>
int CheckStatusBit(int pos, const T &status) {
  return (status >> pos) & 1U;
}

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
  void updateStatus();

private:
  Ui::PlcPanelWidget *ui;

  Gas *gas_;
  PlcClient plc_client_;

  QTimer update_timer_;
  QTime t;
};

#endif // PLCPANELWIDGET_H
