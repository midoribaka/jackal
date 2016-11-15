#include "MainWin.h"

#include <QFile>
#include <QLayout>
#include <QSizePolicy>

#include "GameScene.h"

MainWin::MainWin(QWidget *_parent) : QMainWindow(_parent)
{
	QFile styleFile(":/Resources/common_stylesheet.qss");
	if (styleFile.open(QFile::ReadOnly))
	{
		setStyleSheet(QLatin1String(styleFile.readAll()));
		styleFile.close();
	}

	QWidget* main_widget = new QWidget();
	QHBoxLayout* main_layout = new QHBoxLayout();
	main_layout->setContentsMargins(padding, padding, padding, padding);
	main_layout->setSpacing(spacing);

	//

	QWidget* some_left = new QWidget();
	view_widget = new QGraphicsView();
	QWidget* some_right = new QWidget();

	some_left->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	view_widget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
	some_right->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	main_layout->addWidget(some_left);
	main_layout->addWidget(view_widget, Qt::AlignCenter);
	main_layout->addWidget(some_right);


	view_widget->setScene(new GameScene(active_screen_h));
	view_widget->setRenderHints(QPainter::Antialiasing| QPainter::SmoothPixmapTransform| QPainter::TextAntialiasing);

	main_widget->setLayout(main_layout);
	setCentralWidget(main_widget);

	setFixedSize(screen_w, screen_h);
}

MainWin::~MainWin()
{

}
