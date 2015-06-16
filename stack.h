#ifndef STACK_H
#define STACK_H
#include "sequence.h"
#include "queue.h"

template <typename TYPE>
class Stack {
    Sequence<TYPE> seq;
    Sequence<TYPE> seq2;

public:
    /**
        *Descrição: Metodo construtor.
        *
        *Parâmetros: Não possui.
        *
        *@return .
    **/
    Stack()  {};

      /**
        *Descrição: Metodo destrutor.
        *
        *Parâmetros: Não possui.
        *
        *@return .
    **/
    ~Stack() {};

      /**
        *Descrição: Metodo responsável por pegar o ultimo elemento da pilha.
        *
        *Parâmetros: Não possui.
        *
        *@return TYPE.
        *valor retornado: Retorna o valor do ultimo elemento.
    **/
    TYPE top()
    {
       return seq.getLast();
    }


      /**
        *Descrição: Metodo responsável por adicionar o ultimo elemento da pilha.
        *
        *Parâmetros:Value(o valor a ser adicionado).
        *
        *@return void.
      **/
    void push(TYPE value)
    {
        seq.addLast(value);
    }


      /**
        *Descrição: Metodo responsável por remover o ultimo elemento da pilha(seq).
        *
        *Parâmetros: Não possui.
        *
        *@return TYPE.
        *valor retornado: Retorna o valor do ultimo elemento.
      **/
    TYPE pop()
    {
        return seq.removeLast();
    }


      /**
        *Descrição: Metodo responsável por pegar o ultimo elemento da pilha(seq2).
        *
        *Parâmetros: Não possui.
        *
        *@return TYPE.
        *valor retornado: Retorna o valor do ultimo elemento.
    **/
    TYPE pop2()
    {
        return seq2.removeLast();
    }

     /**
        *Descrição: Metodo responsável por verficar se existe elemento da pilha1.
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
        *Descrição: Metodo responsável por verficar se existe elemento da pilha2.
        *
        *Parâmetros: Não possui.
        *
        *@return bool.
        *valor retornado: Se não possui elemento retorna true, se possui retorna false.
    **/
    bool isEmpty2()
    {
        return seq2.isEmpty();
    }

    /**
        *Descrição: Metodo responsável por imprimir os elementos da pilha.
        *
        *Parâmetros: Não possui.
        *
        *@return void.
    **/
    void print()
    {
        seq.print();
    }

    /**
        *Descrição: Metodo responsável por destruir a fila.
        *
        *Parâmetros: Não possui.
        *
        *@return void.
    **/
    void delSeq()
    {
        seq2.~Sequence();
    }
};

#endif // STACK_H
