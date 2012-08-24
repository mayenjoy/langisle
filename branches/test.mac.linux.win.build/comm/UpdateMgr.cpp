#include "stdafx.h"
#include "UpdateMgr.h"
#include "config.h"
UpdateMgr::UpdateMgr(QObject *parent)
    : QObject(parent)
{
    connect(&downer, SIGNAL(downloaded(int)),
            SLOT(downloaded(int)));
    connect(&downer, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(downloadProgress(qint64,qint64)));
}

UpdateMgr::~UpdateMgr()
{

}

void UpdateMgr::checkNewVer()
{
    downer.download(QString(NEWVER_CHECK_URL), Global::makePathStr(VERINFOFILE));
}

void UpdateMgr::downloaded(int error)
{
    qDebug() << "UpdateMgr::downloaded" << error;

    if (error == 0)
    {
#if defined(Q_WS_WIN)
        parse(Global::makePathStr("/verinfo.xml"));
#endif
#if defined(Q_WS_X11)
        parse(Global::makePathStr("/verinfo_linux.xml"));
#endif



        QStringList sl = vernumber.split(".");
        if (sl.size() < 4)
        {
            //			qDebug() << "版本号不正确" << vernumber;
            return;
        }

        if (sl.at(0).toInt() > QString(VER_1).toInt() ||
                sl.at(1).toInt() > QString(VER_2).toInt() ||
                sl.at(2).toInt() > QString(VER_3).toInt() ||
                sl.at(3).toInt() > QString(VER_4).toInt())
        {
            //			qDebug() << "需要更新";
            resultInfo = "需要更新";
            emit checkResult(true);
            return;
            //applyNewVer();
        }
    }
    else
    {
        qDebug() << "UpdateMgr::downloaded faild";


        resultInfo = downer.getErrorInfo();
        emit checkResult(false);
        return;
    }

    resultInfo = "您已经是最新版本了.";
    emit checkResult(false);
}

void UpdateMgr::downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{

}

void UpdateMgr::parse(QString verFile)
{
    QFile file(verFile);
    if (!file.open(QFile::ReadOnly))
    {
        qDebug() << "UpdateMgr::parse error 1" << verFile;
        return;
    }

    QXmlStreamReader  xml(&file);
    xml.readNextStartElement();
    while (xml.readNextStartElement()) {
        if (xml.name() == "vernumber")
            vernumber = xml.readElementText();
        else if (xml.name() == "downurl")
            downurl = xml.readElementText();
        else if (xml.name() == "updateNotes")
            updateNotes = xml.readElementText();
        else
            xml.skipCurrentElement();
    }


    file.close();

    qDebug() << vernumber << downurl << updateNotes;
}

bool UpdateMgr::applyNewVer()
{
    QString updateExePath = Global::makePathStr("/" UPDATE_EXE);
    if (!QFile::exists(updateExePath))
    {
        DBG(qDebug() << "UpdateMgr::applyNewVer" << updateExePath << "不存在");
        return false;
    }

    Global::createRelExeDir(TEMP_DIR);

    QString liupdate = Global::makePathStr("/T" UPDATE_EXE);
    if (QFile::exists(liupdate))
    {
        QFile::setPermissions(liupdate, QFile::WriteOwner);
        if (!QFile::remove(liupdate))	// 删除临时目录里原来的
        {
            DBG(qDebug() << "UpdateMgr::applyNewVer remove" << liupdate << "faild");
            return false;
        }
    }

    if (!QFile::copy(updateExePath, liupdate))	// 复制到临时目录
    {
        DBG(qDebug() << "UpdateMgr::applyNewVer 2" << updateExePath << "复制文件失败" << liupdate);
        return false;
    }

    //QProcess upp;
    //upp.setWorkingDirectory(Global::makePathStr(""));
    //upp.start(liupdate, QStringList() << APPNAME << EXENAME << Global::makePathStr("") << downurl);
    //upp.waitForStarted();

    //qDebug() << liupdate;

    QProcess::startDetached(liupdate, QStringList() << APPNAME << EXENAME << Global::makePathStr("") << downurl << vernumber, Global::makePathStr(""));
    return true;
}
