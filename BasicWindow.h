#pragma once

#include <QDialog>
#include "TitleBar.h"
#include <QFile>
#include <QStyleOption>
#include <QApplication>
#include <QDesktopWidget>

class BasicWindow : public QDialog
{
	Q_OBJECT

public:
	BasicWindow(QWidget *parent = nullptr);
	virtual ~BasicWindow();

	void loadStyleSheet(const QString& sheetName);//������ʽ��
	QPixmap getRoundImage(const QPixmap& src, QPixmap& mask, QSize masksize = QSize(0, 0));//��ȡԲͷ��

private:
	void initBackGroundColor();//��ʼ������

protected:
	void paintEvent(QPaintEvent*);//��ͼ�¼�
	void mousePressEvent(QMouseEvent* event);//��갴ѹ�¼�
	void mouseMoveEvent(QMouseEvent* event);//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent*);//����ͷ��¼�

protected:
	void initTitleBar(enum ButtonType buttontype = MIN_BUTTON);
	void setTitleBarTitle(const QString& title, const QString& icon = "");

public slots:
	void onShowClose(bool);
	void onShowMin(bool);
	void onShowHide(bool);
	void onShowNormal(bool);
	void onShowQuit(bool);
	void onShowSignalSkinChanged(const QColor& color);

	void onButtonMinClicked();
	void onButtonRestoreClicked();
	void onButtonMaxClicked();
	void onButtonCloseClicked();

protected:
	QPoint m_mousePoint;//���λ��
	bool m_mousePressed;//����Ƿ���
	QColor m_colorBackGround;//����ɫ
	QString m_styleName;//��ʽ�ļ�����
	TitleBar* _titleBar;//������
};
