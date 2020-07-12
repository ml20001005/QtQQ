#include "TitleBar.h"

#define BUTTON_WIDTH 27		//��ť���
#define BUTTON_HEIGHT 27	//��ť�߶�
#define TITLE_HEIGHT 27		//�������߶�

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
	//new ������
	m_pIcon = new QLabel(this);
	m_pTitleContent = new QLabel(this);

	m_pButtonMin = new QPushButton(this);
	m_pButtonMax = new QPushButton(this);
	m_pButtonRestore = new QPushButton(this);
	m_pButtonClose = new QPushButton(this);

	//���ò����̶���С
	m_pButtonMin->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonMax->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonRestore->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));
	m_pButtonClose->setFixedSize(QSize(BUTTON_WIDTH, BUTTON_HEIGHT));

	//���ö�����
	m_pTitleContent->setObjectName("TitleContent");
	m_pButtonMin->setObjectName("ButtonMin");
	m_pButtonMax->setObjectName("ButtonMax");
	m_pButtonRestore->setObjectName("ButtonRestore");
	m_pButtonClose->setObjectName("ButtonClose");

	//���ò���
	QHBoxLayout* myLayout = new QHBoxLayout(this);
	myLayout->addWidget(m_pIcon);
	myLayout->addWidget(m_pTitleContent);
	myLayout->addWidget(m_pButtonMin);
	myLayout->addWidget(m_pButtonRestore);
	myLayout->addWidget(m_pButtonMax);
	myLayout->addWidget(m_pButtonClose);

	myLayout->setContentsMargins(5, 0, 0, 0);//���ò��ֵļ�϶
	myLayout->setSpacing(0);//���ò����ﲿ��֮��ļ�϶

	m_pTitleContent->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	this->setFixedHeight(TITLE_HEIGHT);
	this->setWindowFlags(Qt::FramelessWindowHint);//�ޱ߿�
}

void TitleBar::initConnections() {
	//���ӱ��������е��ź����
	connect(m_pButtonMin, SIGNAL(clicked()), this, SLOT(onButtonMinClicked()));
	connect(m_pButtonRestore, SIGNAL(clicked()), this, SLOT(onButtonRestoreClicked()));
	connect(m_pButtonMax, SIGNAL(clicked()), this, SLOT(onButtonMaxClicked));
	connect(m_pButtonClose, SIGNAL(clicked()), this, SLOT(onButtonCloseClicked()));
}

void TitleBar::setTitleIcon(const QString& filePath) {
	QPixmap titleIcon(filePath);//����һ��λͼ����
	m_pIcon->setFixedSize(titleIcon.size());//��λͼ�������ñ�ǩ�̶���С
	m_pIcon->setPixmap(titleIcon);//�Ա�ǩ����λͼ
}

void TitleBar::setTitleContent(const QString& titleContent) {
	m_pTitleContent->setText(titleContent);//���������ݱ�ǩ�����ı�
	m_titleContent = titleContent;//����������
}

void TitleBar::setTitleWidth(int width) {
	this->setFixedWidth(width);//���ñ������ĳ���
}

void TitleBar::setButtonType(enum ButtonType buttonType) {
	m_buttonType = buttonType;//���ð�ť����

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
	//���洰�����ǰ�����꼰��С
	m_restorePos = point;
	m_restoreSize = size;
}

void TitleBar::getRestoreInfo(QPoint& point, QSize& size) {
	//��ȡ�������ǰ�����꼰��С
	point = m_restorePos;
	size = m_restoreSize;
}

void TitleBar::paintEvent(QPaintEvent* event) {//���Ʊ�����
	QPainter painter(this);
	QPainterPath pathBack;
	pathBack.setFillRule(Qt::WindingFill);//����������
	pathBack.addRoundedRect(QRect(0, 0, width(), height()), 3, 3);//���Բ�Ǿ��ε���ͼ·��
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);//�ڻ��������ø�������Ⱦ��ʾ

	if (width() != parentWidget()->width()) {
		this->setFixedWidth(parentWidget()->width());
	}
}

void TitleBar::mouseDoubleClickEvent(QMouseEvent* event) {
	//ֻ�д��������С����ť����Ч
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
		//�ڴ������ʱ��ֹ�϶�����
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
		QPoint movePoint = event->globalPos() - m_startMovePos;//����Ⱥ�֮��Ĳ�ֵ
		QPoint widgetPoint = parentWidget()->pos();//���ظ�����������
		m_startMovePos = event->globalPos();//ˢ�¿�ʼ�ƶ�������
		parentWidget()->move(widgetPoint.x() + movePoint.x(), widgetPoint.y() + movePoint.y());//�ƶ�
	}
	return QWidget::mouseMoveEvent(event);
}

void TitleBar::mouseReleaseEvent(QMouseEvent* event) {//����ͷ��¼�
	m_isPressed = false;
	return QWidget::mouseReleaseEvent(event);
}

void TitleBar::loadStyleSheet(const QString& sheetName) {//������ʽ��
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