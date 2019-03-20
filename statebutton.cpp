#include "statebutton.h"

StateButton::StateButton(QWidget *parent) : QPushButton(parent), on_(false)
{
  connect(this, SIGNAL(clicked(bool)), this, SLOT(onSwitch(bool)));
}

void StateButton::paintEvent(QPaintEvent *event) {
  if (on_) {
    setStyleSheet(QString::fromUtf8("background-color: rgb(0, 170, 0);"));
  } else {
    setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);"));
  }
  QPushButton::paintEvent(event);
}

void StateButton::onSwitch(bool on) {
  on_ = on;
}
