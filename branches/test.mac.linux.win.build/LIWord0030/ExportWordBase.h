#pragma once
#include <QList>
#include <QPair>
#include <QString>
class ExportWordBase
{
public:
	ExportWordBase(void);
	~ExportWordBase(void);

    void AddWord(QString word, QString mean)
	{
		wList.push_back(QPair<QString, QString>(word, mean));
	}
	int count() {return wList.size();};
protected:
    QList< QPair< QString, QString> > wList;
};
