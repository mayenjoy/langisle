#include "stdafx.h"
#include "PageMainBook.h"
#include "../comm/QNoFocusItemDelegate.h"
#include "BCDBModel.h"
#include "DlgOBookDown.h"
#include "../comm/LITableView.h"
#include "DlgImportUBook.h"
#include "DlgEditUBook.h"
#include "BookDB.h"
#include "GlobalVal.h"
#include "LIWord0030.h"
#include "LIConfig.h"
PageMainBook::PageMainBook(QWidget *parent)
	: QWidget(parent)
{
	//LISearchButton *lisButton = new LISearchButton(this);
	//lisButton->setFixedSize(75, 23);
	//buttonLayout->addWidget(lisButton);
	DBG(QDateTime   start   =   QDateTime::currentDateTime());

// 	QPushButton *menuButton = new QPushButton("菜单");
// // 	menuButton->setIcon(QIcon());
// // 	menuButton->setFixedHeight(26);
// //   	menuButton->setFixedWidth(18);
//   	menuButton->setFlat(true);
// 	menuButton->setFocusPolicy(Qt::NoFocus);


	// 顶部
	searchEdit = new QLineEdit;	
	searchEdit->setFixedWidth(180);
	searchEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	searchButton = createButton("搜索", SLOT(search()));
	labelCurPath = new QLabel();
	labelCurPath->setFixedHeight(searchButton->sizeHint().height());
	labelCurPath->setIndent(9);
	labelCurPath->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	QHBoxLayout *searchLayout = new QHBoxLayout;
	searchLayout->addWidget(searchEdit);
	searchLayout->addWidget(searchButton);

	// 字体, QTableView和QTreeWidget字体在XP下偏小,检查字体实际与searchEdit字体一致,但效果不一致,原因未知,新生成的QFont却有效
	QFont font = QFont(searchEdit->font().family(), searchEdit->font().pointSize());

	QHBoxLayout *topLayout = new QHBoxLayout;
	
	topLayout->addWidget(labelCurPath);
//	topLayout->addWidget(menuButton);
	topLayout->addStretch();
	topLayout->addLayout(searchLayout);
	
	// 课程分类
	categoryTree = new QTreeWidget(this);
	categoryTree->setHeaderHidden(true);
	//categoryTree->setFocusPolicy(Qt::NoFocus);
	//categoryTree->setFont(font);
	//categoryTree->setMaximumWidth(400);
	//categoryTree->setMinimumWidth(100);
	QHeaderView *pHeader = categoryTree->header(); 
	pHeader->setResizeMode( QHeaderView::ResizeToContents ); 
	pHeader->setStretchLastSection( false ); 
	//pHeader->setMinimumSectionSize(140);
	//pHeader->setDefaultSectionSize(800);
	//categoryTree->setAutoScroll(false);
	categoryTree->setStyleSheet("QTreeView::item{ height:21px; } ");
	//treeWidget->setStyleSheet("QTreeWidget::item{ height: 50px; background: yellow;}");
	// categoryTree->setIconSize(QSize(24, 24));
	//categoryTree->header()->setDefaultSectionSize(88);
	
	
	// 课程列表
	bookTable = new LITableView(this);
	bookTable->setItemDelegate(new QNoFocusItemDelegate(bookTable));
	bookTable->setShowGrid(false);
	bookTable->setSortingEnabled(true);
	//bookTable->setSelectionMode(QAbstractItemView::ExtendedSelection );
	bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	bookTable->verticalHeader()->hide();
	bookTable->horizontalHeader()->setMovable(true);

// 	QString s="QTableView::item:hover {background-color: rgb(185, 210, 235);}";
// 	bookTable->setStyleSheet(s);
// 	bookTable->setAttribute(Qt::WA_Hover, true);
 	bookTable->setMouseTracking(true);
	//bookTable->setAlternatingRowColors(true);
	//bookTable->setFocusPolicy(Qt::NoFocus);	
	bookTable->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	//bookTable->setFont(font);
	bookTable->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	bookTable->verticalHeader()->setDefaultSectionSize(24);
	//bookTable->horizontalHeader()->setStretchLastSection( true ); 
	bookTable->horizontalHeader()->setResizeMode(QHeaderView::Interactive); 

	//bookTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents); 
	
	// bookTable->setAutoScroll(false); // 上下键选择时不自动滚动了
	bookTable->setTabKeyNavigation(false);
	//bookTable->setObjectName("QTableView");
	//bookTable->setStyleSheet("#QTableView {selection-background-color: #d7e9ff;}");
	connect(bookTable->horizontalHeader(), SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this, SLOT(bookSortIndicatorChanged(int, Qt::SortOrder)));

	// 详细信息
	//tabDetail = new QTabWidget;
	//bookDetail = new TabBookDetail();
	//tabDetail->addTab(bookDetail, "详细信息");
	// 按钮
// 	learnButton = createButton("学习", SLOT(learn()));
// 	delButton = createButton("删除", SLOT(del()));
// 	addUserCategoryButton = createButton("添加分类", SLOT(addCategory()));
// 	delUserCategoryButton = createButton("删除分类", SLOT(delCategory()));
// 	modifyCategoryButton = createButton("修改分类", SLOT(modifyCategory()));
// 	addBookButton = createButton("添加词库", SLOT(addBook()));
// 	editBookButton = createButton("编辑词库", SLOT(editBook()));
	

	// 右边拆分
	QSplitter *splitRight = new QSplitter(Qt::Vertical);
	splitRight->addWidget(bookTable);
	//splitRight->addWidget(tabDetail);
	// splitRight->setSizes(QList<int>() << 240 << 150); // 150高 大字休二行刚好
	splitRight->setSizes(QList<int>() << 240 << 10); // 150高 大字休二行刚好
	splitRight->setStretchFactor(0, 1);	// 扩展列表框
	splitRight->setHandleWidth(3);
	//tabDetail->hide();

	// 拆分1
	QSplitter *split = new QSplitter;
	split->setChildrenCollapsible(false);
	split->addWidget(categoryTree);
	split->addWidget(splitRight);
	split->setSizes(QList<int>() << 140 << 500);
	split->setStretchFactor(1, 1);
	split->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	split->setHandleWidth(3);

	//// 分类,列表Layout
	//QHBoxLayout *catecoryAndCourseLayout = new QHBoxLayout;
	//catecoryAndCourseLayout->addWidget(catecoryTree);
	//catecoryAndCourseLayout->addWidget(courseTable);

	// 下面按钮Layout
	QHBoxLayout *buttonLayout = new QHBoxLayout;	
// 	buttonLayout->addWidget(addUserCategoryButton);
// 	buttonLayout->addWidget(delUserCategoryButton);
// 	buttonLayout->addWidget(modifyCategoryButton);
// 	buttonLayout->addStretch();	
// 	buttonLayout->addWidget(addBookButton);
// 	buttonLayout->addWidget(editBookButton);	
// 	buttonLayout->addWidget(delButton);	
// 	buttonLayout->addWidget(learnButton);

//	buttonLayout->addStrut(learnButton->sizeHint().height());

	QVBoxLayout *mainLayout = new QVBoxLayout;
	
	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(split);
	//mainLayout->addLayout(buttonLayout);
	
	

	//mainLayout->addStretch();
	setLayout(mainLayout);


	myitem = g_pbcmgr->addMTreeItem(categoryTree);
	myitem->setExpanded(true);
	QTreeWidgetItem * uitem = g_pbcmgr->addUTreeItem(categoryTree);
	uitem->setExpanded(true);
	QTreeWidgetItem * oitem = g_pbcmgr->addOTreeItem(categoryTree);
	oitem->setExpanded(true);
	if (g_pbcmgr->getMybcdb()->getSBookCount() > 0)
	{
		categoryTree->setCurrentItem(myitem);
		currentItemChanged(myitem, NULL);
	}
	else
	{
		categoryTree->setCurrentItem(oitem);
		currentItemChanged(oitem, NULL);
	}



	connect(categoryTree, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), 
		this, SLOT(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)));

	connect(searchEdit, SIGNAL(returnPressed()), this, SLOT(search()));	// 搜索框回车

	connect(bookTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),
		this, SLOT(selectionChanged(const QItemSelection & , const QItemSelection & )));

	mainLayout->setSpacing(0);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	
	//bookTable->setColumnWidth(3, 250);	// 一定要放在插入数据后,否则这行代码会不起作用

	connect(bookTable, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(tableDoubleClicked(const QModelIndex &)));
	connect(bookTable, SIGNAL(activated(const QModelIndex &)), this, SLOT(tableActivated(const QModelIndex &)));
	
	bookTable->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(bookTable, SIGNAL(customContextMenuRequested ( const QPoint &)), this, SLOT(bookTableContextMenu ( const QPoint &)));

	categoryTree->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(categoryTree, SIGNAL(customContextMenuRequested ( const QPoint &)), this, SLOT(categoryTreeContextMenu ( const QPoint &)));

	
	DBG(qDebug() << "PageMainBook耗时" << start.msecsTo(QDateTime::currentDateTime()));

}

