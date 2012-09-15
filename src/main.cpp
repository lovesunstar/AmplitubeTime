#include <QApplication>
#include "mainwindow.h"
#include <iostream>
//
int main(int argc, char ** argv)
{
	using namespace std;
	
	QApplication app( argc, argv );

	QString locale = QLocale::system().name();
	QTranslator translator;
	translator.load(":tr/" + locale);
	app.installTranslator(&translator);

	MainWindow win;
	win.setStyle(new QCleanlooksStyle);
	//win.setStyle(new QPlastiqueStyle);
	win.show(); 
	app.connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
	
	return app.exec();
}
