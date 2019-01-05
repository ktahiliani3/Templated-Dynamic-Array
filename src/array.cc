#include<array>
#include<initializer_list>
#include <array.h>

template <typename T>
array<T>::array()
{
    m_elements = nullptr;
    m_size = 0;
    m_reserved_size = 0;
}

//initialize array
template <typename T>
array<T>::array(std::initializer_list<T> rhs)
{
    m_size = rhs.size();
    m_reserved_size = m_size;
    m_elements = (T*)malloc(sizeof(T)*m_size);
    auto here = rhs.begin();
    for(int i = 0; i < m_size; i++){



        new(&m_elements[i]) T(*here);
        here++;
    }

}

// copy
template <typename T>
array<T>::array(const array& rhs)
{

    m_size = rhs.m_size;
    m_reserved_size = rhs.m_reserved_size;
    m_elements = (T*) malloc(m_reserved_size*sizeof(T));
    for(size_t i = 0; i < m_size; i++)
    {
        new (&m_elements[i]) T(rhs.m_elements[i]);
    }


}


//move
template <typename T>
array<T>::array(array&& rhs)
{
    if(this != &rhs)
    {
        m_size = rhs.m_size;
        m_reserved_size = rhs.m_reserved_size;
        m_elements = (T*) malloc(m_reserved_size*sizeof(T));
        for(size_t i = 0; i < m_size; i++)
        {
            new (&m_elements[i]) T(rhs.m_elements[i]);
        }


        rhs.m_size = 0;
        rhs.m_reserved_size = 0;
        free(rhs.m_elements);
        rhs.m_elements = nullptr;
    }
}



//construct (reserved size)

template <typename T>
array<T>::array(size_t n)
{

    m_reserved_size = n;
    m_elements = (T*) malloc(n*sizeof(T));
    m_size = 0;

}



//n copies of T



template <typename T>
array<T>::array(size_t n, const T& t)
{
    m_size = n;
    m_elements = (T*) malloc(n*sizeof(T));
    for (int i = 0; i < m_size; i++)
    {
        new (&m_elements[i]) T(t);
    }
}



//destructor


template <typename T>
array<T>::~array(){

    for (int i = 0; i < m_size; i++){

        m_elements[i].~T();
    }

    free(m_elements);
    m_size = 0;
    m_elements = nullptr;
    m_reserved_size = 0;
}


//reserve

template <typename T>
void array<T>::reserve(size_t m){

 T* temp = (T*)malloc(sizeof(T)*m);

 for (int i = 0; i < m_size; i++){

    new (&temp[i]) T(m_elements[i]);
    m_elements[i].~T();
 }

free(m_elements);
m_elements = temp;
m_reserved_size = m;

}

//push back

template <typename T>
void array<T>::push_back(const T& rhs)
{

    if (m_size >= m_reserved_size)
    {
        T* temp;
        size_t tempC = m_size;
        m_reserved_size++;
        temp = (T*)malloc(sizeof(T)*m_reserved_size);
        for(size_t i = 0; i < length(); i++){

            new(&temp[i]) T(m_elements[i]);
            m_elements[i].~T();
        }

        new(&temp[length()]) T(rhs);
        free(m_elements);
        m_size = tempC + 1;
        m_elements = temp;
    }

    else
    {
        new(&m_elements[length()]) T(rhs);
        m_size++;
    }
}



//push front

template <typename T>
void array<T>::push_front(const T& rhs)
{

    if(m_size >= m_reserved_size)
    {

        T* temp = (T*)malloc(sizeof(T)*(m_size+1));
        for (int i = m_size; i > 0; i--){

            new(&temp[i]) T(std::move(m_elements[i-1]));
            m_elements[i-1].~T();

        }
        free(m_elements);
        new (&temp[0]) T(rhs);
        m_elements = temp;
        m_size++;
        m_reserved_size = m_size;


    }
    else{

        for(int i = m_size; i > 0; i--)
        {
            m_elements[i] = m_elements[i-1];
        }
        new(&m_elements[0]) T(rhs);
        m_size++;

    }

}


