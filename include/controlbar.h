#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#pragma once

#include <QWidget>
#include <QLabel>
class ControlBar : public QWidget
{
  Q_OBJECT
  public:
    ControlBar(QWidget *_parent=nullptr);
    ~ControlBar();
  
  private:
    QLabel *m_text;
};

#endif