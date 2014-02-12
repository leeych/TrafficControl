#include "systeminstaller.h"
#include "mutility.h"
#include <QString>
#include <QDir>

SystemInstaller::SystemInstaller()
{
}

SystemInstaller::SysError SystemInstaller::initUserDir()
{
    QString str;
    MUtility::getUserDir(str);
    SysError res = None;
    QDir file_dir;
    bool status = file_dir.exists(str);
    if (!status)
    {
        if (!file_dir.mkdir(str))
        {
            res = UserDirErr;
        }
    }
    return res;
}
