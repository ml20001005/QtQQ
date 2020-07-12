#include "TitleBar.h"

#define BUTTON_WIDTH 27		//按钮宽度
#define BUTTON_HEIGHT 27	//按钮高度
#define TITLE_HEIGHT 27		//标题栏高度

TitleBar::TitleBar(QWidget *parent)
	: QWidget(parent)
	,m_isPressed(false)
	,m_buttonType(MIN_MAX_BUTTON)
{
	initControl();
	initConnections();
	loadStyleSheet("Title");
}

TitleBar::~TitleBar()
{
}

void TitleBar::initControl() {
	//new 出部件
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//设置部件固定大小
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//设置对象名
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonClose->setObjectName("ButtonClose");

	//设置布局
	QHBoxLayout* myLayout = new QHBoxLayout(this);
	myLayout->addWidget(m_pIcon);
	myLayout->addWidget(m_pTitleContent);
	myLayout->addWidget(m_pButtonMin);
	myLayout->addWidget(m_pButtonRestore);
	myLayout->addWidget(m_pButtonMax);
	myLayout->addWidget(m_pButtonClose);

	myLayout->setContentsMargins(5, 0, 0, 0);//设置布局的间隙
	myLayout->setSpacing(0);//设置布局里部件之间的间隙

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);//无边框
}

void TitleBar::initConnections() {
	//连接标题栏所有的信号与槽
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

void TitleBar::setTitleIcon(const QString& filePath) {
	QPixmap titleIcon(filePath);//构造一个位图对象
	m_pIcon->setFixedSize(titleIcon.size());//用位图对象设置标签固定大小
	m_pIcon->setPixmap(titleIcon);//对标签设置位图
}

void TitleBar::setTitleContent(const QString& titleContent) {
	m_pTitleContent->setText(titleContent);//标题栏内容标签设置文本
	m_titleContent = titleContent;//标题栏内容
}

void TitleBar::setTitleWidth(int width) {
	this->setFixedWidth(width);//设置标题栏的长度
}

void TitleBar::setButtonType(enum ButtonType buttonType) {
	m_buttonType = buttonType;//设置按钮类型

	switch (buttonType) {
	case MIN_BUTTON:
		m_pButtonRestore->setVisible(false);
		m_pButtonMax->setVisible(false);
		break;

	case MIN_MAX_BUTTON:
		m_pButtonRestore->setVisible(false);
		break;

	case ONLY_CLOSE_BUTTON:
		m_pButtonMin->setVisible(false);
		m_pButtonMax->setVisible(false);
		m_pButtonRestore->setVisible(false);
		break;

	default:
		break;
	}
}

void TitleBar::saveRestoreInfo(const QPoint& point, const QSize& size) {
	//保存窗口最大化前的坐标及大小
	m_restorePos = point;
	m_restoreSize = size;
}

void TitleBar::getRestoreInfo(QPoint& point, QSize& size) {
	//获取窗口最大化前的坐标及大小
	point = m_restorePos;
	size = m_restoreSize;
}

void TitleBar::paintEvent(QPaintEvent* event) {//绘制标题栏
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);//设置填充规则
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);//添加圆角矩形到绘图路径
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//在画家上设置给定的渲染提示

	if (width() != parentWidget()->width()) {
		this->setFixedWidth(parentWidget()->width());
	}
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* event) {
	//只有存在最大化最小化按钮才有效
	if (m_buttonType == MIN_MAX_BUTTON) {
		if (m_pButtonMin->isVisible()) {
			onButtonMinClicked();
		}
		else {
			onButtonMaxClicked();
		}
	}
	return QWidget::mouseDoubleClickEvent(event);
}

void TitleBar::mousePressEvent(QMouseEvent* event) {
	if (m_buttonType == MIN_MAX_BUTTON) {
		//在窗口最大化时禁止拖动窗口
		if (m_pButtonMax->isVisible()) {
			m_isPressed = true;
			m_startMovePos = event->globalPos();
		}
	}
	else {
		m_isPressed = true;
		m_startMovePos = event->globalPos();
	}
	return QWidget::mousePressEvent(event);
}

void TitleBar::mouseMoveEvent(QMouseEvent* event) {
	if (m_isPressed) {
		QPoint movePoint = event->globalPos() - m_startMovePos;//鼠标先后之间的差值
		QPoint widgetPoint = parentWidget()->pos();//返回父部件的坐标
		m_startMovePos = event->globalPos();//刷新开始移动的坐标
		parentWidget()->move(widgetPoint.x() + movePoint.x(), widgetPoint.y() + movePoint.y());//移动
	}
	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event) {//鼠标释放事件
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

void TitleBar::loadStyleSheet(const QString& sheetName) {//加载样式表
	QFile file(":/Resources/QSS" + sheetName + ".CSS");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		QString styleSheet = this->styleSheet();
		styleSheet += QLatin1String(file.readAll());
		setStyleSheet(styleSheet);
	}	
}

void TitleBar::onButtonMinClicked() {
	emit signalButtonMinClicked();
}

void TitleBar::onButtonMaxClicked() {
	m_pButtonMax->setVisible(false);
	m_pButtonRestore->setVisible(true);
	emit signalButtonMaxClicked();
}

void TitleBar::onButtonRestoreClicked() {
	m_pButtonMax->setVisible(true);
	m_pButtonRestore->setVisible(false);
	emit signalButtonRestoreClicked();
}

void TitleBar::onButtonCloseClicked() {
	emit signalButtonCloseClicked();
}