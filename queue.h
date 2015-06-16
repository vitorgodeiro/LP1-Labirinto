#ifndef QUEUE_H
#define QUEUE_H
#include "sequence.h"

template <typename TYPE>
class Queue {
Sequence<TYPE> seq;
public:
    /**
        *Descrição: Metodo construtor.
        *
        *Parâmetros: Não possui.
        *
        *@return .
    **/
    Queue()  {}

    /**
    *Descrição: Metodo destrutor
    *
    *Parâmetros: Não possui.
    *
    *@return .
    **/
    ~Queue() {}

    /**
        *Descrição: Metodo responsável por pegar o primeiro elemento da fila.
        *
        *Parâmetros: Não possui.
        *
        *@return TYPE.
        *valor retornado: Retorna o primeiro elemento da fila.
    **/
    TYPE front()
    {
        return seq.getFirst();
    }

    /**
        *Descrição: Metodo responsável por adicionar o ultimo elemento da fila.
        *
        *Parâmetros: value(o valor a ser adicionado).
        *
        *@return bool.
        *valor retornado: Verdadeiro se o elemento foi adicionado ou falso caso contrário (memória cheia, por exemplo).
    **/
    bool enqueue(TYPE value)
    {
        return seq.addLast(value);
    }

    /**
        *Descrição: Metodo responsável por remover o primeiro elemento da fila.
        *
        *Parâmetros: Não possui.
        *
        *@return TYPE.
        *valor retornado: O valor do elemento removido.
    **/
    TYPE dequeue()
    {
        return seq.removeFirst();
    }

    /**
        *Descrição: Metodo responsável por verficar se existe elemento da fila.
        *
        *Parâmetros: Não possui.
        *
        *@return bool.
        *valor retornado: Se não possui elemento retorna true, se possui retorna false.
    **/
    bool isEmpty()
     {
        return seq.isEmpty();
    }

    /**
        *Descrição: Metodo responsável por imprimir os elementos da fila.
        *
        *Parâmetros: Não possui.
        *
        *@return void.
    **/
    void print()
    {
        seq.print();
    }
};

#endif // QUEUE_H
