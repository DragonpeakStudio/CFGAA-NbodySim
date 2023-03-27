#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QString>

class AppWindow : public QMainWindow
{
  Q_OBJECT
  public:
    AppWindow(const QString &_title, QWidget *_parent=nullptr);
    AppWindow(QWidget *_parent=nullptr);

    ~AppWindow();
  
  private:

};

#endif