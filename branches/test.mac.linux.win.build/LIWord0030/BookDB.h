#pragma once
#include "../comm/LISqlite.h"

class BookDB :
	public LISqlite
{
public:
	BookDB(const QString &dbpath, const QString &connName);
	virtual ~BookDB(void);

	void createTable();
	void loadDictDB();
	void unloadDictDB();

	void addEditWord(const QString &word, const QString &mean);
	void replaceWord(const QString &word, const QString &mean);
	bool isWordExist(const QString &word);
	void addEditWord(QList< QMap<QString, QString> > &wList);

	int wordCount();
	void delWord(int bwid);
	void delAllWord();

	int getWidByDict(const QString &word);
};
 
