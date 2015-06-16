/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef COLOR_H
#define COLOR_H

/**
 * @brief Define uma cor
 */
class Color
{
public:
    Color(int r1 = 0, int g1 = 0, int b1 = 0, int a1 = 255) :
        r(r1), g(g1), b(b1), a(a1) {}

    int r, g, b, a;
};

#endif // COLOR_H
