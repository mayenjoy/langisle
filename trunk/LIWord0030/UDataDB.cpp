#include "stdafx.h"
#include "UDataDB.h"
#include "BCMgr.h"
#include "GlobalVal.h"
#include "PictureMgr.h"
#include <QStringList>
UDataDB::UDataDB(const QString &dbpath, const QString &connName) : LISqlite(dbpath, connName)
{
	if (!isTableExist("mdata"))	// 新建的数据库,创建表
	{
		createTable();
		setConfigInfo("defaultFP", "30,120,300,1800,43200,86400,172800,345600,604800,1296000"); // 默认遗忘点
		//setConfigInfo("defaultFP", "12,30"); 
		// DBG(qDebug() << getConfigInfo("defaultFP"));
	}

	initFP();

	QSqlQuery query(*db);
	if (!query.exec(QString("ATTACH DATABASE '%1' as dict;").arg(Global::makePathStr("/odata/dict.db"))))
	{
		DBG(qDebug() << "UDataDB::UDataDB 1 " << query.lastError().text());
	}

	// DBG(qDebug() << rexec("select mean from word where word = 'hello'").toString());

	if (isTableExist("studyData.mdata"))	// 新建的数据库,创建表
	{
		DBG(qDebug() << "UDataDB::UDataDB t2" << "studyData.mdata存在, 上次程序非正常关闭了");
		if (!query.exec("DROP TABLE  [studyData.mdata]"))
		{
			DBG(qDebug() << "UDataDB::UDataDB t1 " << query.lastError().text());
		}
		if (!query.exec("DROP TABLE  [studyData.dslog]"))
		{
			DBG(qDebug() << "UDataDB::UDataDB t2 " << query.lastError().text());
		}
	}
	if (!query.exec("ATTACH DATABASE ':memory:' as studyData;"))
	{
		DBG(qDebug() << "UDataDB::UDataDB 2 " << query.lastError().text());
	}

	if (!isTableExist("tlog"))
	{
		createTLogTable();
	}

	createMemoryMDataTable();


}

UDataDB::~UDataDB(void)
{
	QSqlQuery query(*db);
	if (!query.exec("DROP TABLE  studyData.mdata"))
	{
		DBG(qDebug() << "UDataDB::~UDataDB 1 " << query.lastError().text());
	}

	if (!query.exec("DETACH DATABASE studyData;"))
	{
			DBG(qDebug() << "UDataDB::~UDataDB 2 " << query.lastError().text());
	}

	if (!query.exec("DETACH DATABASE dict;"))
	{
		DBG(qDebug() << "UDataDB::~UDataDB 3" << query.lastError().text());
	}
	
}

void UDataDB::createTLogTable()
{
	db->transaction();

	exec("CREATE TABLE [tlog] ( \
		[tlid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[begintime] DATETIME, \
		[endtime] DATETIME, \
		[stime] INTEGER, \
		[swcount] INTEGER, \
		[snwcount] INTEGER, \
		[rscount] INTEGER, \
		[kncount] INTEGER, \
		[sspeed] INTEGER, \
		[stwcount] INTEGER, \
		[stnwcount] INTEGER);");

	exec("CREATE INDEX [begintimeIndex] ON [tlog] ([begintime]);");


	if (!db->commit())
	{
		DBG(qDebug() << "UDataDB::createTLogTable faild." << db->lastError().text());
	}
}

void UDataDB::loadDict(int dbname)
{
		QSqlQuery query(*db);
		if (!query.exec(QString("ATTACH DATABASE '%1' as db%2;").arg(Global::makePathStr(QString("/udata/%1/book/%2.db").arg(g_pbcmgr->getCurUserName()).arg(dbname))).arg(dbname)))
		{
			DBG(qDebug() << "UDataDB::loadDict" << query.lastError().text());
		}

}


void UDataDB::createMemoryMDataTable()
{
	
	db->transaction();
	// 考虑到同时学习多个课程,需要取原始词库中的mean,所以加入一mean,gid2个字段
	// 如果mdata中还需要加入其它字段,必须加在mean字段前, 
	// 同时syncMData, getWordInfoByMid中要增加相应的字段的操作
	// 
	exec("CREATE TABLE studyData.mdata (\
		[mid] INTEGER, \
		[wtype] INTEGER, \
		[wid] INTEGER, \
		[sc] INTEGER, \
		[st] INTEGER, \
		[lst] DATETIME, \
		[score] INTERGER, \
		[mean] TEXT, \
		[gid] INTEGER, \
		[selstatus] INTERGER);");

	exec("CREATE UNIQUE INDEX studyData.index_mid_unique ON [mdata] ([mid]);");

	exec("CREATE TABLE studyData.grp ([gid] INTEGER, [gname] TEXT);");
	exec("CREATE UNIQUE INDEX studyData.gidUnique ON [grp] ([gid]);");


	exec("CREATE TABLE studyData.dslog (\
		 [dsid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		 [mid] INTEGER, \
		 [stime] DATETIME, \
		 [itime] DATETIME, \
		 [judgetime] INTEGER, \
		 [affirmtime] INTEGER, \
		 [studytime] INTEGER, \
		 [selJudge] INTEGER, \
		 [selAffirm] INTEGER);");
	if (!db->commit())
	{
		DBG(qDebug() << "UDataDB::createMemoryMDataTable faild." << db->lastError().text());
	}
}



void UDataDB::createTable()
{
	db->transaction();

	exec("CREATE TABLE [mdata] (\
		[mid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[wtype] INTEGER, \
		[wid] INTEGER, \
		[sc] INTEGER, \
		[st] INTEGER, \
		[lst] DATETIME, \
		[score] INTERGER);");

	exec("CREATE UNIQUE INDEX [index_wtype_wid_unique] ON [mdata] ([wtype], [wid]);");


	exec("CREATE TABLE [uword] (\
		[wid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[word] TEXT);");
	exec("CREATE UNIQUE INDEX [index_uword_word_unique] ON [uword] ([word]);");

	exec("CREATE TABLE [config] (\
		 [name] TEXT, \
		 [value] TEXT);");
	exec("CREATE UNIQUE INDEX [index_config_name_unique] ON [config] ([name]);");


	exec("CREATE TABLE [dslog] (\
		[dsid] INTEGER PRIMARY KEY AUTOINCREMENT, \
		[mid] INTEGER, \
		[stime] DATETIME, \
		[itime] DATETIME, \
		[judgetime] INTEGER, \
		[affirmtime] INTEGER, \
		[studytime] INTEGER, \
		[selJudge] INTEGER, \
		[selAffirm] INTEGER);");

	if (!db->commit())
	{
		DBG(qDebug() << "UDataDB::createTable faild." << db->lastError().text());
	}


}

