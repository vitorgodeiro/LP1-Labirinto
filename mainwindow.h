/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "game.h"
#include <QPushButton>
#include "digitalclock.h"
#include <QScrollArea>
#include <QLabel>

class Game;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class Game;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QPushButton *getButtonShowPath() const;
    QPushButton *getButtonRestart() const;
    DigitalClock *getDisplayTime() const;
    QScrollArea *getViewArea() const;
    QLabel *getPontosLabel() const;

private slots:
    void on_actionControle_triggered();


    void on_pushButton_Restart_clicked();

    void on_pushButton_showPath_clicked(bool checked);

    void on_pushButton_3_clicked();

    void on_actionSobre_o_jogo_triggered();

    void on_pushButton_9_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_12_clicked();

    void on_modoTreinar_clicked();

    void on_menu_principal_01_clicked();

    void on_menu_principal_02_clicked();

    void on_modoCompetir_clicked();

    void on_modoMultiplayer_clicked();

    void on_modoCarreira_clicked();

    void on_menu_principal_2_clicked();

    void on_menu_pincipal_4_clicked();

    void on_menu_dificuldade_clicked();

    void on_voltar_clicked();

    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;

    /**
     * @brief Mostra uma mensagem de confirmacao para abandonar o jogo atual
     * @return true se o usuario respondeu sim, senao retorna false.
     */
    bool exitQuestion();
};

#endif // MAINWINDOW_H
