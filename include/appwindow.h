#ifndef APPWINDOW_H
#define APPWINDOW_H

#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QMenuBar>
#include <QDockWidget>
#include "renderwidget.h"
/**
 * @brief Main application window, contents all other relevant widgets
 * Also handles communication between widgets via signals and slots
 *
 */
class AppWindow : public QMainWindow
{
  Q_OBJECT
public:
  AppWindow(const QString &_title, QWidget *_parent = nullptr);
  AppWindow(QWidget *_parent = nullptr);

  ~AppWindow();

public slots:
  /**
   * @brief Adds particles to the system from user input
   * This is part of the main app window and not the render widget for simplicities sake, as it also needs data from other widgets
   *
   * @param _particles the particles to add as a const vector
   */
  void addParticles(const std::vector<Particle> &_particles);

private:
  void setupUI();

  QMenu *m_fileMenu;
  QDockWidget *m_controlBar;
  RenderWidget *m_renderWidget;
  QDockWidget *m_frameSlider;
};

#endif