#include "stdafx.h"
#include "LIConfig.h"
#include "GlobalVal.h"

LIConfig::LIConfig(const QString &path)

{
	
	bool bExists = QFile::exists(path);

	cfg = new QSettings(path, QSettings::IniFormat);
	cfg->setIniCodec("utf-8");
	if (!bExists)
	{
		initDefault();
	}
}

LIConfig::~LIConfig()
{
	delete cfg;
}

LISTMAPINFO LIConfig::getLMI(const QString & prefix)
{
	LISTMAPINFO result;
	int size = cfg->beginReadArray(prefix);
	for (int i = 0; i < size; i++)
	{
		cfg->setArrayIndex(i);
		QStringList keys = cfg->childKeys();
		QMap<QString, QVariant> dictInfo;
		for (int j = 0; j < keys.size(); j++)
		{
			dictInfo[keys[j]] = cfg->value(keys[j]).toString();
		}
		result << dictInfo;

	}
	cfg->endArray();
	return result;
}

void LIConfig::setLMI(const QString & prefix, LISTMAPINFO &lmi)
{
	cfg->beginWriteArray(prefix);
	for (int i = 0; i < lmi.size(); i++)
	{
		cfg->setArrayIndex(i);
		QMapIterator<QString, QVariant> iter(lmi.at(i));
		while (iter.hasNext()) 
		{
			iter.next();
			cfg->setValue(iter.key(), iter.value());
		}
	}
	cfg->endArray();
}
