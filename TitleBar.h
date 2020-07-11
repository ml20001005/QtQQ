#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include <QFile>

enum ButtonType {
	MIN_BUTTON,
	MIN_MAX_BUTTON,
	ONLY_CLOSE_BUTTON
};

//自定义标题栏
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	void setTitleIcon(QString& filePath);//设置标题栏图标
	void setTitleContent(QString& titleContent);//设置标题栏内容
	void setTitleWidth(int width);//设置标题栏长度
	void setButtonType(enum ButtonType buttonType);//设置标题栏按钮类型

	//保存,获取窗口最大化前窗口的位置和大小
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	void getRestoreInfo(QPoint& point, QSize& size);

private:
	void paintEvent(QPaintEvent* event);//绘图事件
	void mouseDoubleClickEvent(QMouseEvent* event);//鼠标双击事件
	void mousePressEvent(QMouseEvent* event);//鼠标按压事件
	void mouseMoveEvent(QMouseEvent* event);//鼠标移动事件
	void mouseReleaseEvent(QMouseEvent* event);//鼠标释放事件

	void initControl();//初始化控件
	void initConnections();//初始化信号与槽的连接
	void loadStyleSheet(const QString& sheetName);//加载样式表

signals:
	//按钮发射的信号
	void signalButtonMinClicked();//最小化信号
	void signalButtonRestoreClicked();//最大化还原信号
	void signalButtonMaxClicked();//最大化信号
	void signalButtonCloseClicked();//关闭信号

private slots:
	//按钮响应的槽
	void onButtonMinClicked();//最小化槽方法
	void onButtonRestoreClicked();//最大化还原槽方法
	void onButtonMaxClicked();//最大化槽方法
	void onButtonCloseClicked();//关闭槽方法
	
private:
	QLabel* m_pIcon;//标题栏图标
	QLabel* m_pTitleContent;//标题栏内容
	QPushButton* m_pButtonMin;//最小化按钮
	QPushButton* m_pButtonMax;//最大化按钮
	QPushButton* m_pButtonRestore;//最大化还原按钮
	QPushButton* m_pButtonClose;//最大化按钮

	//最大化还原按钮变量(用于保存窗体的位置及大小)
	QPoint m_restorePos;
	QSize m_restoreSize;

	//移动窗口的变量
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;//标题栏内容
	enum ButtonType m_buttonType;//标题栏右上角按钮类型
};
