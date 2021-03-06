/*
    Digital Clock - beautiful customizable clock with plugins
    Copyright (C) 2017-2018  Nick Korotysh <nick.korotysh@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef DIGITAL_CLOCK_GUI_CONTEXT_MENU_H
#define DIGITAL_CLOCK_GUI_CONTEXT_MENU_H

#include <QObject>

class QMenu;
class QAction;

namespace digital_clock {
namespace gui {

class ContextMenu : public QObject
{
  Q_OBJECT

public:
  explicit ContextMenu(QObject* parent = nullptr);
  ~ContextMenu();

  QMenu* menu() const Q_DECL_NOEXCEPT { return menu_; }
  QAction* visibilityAction() const Q_DECL_NOEXCEPT { return visibility_action_; }

signals:
  void VisibilityChanged(bool visible);
  void PositionChanged(Qt::Alignment pos);
  void ShowSettingsDlg();
  void ShowAboutDlg();
  void CheckForUpdates();
  void AppExit();

private:
  QMenu* menu_;
  QAction* visibility_action_;
};

} // namespace gui
} // namespace digital_clock

#endif // DIGITAL_CLOCK_GUI_CONTEXT_MENU_H
