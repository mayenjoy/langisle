#include "stdafx.h"
#include "UIGDialog.h"

UIGDialog::UIGDialog(QWidget *parent)
	: QDialog(parent)
{

	QLabel *labelVer = new QLabel("版本号:");
	lineEditVer = new QLineEdit("0.3.3.520");
	labelVer->setBuddy(lineEditVer);


	QLabel *labelDownurl = new QLabel("下载地址:");
	lineDownurl = new QLineEdit("http://down.langisle.com/003/v0.3.3.520.patch.zip");
	labelDownurl->setBuddy(lineDownurl);


	QLabel *labelUpdateNotes = new QLabel("更新日志:");
	textEditUpdateNotes = new QTextEdit;
	labelUpdateNotes->setBuddy(textEditUpdateNotes);
	textEditUpdateNotes->setPlainText(
"<html>\
<body>\
<pre>\n\
\n\
</pre>\n\
</body>\
</html>");

	webView = new QWebView(this);
	webView->setHtml(textEditUpdateNotes->toPlainText());
	QPushButton *previewButton = createButton("预览", SLOT(preview()));

	QLabel *labelOutputDir = new QLabel("生成位置:");
	lineOutputDir = new QLineEdit(Global::makePathStr(""));
	QPushButton *buttonOutputDir = createButton("浏览...", SLOT(browse()));
	labelOutputDir->setBuddy(lineOutputDir);

	QPushButton *buttonGenerate = createButton("生成", SLOT(generate()));

	QHBoxLayout *buttonsLayout = new QHBoxLayout;
	buttonsLayout->addStretch();
	buttonsLayout->addWidget(buttonGenerate);


	QGridLayout *mainLayout = new QGridLayout;
	mainLayout->addWidget(labelVer, 0, 0);
	mainLayout->addWidget(lineEditVer, 0, 1, 1, 2);

	mainLayout->addWidget(labelDownurl, 1, 0);
	mainLayout->addWidget(lineDownurl, 1, 1, 1, 2);

	mainLayout->addWidget(labelUpdateNotes, 2, 0);
	mainLayout->addWidget(textEditUpdateNotes, 2, 1, 1, 2);

	mainLayout->addWidget(previewButton, 3, 0);
	mainLayout->addWidget(webView, 3, 1, 1, 2);		

	mainLayout->addWidget(labelOutputDir, 4, 0);
	mainLayout->addWidget(lineOutputDir, 4, 1, 1, 1);
	mainLayout->addWidget(buttonOutputDir, 4, 2, 1, 1);
	mainLayout->addLayout(buttonsLayout, 5, 0, 1, 3);

	setLayout(mainLayout);

	resize(700, 400);
}

UIGDialog::~UIGDialog()
{

}

void UIGDialog::browse()
{
	QString directory = QFileDialog::getExistingDirectory(this,
		tr("Find Files"), Global::makePathStr(""));

	if (!directory.isEmpty()) {
		lineOutputDir->setText(directory);
	}
}

QPushButton *UIGDialog::createButton(const QString &text, const char *member)
{
	QPushButton *button = new QPushButton(text);
	connect(button, SIGNAL(clicked()), this, member);
	return button;
}

void UIGDialog::generate()
{
	QFile file(QDir::convertSeparators(lineOutputDir->text() + "/verinfo.xml"));
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::warning(this, "错误", QString("打个文件%1失败.").arg(file.fileName()));
	}
	QXmlStreamWriter xsw(&file);
	xsw.setAutoFormatting(true);
	xsw.setAutoFormattingIndent(4);
	xsw.writeStartDocument();
	xsw.writeStartElement("verinfo");

	xsw.writeTextElement("vernumber", lineEditVer->text());
	xsw.writeTextElement("downurl", lineDownurl->text());
	//xsw.writeTextElement("updateNotes", textEditUpdateNotes->toPlainText());

	xsw.writeStartElement("updateNotes");
	//xsw.writeCharacters(textEditUpdateNotes->toPlainText());
	xsw.writeCDATA(textEditUpdateNotes->toPlainText());
	xsw.writeEndElement();

	
	xsw.writeEndElement();
	xsw.writeEndDocument ();
	file.close();

	QMessageBox::information(this, "", "OK");
}

void UIGDialog::preview()
{
	webView->setHtml(textEditUpdateNotes->toPlainText());
}
