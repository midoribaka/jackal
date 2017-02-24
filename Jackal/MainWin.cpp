#include "MainWin.h"

#include <QFile>
#include <QLayout>
#include <QSizePolicy>
#include <QPlainTextEdit>

#include <functional>

#include "GameScene.h"

#ifdef _DEBUG
QPlainTextEdit* MainWin::m_dviewer = nullptr;

void MainWin::message_handler(QtMsgType _msg_type, const QMessageLogContext& _context, const QString& _msg)
{
	QByteArray local_msg = _msg.toLocal8Bit();
	switch (_msg_type)
	{
	case QtDebugMsg:
		m_dviewer->appendPlainText(_msg);
		break;
	case QtInfoMsg:
		fprintf(stderr, "Info: %s (%s:%u, %s)\n", local_msg.constData(), _context.file, _context.line, _context.function);
		break;
	case QtWarningMsg:
		fprintf(stderr, "Warning: %s (%s:%u, %s)\n", local_msg.constData(), _context.file, _context.line, _context.function);
		break;
	case QtCriticalMsg:
		fprintf(stderr, "Critical: %s (%s:%u, %s)\n", local_msg.constData(), _context.file, _context.line, _context.function);
		break;
	case QtFatalMsg:
		fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", local_msg.constData(), _context.file, _context.line, _context.function);
		abort();
	}
}

#endif // DEBUG

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

	top->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	view_widget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);

	main_layout->addWidget(top);
	main_layout->addWidget(view_widget, Qt::AlignCenter);

#ifdef _DEBUG
	m_dviewer = new QPlainTextEdit();
	m_dviewer->setReadOnly(true);

	QPalette p = m_dviewer->palette();
	p.setColor(QPalette::Active, QPalette::Base, Qt::black);
	p.setColor(QPalette::Inactive, QPalette::Base, Qt::black);
	p.setColor(QPalette::Text, QColor(0,157,169));
	m_dviewer->setPalette(p);
	m_dviewer->setFont(QFont("Consolas",15,1));

	m_dviewer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	qInstallMessageHandler(&MainWin::message_handler);
	main_layout->addWidget(m_dviewer);
#else
	QWidget* bottom = new QWidget();
	bottom->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	main_layout->addWidget(bottom);
#endif // DEBUG

	view_widget->setScene(new GameScene(active_screen_w, view_widget));
	view_widget->setRenderHints(QPainter::Antialiasing| QPainter::SmoothPixmapTransform| QPainter::TextAntialiasing);

	main_widget->setLayout(main_layout);
	setCentralWidget(main_widget);

	setFixedSize(screen_w, screen_h);
}

MainWin::~MainWin()
{

}