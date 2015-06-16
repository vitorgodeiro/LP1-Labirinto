/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "labirinto.h"
#include "color.h"
#include <QTimer>



/**
 * @brief Representa um jogador
 */
class Player
{
public:
    Player(const std::string &name = "Sem nome",
           const Color& color = Color(255,0,0));
    ~Player();

    /**
     * @brief Obtem a celula na qual o jogador esta.
     * @return a celula na qual o jogador esta
     */
    inline Labirinto::Cell getCell() const { return cell_; }

    /**
     * @brief Define a celula na qual o jogador esta
     * @param cell uma celua valida
     * @return true.
     */
    bool setCell(const Labirinto::Cell &cell);


    inline Color getColor() const { return color_; }
    inline void setColor(const Color &color) { color_ = color; }

private:
    std::string name_;  ///< nome
    Labirinto::Cell cell_; ///< a celula em que o jogador esta
    Color color_; ///< cor
};

#endif // PLAYER_H
