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

#ifndef FASTUSERSWITCH_H
#define FASTUSERSWITCH_H

//Plasma
#include <plasma/popupapplet.h>

namespace Plasma {
  class ExtenderItem;
}

class KIcon;
class SessionWidget;

/**
* @short Applet used to switch between user sessions
*
*/
class FastUserSwitch : public Plasma::PopupApplet
{
  Q_OBJECT

  public:
    /**
    * Constructor of the applet
    * @param parent the parent of this object
    **/
    FastUserSwitch(QObject *parent, const QVariantList &args);

    /**
    * Default destructor
    **/
    ~FastUserSwitch();

    void init();

  protected:
    void popupEvent(bool show);

  private:
    void initExtenderItem(Plasma::ExtenderItem *item);
    
    SessionWidget* m_dialog;
    KIcon* m_icon;
    Plasma::ExtenderItem* m_extender;
};

#endif
