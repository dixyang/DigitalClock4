#include <QLabel>
#include <QGridLayout>
#include <QInputDialog>
#include <QFontMetricsF>
#include "plugin_settings.h"
#include "skin_drawer.h"
#include "text_skin.h"
#include "quick_note_settings.h"
#include "quick_note.h"

namespace quick_note {

QuickNote::QuickNote() : avail_width_(0), last_zoom_(1.0) {
  settings_ = new PluginSettings("Nick Korotysh", "Digital Clock", this);
  drawer_ = new skin_draw::SkinDrawer(this);

  InitTranslator(QLatin1String(":/quick_note/quick_note_"));
  info_.display_name = tr("Quick note");
  info_.description = tr("Allows to display any short message under clock.");
  info_.icon.load(":/quick_note/icon.png");
}

void QuickNote::Init(QWidget* main_wnd) {
  main_layout_ = qobject_cast<QGridLayout*>(main_wnd->layout());
  main_wnd_ = main_wnd;
  QSettings::SettingsMap defaults;
  InitDefaults(&defaults);
  settings_->SetDefaultValues(defaults);
  settings_->Load();
  drawer_->SetSpace(0);
}

void QuickNote::Start() {
  msg_label_ = new QLabel();
  msg_label_->setAlignment(Qt::AlignCenter);
  main_layout_->addWidget(msg_label_, main_layout_->rowCount(), 0, 1, main_layout_->columnCount());
  connect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, [=] (const QImage& img) {
    msg_label_->setPixmap(QPixmap::fromImage(img));
    main_wnd_->adjustSize();
  });
  ApplyString(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
}

void QuickNote::Stop() {
  main_layout_->removeWidget(msg_label_);
  main_wnd_->adjustSize();
  disconnect(drawer_, &skin_draw::SkinDrawer::DrawingFinished, 0, 0);
  delete msg_label_;
}

void QuickNote::Configure() {
  QInputDialog* settings_dlg = new QInputDialog();
  settings_dlg->setAttribute(Qt::WA_DeleteOnClose);
  settings_dlg->setModal(true);

  settings_dlg->setWindowTitle(tr("Quick note settings"));
  settings_dlg->setLabelText(tr("message:"));

  settings_dlg->setInputMode(QInputDialog::TextInput);
  settings_dlg->setTextValue(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());

  connect(settings_dlg, &QInputDialog::textValueSelected, [=] (const QString& str) {
    settings_->SetOption(OPT_QUICK_NOTE_MSG, str);
    settings_->Save();
    this->ApplyString(str);
  });
  connect(settings_dlg, &QInputDialog::rejected, [=] () {
    settings_->Load();
    this->ApplyString(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
  });

  settings_dlg->show();
}

void QuickNote::SettingsListener(Options option, const QVariant& new_value) {
  switch (option) {
    case OPT_SKIN_NAME:
      if (!msg_label_) break;  // init, not started yet
      Stop();
      avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 16;
      Start();
      break;

    case OPT_FONT:
      font_ = new_value.value<QFont>();
      drawer_->ApplySkin(skin_draw::ISkin::SkinPtr(new skin_draw::TextSkin(font_)));
      break;

    case OPT_ZOOM:
    {
      last_zoom_ = new_value.toReal();
      if (avail_width_ == 0) {  // first init
        avail_width_ = main_layout_->cellRect(0, 0).width() / last_zoom_ - 16;
      }
      QFontMetricsF fmf(font_);
      qreal tw = fmf.width(settings_->GetOption(OPT_QUICK_NOTE_MSG).toString());
      drawer_->SetZoom(avail_width_ * last_zoom_ / tw);
      break;
    }

    case OPT_COLOR:
      drawer_->SetColor(new_value.value<QColor>());
      break;

    case OPT_TEXTURE:
      drawer_->SetTexture(new_value.toString());
      break;

    case OPT_TEXTURE_PER_ELEMENT:
      drawer_->SetTexturePerElement(new_value.toBool());
      break;

    case OPT_TEXTURE_DRAW_MODE:
      drawer_->SetTextureDrawMode((skin_draw::SkinDrawer::DrawMode)new_value.toInt());
      break;

    case OPT_CUSTOMIZATION:
      drawer_->SetCustomizationType((skin_draw::SkinDrawer::CustomizationType)new_value.toInt());
      break;
  }
}

void QuickNote::ApplyString(const QString& str) {
  drawer_->SetString(QString());
  QFontMetricsF fmf(font_);
  drawer_->SetZoom(avail_width_ * last_zoom_ / fmf.width(str));
  drawer_->SetString(str);
}

} // namespace quick_note