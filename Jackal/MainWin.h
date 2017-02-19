#ifndef MAINWIN_H
#define MAINWIN_H

#include <QtWidgets/QMainWindow>
#include <QGraphicsView>

class MainWin : public QMainWindow
{
	Q_OBJECT

	const size_t screen_w = 768;
	const size_t screen_h = 1024;

	const size_t padding = 6;
	const size_t spacing = 6;

	const size_t active_screen_w = screen_w - 2*padding;
	const size_t active_screen_h = screen_h - 2*padding;
public:
	MainWin(QWidget *_parent = 0);
	~MainWin();

	QGraphicsView* view_widget;
};

#endif // MAINWIN_H
