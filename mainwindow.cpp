#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Maze RL");

    Game& game_ = Game::getInstance();

    ui->scrollArea->setWidget(&game_);
    ui->lcdTime->show();
    game_.setFocusPolicy(Qt::StrongFocus);
    game_.setFocus();

    //game_.start();
}



MainWindow::~MainWindow()
{
    delete ui;
}

QPushButton *MainWindow::getButtonShowPath() const
{
    return ui->pushButton_showPath;
}

QPushButton *MainWindow::getButtonRestart() const
{
    return ui->pushButton_Restart;

}

DigitalClock *MainWindow::getDisplayTime() const
{
    return ui->lcdTime;
}



QScrollArea *MainWindow::getViewArea() const
{
    return ui->scrollArea;
}

QLabel *MainWindow::getPontosLabel() const
{
    return ui->label_pontos;
}



void MainWindow::on_actionControle_triggered()
{
    QMessageBox::information(NULL, "Controles",
        "Subir\t\t\t-> Tecla 'Seta para Cima'\n"
        "Descer\t\t\t-> Tecla 'Seta para baixo'\n"
        "Ir para esquerda\t-> Tecla 'Seta para esquerda'\n"
        "Ir para direita\t\t-> Tecla 'Seta para direita'\n");
}



void MainWindow::on_pushButton_Restart_clicked()
{
    if ( !exitQuestion())
        return;

    Game::getInstance().start();
}



void MainWindow::on_pushButton_showPath_clicked(bool checked)
{
    if (checked)
        Game::getInstance().setShowPath(true);
    else
        Game::getInstance().setShowPath(false);
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_pushButton_3_clicked()
{

    ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::on_actionSobre_o_jogo_triggered()
{
    QMessageBox::information(NULL, "Sobre o jogo",
        "<b>Carreira</b><br>"
        "    O objetivo é pegar a maior quantidade de objetos e acumular a maior pontuacao possivel<br><br>"
        "<b>Multiplayer</b><br>"
        "    Jogue contra outra pessoa e alcance a saida antes dela<br><br>"
        "<b>Competir</b><br>"
        "    Jogue contra a CPU e encontre a saida antes dele<br><br>"
        "<b>Treinar</b><br>"
        "    Aperfeicoe suas habilidades no jogo<br><br>");
}



void MainWindow::on_pushButton_9_clicked()
{
    QMessageBox::information(NULL, "Sobre o jogo",
        "Este jogo tem por objetivo percorrer o labirinto até encontrar a sua saida <br><br>"
         "<b>Modos de JOGO</b><br><br>"
        "<b>Carreira</b><br>"
        "    O objetivo é pegar a maior quantidade de objetos e acumular a maior pontuacao possivel<br><br>"
        "<b>Multiplayer</b><br>"
        "    Jogue contra outra pessoa e alcance a saida antes dela.Os comandos dos jogadores são as SETAS e o W,A,S,D<br><br>"
        "<b>Competir</b><br>"
        "    Jogue contra a CPU e encontre a saida antes dele<br><br>"
        "<b>Treinar</b><br>"
        "    Aperfeicoe suas habilidades no jogo<br><br>");
}



void MainWindow::on_pushButton_clicked()
{
    QMessageBox::information(NULL, "Sobre o jogo",
        "Este jogo foi desenvolvido pelos alunos:<br><br>"
         "<b>Leno Pereira Ferrira</b><br><br>"
        "<b>Raimundo Vítor de Godeiro Marques</b><br>");
}



void MainWindow::on_pushButton_11_clicked()
{

    Game::getInstance().setDificuldade(Game::Facil);
    Game::getInstance().start();
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_pushButton_10_clicked()
{
    Game::getInstance().setDificuldade(Game::Medio);
    Game::getInstance().start();
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_pushButton_12_clicked()
{
    Game::getInstance().setDificuldade(Game::Dificil);
    Game::getInstance().start();
    ui->stackedWidget->setCurrentIndex(3);
}



void MainWindow::on_modoTreinar_clicked()
{
    Game::getInstance().setMode(Game::Treinar);
    ui->pushButton_showPath->show();
    ui->label_pontos->hide();
    ui->label_pontosTitle->hide();

    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_menu_principal_01_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}



void MainWindow::on_menu_principal_02_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}



void MainWindow::on_modoCompetir_clicked()
{
    Game::getInstance().setMode(Game::Competir);
    ui->pushButton_showPath->hide();
    ui->label_pontos->hide();
    ui->label_pontosTitle->hide();

    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_modoMultiplayer_clicked()
{
    Game::getInstance().setMode(Game::Multiplayer);
    ui->pushButton_showPath->hide();
    ui->label_pontos->hide();
    ui->label_pontosTitle->hide();

    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_modoCarreira_clicked()
{
    Game::getInstance().setMode(Game::Carreira);
    ui->pushButton_showPath->hide();
    ui->label_pontos->show();
    ui->label_pontosTitle->show();

    ui->stackedWidget->setCurrentIndex(2);
}



void MainWindow::on_menu_principal_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}



void MainWindow::on_menu_pincipal_4_clicked()
{
    if ( !exitQuestion() )
        return;

    Game::getInstance().stop();
    ui->stackedWidget->setCurrentIndex(4);
}



void MainWindow::on_menu_dificuldade_clicked()
{
    if ( !exitQuestion() )
        return;

    Game::getInstance().stop();
    ui->stackedWidget->setCurrentIndex(2);
}



bool MainWindow::exitQuestion()
{
    if ( Game::getInstance().isRunning() )
    {
        QMessageBox::StandardButton ret;

        ret = QMessageBox::question(NULL, "Maze RL",
            "Tem certeza de que quer abandonar o jogo atual?",
                   QMessageBox::Yes|QMessageBox::No, QMessageBox::Yes);
        if ( ret == QMessageBox::Yes )
            return true;
    }
    else
        return true;

    return false;
}

void MainWindow::on_voltar_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_pushButton_4_clicked()
{
    Game::getInstance().Ler_Rank();

    std::vector<Game::Rank> rank = Game::getInstance().getRank();
    size_t i;
    QString nome, ponto;

    for (i=0; i<rank.size(); ++i)
    {
        nome += QString::fromStdString(rank[i].nome) + "\n";
        ponto += QString::number(rank[i].pontos) + "\n";
    }

    ui->label_rank->setText( nome );
    ui->label_ponto->setText( ponto );

    ui->stackedWidget->setCurrentIndex(5);

}
