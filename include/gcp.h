#pragma once
#include <list>
#include "stdlib.h"
#include "iterator.h"
#include "info_unit.h"

#define DISPLAY

/**
 * @brief Garbage Collector Pointer реализует тип указателя,
 * для сбора мусора и освобождающего неиспользуемую память.
 * GCP должен использоваться только для указания на динамически
 * выделенную память при помощи new.
 * Если применяется для ссылки на массив, определяется размер массива.
 */
template<typename T, size_t SIZE = 0>
class GCP
{
    private:
        static std::list<GCInfo<T>> m_gcList;    /// список для сбора муссора
        T* m_addr;                          /// указывает на выделенную память, на которую в данный момент указвает GCP
        bool m_isArray;                     /// true если GCP указывает на массив
        size_t m_arraSize;
        static bool m_first;                /// равен true, когда создается первый GCP

        typename std::list<GCInfo<T>>::iterator findPtrInfo(T* ptr);

    public:
        using GCIter = Iterator<T>;

        /// Конструкото для инициализированных и неинициализированных объектах
        GCP(T* ptr = nullptr):m_addr(ptr), m_isArray(SIZE ? true : false), m_arraSize(SIZE)
        {
            if(m_first)             /// регестрирует shutdown() как функцию завершения
                std::atexit(shutdown);
            
            m_first = false;

            auto p = findPtrInfo(m_addr);
            if(p != m_gcList.end())     /// если указатель ptr уже есть в списке, то увеличиваем счетчик ссылок на 1
                p->m_refCount++;
            else
            {
                m_gcList.emplace_front(m_addr, SIZE);  /// если нет, то создаем новый элемент в списке
            }
#ifdef DISPLAY
            std::cout << "Constructor GCP. ";
            if(m_isArray)
                std::cout << "Size is " << m_arraSize << std::endl;
            else
                std::cout << std::endl;    
#endif
        }

        /// Копирующий конструктоа
        GCP(const GCP& _gcp):m_addr(_gcp.m_addr), m_isArray(_gcp.m_isArray), m_arraSize(_gcp.m_arraSize)
        {
                auto p = findPtrInfo(_gcp.m_addr);
                p->m_refCount++;
#ifdef DISPLAY
                std::cout << "Copy constructor GCP. ";
                if(m_isArray)
                    std::cout << "Size is " << m_arraSize << std::endl;
                else
                    std::cout << std::endl;
                std::cout << "Aftre copybale constructor \n";
                showlist();    
#endif           
        }

        /// Присваивающий конструктор
        GCP(GCP&& _gcp):m_addr(_gcp.m_addr), m_isArray(_gcp.m_isArray), m_arraSize(_gcp.m_arraSize)
        {
            auto p = findPtrInfo(_gcp.m_addr);
            if(p != m_gcList.end())
                p->m_refCount++;
            else
            {
                m_gcList.emplace_front(m_addr, SIZE);
            }
            if(_gcp != nullptr)
            {
                _gcp.m_addr = nullptr;          /// обнуляем значение ссылки и увеличиваем счетчик значений на nullptr
                auto it = findPtrInfo(_gcp.m_addr);
                it->m_refCount++;
            }
            
#ifdef DISPLAY
            std::cout << "Assign constructor GCP. ";
            if(m_isArray)
                std::cout << "Size is " << m_arraSize << std::endl;
            else
                std::cout << std::endl;    
            std::cout << "Aftre assign constructor \n";
            showlist();    
#endif           
        }

        ~GCP();
        
        /// ФУНКЦИЯ СБОРА МУСОРА, вызывается в тот момент когда требуется обработать адреса памяти, и освободить память на которую указвает 0 ссылок
        static bool collect();

        GCP& operator= (T* ptr); 
        GCP& operator= (GCP& _gcp);
        GCP& operator= (GCP&& _gcp);

        T& operator*()
        {
            return *m_addr;
        }

