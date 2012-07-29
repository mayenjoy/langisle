#include "stdafx.h"
#include "BCMgr.h"
#include "BCDBModel.h"
#include "LIConfig.h"
#include "GlobalVal.h"
#include "SpeakMgr.h"
BCMgr *g_pbcmgr = NULL;

BCMgr::BCMgr(const QString &userName, QObject *parent)
	: QObject(parent), m_userName(userName)
{
	g_pbcmgr = this;
	fieldsHide << "bid" << "cid" << "orderid" << "comment";
	
	config = new LIConfig(Global::makePathStr("/udata/" + userName + "/config.conf"));
	BCDBModel::initC2hmap();
	BCDBModel::initN2size();
	

	Global::createRelExeDir("/udata/" + userName);
	Global::createRelExeDir("/udata/" + userName + "/book");
	QString udatadbPath = Global::makePathStr("/udata/" + userName + "/udata.db");
	udatadb = new UDataDB(udatadbPath, "UDATADB");
	obcdb = new OBCDBMgr(Global::makePathStr("/odata/book/obc.db"), "OBCDB");
	ubcdb = new UBCDBMgr(udatadbPath, "UBCDB");
	mbcdb = new MBCDBMgr(udatadbPath, "MBCDB");

	updateAllBookExistState();

	GlobalVal::s_pSpeakMgr->reset();

	m_remindMgr = new RemindMgr(parent);
}

BCMgr::~BCMgr()
{
	delete udatadb;
	delete obcdb;
	delete ubcdb;
	delete mbcdb;
	delete config;
	delete m_remindMgr;
	
	
}

QString BCMgr::getBookPath(bookType type, int bid)
{
	if (btUser == type)
	{
		return Global::makePathStr(QString("/udata/%1/book/%2.db").arg(m_userName).arg(bid));
	}
	else if (btOfficial == type)
	{
		return Global::makePathStr(QString("/odata/book/%1.db").arg(bid));
	}
	return "";
}

QTreeWidgetItem *BCMgr::insertRootItem(QTreeWidget *tree, const QString itemName, categoryType type)
{
	QTreeWidgetItem *parentItem = tree->invisibleRootItem();
	QTreeWidgetItem *item = new QTreeWidgetItem(parentItem, QStringList(itemName), type);
	item->setData(0, UserRoleCID, 0);
	parentItem->addChild(item);
	return item;
}

QTreeWidgetItem * BCMgr::addOTreeItem(QTreeWidget *tree)
{
	QTreeWidgetItem *item = insertRootItem(tree, "官方词库", ctOfficial);
	obcdb->fillCagegoryToTree(item);
	return item;
}

QTreeWidgetItem * BCMgr::addUTreeItem(QTreeWidget *tree)
{
	QTreeWidgetItem *item = insertRootItem(tree, "用户词库", ctUser);
	uRootItem = item;
	ubcdb->fillCagegoryToTree(item);
	return item;
}

QTreeWidgetItem * BCMgr::addMTreeItem(QTreeWidget *tree)
{
	QTreeWidgetItem *item = insertRootItem(tree, "我的词库", ctMyself);
	mbcdb->fillCagegoryToTree(item);
	return item;
}

QTreeWidgetItem *BCMgr::getTreeWidgetItemByID(QTreeWidget *tree, int cid, categoryType type)
{
	QList<QTreeWidgetItem *> tl = tree->findItems("*", Qt::MatchWildcard | Qt::MatchRecursive);
	foreach (QTreeWidgetItem* item, tl) 
	{
		if (item->type() == cid && item->data(0, UserRoleCID).toInt() == type)
		{
			return item;
		}
	}
	return NULL;
}

void BCMgr::fillBookToTable(QTableView *table)
{

	//恢复隐藏的列,必须在设置新的model之前
	for (int i = 0; i < table->horizontalHeader()->count(); i++)
	{
		table->showColumn(i);
	}


	if (curbcdb == mbcdb) // 我的课本
	{
		curbcdb->fillBookToTable(table, curcid, "", true);
	}
	else
	{
		curbcdb->fillBookToTable(table, curcid);
	}

	BCDBModel *model = (BCDBModel *)table->model();
	for (int i = 0; i < model->record().count(); i++)
	{
		table->showColumn(i);
		QString name = model->record().field(i).name();
		if (fieldsHide.contains(name))
		{
			table->hideColumn(i);
		}
		int width = model->getSizeSection(name);
		if (width)
		{
			table->horizontalHeader()->resizeSection(i, width);
		}


	}
}

