#include "controlbar.h"
ControlBar::ControlBar(QWidget *_parent) : QWidget(_parent)
{
  m_text = new QLabel("TEST", this);
}

ControlBar::~ControlBar()
{

}