        T& operator[](size_t idx)
        {
            if(idx < 0 || idx >= m_arraSize)
                throw std::out_of_range("Указатель за пределами диапазона");
            return m_addr[idx];
        }

        operator T*()   /// ОПАСНОЕ ПРИВЕДЕНИЕ, ПЕРЕДАЧА УКАЗАТЕЛЯ, ВЫЗЫВАЮЩИЙ КОД МОЖЕТ ВЫЗВАТЬ DELETE
        {
            return m_addr;
        }

        GCIter begin()
        {
            size_t size = 1;
            if(m_isArray)
                size = m_arraSize;
            return GCIter(m_addr, m_addr, m_addr + size);
        }

        GCIter end()
        {
            size_t size = 1;
            if(m_isArray)
                size = m_arraSize;
            return GCIter(m_addr + size, m_addr, m_addr + size);
        }

        /// Функция утилита для отображения списка (указатель -> кол-во ссылок на него)
        static void showlist();

        /// Очищает m_gcList, когда программа завршается
        static void shutdown();
};


/// выделение памяти для статической переменной списка
template<typename T, size_t SIZE>
 std::list<GCInfo<T>> GCP<T, SIZE>::m_gcList;

/// выделение памяти для статической переменной первого использования GCP
template<typename T, size_t SIZE>
 bool GCP<T, SIZE>::m_first = true;

/// Деструктор GCP
template<typename T, size_t SIZE>
GCP<T, SIZE>::~GCP()
{
        auto p = findPtrInfo(m_addr);
        if(p != m_gcList.end())
        {
            if(p->m_refCount)           /// если счетчик ссылок на данную область памяти не пуст, уменьшаем его
                p->m_refCount--;

#ifdef DISPLAY
        std::cout << "Destructor GCP\n";
#endif
        }
        collect();  /// вызываем функцию сбора мусора если объект GCP указывает на валидный адрес
}


/**
 * @brief Функция сбора мусора
 * @return возращает true если хотя бы один объект был удален
 */
template<typename T, size_t SIZE>
bool GCP<T, SIZE>::collect()
{
    bool memfreed = false;

#ifdef DISPLAY
    std::cout << "Befor garbage collector for ";
    showlist();
#endif

    std::list<GCInfo<T>>::iterator it;
    do
    {
        for(it = m_gcList.begin(); it != m_gcList.end(); ++it)
        {
            if(it->m_refCount > 0)  /// если использутеся то переходим к следующему в списке
                continue;
            
            memfreed = true;
            if(it->m_ptr)           /// если указатель не nullptr, вызываем очистку памяти
            {
                if(it->m_isArray)   /// если указывал на масси, вызываем delete[]
                {
#ifdef DISPLAY
                    std::cout << "Deleting array of size ";
                    std::cout << it->m_arraySize << std::endl;
#endif                 
                    delete[] it->m_ptr;
                }
                else
                {
#ifdef DISPLAY
                    std::cout << "Deleting: "
                              << *(T*) it->m_ptr << std::endl;
#endif
                    delete it->m_ptr;
                }
            }
            it = m_gcList.erase(it);   /// удаляем значение типа GCInfo из списка

            break;
        }
    } while (it != m_gcList.end());

#ifdef DISPLAY
    std::cout << "After garbage collector for ";
    showlist();
#endif
    return memfreed;
}

template<typename T, size_t SIZE>
GCP<T, SIZE>& GCP<T, SIZE>::operator=(T* ptr)
{
    if(m_addr != ptr)
    {
        auto it = findPtrInfo(m_addr);
        it->m_refCount--;               /// уменьшаем кол-во ссылок на которое сейчас указывал GCP

        it = findPtrInfo(ptr);          /// ищем в списке новый адресс
        if(it != m_gcList.end())
            it->m_refCount++;           /// если есть, увеличиваем его счетчик ссылок,
        else
        {
            m_gcList.emplace_front(ptr, SIZE);  /// если нет, создаем новый элемент в списке
        }
        m_addr = ptr;
    }
#ifdef DISPLAY
                    std::cout << "Operator = for (T*): "
                              << "ptr = ["<< (void*)ptr <<']' << " m_addr = [" << (void*)m_addr << ']' << std::endl;
#endif
    return *this;
}

