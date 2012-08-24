#include "stdafx.h"
#include "DlgUpdate.h"

DlgUpdate::DlgUpdate(QWidget *parent)
	: QDialog(parent)
{
	setWindowTitle("更新程序");

	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QHBoxLayout *buttonLayout = new QHBoxLayout;
	QPushButton *okButton = new QPushButton("更新");
	QPushButton *cancelButton = new QPushButton("以后再说");
	buttonLayout->addStretch();
	buttonLayout->addWidget(okButton);
	buttonLayout->addWidget(cancelButton);

	webView = new QWebView;

	QFrame *line = new QFrame;
	line->setFrameShape(QFrame::HLine);
	line->setFrameShadow(QFrame::Sunken);

	mainLayout->addWidget(webView);
	mainLayout->addWidget(line);
	mainLayout->addLayout(buttonLayout);
	
	connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

	resize(480, 320);
	
}

DlgUpdate::~DlgUpdate()
{

}


void DlgUpdate::setHtml(const QString &html)
{
	webView->setHtml(html);
}
