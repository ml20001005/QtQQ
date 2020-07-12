#include "BasicWindow.h"

BasicWindow::BasicWindow(QWidget *parent)
	: QDialog(parent)
{
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground, true);
}

BasicWindow::~BasicWindow()
{
}

void BasicWindow::onShowSignalSkinChanged(const QColor& color)
{
	m_colorBackGround = color;
	loadStyleSheet(m_styleName);
}

void BasicWindow::setTitleBarTitle(const QString& title, const QString& icon) {
	_titleBar->setTitleIcon(icon);
	_titleBar->setTitleContent(title);
}

void BasicWindow::initTitleBar(enum ButtonType buttontype) {
	_titleBar = new TitleBar(this);
	_titleBar->setButtonType(buttontype);
	_titleBar->move(0, 0);

	connect(_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
	connect(_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
	connect(_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
	connect(_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

void BasicWindow::loadStyleSheet(const QString& sheetName) {
	m_styleName = sheetName;
	QFile file(":/Resources/QSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);

	if (file.isOpen()) {
		setStyleSheet("");
		QString qsstyleSheet = QLatin1String(file.readAll());

		//获取用户当前的RGB值
		QString r = QString::number(m_colorBackGround.red());
		QString g = QString::number(m_colorBackGround.green());
		QString b = QString::number(m_colorBackGround.blue());

		qsstyleSheet += QString("QWidget[titleskin=true]\
								{background-color:rgb(%1,%2,%3,100);\
								border-top-left-radius:4px;}\
								QWidget[bottomskin=true]\
								{border-top:1px solid rgba(%1,%2,%3,100);\
								background-color:rgba(%1,%2,%3,50);\
								border-bottom-left-radius:4px;\
								border-bottom-right-radius:4px;}")
								.arg(r).arg(g).arg(b);
		setStyleSheet(qsstyleSheet);
	}
	file.close();
}

void BasicWindow::initBackGroundColor() {
	QStyleOption opt;
	opt.init(this);

	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);	
}

void BasicWindow::paintEvent(QPaintEvent* event) {
	initBackGroundColor();
	QDialog::paintEvent(event);
}

QPixmap BasicWindow::getRoundImage(const QPixmap& src, QPixmap& mask, QSize maskSize) {
	if (maskSize == QSize(0, 0)) {
		maskSize = mask.size();
	}
	else {
		mask.scaled(maskSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	}
	QImage resultImage(maskSize, QImage::Format_ARGB32_Premultiplied);
	QPainter painter(&resultImage);
	painter.setCompositionMode(QPainter::CompositionMode_Source);
	painter.fillRect(resultImage.rect(), Qt::transparent);
	painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
	painter.drawPixmap(0, 0, mask);
	painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
	painter.drawPixmap(0, 0, src.scaled(Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	return QPixmap::fromImage(resultImage);
}

void BasicWindow::onShowClose(bool) {
	close();
}

void BasicWindow::onShowMin(bool) {
	showMinimized();
}

void BasicWindow::onShowHide(bool) {
	hide();
}

void BasicWindow::onShowNormal(bool) {
	show();
	activateWindow();
}

void BasicWindow::onShowQuit(bool) {
	QApplication::quit();
}

void BasicWindow::mouseMoveEvent(QMouseEvent* e) {
	if (m_mousePressed && (e->buttons() & Qt::LeftButton)) {
		move(e->globalPos() - m_mousePoint);
		e->accept();
	}
}

void BasicWindow::mousePressEvent(QMouseEvent* e) {
	if (e->button() == Qt::LeftButton) {
		m_mousePressed = true;
		m_mousePoint = e->globalPos() - pos();
		accept();
	}
}

void BasicWindow::mouseReleaseEvent(QMouseEvent*) {
	m_mousePressed = false;
}

void BasicWindow::onButtonMinClicked() {
	if (Qt::Tool == (windowFlags() & Qt::Tool)) {
		hide();
	}
	else {
		showMinimized();
	}
}

void BasicWindow::onButtonRestoreClicked() {
	QPoint windowPos;
	QSize windowSize;
	_titleBar->getRestoreInfo(windowPos, windowSize);
	setGeometry(QRect(windowPos, windowSize));
}

void BasicWindow::onButtonMaxClicked() {
	_titleBar->saveRestoreInfo(pos(), QSize(width(), height()));
	QRect desktopRect = QApplication::desktop()->availableGeometry();
	QRect factRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3,
		desktopRect.width() + 6, desktopRect.height() + 6);
	setGeometry(factRect);
}

void BasicWindow::onButtonCloseClicked() {
	close();
}