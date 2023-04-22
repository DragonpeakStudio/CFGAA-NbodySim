#include "frameslider.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QIcon>

FrameSlider::FrameSlider(QWidget *_parent) : QWidget(_parent)
{
  setLayout(new QHBoxLayout(this));

  QVBoxLayout *frameNumLayout = new QVBoxLayout(this);
  ((QHBoxLayout*)layout())->addLayout(frameNumLayout);

  m_frameNum = new QSpinBox(this);
  m_frameNum->setValue(0);
  m_frameNum->setMaximum(1920);
  frameNumLayout->addWidget(new QLabel("Frame Num:", this));
  frameNumLayout->addWidget(m_frameNum);

  QVBoxLayout *fpsLayout = new QVBoxLayout(this);
  ((QHBoxLayout*)layout())->addLayout(fpsLayout);
  m_fps = new QSpinBox(this);
  m_fps->setValue(24);
  m_fps->setMaximum(512);
  fpsLayout->addWidget(new QLabel("FPS:", this));
  fpsLayout->addWidget(m_fps);
  m_playbackTimer = new QTimer(this);

  m_toStart = new QPushButton(QIcon("icons/media-step-backward-8x.png"), "", this);
  connect(m_toStart, &QPushButton::pressed, [this](){m_playbackTimer->stop();setFrame(0);});
  layout()->addWidget(m_toStart);

  m_rewind = new QPushButton(QIcon("icons/media-reversed-play-8x.png"), "", this);
  connect(m_rewind, &QPushButton::pressed, [this](){m_frameAdvance = -1; m_playbackTimer->start((1./m_fps->value())*1000);});
  layout()->addWidget(m_rewind);

  m_pause = new QPushButton(QIcon("icons/media-pause-8x.png"), "", this);
  connect(m_pause, &QPushButton::pressed, [this](){m_playbackTimer->stop();});
  layout()->addWidget(m_pause);

  m_play = new QPushButton(QIcon("icons/media-play-8x.png"), "", this);
  connect(m_play, &QPushButton::pressed, [this](){m_frameAdvance = 1; m_playbackTimer->start((1./m_fps->value())*1000);});
  layout()->addWidget(m_play);

  m_toEnd = new QPushButton(QIcon("icons/media-step-forward-8x.png"), "", this);
  connect(m_toEnd, &QPushButton::pressed, [this](){m_playbackTimer->stop();setFrame(m_frameEnd->value());});
  layout()->addWidget(m_toEnd);

  m_slider = new QSlider(Qt::Orientation::Horizontal, this);
  m_slider->setMinimum(0);
  m_slider->setMaximum(1920);
  m_slider->setTickPosition(QSlider::TicksBelow);
  m_slider->setTickInterval(1);
  layout()->addWidget(m_slider);

  QVBoxLayout *frameEndLayout = new QVBoxLayout(this);
  ((QHBoxLayout*)layout())->addLayout(frameEndLayout);

  m_frameEnd = new QSpinBox(this);
  m_frameEnd->setMaximum(9999999);
  m_frameEnd->setValue(1920);
  frameEndLayout->addWidget(new QLabel("End Frame:", this));
  frameEndLayout->addWidget(m_frameEnd);
  connect(m_frameEnd, &QSpinBox::valueChanged, this, &FrameSlider::setEnd);
  connect(m_frameNum, &QSpinBox::valueChanged, this, &FrameSlider::setFrame);
  connect(m_slider, &QSlider::valueChanged, this, &FrameSlider::setFrame);
  connect(m_fps, &QSpinBox::valueChanged, this, &FrameSlider::setFps);
  connect(m_playbackTimer, &QTimer::timeout, this, [this]()
  {
    if(m_frameNum->value()+m_frameAdvance > m_frameEnd->value() || m_frameNum->value()+m_frameAdvance < 0)
    {
      m_playbackTimer->stop();
    }
    else
    {
      setFrame(m_frameNum->value()+m_frameAdvance);
    }
  });
}
FrameSlider::~FrameSlider()
{

}
int FrameSlider::getFrame() const
{
  return m_frameNum->value();
}
void FrameSlider::setFrame(int _frame)
{
  m_frameNum->setValue(_frame);
  m_slider->setValue(_frame);
  emit frameChanged(_frame);
}
void FrameSlider::setEnd(int _end)
{
  m_frameEnd->setValue(_end);
  m_frameNum->setMaximum(_end);
  m_slider->setMaximum(_end);
  emit endFrameChanged(_end);

}

void FrameSlider::setFps(int _fps)
{
  m_fps->setValue(_fps);
  emit fpsChanged(_fps);
}