void UDataDB::setConfigInfo(const QString &name, const QString &value)
{
	exec(QString("replace into config(name, value) values('%1', '%2')").arg(name).arg(value));
}

QString UDataDB::getConfigInfo(const QString &name)
{
	return rexec(QString("select value from config where name = '%1'").arg(name)).toString();
}

void UDataDB::createBookTable(const QString &tableName)
{
	db->transaction();
	db->exec(QString("CREATE TABLE [%1] ( \
					 [bwid] INTEGER, \
					 [mid] INTEGER, \
					 [mean] TEXT, \
					 [gid] INTEGER);").arg(tableName));
	db->exec(QString("CREATE UNIQUE INDEX [unique_%1] ON [%2] ([bwid]);").arg(tableName).arg(tableName));	
	if (!db->commit())
	{
		DBG(qDebug() << "UDataDB::createBookTable" << db->lastError());
	}
}

void UDataDB::CreateGroupTable(const QString &tableName)
{

	db->transaction();
	db->exec(QString("	CREATE TABLE [%1] (\
		[gid] INTEGER, \
		[gname] TEXT);").arg(tableName));
	db->exec(QString("CREATE UNIQUE INDEX [gidUnique] ON [%1] ([gid]);").arg(tableName));	
	db->commit();
}

void UDataDB::importToUWord()
{
	QSqlQuery query(*db);
	if (!query.exec("select * from tbdb.word where wid is null"))
	{
		DBG(qDebug() << "UDataDB::importToUWord 失败1" << query.lastError().text());
		return;
	}

	if (!query.next())
		return;

	int bwidNo = query.record().indexOf("bwid");
	int widNo = query.record().indexOf("wid");
	int wordNo = query.record().indexOf("word");
	int meanNo = query.record().indexOf("mean");
	int gidNo = query.record().indexOf("gid");

	db->transaction();

	int bwid;
	int wid;
	QString word;
	QString mean;
	int gid;
	int mid;
	do
	{
		bwid = query.value(bwidNo).toInt();
		wid = query.value(widNo).toInt();
		word = query.value(wordNo).toString();
		mean = query.value(meanNo).toString();
		gid = query.value(gidNo).toInt();

		// DBG(qDebug() << bwid << wid << word << mean << gid);
		
		if (word.isEmpty()) continue;

		if (wid == 0)
		{
			QSqlQuery query(*db);
			query.prepare("insert into uword(word) values(?)");
			query.bindValue(0, word);
			if (!query.exec())
			{
				//QSqlQuery query(*db);
				//query.prepare("select wid from uword where word = ?");
				//query.addBindValue(word);
				//query.exec();
				//wid = query.value(0).toInt();
				//qDebug() << query.lastError().text() << lastInsertID() << word << wid;
			}
		}
		else
		{

		}
	}while (query.next());
	if (!db->commit())
	{
		DBG(qDebug() << "UDataDB::importToUWord 2 出错");
		return;
	}
}

int UDataDB::importToMData(wordType wtype, int wid)
{
	int mid;
	QSqlQuery query(*db);
	query.prepare("insert into mdata(wtype, wid) values(?, ?)");
	query.addBindValue(wtype);
	query.addBindValue(wid);
	if (!query.exec())
	{
		//DBG(qDebug() << "插入失败2" << query.lastError().text() << query.executedQuery() << wtype << wid);
		mid = rexec(QString("select mid from mdata where wtype = %1 and wid = %2").arg(wtype).arg(wid)).toInt();
	}
	else
		mid = query.lastInsertId().toInt();
	return mid;
}

void UDataDB::importBookData(const QString &tableName, const QString &bookDBPath)
{
	QSqlQuery q1(QString("ATTACH DATABASE '%1' as tbdb;").arg(bookDBPath), *db);

	if (q1.isValid())
	{
		DBG(qDebug() << "UDataDB::importBookData 失败1" << tableName << bookDBPath << db->lastError().text());
		return;
	}

	importToUWord();
	
	QSqlQuery query(*db);
	if (!query.exec("select * from tbdb.word"))
	{
		DBG(qDebug() << "UDataDB::importBookData 失败2" << query.lastError().text());
		return;
	}

	int bwidNo = query.record().indexOf("bwid");
	int widNo = query.record().indexOf("wid");
	int wordNo = query.record().indexOf("word");
	int meanNo = query.record().indexOf("mean");
	int gidNo = query.record().indexOf("gid");

	int bwid;
	int wid;
	QString word;
	QString mean;
	int gid;
	int mid;

	db->transaction();
	while (query.next())
	{
		bwid = query.value(bwidNo).toInt();
		wid = query.value(widNo).toInt();
		word = query.value(wordNo).toString();
		mean = query.value(meanNo).toString();
		gid = query.value(gidNo).toInt();
		// DBG(qDebug() << bwid << wid << word << mean << gid);

		
		if (wid == 0)
		{
			if (word.isEmpty()) continue;
			QSqlQuery query(*db);
			query.prepare("select wid from uword where word = ?");
			query.addBindValue(word);
			if (!query.exec())
			{
				DBG(qDebug() << "UDataDB::importBookData 失败3" << word);
				return;
			}

			query.next();

			wid = query.value(0).toInt();
			//wid = rexec(QString("select wid from uword where word = '%1'").arg(word)).toInt();

			if (wid == 0)
			{
				DBG(qDebug() << "UDataDB::importBookData 失败4" << bwid << wid << word << mean << gid);
				return ;
			}

			mid = importToMData(eUWord, wid);	

		}
		else
		{
			mid = importToMData(eOWord, wid);
		}

		QSqlQuery query1(*db);
		query1.prepare(QString("replace into %1(bwid, mid, mean, gid) values(?, ?, ?, ?)").arg(tableName));
		query1.addBindValue(bwid);
		query1.addBindValue(mid);
		query1.addBindValue(mean);
		query1.addBindValue(gid ? gid :  QVariant(QVariant::Int));
		if (!query1.exec())
		{
			//QMapIterator<QString, QVariant> i(query1.boundValues());
			//while (i.hasNext()) 
			//{
			//	i.next();
			//	qDebug() << i.key().toAscii().data() << ": "
			//		<< i.value().toString().toAscii().data() << endl;
			//}
			DBG(qDebug() << "UDataDB::importBookData 失败5" << query1.lastError().text() << gid);

			return;
		}
	}
	QString bookGrp = tableName + "grp";
	CreateGroupTable(bookGrp);
	QSqlQuery queryGroup(*db);
	queryGroup.exec(QString("replace into  %1 select * from tbdb.grp").arg(bookGrp));
	db->commit();
	exec("DETACH DATABASE tbdb;");
}