int BCMgr::addURootCategory(const QString &cname)
{
	int cid = ubcdb->addCategory(cname, 0);
	if (0 == cid)	// 已经存在
	{
		//DBG(qDebug() << "BCMgr::addCategory" << cname << "已经存在");
		return false;
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(uRootItem, QStringList(cname), ctUser);
	curitem->addChild(item);
	item->setData(0, UserRoleCID, cid);

	return cid;
}


int BCMgr::addCategory(const QString &cname)
{
	int cid = ubcdb->addCategory(cname, curcid);
	if (0 == cid)	// 已经存在
	{
		//DBG(qDebug() << "BCMgr::addCategory" << cname << "已经存在");
		return false;
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(curitem, QStringList(cname), ctUser);
	curitem->addChild(item);
	item->setData(0, UserRoleCID, cid);
	return cid;

}

void BCMgr::delCategory()
{
	qDebug() << "BCMgr::delCategory";

	// 这里的先后顺序一定要对
	ubcdb->removeCategory(curcid);
	curitem->parent()->removeChild(curitem);


}

void BCMgr::modifyCategory(const QString &cname)
{
	qDebug() << "BCMgr::modifyCategory";
	ubcdb->modifyCagegory(cname, curcid);
	curitem->setText(0, cname);

}

bool BCMgr::addBook(const QString &bname)
{
	qDebug() << "BCMgr::addBook" << bname;
	return ubcdb->addBook(bname, curcid);
}

int BCMgr::addUBook(const QString &bname, int cid)
{
	return ubcdb->addBook(bname, cid);
}

bool BCMgr::isCategoryExist(const QString &cname)
{
	return ubcdb->isCategoryExist(curitem->parent()->data(0, UserRoleCID).toInt(), cname);	
}

void BCMgr::updateCurrentCategory(QTreeWidgetItem * current)
{
	curitem = current;
	curcid = getCID(curitem);
	curtype = getCType(curitem);

	switch (curtype)
	{
	case ctMyself:
		curbcdb = mbcdb;
		break;
	case ctUser:
		curbcdb = ubcdb;
		break;
	case ctOfficial:
		curbcdb = obcdb;
		break;
	}
	// qDebug() << "BCMgr::updateCurrentCategory" << curcid << curtype;
	
}

QString BCMgr::getCurrentCategoryName()
{
	return curbcdb->getCategoryNameByCid(curcid);
}



QStringList BCMgr::getCurCategoryPath()
{
	QStringList ret;
	ret << curitem->text(0);
	QTreeWidgetItem *item = curitem;
	while (item = item->parent())
	{
		ret << item->text(0);
	}
	return Global::reversed(ret);
}

void BCMgr::search(const QString &text, QTableView *table)
{
	if (curbcdb == mbcdb) // 我的课本
		curbcdb->fillBookToTable(table, curcid, text, true);
	else
		curbcdb->fillBookToTable(table, curcid, text);


// 	table->sortByColumn(0, Qt::AscendingOrder);
// 
// 	table->hideColumn(0);
// 	table->hideColumn(1);
// 	table->hideColumn(2);
// 	table->hideColumn(5);
}

void BCMgr::updateAllBookExistState()
{
	obcdb->updateAllBookExistState();
}

void BCMgr::updateCurrentBook(const QModelIndex & current)
{
	curbid = curbcdb->getBidByRow(current.row());
}


int BCMgr::getBidByRow(int row)
{
	return curbcdb->getBidByRow(row);
}

QString BCMgr::getBookNameByRow(int row)
{
	return curbcdb->getBookNameByRow(row);
}

QString BCMgr::getBookCommentByRow(int row)
{
	return curbcdb->getBookCommentByRow(row);
}

int BCMgr::getBookWCountByRow(int row)
{
	return curbcdb->getBookWCountByRow(row);
}

int BCMgr::getCurrentBookRow()
{
	return curbcdb->getRowByBID(curbid);
}

void BCMgr::updateBookList(QModelIndexList &ml)
{
	cml = ml;
}


QList<BOOKINFO> BCMgr::getSelectBook()
{

	QList<BOOKINFO> result;
	if (curtype == ctUser || curtype == ctOfficial)
	{
		bookType by = (curtype == ctUser) ? btUser : btOfficial;
		foreach (QModelIndex mi, cml)
		{
			int row = mi.row();
			//__DBG(qDebug() << curcid << curbcdb->getBidByRow(row) << curbcdb->getBookNameByRow(row));
			BOOKINFO bi;
			bi["btype"] = by;
			bi["bid"] = curbcdb->getBidByRow(row);
			bi["name"] = curbcdb->getBookNameByRow(row);
			bi["wcount"] = curbcdb->getBookWCountByRow(row);
			result.append(bi);
		}

	}
	else // 我的课程, 单独课本类型
	{
		foreach (QModelIndex mi, cml)
		{
			int row = mi.row();
			//__DBG(qDebug() << curcid << curbcdb->getBidByRow(row) << curbcdb->getBookNameByRow(row));
			bookType btype = (bookType)(((MBCDBMgr *)(curbcdb))->getBTypeByRow(row));

			BOOKINFO bi;
			bi["btype"] = btype;
			bi["bid"] = curbcdb->getBidByRow(row);
			bi["name"] = curbcdb->getBookNameByRow(row);
			bi["wcount"] = curbcdb->getBookWCountByRow(row);
			result.append(bi);
		}
	}
	return result;
}


bool BCMgr::isBookExist(bookType bt, int bid)
{
	if (bt == btOfficial)
		return obcdb->isBookExist(bid);
	return true;
}

void BCMgr::updateBookExistState(bookType bt, int bid)
{
	if (bt == btOfficial)
		return obcdb->updateBookExistState(bid, obcdb->isBookExist(bid));
}

bool BCMgr::isShowStudyMgrButton()
{
	if (curtype == ctUser || curtype == ctOfficial)
	{
		return false;
	}

	return true;

}


// 是否显示学习按钮
bool BCMgr::isShowStudyButton()
{

	if (curtype == ctMyself && curcid == EKnowwell)
	{
		return false;
	}

	if (curtype == ctOfficial)
	{
		if (cml.size() > 0 && isBookExist(btOfficial, getSelectBook().at(0)["bid"].toInt()))
		{
			return true;
		}
		return false;
	}
		
	return true;
}

// 是否显示搜索栏
bool BCMgr::isShowSearchBar()
{
	if (curtype == ctOfficial && (curcid == 0 || curbcdb->isSecondCategory(curcid)))
	{
		return true;
	}
	return false;
};

bool BCMgr::isCanAddUCategory()	// 显示增加分类按钮
{
	if (curtype == ctUser && (curcid == 0 || curbcdb->isSecondCategory(curcid)))
	{
		return true;
	}
	return false;
};

bool BCMgr::isCanModifyUCategory()	// 显示修改分类按钮
{
	if (curtype == ctUser && curcid != 0)
	{
		return true;
	}
	return false;
};

bool BCMgr::isCanAddUBook()	// 显示增加课本按钮
{
	return isCanModifyUCategory();	
}

bool BCMgr::isCanDelCategory()	// 显示删除分类按钮
{
	return isCanModifyUCategory();	
}

bool BCMgr::isShowEdit()
{
	if (cml.size() != 1 || !isCUser())
		return false;
	return true;
}

bool BCMgr::isCUser()	// 当前分类为用户类型
{
	return curtype == ctUser;	
}	

bool BCMgr::isShowDelButton()
{

	return (curtype == ctUser || curtype == ctMyself) && cml.size() > 0;
}

// 获取节点分类类型
categoryType BCMgr::getCType(QTreeWidgetItem * item)	
{
	return (categoryType)item->type();
};

// 获取节点分类ID
int BCMgr::getCID(QTreeWidgetItem * item)		
{
	return item->data(0, UserRoleCID).toInt();
};

bool BCMgr::isShowDownButton()
{
	if (curtype != ctOfficial)
	{
		return false;
	}

	if (cml.size() != 1)
		return false;

	if (isBookExist(btOfficial, getSelectBook().at(0)["bid"].toInt()))
		return false;
	return true;
}