//pop back

template <typename T>
void array<T>::pop_back()
{
    if(m_size != 0)
    {
        m_size--;
        m_elements[m_size].~T();
    }
}



//pop front

template <typename T>
void array<T>::pop_front()
{
    for (int i = 0; i < m_size - 1; i++){

        m_elements[i] = std::move(m_elements[i+1]);

    }

    pop_back();
}


//give out first element

template <typename T>
T& array<T>::front() const
{
    return *(m_elements);
}


//give out last element

template <typename T>
T& array<T>::back() const
{
    return *(m_elements + m_size - 1);
}


//give out specified element

template <typename T>
const T& array<T>::operator[](size_t p) const
{
    return *(m_elements + p);
}


//give out specified element
template <typename T>
T& array<T>::operator[](size_t p)
{
    return *(m_elements + p);
}




//give out number of elements

template <typename T>
size_t array<T>::length() const
{
    return m_size;
}


//check if the array is empty

template <typename T>
bool array<T>::empty() const
{
    if(m_size == 0)
        return true;
    else
    {
        return false;
    }
}


//clear the array

template <typename T>
void array<T>::clear()
{
    while(m_size != 0)
    {
        pop_back();
    }
}



//access the iterator begin and end
template <typename T>
array_iterator<T> array<T>::begin() const
{
    return array_iterator<T>(m_elements);
}



template <typename T>
array_iterator<T> array<T>::end() const
{

    return array_iterator<T>(m_elements + m_size);

}




//ERASE AND INSERT

template <typename T>
void array<T>::erase(const array_iterator<T>& temp1)
{
    size_t start = 0;
    array_iterator<T> here;
    here = begin();
    while(here != end())
    {

        if(here == temp1) break;
        start++;
        here++;

    }

    m_elements[start].~T();
    for(size_t i = start + 1; i < m_size; i++){

        new(&m_elements[i-1]) T(m_elements[i]);
        m_elements[i].~T();

    }

    m_size--;
}


template <typename T>
void array<T>::insert(const T& temp, const array_iterator<T>& temp1)
{

    size_t start = 0;
    size_t tempC;
    array_iterator<T> here;
    start = begin();

    while(here != end()){

        if(start == temp1) break;
        start++;
        here++;

    }

    m_reserved_size++;
    T* tempV;
    tempV = ( T*)malloc(sizeof(T)*m_reserved_size);
    tempC = m_size;

    for(size_t i = 0; i < start; i++){

        new(&tempV[i]) T(m_elements[i]);
    }

    new(&tempV[start]) T(temp);
    start++;
    for(size_t i = start; i < m_reserved_size; i++){

        new(&tempV[i]) T(m_elements[i - 1]);

    }

    clear();
    free(m_elements);
    m_elements = tempV;
    m_size = tempC;
    m_size++;



}


// Iterators


template <typename T>
array_iterator<T>::array_iterator()
{
    m_current = NULL;
}

template <typename T>
array_iterator<T>::array_iterator(T* rand)
{
    m_current = rand;
}

template <typename T>
array_iterator<T>::array_iterator(const array_iterator<T>& x)
{
    m_current = x.m_current;
}

template <typename T>
T& array_iterator<T>::operator*() const
{
    return *m_current;
}

template <typename T>
array_iterator<T> array_iterator<T>::operator++()
{
    return array_iterator<T>(m_current++);
}


template <typename T>
array_iterator<T> array_iterator<T>::operator++(int)
{

    return array_iterator<T>(m_current++);

}

template <typename T>
bool array_iterator<T>::operator != (const array_iterator<T>& x) const
{
    if(m_current != x.m_current)
        return true;
    else return false;
}


template <typename T>
bool array_iterator<T>::operator == (const array_iterator<T>& x) const
{
    if(m_current == x.m_current)
        return true;
    else return false;
}