void UDataDB::importStudyInfo(QList<BOOKINFO> &bil)
{

	foreach (BOOKINFO bookInfo, bil)
	{		
		importStudyInfo(bookInfo);

	}
}
int UDataDB::getWidByDictWord(const QString &word)
{
	QSqlQuery query(*db);
	query.prepare("select wid from dict.word where word = :word  COLLATE NOCASE");
	query.bindValue(":word", word);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getMidByDictWord faild." << query.lastError());
		return 0;
	}
	if (!query.next())
		return 0;

	return query.record().value(0).toInt();
}
int UDataDB::getMidByDictWord(const QString &word)
{
	QSqlQuery query(*db);

	int wid = getWidByDictWord(word);
	query.prepare(QString("select mid from mdata where wtype = 2 and wid = %1").arg(wid));
	query.exec();
	query.next();

	return query.record().value(0).toInt();

}

int UDataDB::getMidByUWord(const QString &word)
{
	QSqlQuery query(*db);
	query.prepare("select wid from uword where word = :word");
	query.bindValue(":word", word);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getMidByUWord faild." << query.lastError());
		return 0;
	}
	if (!query.next())
		return 0;

	int wid = query.record().value(0).toInt();
	query.prepare(QString("select mid from mdata where wtype = 1 and wid = %1").arg(wid));
	query.exec();
	query.next();

	return query.record().value(0).toInt();
}

void UDataDB::updateWordMInfo2(WordInfo &winfo)
{
	QSqlQuery query(*db);
	query.prepare("update mdata set sc = :sc, st = :st, lst = :lst where mid = :mid");
	//DBG(qDebug() << "UDataDB::updateWordMInfo2" << winfo);
	query.bindValue(":sc", winfo["sc"]);
	query.bindValue(":st",  winfo["st"]);
	query.bindValue(":lst", winfo["lst"]);
	query.bindValue(":mid", winfo["mid"]);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateWordMInfo2 faild.");
	}
}

void UDataDB::updateMDataFrom02XSData(QList<WordInfo> &wil)
{
	db->transaction();
	foreach (WordInfo wi, wil)
	{
		QString word = wi["word"].toString();
		int mid = getMidByDictWord(word);
		if (mid == 0)
		{
			mid = getMidByUWord(word);
		}
// 		wi["word"] = query.record().value("word");
// 		wi["state"] = query.record().value("state");
// 		wi["fp"] = query.record().value("fp");
// 		wi["lasttime"] = query.record().value("lasttime");
		int state = wi["state"].toInt();
		if (state == 2) // 掌握
		{
			wi["st"] = maxfp;
		}

		wi["mid"] = mid;
		updateWordMInfo2(wi);
	}
	db->commit();
}

void UDataDB::importStudyInfo(BOOKINFO &bi)
{
	bookType bt = (bookType)bi["btype"].toInt();
	int bid = bi["bid"].toInt();
	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
	QStringList tables = db->tables(QSql::Tables);
	if (!tables.contains(tableName))
	{
		createBookTable(tableName);
	}

	QString dbpath = g_pbcmgr->getBookPath(bt, bid);

	importBookData(tableName, dbpath);

	insertToMyBook(bi, tableName); // 插入到我的课本中
}

void UDataDB::init()
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	DBG(qDebug() << "UDataDB::init 1");

	m_biList = g_pbcmgr->getSelectBook();
	QStringList tables = db->tables(QSql::Tables);

	foreach (BOOKINFO bookInfo, m_biList)
	{		
		bookType bt = (bookType)bookInfo["btype"].toInt();
		int bid = bookInfo["bid"].toInt();
		QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);

		DBG(qDebug() << "UDataDB::init 2" << tableName);
		if (!tables.contains(tableName))
		{
			createBookTable(tableName);

			QString dbpath = g_pbcmgr->getBookPath(bt, bid);

			importBookData(tableName, dbpath);

			insertToMyBook(bookInfo, tableName); // 插入到我的课本中
		}

		// 导入本次学习数据
		insertCurrentStudyData(tableName);
		// 导入分类信息
		insertCurrentGroupData(tableName + "grp");
		
	}

	m_lstMax = getMaxLst();
	DBG(qDebug() << "UDataDB::init 最后学习时间" << m_lstMax);
	DBG(qDebug() << "耗时" << start.msecsTo(QDateTime::currentDateTime()));
	
}

void UDataDB::insertCurrentGroupData(const QString &groupTableName)
{
	if (isTableExist(groupTableName))
	{
		QSqlQuery query(*db);
		QString sql = QString("insert or ignore into studyData.grp select * from %1").arg(groupTableName);
		if (!query.exec(sql))
		{
			DBG(qDebug() << "UDataDB::insertCurrentGroupData faild." << sql << query.lastError().text());
		}
	}
}

void UDataDB::insertCurrentStudyData(const QString &bookTableName)
{
	//replace
	QSqlQuery query(*db);
	QString sql = QString("insert or ignore into studyData.mdata select mdata.*, %4.mean, %5.gid, 0 from mdata inner join %1 on mdata.mid = %2.mid order by %3.bwid").arg(bookTableName).arg(bookTableName).arg(bookTableName).arg(bookTableName).arg(bookTableName);
	if (!query.exec(sql))
	{
		DBG(qDebug() << "UDataDB::insertCurrentStudyData faild." << sql << query.lastError().text());
	}
}

