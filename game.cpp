#include <QtGui/QMouseEvent>
#include "game.h"
#include "color.h"
#include "rect.h"
#include <QMessageBox>
#include <QTimer>
#define DEBUG
#include "debug.h"
#include <stdlib.h>
#include <QInputDialog>
#include<string>
#include <iostream>
#include<fstream>
#include<stdlib.h>

Game::Game(QWidget *parent) : QGLWidget(parent), show_player1_(false),
    show_player2_(false), labirinto_(NULL), jogando_(false)
{
    TRACE(("Game::Game()\n"));

    setMouseTracking(true);

    setDificuldade(Dificil);
    setCellColor( Color(255, 255, 255) );
    setWallColor( Color(0, 0, 0) );
    setWallThickness(6);
    setCellSize(20);


    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(20);

    player1_timer_.setSingleShot(false);

    connect(&player1_timer_, SIGNAL(timeout()), this, SLOT(updatePlayer1Pos()));
    connect(&player2_timer_, SIGNAL(timeout()), this, SLOT(updatePlayer2Pos()));
    connect(&oground_timer_, SIGNAL(timeout()), this,
            SLOT(updateOGroundColor()));


    oground_timer_.setSingleShot(false);
    oground_timer_.start(10);

    player1_pos_[0] = 0;
    player1_pos_[1] = 0;

    player2_pos_[0] = 0;
    player2_pos_[1] = 0;

    setShowLabirinto(false);
    show_path_ = false;
}



Game::~Game()
{
    TRACE(("Game::~Game()\n"));

    delete labirinto_;
}



void Game::setMode(Game::Mode mode)
{

    mode_ = mode;
}



void Game::setDificuldade(Game::Dificuldade dif)
{
    if (labirinto_ != NULL)
    {
        delete labirinto_;
        labirinto_ = NULL;
    }

    switch(dif)
    {
        case Facil :
        {
            labirinto_ = new Labirinto(8, 12);
            path_finder_ai_.setIQ(70);
            break;
        }

        case Medio :
        {
            labirinto_ = new Labirinto(16, 24);
            path_finder_ai_.setIQ(120);
            break;
        }

        case Dificil :
        {
            labirinto_ = new Labirinto(32, 48);
            path_finder_ai_.setIQ(200);
            break;
        }

        default:
            break;
    }
}



void Game::start()
{
    stop();
    labirinto_->build();
    adjustSizes(); // ajusta o tamanho da celula e espessura da parede

    if ( mode_ == Carreira )
        main_window_->getDisplayTime()->start(CARREIRA_TEMPO); // tempo regressivo
    else
        main_window_->getDisplayTime()->start(0); // tempo progressivo

    main_window_->getButtonShowPath()->setChecked(false);
    setShowPath(false);

    player1_.setCell(Labirinto::Cell(0,0));
    player2_.setCell(Labirinto::Cell(labirinto_->rows()-1,labirinto_->columns()-1));
    setShowLabirinto(true);
    show_player1_ = true;
    if(mode_ == Competir || mode_ == Multiplayer)
        show_player2_ = true;
    else
        show_player2_ = false;

    if(mode_ == Competir)
    {
        path_finder_ai_.setLabirinto(labirinto_);
        path_finder_ai_.setInitialCell( Labirinto::Cell(labirinto_->rows()-1,labirinto_->columns()-1) );
        path_finder_ai_.setFinalCell(
            Labirinto::Cell( 0, 0 ) );

        player2_.setCell( path_finder_ai_.getInitialCell() );
        path_finder_ai_.start();
    }

    iground_color_ = Color();

    player1_.setColor( Color(0, 255, 0) );

    if ( mode_ == Competir)
        player2_.setColor( Color(255, 0, 0) );
    else if ( mode_ == Multiplayer)
        player2_.setColor( Color(0, 0, 255) );
    else if (mode_ == Carreira)
        player1_.setColor( Color(0, 255, 255) );
    else
        player1_.setColor( Color(255, 255, 255) );

    player1_.setCell(Labirinto::Cell(0,0));

    // reseta o modo carreira
    if ( mode_ == Carreira )
    {
        item_pontos_ = pontos_ = 0;
        item_ = Labirinto::Cell(0, 0);
        updatePontuacao();
    }

    jogando_ = true;
}



