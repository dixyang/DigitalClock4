#include <QHBoxLayout>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
  : QWidget(parent) {
  d_clock_ = new DigitalClock();
  QHBoxLayout* main_layout = new QHBoxLayout();
  main_layout->addWidget(d_clock_);
  setLayout(main_layout);

  setWindowFlags(Qt::FramelessWindowHint);
  setAttribute(Qt::WA_TranslucentBackground);

  ConnectAll();
  skin_manager_.AddSkinDir(QDir(QDir::currentPath() + "/skins"));
  skin_manager_.ListSkins();
  settings_.Load();
}

MainWindow::~MainWindow() {
  settings_.SetOption(OPT_POSITION, pos());
  settings_.Save();
}

void MainWindow::SettingsListener(Options opt, const QVariant& value) {
  switch (opt) {
    case OPT_OPACITY:
    {
      qreal opacity = value.toReal();
      setWindowOpacity(opacity > 0.1 ? opacity : 0.75);
      break;
    }

    case OPT_STAY_ON_TOP:
      SetWindowFlag(Qt::WindowStaysOnTopHint, value.toBool());
      break;

    case OPT_TRANSP_FOR_INPUT:
      SetWindowFlag(Qt::WindowTransparentForInput, value.toBool());
      break;

    case OPT_SEPARATOR_FLASH:
      d_clock_->SetSeparatorFlash(value.toBool());
      break;

    case OPT_POSITION:
      move(value.toPoint());
      break;

    case OPT_SKIN_NAME:
    {
      QString skin_name = value.toString();
      skin_manager_.FindSkin(skin_name.isEmpty() ? "Comic Sans" : skin_name);
      break;
    }

    case OPT_ZOOM:
    {
      qreal zoom = value.toReal();
      drawer_.SetZoom((zoom > 0.1) && (zoom <= 4) ? zoom : 1.25);
      break;
    }

    case OPT_COLOR:
    {
      QColor color = value.value<QColor>();
      drawer_.SetColor(color.isValid() ? color : Qt::blue);
      break;
    }

    case OPT_TEXTURE:
      drawer_.SetTexture(value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_.SetTexturePerElement(value.toBool());
      break;
  }
}

void MainWindow::ConnectAll() {
  connect(&settings_, SIGNAL(OptionChanged(Options,QVariant)),
          this, SLOT(SettingsListener(Options,QVariant)));
  connect(&skin_manager_, SIGNAL(SkinFound(QDir)), &drawer_, SLOT(LoadSkin(QDir)));
  connect(&drawer_, SIGNAL(DrawingFinished(QImage)), d_clock_, SLOT(DrawImage(QImage)));
  connect(d_clock_, SIGNAL(ImageNeeded(QString)), &drawer_, SLOT(SetString(QString)));
}

void MainWindow::SetWindowFlag(Qt::WindowFlags flag, bool set) {
  hide();
  Qt::WindowFlags flags = windowFlags();
  set ? flags |= flag : flags &= ~flag;
  setWindowFlags(flags);
  show();
}
