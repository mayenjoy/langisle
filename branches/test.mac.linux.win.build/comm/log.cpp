#include "stdafx.h"
#include "log.h"
#include "global.h"

bool bLogOut = true;

void MyMsgHandler(QtMsgType msgType, const char *buf)
{

	if (bLogOut)
	{
    QString tempstr = QString("%1 - - %2\r\n").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz"))
                      .arg(QString::fromLocal8Bit(buf));
#ifdef __LOG_CONSOLE
    try
    {
        fprintf(stderr, "%s\n", tempstr.toLocal8Bit().constData());
        fflush(stderr);
    }
    catch(...){};
#endif
#ifdef __LOG_FILE
    QString logFilePath = QDir::convertSeparators(QCoreApplication::applicationFilePath() + (".log"));

    QFile logFile(logFilePath);
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Text | QFile::Append))
    {
        /*qt_message_output(QtDebugMsg, QString("%1, log file can not open. ").arg(logFilePath).toStdString().c_str());
		exit(0);*/
        return;
    }

    logFile.write(tempstr.toUtf8());

    logFile.close();
#endif
	}
}
