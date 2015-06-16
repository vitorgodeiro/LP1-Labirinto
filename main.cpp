/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#include "mainwindow.h"
#include "game.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    Game::getInstance().setMainWindow(&w);
    w.show();

    return a.exec();
}
