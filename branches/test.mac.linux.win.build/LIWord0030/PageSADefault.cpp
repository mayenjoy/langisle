#include "stdafx.h"
#include "PageSADefault.h"
#include "qchartviewer.h"
#include "BCMgr.h"

PageSADefault::PageSADefault(QWidget *parent)
	: QFrame(parent)
{
	swc0Y = 0;

	QVBoxLayout *mainLayout = new QVBoxLayout;
	setLayout(mainLayout);

	QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);


	QGridLayout *topLayout = new QGridLayout;


	QLabel *labelTop = new QLabel("基本数据:");
	
	swcountCheckBox	= new QCheckBox("学习个数(总)");
	snwcountCheckBox = new QCheckBox("新学个数");
	rscountCheckBox	= new QCheckBox("复习个数");
	kncountCheckBox	= new QCheckBox("掌握个数");
	sTimesCheckBox	= new QCheckBox("学习时间");

	swcountCheckBox->setCheckState(Qt::Checked);
	sTimesCheckBox->setCheckState(Qt::Checked);
	// 	snwcountCheckBox->setCheckState(Qt::Checked);
	// 	rscountCheckBox->setCheckState(Qt::Checked);
	// 	kncountCheckBox->setCheckState(Qt::Checked);

	stwcountCheckBox	= new QCheckBox("学习单词总数");
	stnwcountCheckBox = new QCheckBox("掌握单词总数");
	
	// 	QCheckBox *swcountCheckBox	= new QCheckBox("学习时间");

	stwcountCheckBox->setCheckState(Qt::Checked);
	stnwcountCheckBox->setCheckState(Qt::Checked);

	QLabel *labelDate = new QLabel("统计日期:");
	QCalendarWidget *calendarWidget = new QCalendarWidget;
	sdateEdit = new QDateEdit(QDate::currentDate().addDays(-7));
	edateEdit = new QDateEdit(QDate::currentDate());
	sdateEdit->setCalendarWidget(calendarWidget);
	edateEdit->setCalendarWidget(calendarWidget);
	sdateEdit->setCalendarPopup(true);
	edateEdit->setCalendarPopup(true);
	sdateEdit->setSizePolicy(sizePolicy);
	edateEdit->setSizePolicy(sizePolicy);

	QPushButton *makeButton  = new QPushButton("生成");

	
	labelTop->setSizePolicy(sizePolicy);
	swcountCheckBox->setSizePolicy(sizePolicy);
	snwcountCheckBox->setSizePolicy(sizePolicy);
	rscountCheckBox->setSizePolicy(sizePolicy);
	kncountCheckBox->setSizePolicy(sizePolicy);
	

	topLayout->addWidget(labelTop, 0, 1, 1, 1);
	topLayout->addWidget(swcountCheckBox, 0, 2, 1, 1);
	topLayout->addWidget(snwcountCheckBox, 0, 3, 1, 1);
	topLayout->addWidget(rscountCheckBox, 0, 4, 1, 1);
	topLayout->addWidget(kncountCheckBox, 0, 5, 1, 1);
	topLayout->addWidget(sTimesCheckBox, 0, 6, 1, 1);

	topLayout->addWidget(new QLabel("总计数据"), 1, 1, 1, 1);
	topLayout->addWidget(stwcountCheckBox, 1, 2, 1, 1);
	topLayout->addWidget(stnwcountCheckBox, 1, 3, 1, 1);

	topLayout->addWidget(labelDate, 2, 1, 1, 1);
	topLayout->addWidget(sdateEdit, 2, 2, 1, 1);
	topLayout->addWidget(edateEdit, 2, 3, 1, 1);

	topLayout->addWidget(makeButton, 3, 1, 1, 1);


	scrollArea = new QScrollArea;
	scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);

	viewer = new QChartViewer;
	scrollArea->setWidget(viewer);

	//viewer->setSizePolicy(sizePolicy);
	//viewer->setFrameShape(QFrame::StyledPanel);
 	//QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
// 	sizePolicy.setHorizontalStretch(0);
// 	sizePolicy.setVerticalStretch(0);
// 	sizePolicy.setHeightForWidth(viewer->sizePolicy().hasHeightForWidth());
 	//viewer->setSizePolicy(sizePolicy1);

	mainLayout->addLayout(topLayout);
	mainLayout->addWidget(scrollArea);

	//mainLayout->addWidget(new QLabel("test2"));
	

