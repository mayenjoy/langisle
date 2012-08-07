#include "stdafx.h"
#include "ExportWordQA.h"

ExportWordQA::ExportWordQA(void)
{
}

ExportWordQA::~ExportWordQA(void)
{
}

void ExportWordQA::WriteFile( QString & fileName )
{
	QFile file(fileName);
	if (!file.open(QIODevice::WriteOnly))
	{
		DBG(qDebug() << "ExportWordQA::WriteFile 打开文件失败" << fileName);
		return;
	}

	QTextStream out(&file);
	out.setCodec("UTF-8");

    QList<QPair<QString, QString>  >::iterator i;
	for(i=wList.begin(); i!=wList.end(); ++i)
	{
		QPair<QString, QString> &pa = (*i);
		out << "Q " << pa.first << "\r\n" << "A " << pa.second << "\r\n\r\n";
	}


	file.close();
}
