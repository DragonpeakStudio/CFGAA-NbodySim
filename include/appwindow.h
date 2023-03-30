#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <QDockWidget>
#include <QFileDialog>
#include <fstream>
#include "controlbar.h"
#include "renderwidget.h"
#include "frameslider.h"
class AppWindow : public QMainWindow
{
  Q_OBJECT
  public:
    AppWindow(const QString &_title, QWidget *_parent=nullptr);
    AppWindow(QWidget *_parent=nullptr);

    ~AppWindow();
  
  private:
    void setupUI();
    
    QMenu *m_fileMenu;
    QDockWidget *m_controlBar;
    RenderWidget *m_renderWidget;
    QDockWidget *m_frameSlider;

};

#endif