// 	resize(500, 320);
// 	updateChartViewer();
	//testViewer();

	connect(makeButton, SIGNAL(clicked()), this, SLOT(makeClick()));

	setLicenseCode("SXZVFNRN9MZ9L8LGA0E2B1BB");
	makeClick();
}

PageSADefault::~PageSADefault()
{
	clear();
}



void PageSADefault::resizeEvent ( QResizeEvent * event )
{
	DBG(qDebug() << "PageSADefault::resizeEvent" << event->size() << viewer->size());
	//updateChartViewer();
	//updateData();
	updateChart();

}


void PageSADefault::makeClick()
{



// 	QDate sd = sdateEdit->date();
// 	QDate ed = edateEdit->date();
// 	DBG(qDebug() << sd.daysTo(ed) + 1);
// 	for (QDate td = sd;td <= ed; td = td.addDays(1))
// 	{
// 		DBG(qDebug() << td);
// 	}
// 	

	updateData();
	updateChart();
	//testViewer();
}

void PageSADefault::clear()
{
	if (swc0Y)
	{
		delete []swc0Y;
		delete []snwc0Y;
		delete []rsc0Y;
		delete []knc0Y;
		delete []stw0Y;
		delete []stnw0Y;
		delete []sTimes0Y;
	}

}

void PageSADefault::updateChart()
{
	const int oset = 12;

	int height = scrollArea->size().height() - oset;
	int width = scrollArea->size().width() - oset;

	XYChart *c = new XYChart(width, height);

	c->setBackground(c->linearGradientColor(0, 0, 0, 100, 0x99ccff, 0xffffff),
		0x888888);
	c->setRoundedFrame();
	c->setDropShadow();

	// Add a title using 18 pts Times New Roman Bold Italic font. #Set top margin to
	// 16 pixels.

	// 	c->addTitle(QString("线损率曲线").toLocal8Bit().constData(), "simsun.ttf", 18)->setMargin(0, 0, 16,
	// 		0);
	c->setDefaultFonts("simsun.ttc", "simsun.ttc", "simsun.ttc", "simsun.ttc");
	c->addTitle(QString("学习数据统计").toUtf8().constData(), "simsun.ttc", 18)->setMargin(0, 0, 16,
		0);
	// Set the plotarea at (60, 80) and of 510 x 275 pixels in size. Use transparent
	// border and dark grey (444444) dotted grid lines
	PlotArea *plotArea = c->setPlotArea(60, 80, width - 80, height - 120, -1, -1, Chart::Transparent,
		c->dashLineColor(0x444444, 0x000101), -1);

	// Add a legend box where the top-center is anchored to the horizontal center of
	// the plot area at y = 45. Use horizontal layout and 10 points Arial Bold font,
	// and transparent background and border.
	LegendBox *legendBox = c->addLegend(plotArea->getLeftX() + plotArea->getWidth() /
		2, 45, false, "arialbd.ttf", 10);
	legendBox->setAlignment(Chart::TopCenter);
	legendBox->setBackground(Chart::Transparent, Chart::Transparent);
	legendBox->setFontStyle("simsun.ttc");

	// Set x-axis tick density to 75 pixels and y-axis tick density to 30 pixels.
	// ChartDirector auto-scaling will use this as the guidelines when putting ticks
	// on the x-axis and y-axis.
	c->yAxis()->setTickDensity(20);
	c->xAxis()->setTickDensity(35);
	c->xAxis()->setDateScale("{value|mm/dd}");
	// Set all axes to transparent
	c->xAxis()->setColors(Chart::Transparent);
	c->yAxis()->setColors(Chart::Transparent);

	// Set the x-axis margins to 15 pixels, so that the horizontal grid lines can
	// extend beyond the leftmost and rightmost vertical grid lines
	c->xAxis()->setMargin(15, 15);

	// Set axis label style to 8pts Arial Bold
	// 	c->xAxis()->setLabelStyle("arialbd.ttf", 8);
	// 	c->yAxis()->setLabelStyle("arialbd.ttf", 8);
	// 	c->yAxis2()->setLabelStyle("arialbd.ttf", 8);

	// Add axis title using 10pts Arial Bold Italic font
	//c->yAxis()->setTitle(QString("学习次数").toUtf8().constData(), "simsun.ttc", 10);


	if (bswcount)
	{
		LineLayer *layer1 = c->addLineLayer();
		//layer1->setDataLabelStyle("simsun.ttc", 18);
		layer1->addDataSet(DoubleArray(swc0Y, dSize), 0xff1111, QString("学习个数(总)").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}
	// 	snwc0Y[i] = Chart::NoValue;
	// 	rsc0Y[i] = Chart::NoValue;
	// 	knc0Y[i] = Chart::NoValue;
	if (bsnwcount)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(snwc0Y, dSize), 0x11ff11, QString("新学个数").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}
	if (brscount)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(rsc0Y, dSize), 0x1111ff, QString("复习个数").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}

	if (bsTimes0Y)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(sTimes0Y, dSize), 0x99ff11, QString("学习时间(单位:分钟)").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}

	if (bkncount)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(knc0Y, dSize), 0xffff11, QString("掌握个数").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}

	if (bstwcount)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(stw0Y, dSize), 0x11ffff, QString("学习单词总数").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}
	if (bstnwcount)
	{
		LineLayer *layer1 = c->addLineLayer();
		layer1->addDataSet(DoubleArray(stnw0Y, dSize), 0xff11ff, QString("掌握单词总数").toUtf8().constData())->setDataSymbol(Chart::GlassSphere2Shape, 11);
		layer1->setXData(data1Start, data1End);
		layer1->setLineWidth(3);
	}

	viewer->setChart(c);
	viewer->setImageMap(c->getHTMLImageMap("clickable", "",
		QString("title='{x|yyyy/mm/dd/}{dataSetName}:{value}'").toUtf8().constData()));

	delete c;
}
void PageSADefault::updateData()
{
	bswcount = swcountCheckBox->checkState() == Qt::Checked;
	bsnwcount = snwcountCheckBox->checkState() == Qt::Checked;
	brscount = rscountCheckBox->checkState() == Qt::Checked;
	bkncount = kncountCheckBox->checkState() == Qt::Checked;
	bsTimes0Y = sTimesCheckBox->checkState() == Qt::Checked;

	bstwcount = stwcountCheckBox->checkState() == Qt::Checked;
	bstnwcount = stnwcountCheckBox->checkState() == Qt::Checked;

	QDate sd = sdateEdit->date();
	QDate ed = edateEdit->date();

	dSize = sd.daysTo(ed) + 1;
	if (dSize < 2)
		return;

	clear();

	swc0Y = new double[dSize];
	snwc0Y = new double[dSize];
	rsc0Y = new double[dSize];
	knc0Y = new double[dSize];
	stw0Y = new double[dSize];
	stnw0Y = new double[dSize];
	sTimes0Y = new double[dSize];

	for (int i =0; i < dSize; i++)
	{
		swc0Y[i] = Chart::NoValue;
		snwc0Y[i] = Chart::NoValue;
		rsc0Y[i] = Chart::NoValue;
		knc0Y[i] = Chart::NoValue;
		stw0Y[i] = Chart::NoValue;
		stnw0Y[i] = Chart::NoValue;
		sTimes0Y[i] = Chart::NoValue;
	}

	data1Start = Chart::chartTime(sd.year(), sd.month(), sd.day());
	data1End = Chart::chartTime(ed.year(), ed.month(), ed.day());


	QSqlDatabase *db = g_pbcmgr->getUDataDB()->getdb();
	QSqlQuery query(*db);
	query.prepare("select date(begintime) as dg, sum(swcount) as swc, sum(snwcount) as snwc, sum(rscount) as rsc, sum(kncount) as knc, max(stwcount) as stwc, max(stnwcount) as stnwc, sum(stime) as st  from tlog where begintime >= :bt and endtime <= :et group by dg");
	query.bindValue(":bt", sdateEdit->minimumDateTime());
	query.bindValue(":et", edateEdit->maximumDateTime());
	if (!query.exec())
	{
		DBG(qDebug() << "PageSADefault::updateDate" << query.lastError());
		return;
	}

	while (query.next())
	{
// 		QStringList dateSL = query.record().value(0).toString().split('-');
// 		DBG(qDebug() << dateSL);
		int offset = sd.daysTo(query.record().value(0).toDate());
		if (offset >= 0)
		{
			swc0Y[offset] = (double)query.record().value(1).toInt();
			snwc0Y[offset] = (double)query.record().value(2).toInt();
			rsc0Y[offset] = (double)query.record().value(3).toInt();
			knc0Y[offset] = (double)query.record().value(4).toInt();
			sTimes0Y[offset] = (double)(query.record().value(7).toInt() / 60);

			stw0Y[offset] = (double)query.record().value(5).toInt();
			stnw0Y[offset] = (double)query.record().value(6).toInt();
		}


	}

	
}