WordInfo UDataDB::getWordInfoByMid(int mid)
{
	QSqlQuery query(*db);
	QString sql = QString("select * from studyData.mdata where mid = %1").arg(mid);	
	if (!query.exec(sql))
	{
		DBG(qDebug() << "UDataDB::getWordInfoByMid faild." << getStudyDataWordCount() << sql << query.lastError().text());
		return WordInfo();
	}

	if (!query.next())
	{
		DBG(qDebug() << "UDataDB::getWordInfoByMid 2" << "未获取对应mid的单词信息" << mid);
		return WordInfo();
	}

	WordInfo wi;

	QSqlRecord sr = query.record();
	int type = sr.value("wtype").toInt();
	int wid = sr.value("wid").toInt();

	wi["mid"] = mid;	// 词根
	wi["sc"] = sr.value("sc");
	wi["st"] = sr.value("st");
	wi["lst"] = sr.value("lst");
	wi["score"] = sr.value("score");
	

	wi["mean"] = sr.value("mean");//.toString().replace("\n", "<br />");

	if (!sr.value("gid").isNull()) // 只有学习单个课本时显示分组信息
	{
		// wi["gname"] = sr.value("gid");
		int gid = sr.value("gid").toInt();
		wi["gname"] = rexec(QString("select gname from studyData.grp where gid = %1").arg(gid)).toString();
		// DBG(qDebug() << "学习单个课程" << "gid=" << gid << rexec(QString("select gname from studyData.grp where gid = %1").arg(gid)).toString());

	}
	else
	{
		wi["gname"] = "";
	}
	


	if (eUWord == type)
	{
		QSqlQuery quword(QString("select * from uword where wid = %1").arg(wid), *db);
		if (!quword.next())
		{
			DBG(qDebug() << "UDataDB::getWordInfoByMid 3" << "未获取对应wid的单词信息" << wid);
			return WordInfo();
		}
		wi["word"] = quword.value(1);
		wi["pron"] = "";
		wi["dmean"] = "";
		wi["mcode"] = "";
		wi["emean"] = "";
		wi["thesaurus"] = "";
		wi["syllable"] = "";
		wi["suppletion"] = "";
		wi["sens"] = "";
	}
	else // eOWord
	{
		QSqlQuery qoword(QString("select * from dict.word where wid = %1").arg(wid), *db);
		if (!qoword.next())
		{
			DBG(qDebug() << "UDataDB::getWordInfoByMid 4" << "未获取对应wid的单词信息" << wid);
			return WordInfo();
		}
		wi["word"] = qoword.value(1).toString();
		wi["pron"] = qoword.value(2).toString();
		wi["dmean"] = qoword.value(3).toString().replace("\n", "<br />");
		wi["mcode"] = qoword.value(4).toString();
		wi["emean"] = qoword.value(5).toString();
		DBG(qDebug() << "UDataDB::getWordInfoByMid 5" << wi["emean"] << wid);
		wi["thesaurus"] = qoword.value(6).toString(); // 同义词
		wi["syllable"] = qoword.value(7).toString();	// 音节
		wi["suppletion"] = qoword.value(8).toString();	// 词性变化
		wi["sens"] = getSens(wid);	// 例句

	}

	
	// 图片
	wi["wimg"] = GlobalVal::s_pPm->getPPath(wi["word"].toString());

	DBG(qDebug() << wi["wimg"]);

	return wi;
}

QString UDataDB::getSens(int wid)
{

	QSqlQuery query(*db);
	query.exec(QString("select * from sens where wid = %1 limit 4").arg(wid));

	QStringList sRetList;
	int i = 0;
	while (query.next())
	{
		i++;
		sRetList << QString("<br />%1.<span class='snese'>").arg(i) + query.record().value("sen").toString() + "</span>&speakico<br />" + "<span class='snesc'>" + query.record().value("tran").toString() + "</span>";
	}

	return sRetList.join("\n\n");

	
}

void UDataDB::uninit()
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	syncMData();
	synDSLog();
	DBG(qDebug() << "LearnMgr::~uninit 耗时1" << start.msecsTo(QDateTime::currentDateTime()));
	QSqlQuery query(*db);
	if (!query.exec("delete from studyData.mdata"))
	{
		DBG(qDebug() << "UDataDB::uninit faild." << query.lastError().text());
	}
	if (!query.exec("delete from studyData.dslog"))
	{
		DBG(qDebug() << "UDataDB::uninit faild. 2" << query.lastError().text());
	}
	DBG(qDebug() << "LearnMgr::~uninit 耗时2" << start.msecsTo(QDateTime::currentDateTime()));
	updateStudyProgress();
	DBG(qDebug() << "LearnMgr::~uninit 耗时3" << start.msecsTo(QDateTime::currentDateTime()));
}

QList<BOOKINFO> UDataDB::getAllStudyBookInfo()
{
	QList<BOOKINFO> result;

	QSqlQuery query(*db);
	query.prepare("select * from mybook");
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getAllStudyBookInfo faild.");
	}

	while (query.next())
	{
		BOOKINFO bi;
		bi["btype"] = query.record().value("btype");
		bi["bid"] = query.record().value("bid");
		bi["name"] = query.record().value("name");
		bi["wcount"] = query.record().value("wcount");
		//DBG(qDebug() << "UDataDB::getAllStudyBookInfo" << query.record().value("btype").toInt() << query.record().value("name").toString());
		result.append(bi);
	}


	return result;
}

void UDataDB::updateStudyProgress()
{
	db->transaction();
	QList<BOOKINFO> bil = getAllStudyBookInfo();
	foreach (BOOKINFO bookInfo, bil)
	{		
		bookType bt = (bookType)bookInfo["btype"].toInt();
		int bid = bookInfo["bid"].toInt();

		updateBookStudyProgress(bid, bt);
// 		QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
// 
// 		EnumMyCategory emc = getBookCid(tableName, maxfp);
// 
// 		updateMBCid(bid, (int)emc);
// 		updateSCount(bid, getSCount(tableName));
// 
// 		updateKNCount(bid, getKNCount(tableName, maxfp));
// 
// 		updateProgress(bid, getProgress(tableName, maxfp));


	}
	db->commit();
}

