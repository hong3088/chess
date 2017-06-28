#include <QApplication>
#include <QTextCodec>
#include "Board.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    QApplication app(argc,argv);
    Board board;
    board.show();
    return app.exec();
}
