#include "stdafx.h"
#include "TabBookDetail.h"

TabBookDetail::TabBookDetail(QWidget *parent)
	: QWidget(parent)
{
	QLabel *labelBookName = new QLabel("当前词库:");
	m_bookName = new QLabel();
	m_bookName->setWordWrap(true) ;
	// m_bookName->setMinimumHeight(m_bookName->sizeHint().height() * 2);

	QLabel *labelVerInfo = new QLabel("版本号:");
	m_verInfo = new QLabel();

	QLabel *labelCommentInfo = new QLabel("说明:");
	m_commentInfo = new QLabel;
	//m_commentInfo->setMinimumHeight(m_commentInfo->sizeHint().height() * 2);
	//m_commentInfo->viewport()->setAutoFillBackground(false);
	//m_commentInfo->setFrameShape(QFrame::NoFrame);
	//m_commentInfo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
	//// qDebug() << m_verInfo->sizeHint() << m_commentInfo->sizeHint();
//	m_commentInfo->setMinimumHeight(m_commentInfo->sizeHint().height() / 8 + 4);
	////m_commentInfo->setMaximumHeight(m_commentInfo->sizeHint().height() / 8 * 2 + 4);
	


	m_commentInfo->setWordWrap(true) ;

	QLabel *labelWordCountInfo = new QLabel("单词数:");
	m_wordCountInfo = new QLabel;

	QFormLayout *mainLayout = new QFormLayout;
	mainLayout->addRow(labelBookName, m_bookName);
	
	mainLayout->addRow(labelCommentInfo, m_commentInfo);
	mainLayout->addRow(labelWordCountInfo, m_wordCountInfo);
	//mainLayout->addRow(labelVerInfo, m_verInfo);
	// QVBoxLayout *mainLayout = new QVBoxLayout;
	//mainLayout->addWidget(labelBookName,);
	//mainLayout->addStretch();
	//mainLayout->setRowWrapPolicy(QFormLayout::WrapLongRows);

	//mainLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);


	setLayout(mainLayout);
}

TabBookDetail::~TabBookDetail()
{

}


void TabBookDetail::setBookName(const QString &bookName)
{
	m_bookName->setText(bookName);
}
void TabBookDetail::setCommentInfo(const QString &commentInfo)
{
	 m_commentInfo->setText(commentInfo);
}
void TabBookDetail::setWordCountInfo(int wcount)
{
		m_wordCountInfo->setText(QString::number(wcount));
}

void TabBookDetail::clear()
{
	m_bookName->setText("");
	m_commentInfo->setText("");
	m_wordCountInfo->setText("");
}
