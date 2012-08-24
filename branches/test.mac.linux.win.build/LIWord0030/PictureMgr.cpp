#include "stdafx.h"
#include "PictureMgr.h"
#include "GlobalVal.h"
#include "LIConfig.h"
#include "BCMgr.h"
PictureMgr::PictureMgr(QObject *parent)
	: QObject(parent)
{

}

PictureMgr::~PictureMgr()
{

}

QString PictureMgr::getPPath(const QString &word)
{
	QString hash = QCryptographicHash::hash(word.toLower().toUtf8(), QCryptographicHash::Md5).toHex().mid(8, 16).toLower();
	QString path = QDir::convertSeparators(m_path + "/" + word.at(0).toLower() + "/" + hash);

	if (QFile::exists(path))
	{
		return path;
	}

	return "";
}

void PictureMgr::reset()
{
	m_path = g_pbcmgr->getConfig()->getPicturePath();
}