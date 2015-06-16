/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef GLWIDGET_H
#define GLWIDGET_H



#include <QGLWidget>
#include <QtOpenGL/QGLWidget>
#include "player.h"
#include "labirinto.h"
#include "color.h"
#include "rect.h"
#include "surface.h"
#include "mainwindow.h"
#include "pathfinderai.h"
#include <string>
// tempo para o modo carreira, em milissegundos
#define CARREIRA_TEMPO 180000


class MainWindow;

/**
 * @brief Gerencia o jogo
 */
class Game : public QGLWidget {
    Q_OBJECT

public:
    /**
     * @brief Define os niveis de dificuldade do jogo
     */
    enum Dificuldade
    {
        Facil,
        Medio,
        Dificil
    };

    /**
     * @brief Define os modos de jogo
     */
    enum Mode
    {
        Carreira,
        Multiplayer,
        Competir,
        Treinar
    };

    typedef struct
    {
        std::string nome;
        int pontos = 0;

    } Rank;
    /**
     * @brief Obtem a referencia desta classe
     * @return a referencia desta classe
     */
    static Game& getInstance()
    {
        static Game* instance = NULL;

        if ( !instance )
            instance = new Game();

        return *instance;
   }

    ~Game();

    /**
     * @brief Exibe ou oculta o labirinto.
     * @param yes Se true, exibe o labirinto, senao nao exibe.
     */
    void setShowLabirinto(bool yes=true) { show_labirinto_ = yes; }

    /**
     * @brief Define a cor da parede
     * @param color uma cor qualquer
     */
    void setWallColor(const Color& color) { wall_color_ = color; }

    /**
     * @brief Define a cor da celula
     * @param color uma cor qualquer
     */
    void setCellColor(const Color& color) { cell_color_ = color; }

    /**
     * @brief Define o modo de jogo
     * @param mode um modo de jogo
     */
    void setMode(Mode mode);

    /**
     * @brief Define a dificuldade do jogo
     * @param dif uma dificuldade do jogo
     */
    void setDificuldade(Dificuldade dif);

    /**
     * @brief Define a espessura da parede.
     * @param thickness a espessura da parede.
     */
    void setWallThickness(int thickness=1) { wall_thickness_ = thickness; }

    /**
     * @brief Define o tamanho de cada celula do labirinto
     * @param size o tamanho da celula
     */
    void setCellSize(int size=10) { cell_size_ = size; }

    /**
     * @brief Inicia o jogo.
     */
    void start();

    /**
     * @brief Para o jogo.
     */
    void stop();

    /**
     * @brief Checa se o jogo esta em andamento.
     * @return true se o jogo esta em andamento, senao retorna false
     */
    bool isRunning() const { return jogando_; }
    /**
     * @brief Oculta ou exibe o caminho para sair do labirinto.
     * @param yes se true, revela o caminho, senao nao revela.
     */
    void setShowPath(bool yes) { show_path_ = yes; }

    /**
     * @brief Oculta ou mostra o player1_
     * @param yes se true, mostra o player1, senao nao mostra
     */
    void setShowPlayer1(bool yes) { show_player1_ = yes; }

    /**
     * @brief Oculta ou mostra o player2_
     * @param yes se true, mostra o player2, senao nao mostra
     */
    void setShowPlayer2(bool yes) { show_player2_ = yes; }

    /**
     * @brief Indica se o caminho de saida esta ou nao sendo exibido
     * @return true se o caminho de saida esta sendo exibido,
     * senao retorna false
     */
    bool getShowPath() const { return show_path_; }

    /**
     * @brief getViewRect
     * @return
     */
    inline Rect getViewRect() const { return view_rect_; }
    inline void setViewRect(const Rect &rect) { view_rect_ = rect; }
    inline MainWindow *getMainWindow() { return main_window_; }
    void setMainWindow(MainWindow* main_window) { main_window_ = main_window;}
    /**
     * @brief Ler_Rank é responsável por fazer a leitura do rank
     * @return
     */
    void Ler_Rank();

    std::vector<Rank> getRank() const;
    /**
     * @brief Ranke é responsável por imprimir no arquivo txt o rank
     * @return
     */
    void Ranke();

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

protected slots:
    void updatePlayer1Pos();
    void updatePlayer2Pos();
    void updateOGroundColor();

private:
    Game(QWidget *parent = NULL);

    bool adjustSizes();

    void drawRectangle(const Rect &rect, const Color &color);
    void drawRectangleTex(const Rect &rect, GLuint tex_id,
                          const Color& color = Color(255, 255, 255, 255));
    void drawLabirinto();
    void drawPath();

    void drawPlayer(const Player& player);
    void drawItem();

    void updatePontuacao();

    Color cell_color_;
    Color wall_color_;
    Color oground_color_;
    Color iground_color_;
    Player player1_;
    Player player2_;
    bool show_player1_;
    bool show_player2_;
    int player1_pos_[2];
    int player2_pos_[2];
    QTimer player1_timer_;
    QTimer player2_timer_;
    int cell_size_;
    int wall_thickness_;
    Labirinto* labirinto_;
    Rect view_rect_;
    Rect lab_rect_;
    GLuint textures_[10];
    QTimer oground_timer_;
    bool show_labirinto_;
    bool show_path_;
    Mode mode_;
    MainWindow* main_window_;
    PathFinderAI path_finder_ai_;
    bool jogando_;
    Rank jogadores_[10];
    Labirinto::Cell item_; ///< usado no modo carreira
    int item_pontos_;  ///< quantos pontos vale o item
    int pontos_; ///< total de pontos obtidos
};


#endif // GLWIDGET_H
