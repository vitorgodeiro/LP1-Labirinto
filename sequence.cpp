#include "sequence.h"
template <typename TYPE>
Sequence<TYPE>::Sequence() {
}
template <typename TYPE>
Sequence<TYPE>::~Sequence()
{
    while(!isEmpty())
    {
        removeFirst();
    };
}

template <typename TYPE>
void Sequence<TYPE>::swap(Node *n1, Node *n2){
    Node *aux = &list;
    Node *auxx = new Node;
    while (aux->next != NULL)
    {
        if(aux==n1)
        {
            auxx->data=n1->data;
            aux->data=n2->data;
        }
        if(aux==n2)
        {
            aux->data=auxx->data;
        }
        aux = aux->next;
    }
    return;
}

template <typename TYPE>
bool Sequence<TYPE>::isEmpty()
{
    return (list.next == NULL);
}

template <typename TYPE>
int Sequence<TYPE>::getSize()
{
    int cont =0;
    Node *aux = &list;
    while (aux->next != NULL)
    {
        cont++;
        aux = aux->next;
    }
    return cont;
}

template <typename TYPE>
bool Sequence<TYPE>::addFirst(const TYPE &value) {
    Node *node = new Node;
    if(node == NULL)
        return false;
    node->data = value;
    node->next = list.next;
    list.next = node;
    if(node->next != NULL)
        node->next->prev = node;
    if(list.prev == NULL)
        list.prev = node;
    return true;
}

template <typename TYPE>
bool Sequence<TYPE>::addLast(const TYPE &value) {
    Node *node = new Node;
    if(node == NULL)
        return false;
    node->data = value;
    node->prev = list.prev;
    list.prev = node;
    if(node->prev != NULL)
        node->prev->next = node;
    if(list.next == NULL)
        list.next = node;
    return true;
}

template <typename TYPE>
bool Sequence<TYPE>::add(const TYPE &value, int pos)
 {
    Node *aux = &list;
    Node *aux7 = &list;
    while (aux->next != NULL)
    {
        if(pos < 0)
            return addFirst(value);
        else
        {
            if(pos == 0)
            {
                if(aux->next != NULL)
                {
                    Node *aux5 = new Node;
                    aux5->data=value;
                    aux5->next=aux->next;
                    aux5->prev=aux->prev;
                    aux->next = aux5;
                    return true;
                }
                else
                    break;
            }
        }
        pos--;
        aux = aux->next;
    }
    if(aux7->next != NULL)
        addLast(value);
    return false;
}

template <typename TYPE>
TYPE Sequence<TYPE>::removeFirst() {
    Node *node = list.next;
    if(node != NULL)
    {
        TYPE value = node->data;
        list.next = node->next;
        if(node->next != NULL)
            node->next->prev = NULL;
        if(list.prev == node)
            list.prev = NULL;
        delete node;
        return value;
    }
}

template <typename TYPE>
TYPE Sequence<TYPE>::removeLast() {
    Node *node = list.prev;
    if(node != NULL)
    {
        TYPE value = node->data;
        list.prev = node->prev;
        if(node->prev != NULL)
            node->prev->next = NULL;
        if(list.next == node)
            list.next = NULL;
        delete node;
        return value;
    }
}

template <typename TYPE>
TYPE Sequence<TYPE>::remove(int pos)
{
    Node *aux = &list;
    Node *aux7 = &list;
    while (aux->next != NULL)
    {
        if(pos <= 0)
            return removeFirst();
        else
        {
            if(pos == 0)
            {
                if(aux->next != NULL)
                {
                    Node *toBeDeleted = aux->next;
                    aux->next = aux->next->next;
                    delete toBeDeleted;
                    return true;
                }
                else
                    break;
            }
        }
        pos--;
        aux = aux->next;
    }
    if(aux7->next != NULL)
        removeLast();
    return false;
}

template <typename TYPE>
TYPE Sequence<TYPE>::getFirst() {
    if(list.next != NULL)
        return list.next->data;
}

template <typename TYPE>
TYPE Sequence<TYPE>::getLast() {
    if(list.prev != NULL)
        return list.prev->data;
}

template <typename TYPE>
TYPE Sequence<TYPE>::get(int pos) {
    Node *aux = &list;
    while (aux->next != NULL)
    {
            if(pos == 0)
            {
                if(aux->next != NULL)
                {
                    Node *toBeDeleted = aux->next;
                    aux->next = aux->next->next;
                    return toBeDeleted->data;
                }
                else
                    break;
            }
        pos--;
        aux = aux->next;
    }
    return false;
}

template <typename TYPE>
int Sequence<TYPE>::search(const TYPE &elm) {
    int pos = 0;
    Node *aux = &list;
    while (aux->next != NULL)
    {
                if(aux->next->data == elm)
                {
                    return pos;
                }
        pos++;
        aux = aux->next;
    }
    return -1;
}

template <typename TYPE>
bool Sequence<TYPE>::isEqual(Sequence<TYPE> &s) {
    Node *aux = &list;
    Node *k = &s.list;
    while (aux->next != NULL )
    {
                if(aux->next->data != k->next->data)
                {
                    return false;
                }
        aux = aux->next;
        k = k->next;
    }
    if(aux->next == NULL && k->next == NULL)
        return true;
    else
        return false;
}

template <typename TYPE>
void Sequence<TYPE>::reverse()
{
    int x = getSize();
    Node *aux = &list;
    if(list.next ==NULL)
        return;
    Node *aux3 = new Node;
    aux3->data=list.next->data;
    Node *aux1 =&list,*aux2 = &list;
    aux1=list.next;
    aux2 = list.prev;
    for(int cont=0;cont<x/2;cont++)
    {
        swap(aux1,aux2);
        if(cont ==0)
            aux2->data= aux3->data;
        aux1=aux1->next;
        aux2=aux2->prev;
    }
}

template <typename TYPE>
void Sequence<TYPE>::sort()
{
     int x = getSize();
     int k=x;
     Node *aux3 = new Node;
     Node *aux4 = new Node;
     Node *aux = &list;
     Node *aux2 = &list;
     for(int cont =0;cont<x;cont++)
     {
        aux2=aux2->next;
        aux3->data=aux2->data;
        aux=aux2;
        for(int f =0;f<k;f++)
        {
            if(aux3->data>aux->data)
            {
                aux4=aux;
                aux3->data=aux->data;
            }
            aux=aux->next;
        }
         aux4->data=aux2->data;
         aux2->data = aux3->data;
         k--;
     }
}

template <typename TYPE>
void Sequence<TYPE>::print()
{
    Node *aux = list.next;
    while (aux != NULL)
    {
        std::cout << aux->data ;
        aux = aux->next;
    }
}


