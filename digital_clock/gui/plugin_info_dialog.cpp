#include "plugin_info_dialog.h"
#include "ui_plugin_info_dialog.h"

PluginInfoDialog::PluginInfoDialog(QWidget* parent)
  : CenteredDialog(parent), ui(new Ui::PluginInfoDialog) {
  ui->setupUi(this);
}

PluginInfoDialog::~PluginInfoDialog() {
  delete ui;
}

void PluginInfoDialog::SetInfo(const TPluginInfo& info) {
  ui->name_value->setText(info[PI_NAME]);
  ui->version_value->setText(info[PI_VERSION]);
  ui->description_value->setText(info[PI_COMMENT]);
  ui->type_value->setText(info[PI_TYPE]);
  ui->author_value->setText(info[PI_AUTHOR]);
  ui->email_value->setText(info[PI_EMAIL]);
  adjustSize();
}