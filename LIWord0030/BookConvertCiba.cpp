#include "stdafx.h"
#include "BookConvertCiba.h"

BookConvertCiba::BookConvertCiba(void)
{
	typeName = "金山词霸单词本";
}

BookConvertCiba::~BookConvertCiba(void)
{
}

bool BookConvertCiba::Convert(BookDB *bdb, const QString &srcPath)
{
	QFile file(srcPath);
	if (!file.open(QFile::ReadOnly))
	{
		DBG(qDebug() << "BookConvertCiba::Convert 打开文件失败" << srcPath);
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
		switch (line.at(0).toAscii())
		{
		case '+':
			if (!word.isEmpty())
			{
				QMap<QString, QString> wInfo;
				wInfo["word"] = word;
				wInfo["trans"] = mean;
				wList.push_back(wInfo);
				word.clear();
				mean.clear();
			}
			word = line.mid(1);
			break;
		case '#':
			{
				if (mean.isEmpty())
					mean = line.mid(1);
				else
					mean += "\n" + line.mid(1);
			}

			break;
		}
	}
	if (!word.isEmpty())
	{
		QMap<QString, QString> wInfo;
		wInfo["word"] = word;
		wInfo["trans"] = mean;
		wList.push_back(wInfo);
		word.clear();
		mean.clear();
	}

	bdb->addEditWord(wList);

	file.close();
	return true;
}
