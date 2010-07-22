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

#include "common.h"
#include "fastuserswitch.h"
#include "sessionwidget.h"

#include <QtGui/QGraphicsLinearLayout>
#include <QtGui/QGraphicsSceneResizeEvent>
#include <QtGui/QLabel>

#include <kiconloader.h>
#include <kconfigdialog.h>
#include <ksharedconfig.h>

#include <plasma/extenderitem.h>
#include <plasma/tooltipcontent.h>
#include <plasma/tooltipmanager.h>
#include <plasma/widgets/label.h>

using namespace Plasma;

K_EXPORT_PLASMA_APPLET(fastuserswitch, FastUserSwitch)

#define ICON_PLUS_TEXT "showIconPlusText"
#define ICON_ONLY "showOnlyIcon"
#define TEXT_ONLY "showOnlyText"
#define USE_USER_IMAGE "useUserImage"
#define USE_COMPLETE_NAME "useCompleteName"

#define MARGINSIZE 6

FastUserSwitch::FastUserSwitch(QObject *parent, const QVariantList &args)
  : Plasma::PopupApplet(parent, args),
    m_dialog(0),
    m_labelIcon(0)
{
  setAspectRatioMode(Plasma::IgnoreAspectRatio);
  setHasConfigurationInterface(true);

  //create extender
  m_extender = new Plasma::ExtenderItem(extender());
  m_extender->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
  initExtenderItem(m_extender);
}

FastUserSwitch::~FastUserSwitch()
{
}

void FastUserSwitch::init()
{
  configChanged();

  QString username = currentUsername(!m_useCompleteName);
  m_userpixmap = currentUserIcon();

  m_layout = new QGraphicsLinearLayout(Qt::Horizontal, this);
  m_layout->setSpacing(0);

  m_labelIcon = new Plasma::Label();
  m_labelIcon->nativeWidget()->setSizePolicy(QSizePolicy::Preferred,
                                             QSizePolicy::Preferred);

  m_labelName = new Plasma::Label();
  m_labelName->setAlignment(Qt::AlignCenter);
  m_labelName->nativeWidget()->setMargin(10);
  m_labelName->nativeWidget()->setSizePolicy(QSizePolicy::MinimumExpanding,
                                             QSizePolicy::MinimumExpanding);

  setLayout(m_layout);

  checkLayout();
  setupTooltip();
}

void FastUserSwitch::setupTooltip()
{
  QString username = currentUsername(!m_useCompleteName);
  QPixmap pixmap;

  if (m_useUserImage) {
    pixmap = m_userpixmap.scaledToHeight(qMin(IconSize(KIconLoader::Desktop),
                                              m_userpixmap.height()));
  } else
    pixmap = KIcon(DEFAULT_ICON_NAME).pixmap(IconSize(KIconLoader::Desktop));

  Plasma::ToolTipContent data;
  data.setMainText(i18n("Fast user switch"));
  data.setSubText(i18n("You are currently logged in as <em>%1</em>.",
                       username));
  data.setImage(pixmap);
  Plasma::ToolTipManager::self()->setContent(this, data);
}

void FastUserSwitch::initExtenderItem(Plasma::ExtenderItem *item)
{
  if (m_dialog)
    delete m_dialog;

  m_dialog = new SessionWidget(item);
  connect(m_dialog, SIGNAL(switching()), this, SLOT(hidePopup()));
  item->setWidget(m_dialog);
  item->setTitle(i18n("Fast user switch"));
}

