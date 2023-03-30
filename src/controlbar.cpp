#include "controlbar.h"
ControlBar::ControlBar(QWidget *_parent) : QWidget(_parent)
{
  setLayout(new QVBoxLayout(this));
  m_reset = new QPushButton("Reset Sim", this);
  connect(m_reset, &QPushButton::pressed, [this](){emit resetPress();});
  layout()->addWidget(m_reset);

}

ControlBar::~ControlBar()
{

}