PageMainBook::~PageMainBook()
{

}

void PageMainBook::currentItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
	QModelIndexList ml;
	g_pbcmgr->updateBookList(ml);

	// qDebug() << "PageMainBook::currentItemChanged 1";
	g_pbcmgr->updateCurrentCategory(current);

	searchEdit->setVisible(g_pbcmgr->isShowSearchBar());
	searchButton->setVisible(g_pbcmgr->isShowSearchBar());
	
// 	addUserCategoryButton->setVisible(g_pbcmgr->isCanAddUCategory());
// 	addBookButton->setVisible(g_pbcmgr->isCanAddUBook());
// 	delUserCategoryButton->setVisible(g_pbcmgr->isCanDelCategory());
// 	modifyCategoryButton->setVisible(g_pbcmgr->isCanModifyUCategory());

	g_pbcmgr->fillBookToTable(bookTable);
	//bookTable->resizeColumnsToContents();	// 开启后 最后段扩展有问题
	labelCurPath->setText(g_pbcmgr->getCurCategoryPath().join("\\"));

// 	connect(bookTable->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
// 		this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));
	connect(bookTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),
		this, SLOT(selectionChanged(const QItemSelection & , const QItemSelection & )));

	bookTable->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
	//bookTable->horizontalHeader()->reset ();
	//bookTable->sortByColumn(0, Qt::AscendingOrder);

