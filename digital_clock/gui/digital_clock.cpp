#include <QTime>
#include <QLocale>
#include "digital_clock.h"

DigitalClock::DigitalClock(QWidget* parent)
  : QLabel(parent) {
  timer_ = new QTimer(this);
  timer_->setInterval(500);
  connect(timer_, SIGNAL(timeout()), this, SLOT(TimeoutHandler()));
  timer_->start();

  sep_visible_ = false;
  sep_flashes_ = true;
}

DigitalClock::~DigitalClock() {
  timer_->stop();
}

void DigitalClock::DrawImage(const QImage& image) {
  setPixmap(QPixmap::fromImage(image));
  parentWidget()->adjustSize();
}

void DigitalClock::SetSeparatorFlash(bool set) {
  sep_flashes_ = set;
  sep_visible_ = !set;
}

void DigitalClock::SetTimeFormat(const QString& format) {
  time_format_ = format;
}

void DigitalClock::TimeoutHandler() {
  if (time_format_.isEmpty()) time_format_ = QLocale::system().timeFormat();
  QString time = QTime::currentTime().toString(time_format_);
  int sep_pos = time.indexOf(':');
  time = time.mid(0, time.lastIndexOf(':'));
  if (sep_flashes_) {
    time[sep_pos] = sep_visible_ ? ':' : ' ';
    sep_visible_ = !sep_visible_;
  }
  emit ImageNeeded(time);
}