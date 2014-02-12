#ifndef SYSTEMINSTALLER_H
#define SYSTEMINSTALLER_H

class SystemInstaller
{
public:
    SystemInstaller();
    enum SysError
    {
        None = 0,
        UserDirErr
    };

    SysError initUserDir();
};

#endif // SYSTEMINSTALLER_H
