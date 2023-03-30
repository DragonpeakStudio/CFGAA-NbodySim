#ifndef CONTROLBAR_H
#define CONTROLBAR_H

#pragma once

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
class ControlBar : public QWidget
{
  Q_OBJECT
  public:
    ControlBar(QWidget *_parent=nullptr);
    ~ControlBar();
  
  signals:
    void resetPress();
  
  private:
    QLabel *m_text;
    QPushButton *m_reset;
};

#endif