void Game::stop()
{
    main_window_->getDisplayTime()->stop();
    path_finder_ai_.stop();
    jogando_ = false;
}



bool Game::adjustSizes()
{
    int w, h;

    w = view_rect_.w;
    h = view_rect_.h;

    int t1, t2;
    t1 = w / (4 * labirinto_->columns() + 1) ;
    t2 = h / (4 * labirinto_->rows() + 1) ;

    if ( t1 > t2 )
        t1 = t2;

    if ( t1 == 0 )
        t1 = 1;

    w = t1 * ( 4 * labirinto_->columns() + 1);
    h = t1 * ( 4 * labirinto_->rows() + 1);

    lab_rect_.x = (view_rect_.w - w)/2;
    lab_rect_.y = (view_rect_.h - h)/2;
    lab_rect_.w = w;
    lab_rect_.h = h;

    setWallThickness( t1 );
    setCellSize( 3 * t1 );

    return true;
}



void Game::initializeGL()
{
    Surface img1("oground.tga");
    Surface img2("iground.tga");
    Surface img3("wall.tga");
    Surface img4("player.tga");
    Surface img5("item.tga");

    Rect rect;

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glEnable(GL_POLYGON_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0, 0, 0, 0);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(10, textures_);

    glBindTexture(GL_TEXTURE_2D, textures_[0]);
    rect = img1.getRect();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rect.w, rect.h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img1.getPixels());

    glBindTexture(GL_TEXTURE_2D, textures_[1]);
    rect = img2.getRect();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rect.w, rect.h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img2.getPixels());

    glBindTexture(GL_TEXTURE_2D, textures_[2]);
    rect = img3.getRect();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rect.w, rect.h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img3.getPixels());

    glBindTexture(GL_TEXTURE_2D, textures_[3]);
    rect = img4.getRect();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rect.w, rect.h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img4.getPixels());


    glBindTexture(GL_TEXTURE_2D, textures_[4]);
    rect = img5.getRect();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, rect.w, rect.h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, img5.getPixels());
}



void Game::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, h, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    TRACE(("Resize: %d, %d", w, h));
    setViewRect( Rect(0,0, w, h) );

    adjustSizes();
}



void Game::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);


    // desenha o plano de fundo
    drawRectangleTex(view_rect_, textures_[0], oground_color_);

    if (show_labirinto_)
        drawLabirinto();

    if (show_path_)
        drawPath();

    if ( mode_ == Carreira)
        drawItem();

    if (show_player1_)
        drawPlayer(player1_);

    if (show_player2_)
    {
        if(mode_ == Competir)
            drawPlayer(player2_);
        else if(mode_ == Multiplayer)
        {
            drawPlayer(player2_);
        }
    }
}



void Game::mousePressEvent(QMouseEvent *event)
{
    (void)event;
}



void Game::mouseMoveEvent(QMouseEvent *event)
{
    (void)event;
}



