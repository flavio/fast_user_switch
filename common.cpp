#include "common.h"

#include <QtCore/QFile>

#include <kicon.h>
#include <kiconloader.h>
#include <kuser.h>

const QString getUsername(bool useShortName, const KUser& user)
{
  if (useShortName)
    return user.loginName();

  QString username = user.property(KUser::FullName).toString();
  if (username.isEmpty())
    username = user.loginName();

  return username;
}

QPixmap getUserIcon(const KUser& user)
{
  QPixmap pixmap;
  int iconSize = IconSize(KIconLoader::Desktop);
  if (QFile::exists(user.faceIconPath())) {
    pixmap.load(user.faceIconPath());
  } else
    pixmap = KIcon(DEFAULT_ICON_NAME).pixmap(iconSize);

  return pixmap;
}
