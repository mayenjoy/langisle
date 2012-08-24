#include "stdafx.h"
#include "ThemeMgr.h"

ThemeMgr::ThemeMgr(QObject *parent)
	: QObject(parent)
{
	reset();
}

ThemeMgr::~ThemeMgr()
{

}

QList<QVariant> ThemeMgr::getThemeList()
{
	return m_tl;
}

void ThemeMgr::reset()
{
	m_tl.clear();
	QString themePath = Global::makePathStr("/html/theme");
	QDir dir(themePath);

	QStringList sl = dir.entryList(QStringList() << "", QDir::AllDirs | QDir::NoDot | QDir::NoDotDot);

	for (int i = 0; i < sl.size(); i++)
	{
		QString tnPath = Global::makePathStr(QString("/html/theme/%1/config.ini").arg(sl.at(i)));
		QSettings cfg(tnPath, QSettings::IniFormat);
		cfg.setIniCodec("UTF-8");
// 		cfg.setValue("Main/Name", "LI׼1");
// 		cfg.setValue("Main/Author", "prehisle1");
// 		cfg.setValue("Main/Comment", "Եܼǵʱ׼1");
		TInfo ti;
 		//DBG(qDebug() << cfg.value("Main/Name", ""));
		ti["Path"] = sl.at(i) + "/" + cfg.value("Main/htmlfile", "").toString();;
		ti["Name"] = cfg.value("Main/Name", "");
		ti["ID"] = cfg.value("Main/ID", "");
		ti["Author"] = cfg.value("Main/Author", "");
		ti["Comment"] = cfg.value("Main/Comment", "");
		// ti["htmlfile"] = cfg.value("Main/htmlfile", "");
		m_tl << ti;
	}

}

QString ThemeMgr::getCurrentThemeHtmlPath(const QString &id)
{
	for (int i = 0; i < m_tl.size(); i++)
	{
		//DBG(qDebug() << "ThemeMgr::getCurrentThemeHtmlPath" << m_tl.at(i).toMap()["ID"].toString());
		if (m_tl.at(i).toMap()["ID"].toString().compare(id, Qt::CaseInsensitive) == 0)
		{
			return "/" + m_tl.at(i).toMap()["Path"].toString();
		}
	}
	return "";
}