#include "stdafx.h"
#include "BookConvertYoudao.h"

BookConvertYoudao::BookConvertYoudao(void)
{
	typeName = "有道词典单词本";
}

BookConvertYoudao::~BookConvertYoudao(void)
{
}
bool BookConvertYoudao::Convert(BookDB *bdb, const QString &srcPath)
{
	QFile file(srcPath);
	if (!file.open(QFile::ReadOnly))
	{
		DBG(qDebug() << "CourseConvertYoudao::startConvert 打开文件失败" << srcPath);
		return false;
	}

	QXmlStreamReader xml(&file);
	QList< QMap<QString, QString> > wList;

	xml.readNextStartElement();

	if (xml.name() == "wordbook")
	{
		QMap<QString, QString> wInfo;
		while (xml.readNextStartElement())
		{

			if (xml.name() == "item")
			{
				while (xml.readNextStartElement())
				{
					wInfo[xml.name().toString()] = xml.readElementText();
				}
			}
			wList.push_back(wInfo);
		}
	}

	bdb->addEditWord(wList);
	file.close();
	return true;
}