void FastUserSwitch::configChanged()
{
  KConfigGroup cg = config();
  m_showIconPlusText = cg.readEntry(ICON_PLUS_TEXT, false);
  m_showOnlyIcon = cg.readEntry(ICON_ONLY, true);
  m_showOnlyText = cg.readEntry(TEXT_ONLY, false);
  m_useUserImage = cg.readEntry(USE_USER_IMAGE, false);
  m_useCompleteName = cg.readEntry(USE_COMPLETE_NAME, true);
}
void FastUserSwitch::configAccepted()
{
  bool changed = false;
  KConfigGroup cg = config();

  if (m_showIconPlusText != ui.radioButton_iconPlusText->isChecked()) {
    m_showIconPlusText = !m_showIconPlusText;
    cg.writeEntry(ICON_PLUS_TEXT, m_showIconPlusText);
    changed = true;
  }

  if (m_showOnlyIcon != ui.radioButton_iconOnly->isChecked()) {
    m_showOnlyIcon = !m_showOnlyIcon;
    cg.writeEntry(ICON_ONLY, m_showOnlyIcon);
    changed = true;
  }

  if (m_showOnlyText != ui.radioButton_textOnly ->isChecked()) {
    m_showOnlyText = !m_showOnlyText;
    cg.writeEntry(TEXT_ONLY, m_showOnlyText);
    changed = true;
  }

  if (m_useCompleteName != ui.checkBox_completeName->isChecked()) {
    m_useCompleteName = !m_useCompleteName;
    cg.writeEntry(USE_COMPLETE_NAME, m_useCompleteName);
    changed = true;
  }

  if (m_useUserImage != ui.checkBox_userImage->isChecked()) {
    m_useUserImage = !m_useUserImage;
    cg.writeEntry(USE_USER_IMAGE, m_useUserImage);
    changed = true;
  }

  if (changed) {
    checkLayout();
    setupTooltip();
    emit configNeedsSaving();
  }
}

void FastUserSwitch::createConfigurationInterface(KConfigDialog *parent)
{
  QWidget *widget = new QWidget(parent);
  ui.setupUi(widget);
  parent->addPage(widget, i18n("General"), Applet::icon());
  connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
  connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));

  ui.radioButton_iconPlusText->setChecked(m_showIconPlusText);
  ui.radioButton_iconOnly->setChecked(m_showOnlyIcon);
  ui.radioButton_textOnly->setChecked(m_showOnlyText);
  ui.checkBox_completeName->setChecked(m_useCompleteName);
  ui.checkBox_userImage->setChecked(m_useUserImage);
}

void FastUserSwitch::checkLayout()
{
  if ((!m_labelIcon) || (!m_labelName))
    return;

  int currentHeight = geometry().height() - MARGINSIZE;

  m_layout->removeItem(m_labelIcon);
  m_layout->removeItem(m_labelName);

  m_labelIcon->setVisible(m_showOnlyIcon || m_showIconPlusText);
  m_labelName->setVisible(m_showOnlyText || m_showIconPlusText);

  if ((!m_showOnlyIcon) || m_showIconPlusText)
    m_layout->addItem(m_labelName);

  if ((!m_showOnlyText) || m_showIconPlusText) {
    QPixmap pixmap;
    if (m_useUserImage) {
      pixmap = m_userpixmap.scaledToHeight(qMin(m_userpixmap.height(),
                                           currentHeight));
    } else {
      pixmap = KIcon(DEFAULT_ICON_NAME).pixmap(qMin(IconSize(KIconLoader::Desktop),
                                                    currentHeight));
    }

    m_labelIcon->nativeWidget()->setPixmap(pixmap);
    m_labelIcon->setPreferredSize(pixmap.width(), pixmap.height());
    m_labelIcon->setMaximumSize(pixmap.width(), pixmap.height());
    m_layout->addItem(m_labelIcon);
  }

  m_labelName->setText(QString("<strong>%1</strong>").arg(currentUsername(!m_useCompleteName)));
}

void FastUserSwitch::constraintsEvent(Plasma::Constraints constraints)
{
  if (constraints & Plasma::FormFactorConstraint ||
      constraints & Plasma::SizeConstraint) {
    checkLayout();
  }
}

void FastUserSwitch::popupEvent(bool show)
{
  if (show) {
    initExtenderItem(m_extender);
  }
}

#include "fastuserswitch.moc"