void UDataDB::updateBookStudyProgress(int bid, bookType bt)
{
	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);

	EnumMyCategory emc = getBookCid(tableName, maxfp);

	int SCount = getSCount(tableName);
	int KNCount = getKNCount(tableName, maxfp);
	float progress =  getProgress(tableName, maxfp);

	
	updateMBCid(bid, bt, (int)emc);
	updateSCount(bid, bt, SCount);

	updateKNCount(bid, bt, KNCount);

	updateProgress(bid, bt, progress);
	
	
}

QString UDataDB::getCurrentBookInfo()
{
// 	QStringList bInfoL;
// 	bool bMuch = false;
// 	for (int i = 0; i <  m_biList.size(); i++)//BOOKINFO bookInfo, m_biList)
// 	{
// 		if (i < 1 || i > m_biList.size() - 2)
// 			bInfoL << m_biList.at(i)["name"].toString();
// 		else if (!bMuch)
// 		{
// 			bInfoL << "……";
// 			bMuch = true;
// 		}
// 	}
// 	return bMuch ? bInfoL.join("、") + QString("等%1个词库").arg(m_biList.size()) :  bInfoL.join(",");

	return m_biList.size() > 1 ? m_biList.at(0)["name"].toString() + QString("等%1个词库").arg(m_biList.size()) : m_biList.at(0)["name"].toString();

}
/*
-1 随机模式取词sql语句执行失败
-2 顺序模式取词sql语句执行失败
-3 没有新单词可以学习
-4 取复习单词sql语句执行失败
-5 没有需要复习的单词
-6 getGreaterThanStCount的sql语句执行失败
-7 所有单词都超过了最大遗忘点最大间隔
-8 getLEThanStCount的sql语句执行失败
*/
int UDataDB::getNotLearnMID(EnumGWOrder GWRandom)
{
	QSqlQuery query(*db);
	if (GWRandom == EGWbac)
	{
		if (!query.exec("select mid from studyData.mdata where lst is null order by random() limit 1"))
		{
			DBG(qDebug() << "UDataDB::getNotLearnMID faild. 1" << query.lastError().text());
			return -1;
		}

	}
	else if (GWRandom == EGWabc)
	{
		if (!query.exec("select mid from studyData.mdata where lst is null limit 1"))
		{
			DBG(qDebug() << "UDataDB::getNotLearnMID faild. 2" << query.lastError().text());
			return -2;
		}
	}
	else if (GWRandom == EGWcba)
	{
		if (!query.exec("select mid from studyData.mdata where lst is null order by mid desc limit 1"))
		{
			DBG(qDebug() << "UDataDB::getNotLearnMID faild. 2" << query.lastError().text());
			return -2;
		}
	}

	if (!query.next())
	{
		DBG(qDebug() << "没有新单词可以学习");
		return -3;		
	}
	return query.record().value("mid").toInt();
}

int UDataDB::getGreaterThanStCount(int st)
{
	QSqlQuery query(*db);
	QString sql = QString("SELECT count(mid) from studyData.mdata where st > %1").arg(st);
	if (!query.exec(sql))
	{
		DBG(qDebug() << "UDataDB::getGreaterThanStCount faild ." << query.lastError().text());
		return -6;
	}

	query.next();

	return query.record().value(0).toInt();
}

int UDataDB::getLEThanStCount(int st)
{
	QSqlQuery query(*db);
	QString sql = QString("SELECT count(mid) from studyData.mdata where st < %1 or st is null").arg(st);
	//DBG(qDebug() << "UDataDB::getLEThanStCount" << sql << st);
	if (!query.exec(sql))
	{
		DBG(qDebug() << "UDataDB::getLEThanStCount faild ." << query.lastError().text());
		return -8;
	}

	query.next();

	return query.record().value(0).toInt();
}

int UDataDB::getWantReviewMID()
{

	QSqlQuery query(*db);
	if (!query.exec(QString("SELECT mid from studyData.mdata where (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < %1 order by st, lst LIMIT 1").arg(maxfp)))
	{
		DBG(qDebug() << "UDataDB::getWantReviewMID faild ." << query.lastError().text());
		return -4;
	}

	if (!query.next())	{
		
		return -5;		
	}
	return query.record().value("mid").toInt();
}
int UDataDB::getWantReviewMID(EnumREVWOrder RVWOrder)
{
	
	QString odby = RVWOrder == RVWabc ? "lst" : "random()";
	DBG(qDebug() << "UDataDB::getWantReviewMID" << odby);
	QSqlQuery query(*db);
	if (!query.exec(QString("SELECT mid from studyData.mdata where (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < %1 order by st, %2 LIMIT 1").arg(maxfp).arg(odby)))
	{
		DBG(qDebug() << "UDataDB::getWantReviewMID1 faild ." << query.lastError().text());
		return -4;
	}

	if (!query.next())	{

		return -5;		
	}
	return query.record().value(0).toInt();
}
int UDataDB::getStudyDataWordCount()
{
	return rexec("select count(*) from studyData.mdata").toInt();
}



int UDataDB::getTodaySWCount()	// 当天学习单词数
{
	return rexec("select sum(swcount) from tlog where date('now', 'localtime') = date(begintime)").toInt();	
}
int UDataDB::getTodaySNWCount() 	// 当天新学单词数
{
	return rexec("select sum(snwcount) from tlog where date('now', 'localtime') = date(begintime)").toInt();	
}
int UDataDB::getTodayRSCount()	// 当天复习单词数
{
	return rexec("select sum(rscount) from tlog where date('now', 'localtime') = date(begintime)").toInt();	
}
int UDataDB::getTodayKNCount()	// 当天掌握单词数
{	
	return rexec("select sum(kncount) from tlog where date('now', 'localtime') = date(begintime)").toInt();	
}
int UDataDB::getTodaySTime()	// 当天学习时间
{
	return rexec("select sum(stime) from tlog where date('now', 'localtime') = date(begintime)").toInt();	
}

