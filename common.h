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

#ifndef COMMON_H
#define COMMON_H

#include <kuser.h>

#include <QtCore/QString>
#include <QtGui/QPixmap>

static const char DEFAULT_ICON_NAME[]     = "user-identity";
static const char LOCK_SCREEN_ICON_NAME[] = "system-lock-screen";
static const char LOGOUT_ICON_NAME[]      = "system-shutdown";
static const char SWITCH_ICON_NAME[]      = "system-switch-user";

const QString getUsername(bool useShortName=false, const KUser& user = KUser());
QPixmap getUserIcon(const KUser& user = KUser());
QPixmap getLockScreenIcon();
QPixmap getLogoutIcon();
QPixmap getSessionSwitchIcon();

#endif
