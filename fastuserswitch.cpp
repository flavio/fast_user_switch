/***************************************************************************
 *   Copyright (C) 2010 by Flavio Castelli <flavio@castelli.name>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

#include "fastuserswitch.h"
#include "sessionwidget.h"

#include <kicon.h>
#include <kiconloader.h>
#include <kuser.h>
#include <plasma/extenderitem.h>
#include <plasma/tooltipcontent.h>
#include <plasma/tooltipmanager.h>

using namespace Plasma;

static const char DEFAULT_ICON_NAME[] = "system-switch-user";

K_EXPORT_PLASMA_APPLET(fastuserswitch, FastUserSwitch)

FastUserSwitch::FastUserSwitch(QObject *parent, const QVariantList &args)
  : Plasma::PopupApplet(parent, args),
    m_dialog(0)
{
  setPopupIcon(QIcon());
  int iconSize = IconSize(KIconLoader::Desktop);
  resize(iconSize*2 , iconSize*2 );
  setAspectRatioMode(Plasma::ConstrainedSquare );
  setPopupIcon(QIcon());
  //we load some icon
  m_icon= new KIcon(DEFAULT_ICON_NAME);
  //create extender
  m_extender = new Plasma::ExtenderItem(extender());
  m_extender->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  initExtenderItem(m_extender);
}

FastUserSwitch::~FastUserSwitch()
{
  delete m_icon;
}

void FastUserSwitch::init()
{
  setHasConfigurationInterface(false);
  setPopupIcon(DEFAULT_ICON_NAME);

  //init tooltip
  KUser user;
  QString username = user.property(KUser::FullName).toString();
  if (username.isEmpty())
    username = user.loginName();

  Plasma::ToolTipContent data;
  data.setMainText(i18n("Fast user switch"));
  data.setSubText(i18n("You are currently logged in as %1", username));
  data.setImage(KIcon(DEFAULT_ICON_NAME).pixmap(IconSize(KIconLoader::Desktop)));
  Plasma::ToolTipManager::self()->setContent(this, data);
}

void FastUserSwitch::initExtenderItem(Plasma::ExtenderItem *item)
{
  if (m_dialog)
    delete m_dialog;

  m_dialog = new SessionWidget(item);
  connect(m_dialog, SIGNAL(switching()), this, SLOT(hidePopup()));
  item->setWidget(m_dialog);
  item->setTitle(i18n("Switch User"));
}

void FastUserSwitch::popupEvent(bool show)
{
  if (show) {
    initExtenderItem(m_extender);
  }
}

#include "fastuserswitch.moc"