void UDataDB::synDSLog()
{
	QSqlQuery query(*db);
	query.prepare("insert into dslog select null, mid, stime, itime, judgetime, affirmtime, studytime, selJudge, selAffirm from studyData.dslog");

	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::synDSLog faild." << query.lastError().text());
	}
}
void UDataDB::syncMData()
{
	/*
	[mid] INTEGER PRIMARY KEY AUTOINCREMENT, \
	[wtype] INTEGER, \
	[wid] INTEGER, \
	[sc] INTEGER, \
	[jtc] INTEGER, \
	[stc] INTEGER, \
	[st] INTEGER, \
	[lst] DATETIME);");
	*/
	QSqlQuery query(*db);
	query.prepare("replace into mdata select mid, wtype, wid, sc, st, lst, score  from studyData.mdata");
	//query.bindValue(":maxLst", m_lstMax.isValid() ? m_lstMax : QDateTime::fromString("1.30.1", "M.d.s"));
// 	query.prepare("select mid, wtype, wid, sc, st, lst, score  from studyData.mdata where lst is not null and lst > :maxLst)");
// 	query.bindValue(":maxLst", m_lstMax.isValid() ? m_lstMax : QDateTime::fromString("1.30.1", "M.d.s"));
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::syncMData faild." << query.lastError().text());
	}


}

void UDataDB::updateWordMInfo(WordInfo &winfo)
{
	QSqlQuery query(*db);
	query.prepare("update studyData.mdata set sc = :sc, st = :st, lst = :lst, score = :score where mid = :mid");
	query.bindValue(":sc", winfo["sc"].toInt() + 1);
// 	int st = winfo["st"].toInt();
// 	st = st ? st * 1.5 :  5 * 60;
	query.bindValue(":st",  winfo["st"]);
	query.bindValue(":lst", QDateTime::currentDateTime()/*winfo["lst"]*/);
	query.bindValue(":mid", winfo["mid"]);
	query.bindValue(":score", winfo["score"]);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateWordMInfo faild.");
	}
}

void UDataDB::insertToMyBook(BOOKINFO &bookInfo, QString &tableName)
{
	DBG(qDebug() << "UDataDB::insertToMyBook" << bookInfo["name"].toString() << tableName);

	QSqlQuery query(*db);
	query.prepare("insert into mybook(bid, btype, cid, orderid, name,wcount, createTime) values(:bid, :btype, :cid, 0, :name, :wcount, :createTime)");
	query.bindValue(":bid", bookInfo["bid"]);
	query.bindValue(":bytpe", bookInfo["btype"]);
	query.bindValue(":cid", ELearning);
	query.bindValue(":name", bookInfo["name"]);
	query.bindValue(":wcount", bookInfo["wcount"]);
	query.bindValue(":createTime", QDateTime::currentDateTime());
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::insertToMyBook faild.");
	}

}

EnumMyCategory UDataDB::getBookCid(const QString &bookTableName, int maxst)
{

	QSqlQuery query(*db);
	QString sql = QString("SELECT count(%1.mid) \
		FROM %2 \
		LEFT JOIN mdata \
		ON %3.mid = mdata.mid where mdata.st < %4 or mdata.st is null")
		.arg(bookTableName)
		.arg(bookTableName)
		.arg(bookTableName)
		.arg(maxst);
	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getBookCid faild." << query.lastError());
	}

	query.next();

	EnumMyCategory emc = query.record().value(0).toBool() ? ELearning : EKnowwell;

	return emc;
}

void UDataDB::initFP()
{
	m_fpList.clear();
	
	QString strFP = getConfigInfo("defaultFP");
	QStringList strFPL = strFP.split(',');
	foreach (QString str, strFPL)
	{
		m_fpList << str.toInt();
		//DBG(qDebug() << str.toInt());
	}

	qSort(m_fpList.begin(), m_fpList.end());
	DBG(qDebug() << "UDataDB::initFP 1" << m_fpList);
	maxfp = getMaxFP();
}

int UDataDB::getNextFP(int CFP)
{
	for (int i = 0; i < m_fpList.size(); ++i) 
	{
		if (m_fpList.at(i) > CFP)
			return m_fpList.at(i);
	}
	DBG(qDebug() << "UDataDB::getNextFP 1 faild" << CFP << m_fpList);
	return 0;

}

double UDataDB::getNowFP(int CFP)
{
	for (int i = m_fpList.size(); i >  0 ; --i) 
	{
		if (m_fpList.at(i - 1) <= CFP)
			return (i + 0.0) / m_fpList.size();
	}


	return 0;
}

int UDataDB::getMaxFP()
{
	return m_fpList.at(m_fpList.size() - 1); // 不要改成maxfp,会引起奇怪的错误
}

void UDataDB::updateWordLearnLog(QMap<QString, QVariant> &tval)
{
	//DBG(qDebug() << "UDataDB::updateWordLearnLog" << QDateTime::currentDateTime().toTime_t());
	QSqlQuery query(*db);
	query.prepare("insert into studyData.dslog(mid, stime, itime, judgetime, affirmtime, studytime, selJudge, selAffirm) values(:mid, :stime, :itime, :judgetime, :affirmtime, :studytime, :selJudge, :selAffirm)");
	query.bindValue(":mid", tval["mid"]);
	query.bindValue(":stime", QDateTime::fromTime_t(tval["stime"].toUInt()));
	query.bindValue(":itime", QDateTime::currentDateTime());
	query.bindValue(":judgetime", tval["judgetime"]);
	query.bindValue(":affirmtime", tval["affirmtime"]);
	query.bindValue(":studytime", tval["studytime"]);
	query.bindValue(":selJudge", tval["selJudge"]);
	query.bindValue(":selAffirm", tval["selAffirm"]);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateWordLearnLog faild.");
	}
}

void UDataDB::updateMBCid(int bid, bookType bt, int cid)
{
	QSqlQuery query(*db);
	query.prepare("update mybook set cid = :cid where bid = :bid and btype = :btype");
	query.bindValue(":bid",bid);
	query.bindValue(":cid",cid);
	query.bindValue(":btype",bt);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateBCid faild.");
	}
}


int UDataDB::getWcount(const QString &bookTableName)
{
	QSqlQuery query(*db);
	QString sql = QString("SELECT count(%1.mid)\
						  FROM %2")
						  .arg(bookTableName)
						  .arg(bookTableName);

	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getSCount faild.");
	}

	query.next();

	return query.record().value(0).toInt();

}