// 	learnButton->setVisible(false);
// 	delButton->setVisible(false);
// 	editBookButton->setVisible(false);
	//bookDetail->clear();

	//tabDetail->setVisible(false);

	emit uptoolbarState();
}

QPushButton *PageMainBook::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void PageMainBook::learnAllBook()
{

	QTimer::singleShot(0, GlobalVal::s_pmw, SLOT(topShow()));

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	categoryTree->setCurrentItem(myitem);
	currentItemChanged(myitem, NULL);
	bookTable->selectAllRow();
	learn();
	QApplication::restoreOverrideCursor();
}

void PageMainBook::down()
{
	QModelIndexList ml = bookTable->selectionModel()->selectedRows();

	g_pbcmgr->updateBookList(ml);

	QList<BOOKINFO> biList = g_pbcmgr->getSelectBook();

	for (int i = 0; i < biList.size(); i++)
	{
		bookType bt = (bookType)(biList.at(i)["btype"].toInt());
		if (bt == btOfficial)
		{
			// DBG(qDebug() << biList.at(i)["name"].toString());
			int bid = biList.at(i)["bid"].toInt();
			if (!g_pbcmgr->isBookExist(btOfficial, bid))
			{
				DBG(qDebug() << biList.at(i)["name"].toString() << "不存在";);
				DlgOBookDown dobd(bid);
				if (QDialog::Accepted != dobd.exec())
				{
					break;
				}
				else
				{
					g_pbcmgr->updateBookExistState(btOfficial, bid);
				}
			}
			DBG(qDebug() << bid << biList.at(i)["name"].toString());
			// 			bi["btype"] = by;
			// 			bi["bid"] = curbcdb->getBidByRow(row);
			// 			bi["name"] = curbcdb->getBookNameByRow(row);
			// 			bi["wcount"] = curbcdb->getBookWCountByRow(row);
		}
	}

	update();

	foreach(QModelIndex mi, ml)
	{
		bookTable->selectionModel()->select(mi,  QItemSelectionModel::Select | QItemSelectionModel::Rows);
	}
	
	


}

void PageMainBook::learn()
{
	if (g_pbcmgr->isShowDownButton())
	{
		down();

		return;
	}
	if (getSelSize() == 0)
	{
		learnAllBook();
		return;
	}
	qDebug() << "PageMainBook::learn()";



	

	emit slearn();
}

