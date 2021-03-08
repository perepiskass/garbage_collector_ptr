#pragma once
#include <stdexcept>

/**
 * @brief Класс-итератор для циклического перемещения в массивах, на которые указывают указатели GCP(Garbage Collector Pointer)
 */
template <typename T>
class Iterator
{
    private:
        T* m_ptr;         /// значение текущего указателя
        T* m_end;         /// указатель на элемент, следующий за последним элементом
        T* m_begin;       /// указатель на начало распределенного массива
        size_t m_length;  /// длина массива

    public:
        Iterator():m_ptr(nullptr), m_end(nullptr), m_begin(nullptr), m_length(0)
        {}

        Iterator(T* p, T* first, T* last):m_ptr(p), m_end(last), m_begin(first), m_length(last - first)
        {}

        size_t size()       /// возвращает длинну последовательности на которую указывает итератор
        {
            return m_length;
        }

        T& operator*()      /// возвращает значение, на которое указывает ptr, проверяет на выход за пределы диапазона
        {
            if(m_ptr >= m_end || m_ptr < m_begin)
                throw std::out_of_range("Указатель за пределами диапазона");
            return *m_ptr;
        }

        T* operator->()     /// возвращает адресс содержащийся в ptr, проверяет на выход за пределы диапазона
        {
            if(m_ptr >= m_end || m_ptr < m_begin)
                throw std::out_of_range("Указатель за пределами диапазона");
            return m_ptr;
        }

        Iterator operator++()   /// префиксный оператор ++
        {
            ++m_ptr;
            return *this;
        }

        Iterator operator--()   /// префиксный оператора --
        {
            --m_ptr;
            return *this;
        }

        Iterator operator++(int NOTUSED)    /// постфиксный оператор ++
        {
            T* tmp = m_ptr++;
            return Iterator<T>(tmp, m_begin, m_end);
        }

        Iterator operator--(int NOTUSED)    /// постфиксный оператор --
        {
            T* tmp = m_ptr--;
            return Iterator<T>(tmp, m_begin, m_end);
        }

        Iterator operator+(int n)   /// прибавляет целое к итератору
        {
            m_ptr += n;
            return *this;
        }

        Iterator operator-(int n)   /// вычитает целое из итератора
        {
            m_ptr -= n;
            return *this;
        }

        T& operator[](size_t idx)       /// возвращает ссылку на объект с заданным индексом, проверяет на выход за пределы диапазона
        {
            if(idx < 0 || idx >= (m_end - m_begin))
                throw std::out_of_range("Указатель за пределами диапазона");
            return m_ptr[i];
        }

        friend int operator- (const Iterator<T>& first, const Iterator<T>& second); /// возвращает число элементов между двумя итераторами
        friend bool operator==(const Iterator<T>& first, const Iterator<T>& second);
        friend bool operator!=(const Iterator<T>& first, const Iterator<T>& second);
        friend bool operator<(const Iterator<T>& first, const Iterator<T>& second);
        friend bool operator<=(const Iterator<T>& first, const Iterator<T>& second);
        friend bool operator>(const Iterator<T>& first, const Iterator<T>& second);
        friend bool operator>=(const Iterator<T>& first, const Iterator<T>& second);
};

template<typename T>
int operator-(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr - second.m_ptr;
}

template<typename T>
bool operator==(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr == second.m_ptr;
}

template<typename T>
bool operator!=(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr != second.m_ptr;
}

template<typename T>
bool operator<(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr < second.m_ptr;
}

template<typename T>
bool operator<=(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr <= second.m_ptr;
}

template<typename T>
bool operator>(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr > second.m_ptr;
}

template<typename T>
bool operator>=(const Iterator<T>& first, const Iterator<T>& second)
{
    return first.m_ptr >= second.m_ptr;
}