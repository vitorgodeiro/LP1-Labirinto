/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef LABIRINTO_H
#define LABIRINTO_H

#include <vector>
#include <stdlib.h>

/**
 * @brief Classe para criar um labirinto
 */
class Labirinto
{
public:
    /**
     * @brief As 4 possiveis paredes de uma celula.
     */
    enum Wall
    {
        None = 0,
        NorthWall   = 1, ///< Parede Norte
        EastWall    = 2, ///< Parede Leste
        SouthWall   = 4, ///< Parede Sul
        WestWall    = 8  ///< Parede Oeste
    };


    /**
     * @brief Uma celula do labirinto. Uma celula representa uma posicao unica
     * dentro de um labirinto. Cada celula pode ter ate 4 paredes erguidas, e
     * pelo menos duas paredes dela sao compartilhadas com outras celulas.
     */
    struct Cell
    {
        int row, column;
        Cell(int row1 = 0, int column1 = 0) : row(row1), column(column1) {}
        inline bool operator==(const Cell &cell)
            { return cell.row == row && cell.column == column; }

        inline bool operator!=(const Cell &cell)
            { return !(*this == cell); }
    };

    /**
     * @brief Cria um labirinto.
     * @param rows O numero de linhas, ou seja, o numero de celulas dispostas
     * horizontalmente no labirinto.
     * @param columns O numero de colunas, ou seja, o numero de celulas
     * dispostas verticalmente no labirinto.
     */
    Labirinto(int rows, int columns);

    ~Labirinto();

    /**
     * @brief Constroi o labirinto. O processo de construcao consiste em criar
     * uma entrada e saida, o caminho de saida e suas ramificacoes de modo a
     * tornar o labirinto totalmente conexo.
     *
     * @return sempre true.
     */
    bool build();

    /**
     * @brief Obtem o caminho para sair do labirinto.
     * @param cell A celula onde o caminho sera iniciado.
     * @return Um vetor de celulas que representa o caminho para
     * sair do labirinto.
     */
    std::vector<Cell> getPath(const Cell &c1, const Cell &c2) const;

    /**
     * @brief Obtem as paredes derrubadas de uma celula.
     * Para extrair as paredes do inteiro retornado, use o operador binario &.
     * Por exemplo, supondo que a funcao retorne 3, para saber se a parede
     * Norte esta destruida, use a expressao '3 & Labirinto::NorthWall'.
     *
     * @param cell uma celula do labirinto.
     * @return um inteiro contendo as paredes destruidas da celula.
     */
    int getDestroyedWalls(const Cell &cell) const;

    /**
     * @brief Verifica se uma celula e valida, ou seja, esta no labirinto.
     *
     * @param cell uma celula qualquer.
     * @return true se a celula especificada e valida, senao retorna false.
     */
    bool isCellValid(const Cell &cell) const;

    /**
     * @brief Obtem as celulas vizinhas em relacao as paredes especificadas.
     * @param cell uma celula do labirinto
     * @param walls uma ou mais paredes da celula
     * @return uma lista das celulas vizinhas
     */
    std::vector<Cell> getNeighbors(const Cell &cell, int walls) const;

    /**
     * @brief Verifica se uma parede e compartilhada, ou seja, pertence a duas
     * celulas. Se a parede for compartilhada e o parametro opcional nao for
     * nulo, entao ele recebe a outra celula que possui a parede. Note que uma
     * parede nao e compartilhada se, e somente se, ela delimita o labirinto.
     *
     * @param wall uma das paredes da celula especificada
     * @param cell uma celula do labirinto
     * @param other_cell opcional. Ponteiro que recebera a outra celula que
     * compartilha a parede especificada.
     * @return true se a parede e compartilhada, senao retorna false.
     */
    bool isWallShared(Wall wall, const Cell& cell,
                      Cell *other_cell = NULL) const;

