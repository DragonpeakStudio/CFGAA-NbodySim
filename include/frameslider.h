#ifndef FRAMESLIDER_H
#define FRAMESLIDER_H

#pragma once

#include <QWidget>  
#include <QSlider>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QTimer>
#include <QPushButton>
#include <QIcon>

class FrameSlider : public QWidget
{
  Q_OBJECT
  public:
    FrameSlider(QWidget *_parent=nullptr);
    ~FrameSlider();
    int getFrame() const;
  
  public slots:
    void setFrame(int _frame);
    void setEnd(int _end);
    void setFps(int _fps);
  signals:
    void frameChanged(int _newFrame);
    void endFrameChanged(int _newEndFrame);
    void fpsChanged(int _newFps);
  private:
    QSlider *m_slider;
    QSpinBox *m_frameEnd;
    QSpinBox *m_frameNum;
    QSpinBox *m_fps;
    QTimer *m_playbackTimer;
    QPushButton *m_toStart;
    QPushButton *m_rewind;
    QPushButton *m_pause;
    QPushButton *m_play;
    QPushButton *m_toEnd;;
    int m_frameAdvance = 0;
};

#endif