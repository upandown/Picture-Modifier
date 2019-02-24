#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include <qlabel.h>
//connect(sender,   signal,
//receiver, slot);
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGuiApplication1 w;
	w.setWindowTitle("Picture processor");
    // connect

	w.show();
	return a.exec();
}