void PageMainBook::endlearn()
{
	update();
	emit upASInfo();
	g_pbcmgr->getRemindMgr()->reset(); // 重新开始计时
// 	QModelIndexList ml = bookTable->selectionModel()->selectedRows();
// 
// 	((LISqlQueryModel *)(bookTable->model()))->update();
// 
// 	// 在本次学习中如果单词全部掌握了,返回时选择会出错,因为记完的课程会转移到已掌握分类中去
// 	for (int i = 0; i < ml.size(); i++)
// 	{
// 		bookTable->selectionModel()->select(ml.at(i), QItemSelectionModel::Select | QItemSelectionModel::Rows);
// 	}
	//g_pbcmgr->fillBookToTable(bookTable);
	// 切换到学习界面
}

void PageMainBook::addCategory()
{
	QInputDialog dialog(this);
	dialog.setWindowTitle("添加分类");
	dialog.setLabelText("分类名:");

	dialog.setOkButtonText("确定");
	dialog.setCancelButtonText("取消");

	QPoint pd;

	do 
	{
		if (!pd.isNull())
			dialog.move(pd);
		if (QDialog::Rejected == dialog.exec())
			break;

		pd = dialog.pos();

		QString category = dialog.textValue();

		if (category.isEmpty())
		{
			QMessageBox::information(&dialog, "添加分类", "分类名不能为空, 请重新输入.");
			continue;
		}

		if (!g_pbcmgr->addCategory(category))
		{
			QMessageBox::information(&dialog, "添加分类", "添加的分类已经存在, 请使用其他分类名.");
			continue;
		}
		else
		{
			break;
		}
	} while (true);	
		
}

void PageMainBook::delCategory()
{
	if (g_pbcmgr->getUbcdb()->isCanDeleteCategory(g_pbcmgr->getCID()))
	{
		if (QMessageBox::Yes == QMessageBox::question(this, "删除分类", QString("确认要删除[%1]分类吗?").arg(g_pbcmgr->getCurrentCategoryName()), QMessageBox::Yes | QMessageBox::No))
			g_pbcmgr->delCategory();
	}
	else
		QMessageBox::warning(this, "提示信息", "请先删除该分类下词库和下级分类!", QMessageBox::Ok);
}

void PageMainBook::modifyCategory()
{
	QString curCName = g_pbcmgr->getCurrentCategoryName();
	QInputDialog dialog(this);
	dialog.setWindowTitle("修改分类");
	dialog.setLabelText("分类名:");
	dialog.setTextValue(curCName);

	dialog.setOkButtonText("确定");
	dialog.setCancelButtonText("取消");

	//QPoint pd;

	do 
	{
		//if (!pd.isNull())
		//	dialog.move(pd);
		if (QDialog::Rejected == dialog.exec())
			break;

		//pd = dialog.pos();

		QString category = dialog.textValue();

		if (category.isEmpty())
		{
			QMessageBox::information(&dialog, "修改分类", "分类名不能为空, 请重新输入.");
			continue;
		}

		if (curCName == category)	// 一样的分类名
		{
			qDebug() << "一样的分类名,直接修改成功" << category;
			break;
		}

		if (g_pbcmgr->isCategoryExist(category))
		{
			QMessageBox::information(&dialog, "修改分类", "分类名已经存在, 请使用其他分类名.");
			continue;
		}
		else
		{
			g_pbcmgr->modifyCategory(category);
			break;
		}
	} while (true);	



	//QString curCName = g_pbcmgr->getCurrentCategoryName();
	//QString category = curCName;
	//while( true )
	//{
	//	bool ok;
	//	category = QInputDialog::getText(this, "修改分类",
	//		"分类名:", QLineEdit::Normal,
	//		category, &ok);

	//	if (ok)
	//	{
	//		if (category.isEmpty())
	//		{
	//			QMessageBox::information(this, "修改分类", "分类名不能为空,请重新输入.");
	//			continue;
	//		}

	//		if (curCName == category)	// 一样的分类名
	//		{
	//			qDebug() << "一样的分类名,直接修改成功" << category;
	//			break;
	//		}

	//		if(g_pbcmgr->isCategoryExist(category))
	//		{
	//			QMessageBox::information(this, "修改分类", "分类名已经存在, 请使用其他分类名.");
	//			continue;
	//		}
	//		g_pbcmgr->modifyCategory(category);
	//	}
	//	break;
	//}
}