    /**
     * @brief Obtem a parede oposta a uma parede. Por exemplo, a parede oposta
     * a parede Norte e a parede Sul, e a parede oposta a parede Leste e a
     * parede Oeste.
     *
     * @param wall uma parede qualquer.
     * @return a parede oposta a parede especificada.
     */
    static Wall getOppositeWall(Wall wall);

    /**
     * @brief Escolhe aleatoriamente uma parede.
     * @param walls as paredes que podem ser escolhidas
     * @return uma parede
     */
    static Wall pickWallRandomly(int walls);

    /**
     * @brief Obtem o numero de linhas do labirinto.
     *
     * @return o numero de linhas do labirinto
     */
    inline int rows() const { return rows_; }

    /**
     * @brief Obtem o numero de colunas do labirinto
     *
     * @return o numero de colunas do labirinto
     */
    inline int columns() const { return columns_; }


private:
    struct CellInfo
    {
        int destroyed; ///< paredes destruidas
        int id; ///< nome do conjunto
        bool visited;
    };

    /**
     * @brief Destroi uma parede de uma celula.
     * @param ci uma celula da matriz
     * @param wall a parede a ser destruida.
     */
    void destroyWall(CellInfo &ci, Wall wall);


    Labirinto::Cell toCell(const CellInfo &ci) const;

    /**
     * @brief Obtem as paredes que podem ser destruidas de uma celula, excluindo
     * aquelas que ja estao destruidas.
     *
     * @param ci uma celula da matriz
     * @return as paredes que podem ser destruidas
     */
    int getDestroyableWalls(const CellInfo &ci) const;

    /**
     * @brief Obtem as paredes destruidas de uma celula
     * @param ci uma celula da matriz
     * @return as paredes destruidas
     */
    inline int getDestroyedWalls(const CellInfo &ci) const {return ci.destroyed;}

    /**
     * @brief Obtem a celula vizinha em relacao a uma parede.
     * @param ci uma celula da matriz
     * @param wall uma parede.
     * @return a celula vizinha, ou NULL, caso ela nao exista.
     */
    CellInfo* getNeighbor(const CellInfo &ci, Wall wall) const;

    /**
     * @brief Define o estado inicial do labirinto, a fim de reconstrui-lo
     */
    void reset();

    /**
     * @brief Converte dois indices, representando a linha e coluna de uma
     * celula, em um offset de um vetor unidimensional
     *
     * @param i a linha da celula
     * @param j a coluna da celula
     * @return o offset de um vetor unidimensional
     */
    inline int offset(int i, int j) const { return columns_ * i + j; }

    /**
     * @brief Obtem a celula da matriz correspondente a celula do labirinto
     * @param cell celula do labirinto
     * @return ponteiro para uma celula da matriz
     */
    CellInfo *getCellInfo(const Cell &cell) const;

    /**
     * @brief Funcao auxiliar para propagar o id em um cacho de celulas.
     * @param ci uma celula da matriz
     * @param param ponteiro para o id a ser propagado.
     * @return sempre true
     */
    static bool setIdCallback(CellInfo &ci, void *param);

    /**
     * @brief Funcao auxiliar para encontrar o caminho para uma celula
     * @param c uma celula da matriz
     * @param param ponteiro para a celula da matriz a ser encontrada
     * @return true se achar a celula, senao retorna false
     */
    static bool findPathCallback(CellInfo &c, void *param);

    /**
     * @brief Percorre um cacho de celulas.
     * Para cada celula, a funcao de
     * callback e chamada com uma referencia a essa celula. Se ela retornar
     * true, entao o cacho nao e mais percorrido, e a funcao retorna.
     *
     * @param root a celula raiz do cacho
     * @param arg um argumento a ser passado para a funcao callback
     * @return o numero de celulas percorridas no cacho
     */
    int traverse(CellInfo &root, bool (*cb)(CellInfo &c, void* param),
                 void *arg);

    std::vector<Cell> path_;
    CellInfo *matrix_;
    int rows_;
    int columns_;
};

#endif // LABIRINTO_H