template<typename T, size_t SIZE>
GCP<T, SIZE>& GCP<T, SIZE>::operator=(GCP& _gcp)
{
    if(m_addr != _gcp.m_addr)          /// если адреса на которые смотрят указатели GCP разные то выполняем
    {
        auto it = findPtrInfo(m_addr);
        it->m_refCount--;               /// уменьшаем кол-во ссылок на которое сейчас указывал GCP
        it = findPtrInfo(_gcp.m_addr);  /// ищем в списке новый адресс
        it->m_refCount++;           /// если есть, увеличиваем его счетчик ссылок,
        m_addr = _gcp.m_addr;
    }
#ifdef DISPLAY
                    std::cout << "Operator= copybale "
                              << "right = ["<< (void*)_gcp.m_addr <<']' << " left = [" << (void*)m_addr << ']' << std::endl;
#endif
    return *this;
}

/// Оператор присваивающего копирования
template<typename T, size_t SIZE>
GCP<T, SIZE>& GCP<T, SIZE>::operator=(GCP&& _gcp)
{

    if(m_addr != _gcp.m_addr)          /// если адреса на которые смотрят указатели GCP разные то выполняем
    {
        auto it = findPtrInfo(m_addr);
        it->m_refCount--;               /// уменьшаем кол-во ссылок на которое сейчас указывал GCP
        m_addr = _gcp.m_addr;
    }
    else
    {
        auto it = findPtrInfo(m_addr);
        it->m_refCount--;               /// уменьшаем кол-во ссылок на которое сейчас указывал GCP
    }
#ifdef DISPLAY
                    std::cout << "Operator= assign "
                              << "right&& = ["<< (void*)_gcp.m_addr <<']' << " left = [" << (void*)m_addr << ']' << std::endl;
#endif
    _gcp.m_addr = nullptr;          /// обнуляем значение ссылки и увеличиваем счетчик значений на nullptr
    auto it = findPtrInfo(_gcp.m_addr);
    it->m_refCount++;

    return *this;
}

template<typename T, size_t SIZE>
void GCP<T, SIZE>::showlist()
{
    std::cout << "gclist<" << typeid(T).name() << ", " << SIZE << ">:\n";
    std::cout << "memPrt\t\t\trefcount\t\tvalue\n";
    if(m_gcList.begin() == m_gcList.end())
    {
        std::cout << " -- Empty --\n\n";
        return;
    }

    for(auto& p : m_gcList)
    {
        std::cout << '[' << (void*)p.m_ptr << ']'
                  << "\t\t" << p.m_refCount;
        if(p.m_ptr)
                  std::cout << "\t\t" << *p.m_ptr;
        else
                  std::cout << "\t\t---";
        std::cout << std::endl;
    }
    std::cout << "=====================================================" << std::endl;
}


template<typename T, size_t SIZE>
typename std::list<GCInfo<T>>::iterator GCP<T, SIZE>::findPtrInfo(T* ptr)
{
    std::list<GCInfo<T>>::iterator it;
    for(it = m_gcList.begin(); it != m_gcList.end(); ++it)
    {
        if(it->m_ptr == ptr)
            return it;
    }
    return it;
}

template<typename T, size_t SIZE>
void GCP<T, SIZE>::shutdown()
{
    if(m_gcList.size() == 0)
        return;
    
    for(auto& it : m_gcList)
    {
        it.m_refCount = 0;
    }

#ifdef DISPLAY
    std::cout << "Befor collecting for shutdown() for "
              << typeid(T).name() << "\n";
#endif

    collect();

#ifdef DISPLAY
    std::cout << "After collecting for shutdown() for "
              << typeid(T).name() << "\n";
#endif  

}