void PageMainBook::addBook()
{
	QInputDialog dialog(this);
	dialog.setWindowTitle("添加词库");
	dialog.setLabelText("词库名:");

	dialog.setOkButtonText("确定");
	dialog.setCancelButtonText("取消");

	QPoint pd;

	do 
	{
		if (!pd.isNull())
			dialog.move(pd);
		if (QDialog::Rejected == dialog.exec())
			break;

		pd = dialog.pos();

		QString bookName = dialog.textValue();

		if (bookName.isEmpty())
		{
			QMessageBox::information(&dialog, "添加词库", "词库名不能为空, 请重新输入.");
			continue;
		}

		if (!g_pbcmgr->addBook(bookName))
		{
			QMessageBox::information(&dialog, "添加词库", "添加的词库已经存在, 请使用其他词库名.");
			continue;
		}
		else
		{
			//((LISqlQueryModel *)(bookTable->model()))->update();
			break;
		}
	} while (true);	

	update();

}

void PageMainBook::search()
{
	qDebug() << searchEdit->text();
	QString text = searchEdit->text();
	if (text.isEmpty())
	{
		return;
	}
	
	g_pbcmgr->search(text, bookTable);

	labelCurPath->setText("搜索:" + g_pbcmgr->getCurCategoryPath().join("\\"));

// 	connect(bookTable->selectionModel(), SIGNAL(currentRowChanged(const QModelIndex &, const QModelIndex &)),
// 		this, SLOT(currentRowChanged(const QModelIndex &, const QModelIndex &)));
	connect(bookTable->selectionModel(), SIGNAL(selectionChanged(const QItemSelection & , const QItemSelection & )),
		this, SLOT(selectionChanged(const QItemSelection & , const QItemSelection & )));
	//learnButton->setVisible(false);
	//tabDetail->setVisible(false);
	//bookDetail->clear();
}


// void PageMainBook::currentRowChanged(const QModelIndex & current, const QModelIndex & previous)
// {
// 
// 	BCDBModel *model = (BCDBModel *)bookTable->model();
// 	if (current.row() == -1)
// 	{
// 		return;
// 	}
// 
// 	g_pbcmgr->updateCurrentBook(current);	
// 
// // 	int row = current.row();
// // 	bookDetail->setBookName(g_pbcmgr->getBookNameByRow(row));
// // 	bookDetail->setCommentInfo(g_pbcmgr->getBookCommentByRow(row));
// // 	bookDetail->setWordCountInfo(g_pbcmgr->getBookWCountByRow(row));
// 	learnButton->setVisible(g_pbcmgr->isShowStudyButton());
// 	editBookButton->setVisible(g_pbcmgr->isCUser());
// 	delButton->setVisible(g_pbcmgr->isCUser());
// 	// tabDetail->setVisible(true);
// }

void PageMainBook::bookSortIndicatorChanged(int logicalIndex, Qt::SortOrder order )
{
	//bookTable->selectRow(g_pbcmgr->getCurrentBookRow());
	//qDebug() << "PageMainBook::bookSortIndicatorChanged" << row << bookTable->currentIndex() << cbid;
	//bookTable->scrollTo(bookTable->currentIndex(),  QAbstractItemView::EnsureVisible);

}


void PageMainBook::tableDoubleClicked( const QModelIndex & index )
{
	DBG(qDebug() << "PageMainBook::tableDoubleClicked" << index);
	learn();
}

void PageMainBook::tableActivated ( const QModelIndex & index )
{
	DBG(qDebug() << "PageMainBook::tableActivated" << index);
}



void PageMainBook::editBook()
{


	DlgEditUBook dlg(this);
	dlg.exec();

	QList<BOOKINFO> m_biList = g_pbcmgr->getSelectBook();
	bookType bt = (bookType)m_biList.at(0)["btype"].toInt();
	int bid = m_biList.at(0)["bid"].toInt();

	QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
	g_pbcmgr->getMybcdb()->updateWordCount(bid, bt, dlg.getbdb()->wordCount());
	g_pbcmgr->getUDataDB()->updateStudyDBInfo(bid, bt);
	QApplication::restoreOverrideCursor();
	

	update();

	emit upASInfo();
	
}

