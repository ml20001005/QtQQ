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

//�Զ��������
class TitleBar : public QWidget
{
	Q_OBJECT

public:
	TitleBar(QWidget *parent = nullptr);
	~TitleBar();

	void setTitleIcon(QString& filePath);//���ñ�����ͼ��
	void setTitleContent(QString& titleContent);//���ñ���������
	void setTitleWidth(int width);//���ñ���������
	void setButtonType(enum ButtonType buttonType);//���ñ�������ť����

	//����,��ȡ�������ǰ���ڵ�λ�úʹ�С
	void saveRestoreInfo(const QPoint& point, const QSize& size);
	void getRestoreInfo(QPoint& point, QSize& size);

private:
	void paintEvent(QPaintEvent* event);//��ͼ�¼�
	void mouseDoubleClickEvent(QMouseEvent* event);//���˫���¼�
	void mousePressEvent(QMouseEvent* event);//��갴ѹ�¼�
	void mouseMoveEvent(QMouseEvent* event);//����ƶ��¼�
	void mouseReleaseEvent(QMouseEvent* event);//����ͷ��¼�

	void initControl();//��ʼ���ؼ�
	void initConnections();//��ʼ���ź���۵�����
	void loadStyleSheet(const QString& sheetName);//������ʽ��

signals:
	//��ť������ź�
	void signalButtonMinClicked();//��С���ź�
	void signalButtonRestoreClicked();//��󻯻�ԭ�ź�
	void signalButtonMaxClicked();//����ź�
	void signalButtonCloseClicked();//�ر��ź�

private slots:
	//��ť��Ӧ�Ĳ�
	void onButtonMinClicked();//��С���۷���
	void onButtonRestoreClicked();//��󻯻�ԭ�۷���
	void onButtonMaxClicked();//��󻯲۷���
	void onButtonCloseClicked();//�رղ۷���
	
private:
	QLabel* m_pIcon;//������ͼ��
	QLabel* m_pTitleContent;//����������
	QPushButton* m_pButtonMin;//��С����ť
	QPushButton* m_pButtonMax;//��󻯰�ť
	QPushButton* m_pButtonRestore;//��󻯻�ԭ��ť
	QPushButton* m_pButtonClose;//��󻯰�ť

	//��󻯻�ԭ��ť����(���ڱ��洰���λ�ü���С)
	QPoint m_restorePos;
	QSize m_restoreSize;

	//�ƶ����ڵı���
	bool m_isPressed;
	QPoint m_startMovePos;

	QString m_titleContent;//����������
	enum ButtonType m_buttonType;//���������Ͻǰ�ť����
};
