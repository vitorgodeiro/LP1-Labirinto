/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef RECT_H
#define RECT_H

/**
 * @brief Define um retangulo
 */
class Rect
{
public:
    Rect(int x1 = 0, int y1 = 0, int w1 = 0, int h1 = 0) :
        x(x1), y(y1), w(w1), h(h1) {}

    int x, y, w, h;
};

#endif // RECT_H