void PageMainBook::del()
{
	QMessageBox::StandardButton button;
	QList<BOOKINFO> m_biList = g_pbcmgr->getSelectBook();
	if (g_pbcmgr->isCUser())
	{
		button = QMessageBox::question(this, QString("删除选中的%1个词库").arg(m_biList.size()), "Yes:\t删除选中的词库并删除学习记录!\nNo:\t删除选中的词库,不删除学习记录.\nCancel:\t放弃本操作.", QMessageBox::Cancel | QMessageBox::Yes | QMessageBox::No, QMessageBox::Cancel);
		if (QMessageBox::Cancel != button)
		{
			foreach (BOOKINFO bi, m_biList)
			{
				bookType bt = (bookType)bi["btype"].toInt();
				int bid = bi["bid"].toInt();
				g_pbcmgr->getUDataDB()->deleteStudyDBInfo(bid, bt, QMessageBox::Yes == button);
				g_pbcmgr->getUbcdb()->delBook(bid);

				g_pbcmgr->getMybcdb()->delBook(bid, bt);

				if (!QFile::remove(g_pbcmgr->getBookPath(bt, bid)))
				{
					DBG(qDebug() << "PageMainBook::del faild");
				}
// 
// 				QString path = g_pbcmgr->getBookPath(bt, bid);
// 				BookDB *bdb = new BookDB(path, path);
// 				
// 				g_pbcmgr->getMybcdb()->updateWordCount(bid, bt, bdb->wordCount());
// 				g_pbcmgr->getUDataDB()->updateStudyDBInfo(bid, bt);
// 				delete bdb;

			}

			// g_pbcmgr->getUDataDB()->cleanSuspendWord();

			update();
			emit upASInfo();
		}
	}
	else // 我的词库
	{
		button = QMessageBox::question(this, QString("删除选中的%1个词库").arg(m_biList.size()), "Yes:\t删除选中的词库并删除学习记录!\nNo:\t只从列表中删除,不删除学习记录.\nCancel:\t放弃本操作.", QMessageBox::Cancel | QMessageBox::Yes | QMessageBox::No, QMessageBox::Cancel);
		if (QMessageBox::Cancel != button)
		{
			foreach (BOOKINFO bi, m_biList)
			{
				bookType bt = (bookType)bi["btype"].toInt();
				int bid = bi["bid"].toInt();
				g_pbcmgr->getUDataDB()->deleteStudyDBInfo(bid, bt, QMessageBox::Yes == button);
				g_pbcmgr->getMybcdb()->delBook(bid, bt);
			}

			// g_pbcmgr->getUDataDB()->cleanSuspendWord();
			update();
			emit upASInfo();
		}
	}




	

}


void PageMainBook::selectionChanged ( const QItemSelection & selected, const QItemSelection & deselected )
{
	QModelIndexList ml = bookTable->selectionModel()->selectedRows();
	g_pbcmgr->updateBookList(ml);

// 	learnButton->setVisible(g_pbcmgr->isShowStudyButton());
// 	editBookButton->setVisible(g_pbcmgr->isCUser());
// 	delButton->setVisible(g_pbcmgr->isShowDelButton());

	emit uptoolbarState();

}

void PageMainBook::update()
{
	//bookTable->horizontalHeader()->setSortIndicator(-1, Qt::AscendingOrder);
//	QModelIndexList ml = bookTable->selectionModel()->selectedRows();

	((LISqlQueryModel *)(bookTable->model()))->update();

// 	// 在本次学习中如果单词全部掌握了,返回时选择会出错,因为记完的课程会转移到已掌握分类中去
// 	for (int i = 0; i < ml.size(); i++)
// 	{
// 		bookTable->selectionModel()->select(ml.at(i), QItemSelectionModel::Select | QItemSelectionModel::Rows);
// 	}

// 	learnButton->setVisible(false);
// 	editBookButton->setVisible(false);
// 	delButton->setVisible(false);



	emit uptoolbarState();
}



void PageMainBook::bookTableContextMenu ( const QPoint & pos )
{
	DBG(qDebug() << "PageMainBook::bookTableContextMenu" << pos);
	//if (bookTable->indexAt(pos).isValid())
	{
		GlobalVal::s_pmw->getBookTableMenu()->exec(QCursor::pos());
	}
}

void PageMainBook::categoryTreeContextMenu ( const QPoint & pos )
{
	DBG(qDebug() <<  "PageMainBook::categoryTreeContextMenu" << pos);
	//if (categoryTree->indexAt(pos).isValid())
	{
		GlobalVal::s_pmw->getCateTreeMenu()->exec(QCursor::pos());
	}
}

int PageMainBook::getSelSize()
{
	return bookTable->selectionModel()->selectedRows().size();
}

void PageMainBook::selAll()
{
	bookTable->selectAllRow();
}
int PageMainBook::getBookCount()
{
	return ((BCDBModel *)bookTable->model())->getRowCount();
}