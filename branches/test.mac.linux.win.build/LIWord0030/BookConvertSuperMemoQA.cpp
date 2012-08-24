#include "stdafx.h"
#include "BookConvertSuperMemoQA.h"

BookConvertSuperMemoQA::BookConvertSuperMemoQA(void)
{
	typeName = "SuperMemoQ&A词库";
}

BookConvertSuperMemoQA::~BookConvertSuperMemoQA(void)
{
}

bool BookConvertSuperMemoQA::Convert(BookDB *bdb, const QString &srcPath)
{

	QFile file(srcPath);
	if (!file.open(QFile::ReadOnly))
	{
		DBG(qDebug() << "BookConvertSuperMemoQA::Convert 打开文件失败" << srcPath);
		return 0;
	}

	QString word;
	QString mean;

	QList< QMap<QString, QString> > wList;

	QTextStream in(&file);
	in.setCodec("UTF-8");

	while (!in.atEnd()) 
	{
		QString line = in.readLine();
		if (line.isEmpty())
			continue;
		switch (line.at(0).toAscii())
		{
		case 'Q':
			word = line.mid(2);
			break;
		case 'A':
			{
					mean = line.mid(2);
					QMap<QString, QString> wInfo;
					wInfo["word"] = word.trimmed();
					wInfo["trans"] = mean.trimmed();
					wList.push_back(wInfo);
			}

			break;
		}
	}


	bdb->addEditWord(wList);

	file.close();
	return true;
}
