/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef SURFACE_H
#define SURFACE_H

#include <string>
#include <GL/gl.h>
#include "rect.h"



/**
 * @brief Cria uma imagem
 */
class Surface
{
public:
    /**
     * @brief Constroe a imagem a partir do arquivo especificado
     * @param file um arquivo TGA
     */
    Surface(const std::string &file);

    /**
     * @brief Obtem o retangulo com as dimensoes da imagem
     * @return as dimensoes da imagem
     */
    Rect getRect() const { return rect_; }

    /**
     * @brief Obtem os pixels da imagem, no formato RGBA
     * @return os pixels da imagem, no formato RGBA
     */
    GLubyte* getPixels() const { return pixels_; }

    ~Surface();

private:
    GLubyte* pixels_; ///< os pixels da imagem
    Rect rect_; ///< as dimensoes da imagem
};

#endif // SURFACE_H
