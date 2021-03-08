#pragma once

/**
 * @brief Класс определяет элемент, который запоминается в информационном списке сбора мусора
 */
template<typename T>
class GCInfo
{
    public:
        size_t m_refCount;    /// текущее число ссылок
        T* m_ptr;             /// указатель на выделенную память
        bool m_isArray;       /// флаг, показывает на что указывает m_ptr(на массив или нет)
        size_t m_arraySize;   /// если m_isarray true, то тут храниться размер массива

    public:
        GCInfo(T* memoryPtr, size_t size = 0):m_refCount(1), m_ptr(memoryPtr), m_isArray(size), m_arraySize(size)
        {}
};

template<typename T>
bool operator==(const GCInfo<T>& first, const GCInfo<T>& second)
{
    return (first.m_ptr == second.m_ptr);
}