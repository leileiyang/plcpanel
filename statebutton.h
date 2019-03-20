#ifndef STATEBUTTON_H
#define STATEBUTTON_H

#include <QWidget>
#include <QtGui/QPushButton>

class StateButton : public QPushButton
{
  Q_OBJECT
public:
  explicit StateButton(QWidget *parent = 0);

public slots:
  void onSwitch(bool on);

protected:
  void paintEvent(QPaintEvent *event);

  bool on_;
};

#endif // STATEBUTTON_H
