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

#include "sessionwidget.h"
#include "common.h"

#include <QtCore/QList>
#include <QtCore/QSignalMapper>
#include <QtCore/QVariant>
#include <QtDBus/QDBusInterface>
#include <QtGui/QLabel>
#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QShowEvent>

#include <kiconloader.h>
#include <ksharedconfig.h>
#include <kuser.h>
#include <kworkspace/kdisplaymanager.h>
#include <plasma/widgets/iconwidget.h>
#include <plasma/widgets/separator.h>

bool operator <(const Plasma::IconWidget& a, const Plasma::IconWidget& b)
{
  return (a.text() < b.text());
}

static Plasma::IconWidget *createButton(QGraphicsWidget *parent)
{
    Plasma::IconWidget *button = new Plasma::IconWidget(parent);
    button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    button->setOrientation(Qt::Horizontal);
    button->setDrawBackground(true);
    button->setTextBackgroundColor(QColor(Qt::transparent));
    return button;
}

SessionWidget::SessionWidget( QGraphicsItem * parent, Qt::WindowFlags wFlags)
  : QGraphicsWidget ( parent, wFlags)
{
  this->setFocusPolicy(Qt::ClickFocus);

   m_signalMapper = new QSignalMapper(this);

  QGraphicsLinearLayout* mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
  mainLayout->setSpacing(0);
  mainLayout->addStretch();
  this->setLayout(mainLayout);

  connect( m_signalMapper, SIGNAL(mapped(int)),
           this, SLOT(slotSwitchSession(int)));

  Q_ASSERT(mainLayout->count() == 0);

  //Cache the icon pixmaps
  QSize iconSize = QSize(KIconLoader::SizeSmallMedium, KIconLoader::SizeSmallMedium);

  SessList sessions;
  KDisplayManager manager;
  manager.localSessions(sessions);

  QList<QGraphicsWidget*> entries;

  foreach(SessEnt session, sessions) {
    QPixmap pixmap;
    KUser user (session.user);
    if (!user.isValid())
      continue;

    if (session.tty)
      continue;

    Plasma::IconWidget* entry = createButton(this);
    pixmap = getUserIcon(user);

    entry->setIcon(pixmap);

    QString username = getUsername(false, user);
    entry->setText(username);

    connect(entry, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
    m_signalMapper->setMapping(entry, session.vt);

    if (session.self)
      entry->setEnabled(false);

    entries << entry;
  }

  // sort the entries alphabetically
  qSort(entries);

  // add separator
  Plasma::Separator* separator = new Plasma::Separator(this);
  separator->setOrientation(Qt::Horizontal);
  entries << separator;

  // add "Login window"
  Plasma::IconWidget* entry = createButton(this);
  entry->setIcon(getSessionSwitchIcon());
  entry->setText(i18n("Open login window"));
  connect(entry, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(entry, SessionWidget::OPEN_LOGIN);
  entries << entry;

  // add "Lock screen"
  Plasma::IconWidget* entryLockScreen = createButton(this);
  entryLockScreen->setIcon(getLockScreenIcon());
  entryLockScreen->setText(i18n("Lock Screen"));
  connect(entryLockScreen, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(entryLockScreen, SessionWidget::LOCK_SCREEN);
  entries << entryLockScreen;
  
  // add "Logout"
  Plasma::IconWidget* entryLogout = createButton(this);
  entryLogout->setIcon(getLogoutIcon());
  entryLogout->setText(i18n("Leave..."));
  connect(entryLogout, SIGNAL(clicked()), m_signalMapper, SLOT(map()));
  m_signalMapper->setMapping(entryLogout, SessionWidget::LEAVE);
  entries << entryLogout;

  foreach(QGraphicsWidget* entry, entries) {
    mainLayout->addItem(entry);
  }
}

void SessionWidget::slotSwitchSession(int vt)
{
  emit switching();

  KDisplayManager manager;
  if (vt == SessionWidget::OPEN_LOGIN) {
    // Figure out screensaver's 'lock' option value
    KSharedConfigPtr config = KSharedConfig::openConfig("kscreensaverrc");
    KConfigGroup screensaverGroup = config->group("ScreenSaver");
    bool lockToggled = screensaverGroup.readEntry("Lock", false);

    // lock screen and start new session
    QDBusInterface screensaver("org.freedesktop.ScreenSaver",
                               "/ScreenSaver", "org.freedesktop.ScreenSaver");
    if (lockToggled)
      screensaver.call( "Lock" );
    else
      screensaver.call( "SetActive", true );

    manager.startReserve();
  } else if (vt == SessionWidget::LOCK_SCREEN) {
    // lock screen
    QDBusInterface screensaver("org.freedesktop.ScreenSaver",
                               "/ScreenSaver", "org.freedesktop.ScreenSaver");
    screensaver.call( "Lock" );
  } else if (vt == SessionWidget::LEAVE) {
    // logout
    KWorkSpace::requestShutDown( KWorkSpace::ShutdownConfirmDefault,
                                 KWorkSpace::ShutdownTypeDefault,
                                 KWorkSpace::ShutdownModeDefault);
  } else
    manager.lockSwitchVT(vt);
}
