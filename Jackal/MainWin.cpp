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
	QVBoxLayout* main_layout = new QVBoxLayout();
	main_layout->setContentsMargins(padding, padding, padding, padding);
	main_layout->setSpacing(spacing);

	//
	QWidget* top = new QWidget();
	view_widget = new QGraphicsView();
	QWidget* bottom = new QWidget();

	top->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	view_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	bottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	main_layout->addWidget(top);
	main_layout->addWidget(view_widget, Qt::AlignCenter);
	main_layout->addWidget(bottom);

	view_widget->setScene(new GameScene(active_screen_w, view_widget));
	view_widget->setRenderHints(QPainter::Antialiasing| QPainter::SmoothPixmapTransform| QPainter::TextAntialiasing);

	main_widget->setLayout(main_layout);
	setCentralWidget(main_widget);

	setFixedSize(screen_w, screen_h);
}

MainWin::~MainWin()
{

}