int UDataDB::getSCount(const QString &bookTableName) // 已学单词数
{
	QSqlQuery query(*db);
	QString sql = QString("SELECT count(%1.mid)\
						  FROM %2\
						  INNER JOIN mdata\
						  ON %3.mid = mdata.mid where mdata.sc is not null")
						  .arg(bookTableName)
						  .arg(bookTableName)
						  .arg(bookTableName);

	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getSCount faild.");
	}

	query.next();

	return query.record().value(0).toInt();

}

void UDataDB::updateSCount(int bid, bookType bt, int scount)
{
	QSqlQuery query(*db);
	query.prepare("update mybook set scount = :scount where bid = :bid and btype = :btype");
	query.bindValue(":bid",bid);
	query.bindValue(":scount",scount);
	query.bindValue(":btype",bt);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateSCount faild.");
	}
}

void UDataDB::updateKNCount(int bid, bookType bt, int kncount)
{
	QSqlQuery query(*db);
	query.prepare("update mybook set kncount = :kncount where bid = :bid and btype = :btype");
	query.bindValue(":bid",bid);
	query.bindValue(":kncount",kncount);
	query.bindValue(":btype",bt);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateKNCount faild.");
	}
}

int UDataDB::getKNCount(const QString &bookTableName,int maxst)
{
	QSqlQuery query(*db);
	QString sql = QString("SELECT count(%1.mid)\
						  FROM %2\
						  INNER JOIN mdata\
						  ON %3.mid = mdata.mid where mdata.st >= %4")
						  .arg(bookTableName)
						  .arg(bookTableName)
						  .arg(bookTableName)
						  .arg(maxst);

	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getKNCount faild.");
	}

	query.next();

	return query.record().value(0).toInt();
}



void UDataDB::updateProgress(int bid, bookType bt, float progress)
{
	QSqlQuery query(*db);
	query.prepare("update mybook set progress = :progress where bid = :bid and btype = :btype");
	query.bindValue(":bid",bid);
	query.bindValue(":progress",progress);
	query.bindValue(":btype",bt);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateProgress faild.");
	}
}

float UDataDB::getProgress(const QString &bookTableName,int maxst)
{
	// case when mdata.wtype = 2 then dict.word.word else uword.word end as word,
	QSqlQuery query(*db);
	QString sql = QString("SELECT sum(case when mdata.st > %1 then %2 else mdata.st end) * 1.0 / (%3 * count(%4.mid))\
						  FROM %5\
						  LEFT JOIN mdata\
						  ON %6.mid = mdata.mid")
						  .arg(maxst)
						  .arg(maxst)
						  .arg(maxst)
						  .arg(bookTableName)
						  .arg(bookTableName)
						  .arg(bookTableName);

	query.prepare(sql);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::getProgress faild.");
	}

	query.next();

	return  query.record().value(0).toFloat();
}

void UDataDB::clearBookStudyInfo(int mid)
{
	QSqlQuery query(*db);
	if (!query.exec(QString("update mdata set sc = null, st = null, lst = null, score = null where mid=%1").arg(mid)))
	{
		DBG(qDebug() << "UDataDB::clearBookStudyInfo" << query.lastError());
	}

}

void UDataDB::updateBookStudyInfoknown(int mid)
{
	QSqlQuery query(*db);
	query.prepare("update mdata set sc = 1, st = :st, lst = :lst, score = null where mid=:mid");
	query.bindValue(":st", getMaxFP());
	query.bindValue(":lst", QDateTime::currentDateTime());
	query.bindValue(":mid", mid);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::updateBookStudyInfoknown faild." << query.lastError());
	}

}

void UDataDB::clearBookStudyInfo(int bid, bookType bt)
{
	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
	QSqlQuery query(*db);
	if (!query.exec(QString("update mdata set sc = null, st = null, lst = null, score = null where mid in (select mid from %1)").arg(tableName)))
	{
		DBG(qDebug() << "UDataDB::clearBookStudyInfo" << query.lastError());
	}
}

void UDataDB::deleteStudyDBInfo(int bid, bookType bt, bool bClearBookStudyInfo)
{
	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
	if (isTableExist(tableName))
	{
		if (bClearBookStudyInfo)
		{
			// 该课程单词学习状态设0
			clearBookStudyInfo(bid, bt);
			updateStudyProgress();
		}

		QSqlQuery query(*db);
		if (!query.exec(QString("DROP TABLE %1").arg(tableName)))
		{ 
			DBG(qDebug() << "UDataDB::deleteStudyDBInfo 1" << query.lastError());
		}
		if (!query.exec(QString("DROP TABLE %1grp").arg(tableName)))
		{
			DBG(qDebug() << "UDataDB::deleteStudyDBInfo 2" << query.lastError());
		}
	}
}



void UDataDB::updateStudyDBInfo(int bid, bookType bt)
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());

	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
	if (isTableExist(tableName))
	{
		QString dbpath = g_pbcmgr->getBookPath(bt, bid);
		deleteBookData(tableName);
		importBookData(tableName, dbpath);
		updateStudyProgress();
	}
	DBG(qDebug() << "UDataDB::updateStudyDBInfo 耗时" << start.msecsTo(QDateTime::currentDateTime()));
}

void UDataDB::deleteBookData(const QString &tableName)
{
	QSqlQuery query(*db);
	if (!query.exec(QString("delete from %1").arg(tableName)))
	{
		DBG(qDebug() << "UDataDB::delectBookData" << query.lastError());
	}
	
}	

bool UDataDB::isStudying(int bid, bookType bt)
{
	QString tableName = QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);

	return isTableExist(tableName);
}

int UDataDB::getBookReviewCount(int bid, bookType bt)
{
	QSqlQuery query(*db);
	if (!query.exec(QString("select count(mdata.mid) from %1 as book left join mdata on book.mid = mdata.mid where lst is not null and (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < %2").arg(getTableName(bid, bt)).arg(maxfp)))
	{
		DBG(qDebug() << "UDataDB::getBookReviewCount faild ." << query.lastError());
		return -1;
	}

	query.next();

	return query.record().value(0).toInt();
}

QString UDataDB::getTableName(int bid, bookType bt)
{
	return QString("%1%2").arg(btUser == bt ? "u" : "o").arg(bid);
}