void Game::keyPressEvent(QKeyEvent* event)
{
    if (!jogando_)
        return;

    Labirinto::Cell cell = player1_.getCell();
    Labirinto::Cell cell2 = player2_.getCell();
    Labirinto::Cell old_cell = cell;
    Labirinto::Cell old_cell2 = cell2;
    bool is_player1 = false;
    int pos[4] = {0, 0};
    int d = labirinto_->getDestroyedWalls(cell);
    int d2 = labirinto_->getDestroyedWalls(cell2);

    switch(event->key()) {
        case Qt::Key_Up:
            TRACE(("Tecla Up pressionada\n"));

            is_player1 = true;

            if ( d & Labirinto::NorthWall )
            {
                --cell.row;
                pos[1] = -(cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_Right:
            TRACE(("Tecla Right pressionada\n"));

            is_player1 = true;

            if ( d & Labirinto::EastWall )
            {
                ++cell.column;
                pos[0] = (cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_Down:
            TRACE(("Tecla Down pressionada\n"));

            is_player1 = true;

            if ( d & Labirinto::SouthWall )
            {
                ++cell.row;
                pos[1] = (cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_Left:
            TRACE(("Tecla Left pressionada\n"));

            is_player1 = true;

            if ( (d & Labirinto::WestWall) )
            {
                --cell.column;
                pos[0] = -(cell_size_+wall_thickness_);
            }
        break;

// teclas do player2
        case Qt::Key_W:
            TRACE(("Tecla W pressionada\n"));

            if ( (d2 & Labirinto::NorthWall) )
            {
                --cell2.row;
                pos[3] = -(cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_D:
            TRACE(("Tecla D pressionada\n"));

            if ( d2 & Labirinto::EastWall )
            {
                ++cell2.column;
                pos[2] = (cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_S:
            TRACE(("Tecla S pressionada\n"));

            if ( d2 & Labirinto::SouthWall )
            {
                ++cell2.row;
                pos[3] = (cell_size_+wall_thickness_);
            }
        break;

        case Qt::Key_A:
            TRACE(("Tecla A pressionada\n"));

            if ( (d2 & Labirinto::WestWall) )
            {
                --cell2.column;
                pos[2] = -(cell_size_+wall_thickness_);
            }
        break;

        default:
            event->ignore();
            return;
    }



    if (is_player1)
    {
        if ( labirinto_->isCellValid(cell) )
        {
            if ( !player1_timer_.isActive() )
            {
                player1_pos_[0] = pos[0];
                player1_pos_[1] = pos[1];
                player1_timer_.start(10);
                player1_.setCell(cell);
            }
        }
        else if ( mode_ != Carreira && old_cell != Labirinto::Cell(0,0) )
        {
            setShowPlayer1(false);
            stop();
            // ganhou!!!

            if ( mode_ ==  Treinar)
            {
                QMessageBox::information(NULL, "Maze RL",
                    "Parabens!\n Voce conseguiu sair do labirinto.");
                start();
            }
            else
            {
                QMessageBox::information(NULL, "Maze RL",
                    "Parabens, Player1!\nVoce conseguiu sair do labirinto primeiro");

            //start();
            }
        }
    }
    else if ( mode_ != Competir )
    {
        if ( labirinto_->isCellValid(cell2) )
        {
            if ( !player2_timer_.isActive() )
            {
                player2_pos_[0] = pos[2];
                player2_pos_[1] = pos[3];
                player2_timer_.start(10);
                player2_.setCell(cell2);
            }
        }
        else if ( mode_ != Carreira && old_cell2 == Labirinto::Cell(0,0) )
        {
            setShowPlayer2(false);
            stop();
            // ganhou!!!
            TRACE(("TEMPO = %lf",main_window_->getDisplayTime()->getTime()/1000.f));
            QMessageBox::information(NULL, "Maze RL",
                "Parabens, Player2!\nVoce conseguiu sair do labirinto primeiro");

            //start();
        }
    }




}


void Game::updatePlayer1Pos()
{
    int inc = (int)(cell_size_/8.0 + 0.5);
    if (inc < 1) inc = 1;

    if (player1_pos_[0] == 0 && player1_pos_[1] == 0)
    {
        if ( mode_ == Carreira )
        {
            // Se capturou o item
            if ( player1_.getCell() == item_ &&
                 player1_pos_[0] == 0 && player1_pos_[1] == 0 )
            {
                updatePontuacao();
            }
        }
        player1_timer_.stop();
        return;
    }

   if (player1_pos_[0] > 0)
    {
        player1_pos_[0] -= inc;
        if (player1_pos_[0] < 0)
            player1_pos_[0] = 0;
    }
   if (player1_pos_[0] < 0)
   {
        player1_pos_[0] += inc;
        if (player1_pos_[0] > 0)
            player1_pos_[0] = 0;
    }

   if (player1_pos_[1] > 0)
   {
        player1_pos_[1] -= inc;
        if (player1_pos_[1] < 0)
            player1_pos_[1] = 0;
    }

   if (player1_pos_[1] < 0)
   {
        player1_pos_[1] += inc;
        if (player1_pos_[1] > 0)
            player1_pos_[1] = 0;
    }
}


void Game::updatePlayer2Pos()
{
    int inc = (int)(cell_size_/8.0 + 0.5);
    if (inc < 1) inc = 1;

    if (player2_pos_[0] == 0 && player2_pos_[1] == 0)
    {
        player2_timer_.stop();
        return;
    }

  if (player2_pos_[0] > 0)
   {
       player2_pos_[0] -= inc;
       if (player2_pos_[0] < 0)
           player2_pos_[0] = 0;
   }
  if (player2_pos_[0] < 0)
  {
       player2_pos_[0] += inc;
       if (player2_pos_[0] > 0)
           player2_pos_[0] = 0;
   }

  if (player2_pos_[1] > 0)
  {
       player2_pos_[1] -= inc;
       if (player2_pos_[1] < 0)
           player2_pos_[1] = 0;
   }

  if (player2_pos_[1] < 0)
  {
       player2_pos_[1] += inc;
       if (player2_pos_[1] > 0)
           player2_pos_[1] = 0;
   }
}



void Game::updateOGroundColor()
{
    if ( jogando_ )
    {
        if ( mode_ == Competir )
        {
            Labirinto::Cell cur = path_finder_ai_.getCurrentCell();
            Labirinto::Cell old = player2_.getCell();
            int pos[2] = {0, 0};

            if (show_path_)
            {
                 stop();
                 show_path_ = false;
                 show_player2_ = false;
                QMessageBox::information(NULL, "Maze RL",
                    "Voce perdeu. O computador saiu do labirinto primeiro");
                //start();
             }

            if ( old == Labirinto::Cell(0, 0) && player2_pos_[0] == 0 &&
                player2_pos_[1] == 0  )
                   show_path_ = true;


            if ( old != cur )
            {
                int diff_h = cur.column - old.column;
                int diff_v = cur.row - old.row;

                if ( diff_h != 0) // movimento horizontal
                {
                    pos[0] = diff_h * (cell_size_+wall_thickness_);
                }

                if ( diff_v != 0 ) // movimento vertical
                {
                    pos[1] = diff_v * (cell_size_+wall_thickness_);
                }

                player2_pos_[0] = pos[0];
                player2_pos_[1] = pos[1];
                player2_timer_.start(10);
                player2_.setCell( cur );
            }
        }

        if ( mode_ == Carreira )
        {
            if ( main_window_->getDisplayTime()->getTime() > CARREIRA_TEMPO )
            {
                stop();

                QMessageBox::information(NULL, "Maze RL",
                    "Tempo esgotado!");
                Ler_Rank();
                Ranke();
               // start();
            }
        }
    }


    static int inc = 1;

    if (oground_color_.g == 255)
        inc = -1;
    else if (oground_color_.g <= 150)
        inc = 1;

    oground_color_.r += inc;
    oground_color_.g += inc;
    oground_color_.b += inc;
}




void Game::drawRectangle(
        const Rect &rect, const Color &color)
{
    float x, y, w, h;

    glDisable(GL_TEXTURE_2D);

    x = rect.x + 0.375f;
    y = rect.y + 0.375f;
    w = rect.w;
    h = rect.h;

    glColor4ub(color.r, color.g, color.b, color.a);
    glBegin(GL_QUADS);
     glVertex2f(x, y);
     glVertex2f(x, y+h);
     glVertex2f(x+w, y+h);
     glVertex2f(x+w, y);
     glEnd();

}



void Game::drawRectangleTex(const Rect &rect, GLuint tex_id, const Color& color)
{
    float x, y, w, h;

    x = rect.x + 0.375f;
    y = rect.y + 0.375f;
    w = rect.w;
    h = rect.h;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glColor4ub(color.r, color.g, color.b, color.a);

    glBegin(GL_QUADS);
     glTexCoord2f(0.0f, 0.0f);
     glVertex2f(x, y);

     glTexCoord2f(0.0f, 1.0f);
     glVertex2f(x, y+h);

     glTexCoord2f(1.0f, 1.0f);
     glVertex2f(x+w, y+h);

     glTexCoord2f(1.0f, 0.0f);
     glVertex2f(x+w, y);
    glEnd();
}



void Game::drawLabirinto()
{
    int rows = labirinto_->rows();
    int columns = labirinto_->columns();
    Labirinto::Cell cell;
    Rect rect;
    int x, y;
    Color wall_color(200,200,200);
    Color cell_color(255,255,255);

    x = lab_rect_.x;
    y = lab_rect_.y;

    if (mode_ == Treinar)
    {
        wall_color = Color(0,255,255);
        cell_color = Color(255,255,255);
    }

    if (!jogando_)
        cell_color = Color(150,150,150,255);

    drawRectangleTex(lab_rect_, textures_[1], cell_color );

    if (mode_ == Carreira)
    {
        rect.x = x;
        rect.y = y;
        rect.w = wall_thickness_;
        rect.h = cell_size_+ wall_thickness_*2;
        drawRectangleTex(rect, textures_[2], wall_color);
        rect.x =(cell_size_+wall_thickness_)*columns+lab_rect_.x;
        rect.y =(cell_size_+wall_thickness_)*(rows-1)+lab_rect_.y;
        drawRectangleTex(rect, textures_[2], wall_color);
    }

    for (cell.row = 0; cell.row < rows; ++cell.row)
    {
        for (cell.column=0; cell.column < columns; ++cell.column)
        {
            int d = labirinto_->getDestroyedWalls(cell);
            int ofs_x = x + cell.column * (cell_size_ + wall_thickness_);
            int ofs_y = y + cell.row * (cell_size_ + wall_thickness_);


            if ( !(d & Labirinto::NorthWall) )
            {
                rect.x = ofs_x;;
                rect.y = ofs_y;
                rect.w = cell_size_+ wall_thickness_*2;
                rect.h = wall_thickness_;
                drawRectangleTex(rect, textures_[2], wall_color);
            }

            if ( !(d & Labirinto::EastWall) )
            {
                rect.x = ofs_x + wall_thickness_ + cell_size_;
                rect.y = ofs_y;
                rect.w = wall_thickness_;
                rect.h = cell_size_+ wall_thickness_*2;

                drawRectangleTex(rect, textures_[2], wall_color);

            }

            if ( !(d & Labirinto::SouthWall) )
            {
                rect.x = ofs_x;
                rect.y = ofs_y + wall_thickness_ + cell_size_;
                rect.w = cell_size_+ wall_thickness_*2;
                rect.h = wall_thickness_;
                drawRectangleTex(rect, textures_[2], wall_color);


            }

            if ( !(d & Labirinto::WestWall) )
            {
                rect.x = ofs_x;
                rect.y = ofs_y;
                rect.w = wall_thickness_;
                rect.h = cell_size_+ wall_thickness_*2;

                drawRectangleTex(rect, textures_[2], wall_color);
            }
        }
    }
}



void Game::drawPath()
{
    Rect rect;
    int i;
    int x = lab_rect_.x;
    int y = lab_rect_.y;
    int rows = labirinto_->rows();
    int columns = labirinto_->columns();
    std::vector<Labirinto::Cell> path =
            labirinto_->getPath(
                player1_.getCell(), Labirinto::Cell(rows-1, columns-1));



    for (i=1; i<(int)path.size(); ++i)
    {
        int ofs_x = x + path[i].column * (cell_size_ + wall_thickness_);
        int ofs_y = y + path[i].row * (cell_size_ + wall_thickness_);
        int dr = path[i].row - path[i-1].row;
        int dc = path[i].column - path[i-1].column;

        if (dr != 0)
        {
            rect.w = cell_size_;
            rect.h = cell_size_ + wall_thickness_;
            rect.x = ofs_x + wall_thickness_;

            if (dr < 0)
                rect.y = ofs_y + wall_thickness_;
            else
                rect.y = ofs_y;
        }

        else if (dc != 0)
        {
            rect.w = cell_size_ + wall_thickness_;
            rect.h = cell_size_;
            rect.y = ofs_y + wall_thickness_;

            if (dc < 0)
                rect.x = ofs_x + wall_thickness_;
            else
                rect.x = ofs_x;
        }

        drawRectangle(rect, Color(0,255,255, 100));
    }
}



void Game::drawPlayer(const Player &player)
{
    Rect rect;
    Labirinto::Cell cell = player.getCell();
    int* pos;

    if ( &player == &player1_ )
        pos = player1_pos_;
    else
        pos = player2_pos_;

    int x, y;
    int rows = labirinto_->rows();
    int columns = labirinto_->columns();

    x = (view_rect_.w - (wall_thickness_*(columns+1) + cell_size_*columns))/2;
    y = (view_rect_.h - (wall_thickness_*(rows+1) + cell_size_*rows))/2;


    int ofs_x = x + cell.column * (cell_size_ + wall_thickness_);
    int ofs_y = y +cell.row * (cell_size_ + wall_thickness_);

    rect.x = ofs_x + wall_thickness_ - pos[0];
    rect.y = ofs_y + wall_thickness_ - pos[1];
    rect.w = rect.h = cell_size_;


   drawRectangleTex(rect, textures_[3], player.getColor());
   //drawRectangle(rect, Color(0,0,255));
}



void Game::drawItem()
{
    Rect rect;
    Labirinto::Cell cell = item_;

    int x, y;
    int rows = labirinto_->rows();
    int columns = labirinto_->columns();

    x = (view_rect_.w - (wall_thickness_*(columns+1) + cell_size_*columns))/2;
    y = (view_rect_.h - (wall_thickness_*(rows+1) + cell_size_*rows))/2;

    int ofs_x = x + cell.column * (cell_size_ + wall_thickness_);
    int ofs_y = y +cell.row * (cell_size_ + wall_thickness_);

    rect.x = ofs_x + wall_thickness_;
    rect.y = ofs_y + wall_thickness_;
    rect.w = rect.h = cell_size_;

   // drawRectangle(rect, Color(0,0,255));

    drawRectangleTex(rect, textures_[4], oground_color_);
}


void Game::updatePontuacao()
{
    int x = rand()% labirinto_->rows();
    int y = rand()% labirinto_->columns();

    // nova celula do item
    item_ = Labirinto::Cell(x, y);

    // incrementa a pontuacao total
    pontos_ += item_pontos_;

    // define o valor do item no instance da nova posicao do item
    item_pontos_ = labirinto_->getPath( item_, player1_.getCell() ).size();

    // atualize o display
    main_window_->getPontosLabel()->setText( QString::number(pontos_) );
}


void Game::Ler_Rank()
{
    FILE *file;
    int a;
    file=fopen("../lab5/rankponto.txt","r");
    std::ifstream entrada("../lab5/rankname.txt", std::ios_base :: in);
    if(file == NULL)
    {
        printf ("Não foi possivel abrir o arquivo.\n");
        getchar();
        exit(0);
    }


    for(a=0;a<10;a++)
    {
        getline(entrada,jogadores_[a].nome);
        fscanf(file,"%d",&jogadores_[a].pontos);
    }
    fclose(file);
    entrada.close();
}



std::vector<Game::Rank> Game::getRank() const
{
    int i;
    std::vector<Game::Rank> ret;

    for (i=0; i<10; ++i)
        ret.push_back( jogadores_[i] );

    return ret;
}



void Game::Ranke()
{
    FILE *file;
    FILE *file2;
    int aux;
    int b = 0;
    std::string auxname;
    std::string auxname2;
    file=fopen("../lab5/rankponto.txt","w");
    file2=fopen("../lab5/rankname.txt","w");
    for(int a=0;a < 10;a++)
    {

        if((jogadores_[a]).pontos <= pontos_)
        {

            aux = jogadores_[a].pontos;
            jogadores_[a].pontos = pontos_;
            pontos_=aux;
            if(b==0)
            {
                auxname = jogadores_[a].nome;
                QString response = QInputDialog::getText(0,"Maze RL",
                            "Parabens!! Você agora é um recordista. Sua classificação é" + QString::number(a+1) + "\n"
                            "Qual o seu nome?");
                jogadores_[a].nome = response.toLocal8Bit().constData();
                b++;
            }
            else
            {
                auxname2 = jogadores_[a].nome;
                jogadores_[a].nome = auxname;
                auxname = auxname2;
            }
        }

            fprintf(file2,"%s\r\n",jogadores_[a].nome.c_str());
            fprintf(file,"%d\r\n",jogadores_[a].pontos);
    }
    fclose(file);
    fclose(file2);

}
