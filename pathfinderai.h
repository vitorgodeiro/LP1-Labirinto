/**
 * Projeto: Maze
 * Versao:  1.0b
 * Autores: Leno
 *          Raimundo
 */

#ifndef PATHFINDERAI_H
#define PATHFINDERAI_H

#include "labirinto.h"
#include <vector>
#include <QTimer>
#include <QObject>
#include <QMutex>

/**
 * @brief Cria a inteligencia artificial para caminhar no labirinto ate atingir
 * uma celula.
 * A IA foi modelada usando o seguinte:
 * - Tempo para tomar uma decisao, ou seja, o tempo para ir para a proxima celula;
 * - Probabilidade de tomar a decisao certa, ou seja, a probabilidade de a proxima
 *   celula ser a certa;
 * - Capacidade de memoria:  quantidade de celulas ja visitadas que a AI consegue
 *   lembrar
 */
class PathFinderAI : public QObject
{
    Q_OBJECT

public:
    PathFinderAI();
    ~PathFinderAI();

    /**
     * @brief Inicia a procura pela celula final
     * @return sempre true.
     */
    bool start();

    /**
     * @brief Interrompe a procura pela celula final
     * @return sempre true.
     */
    bool stop();

    /**
     * @brief Obtem o coeficiente de inteligencia da busca pela celula final
     *
     * @return um inteiro de 1 a 200 que representa o coeficiente de
     * inteligencia usado na busca
     */
    int getIQ() const { return iq_;}

    /**
     * @brief Define o coeficiente de inteligencia da busca pela celula final
     *
     * @param iq um numero de 1 a 200. Quanto maior, mais rapida e eficiente e a
     * busca pela celula final.
     * @return sempre true.
     */
    bool setIQ(int iq);

    /**
     * @brief Obtem o labirinto usado na busca.
     * @return o labirinto usado na busca, ou NULL se ele nao existe.
     */
    Labirinto *getLabirinto() const { return labirinto_;}

    /**
     * @brief Define o labirinto usado na busca
     * @param labirinto um labirinto
     */
    void setLabirinto(Labirinto* labirinto) { labirinto_ = labirinto;}

    /**
     * @brief Obtem a celula inicial, ou seja, aquela da qual comecara a busca.
     * @return a celula inicial
     */
    Labirinto::Cell getInitialCell() const { return initial_cell_; }

    /**
     * @brief Obtem a celula final, ou seja, aquela na qual terminara a busca.
     * @return a celula final
     */
    Labirinto::Cell getFinalCell() const { return final_cell_; }


    /**
     * @brief Obtem a celula atual na busca.
     * @return a celula atual
     */
    Labirinto::Cell getCurrentCell();


    /**
     * @brief Define a celula inicial
     * @param cell uma celula valida
     */
    void setInitialCell(const Labirinto::Cell& cell) { initial_cell_ = cell; }

    /**
     * @brief Define a celula final
     * @param cell uma celula valida
     */
    void setFinalCell(const Labirinto::Cell& cell) { final_cell_ = cell; }

    /**
     * @brief Define a celula atual
     * @param cell uma celula valida
     */
    void setCurrentCell(const Labirinto::Cell& cell) { current_cell_ = cell; }


private slots:
    /**
     * @brief Atualiza a posicao da celula na busca pela celula final.
     */
    void update();

private:
    QMutex mutex_; ///< garante exclusao mutua no acesso a celula atual
    QTimer timer_; ///< timer usado na atualizacao da celula atual
    Labirinto *labirinto_; ///< o labirinto usado na busca do caminho
    Labirinto::Cell initial_cell_; ///< a celula inicial
    Labirinto::Cell final_cell_; ///< a celula final
    Labirinto::Cell current_cell_; ///< a celula atual
    std::vector<Labirinto::Cell> memory_; ////< as ultimas celulas visitadas
    int memory_size_; ///< define o tamanho maximo da memoria
    int iq_; /// o coeficiente de inteligencia
};

#endif // PATHFINDERAI_H