int UDataDB::getReviewCount()
{
	QSqlQuery query(*db);
	if (!query.exec(QString("select count(mid) from mdata where lst is not null and (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < %1").arg(maxfp)))
	{
		DBG(qDebug() << "UDataDB::getReviewCount faild ." << query.lastError());
		return -1;
	}

	query.next();

	return query.record().value(0).toInt();
}

QDateTime UDataDB::getMaxLst()
{
	QSqlQuery query(*db);
	if (!query.exec("select max(lst) from studyData.mdata"))
	{
		DBG(qDebug() << "UDataDB::getMaxLst faild ." << query.lastError());
		return QDateTime();
	}

	query.next();

	return query.record().value(0).toDateTime();
}

void UDataDB::getUserBookBookListInfo(QList<QVariant> &info)
{
	QSqlQuery query(*db);
	if (!query.exec("select * from ubook"))
	{
		DBG(qDebug() << "UDataDB::getUserBookBookListInfo faild ." << query.lastError());
		return;
	}


	while (query.next())
	{
		QMap<QString, QVariant> oi;
		oi["bid"] = query.record().value("bid");
		oi["name"] = query.record().value("name");
		info.push_back(oi);
		
	}
	

	return;
}

WordInfo UDataDB::getWordInfoByWid(int wid)
{
	WordInfo wi;
	QSqlQuery qoword(QString("select * from dict.word where wid = %1").arg(wid), *db);
	if (!qoword.next())
	{
		DBG(qDebug() << "UDataDB::getWordInfoByWid" << "未获取对应wid的单词信息" << wid);
		return WordInfo();
	}
	wi["word"] = qoword.value(1);
	wi["pron"] = qoword.value(2);
	wi["dmean"] = qoword.value(3);
// 	wi["mcode"] = qoword.value(4);
// 	wi["emean"] = qoword.value(5);
// 	wi["thesaurus"] = qoword.value(6); // 同义词
// 	wi["syllable"] = qoword.value(7);	// 音节
// 	wi["suppletion"] = qoword.value(8);	// 词性变化
	return wi;

}

void UDataDB::cleanSuspendWord()
{
	QStringList tnl;
	if (getAllBookTableName(tnl))
	{
		QString sql;
		if (tnl.size() > 0)
		{
			sql = QString("select mid from %1").arg(tnl[0]);
			for (int i = 1; i < tnl.size(); i++)
			{
				sql += QString(" union select mid from %1").arg(tnl[i]);
			}

			sql = QString("delete from mdata where mid not in (%1)").arg(sql);

		}
		else
		{
			sql = "delete from mdata";
		}
		QSqlQuery query(*db);
		if (!query.exec(sql))
		{
			DBG(qDebug() << "UDataDB::cleanSuspendWord faild ." << query.lastError() << sql);
		}
	}



	
}

bool UDataDB::getAllBookTableName(QStringList &sl)
{

	QSqlQuery query(*db);
	if (!query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name GLOB '[ou][1-9]*' and name NOT GLOB '*[grp]';"))
	{
		DBG(qDebug() << "UDataDB::getAllBookTableName faild ." << query.lastError());
		return false;
	}


	while (query.next())
	{
		sl << query.record().value(0).toString();
	}


	return true;
}

void UDataDB::insertTLog(QMap<QString, QVariant> &tlog)
{
	DBG(qDebug() << "UDataDB::insertTLog" << tlog); 
	QSqlQuery query(*db);
	query.prepare("insert into tlog(begintime, endtime, stime, swcount, snwcount, rscount, kncount, sspeed, stwcount, stnwcount) values(:begintime, :endtime, :stime, :swcount, :snwcount, :rscount, :kncount, :sspeed, :stwcount, :stnwcount)");
	query.bindValue(":begintime", tlog["begintime"]);
	query.bindValue(":endtime", tlog["endtime"]);
	query.bindValue(":stime", tlog["stime"]);
	query.bindValue(":swcount", tlog["swcount"]);
	query.bindValue(":snwcount", tlog["snwcount"]);
	query.bindValue(":rscount", tlog["rscount"]);
	query.bindValue(":kncount", tlog["kncount"]);
	query.bindValue(":sspeed", tlog["sspeed"]);
	query.bindValue(":stwcount", tlog["stwcount"]);
	query.bindValue(":stnwcount", tlog["stnwcount"]);
	if (!query.exec())
	{
		DBG(qDebug() << "UDataDB::insertTLog faild." << query.lastError());
	}
}

QList<QVariant> UDataDB::getAllWordList()
{
	DBG(QDateTime   start   =   QDateTime::currentDateTime());
	QList<QVariant> sl;
	QSqlQuery query(*db);
// 	if (!query.exec("select wtype,wid,sc,st,word from studyData.mdata as mdata INNER join dict.word on mdata.wtype = 2 and mdata.wid = dict.word.wid \
// 		union \
// 		select wtype,wid,sc,st,word from studyData.mdata as mdata INNER join uword on mdata.wtype = 1 and mdata.wid = uword.wid ;"))
	if (!query.exec("select word,case when lst is null then 0 when (julianday(\'now\', \'localtime\') - julianday(lst)) * 86400.0 > st and st < 1296000 then 1 else 2 end,st,sc,lst from \
		(select wtype,mdata.wid,sc,st,word,lst from studyData.mdata as mdata inner join dict.word on mdata.wtype = 2 and mdata.wid = dict.word.wid \
		union \
		select wtype,mdata.wid,sc,st,word,lst from studyData.mdata as mdata INNER join uword on mdata.wtype = 1 and mdata.wid = uword.wid) ;"))
	{
		DBG(qDebug() << "UDataDB::getAllWordList faild ." << query.lastError());
	}

	QList<QVariant> one;
	while (query.next())
	{
        QSqlRecord re = query.record();
		one.clear();
		one.push_back(true);
		one.push_back(re.value(0));
		one.push_back(re.value(1));
		one.push_back(re.value(2));
		one.push_back(re.value(3));
		one.push_back(re.value(3));
		sl.push_back(one);
	}
	DBG(qDebug() << "UDataDB::getAllWordList 耗时" << start.msecsTo(QDateTime::currentDateTime()));
	return sl;
}
