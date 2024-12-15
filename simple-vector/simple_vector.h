#pragma once

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include <utility>


#include "array_ptr.h"
//#include "main.cpp"


class ReserveProxyObj
{
public:
    ReserveProxyObj(size_t new_capacity) : reserve_capacity_(new_capacity) {};
    size_t Get()
    {
        return reserve_capacity_;
    }

private:
    const size_t reserve_capacity_;
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) : data_(ArrayPtr<Type>(size))
    {
        // Напишите тело конструктора самостоятельно
        capacity_ = size;
        current_size_ = capacity_;

        //std::fill(std::make_move_iterator(begin()), std::make_move_iterator(end()), default_value);
        for (auto it_b = begin(); it_b != end(); ++it_b)
        {
            Type tmp{};
            *it_b = std::move(tmp);
        }
    }

    explicit SimpleVector(ReserveProxyObj obj_whith_new_capacity) : data_(ArrayPtr<Type>(obj_whith_new_capacity.Get()))
    {
        // Напишите тело конструктора самостоятельно
        capacity_ = obj_whith_new_capacity.Get();

        //  std::fill(data_.Get(), data_.Get() + capacity_, default_value);
        for (auto it_b = data_.Get(); it_b != data_.Get() + capacity_; ++it_b) // куратор разрешил такую замену
        {
            Type tmp{};
            *it_b = std::move(tmp);
        }

        current_size_ = 0;
    }


    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) : data_(ArrayPtr<Type>(size))
    {
        capacity_ = size;
        current_size_ = capacity_;

        for (auto it_b = begin(); it_b != end(); ++it_b)
        {
            Type tmp{ value };
            *it_b = std::move(tmp);
        }
    }

    SimpleVector(size_t size, Type&& value) : data_(ArrayPtr<Type>(size))
    {
        capacity_ = size;
        current_size_ = capacity_;
        std::fill(begin(), end(), value);
    }

    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) : data_(ArrayPtr<Type>(init.size()))
    {
        // Напишите тело конструктора самостоятельно
        capacity_ = init.size();
        current_size_ = capacity_;

        auto it = begin();
        for (const auto& elem : init)
        {
            *it = elem;
            ++it;
        }
    }

    void Reserve(size_t new_capacity)
    {
        if (new_capacity > capacity_)
        {
            ArrayPtr<Type> tmp_arr(new_capacity);
            std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), tmp_arr.Get());
            data_.swap(tmp_arr);
            capacity_ = new_capacity;
        }
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return current_size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return current_size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        // Напишите тело самостоятельно
        if (index >= current_size_)
        {
            index = current_size_ - 1;
        }

        return data_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Напишите тело самостоятельно
        if (index >= current_size_)
        {
            index = current_size_ - 1;
        }

        return data_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        // Напишите тело самостоятельно
        if (index >= current_size_)
        {
            throw std::out_of_range("index >= size");
        }

        return data_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        // Напишите тело самостоятельно
        if (index >= current_size_)
        {
            throw std::out_of_range("index >= size");
        }

        return data_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        // Напишите тело самостоятельно
        current_size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size == current_size_)
        {
            return;
        }

        if (new_size <= capacity_)
        {
            if (new_size > current_size_)
            {
                //  std::fill(data_.Get() + current_size_, data_.Get() + new_size, default_value);
                for (auto it_b = data_.Get() + current_size_; it_b != data_.Get() + new_size; ++it_b)
                {
                    Type tmp{};
                    *it_b = std::move(tmp);
                }

            }
            current_size_ = new_size;
        }
        else  // new_size > capacity_
        {
            size_t new_capacity;
            if (capacity_)
            {
                new_capacity = (1 * new_size / capacity_) * capacity_;
                if (new_size % capacity_ > 0)
                {
                    new_capacity += capacity_;
                }
            }
            else
            {
                new_capacity = new_size;
            }

            ArrayPtr<Type> tmp_arr(new_capacity);
            auto it_b = std::copy(std::make_move_iterator(begin()), std::make_move_iterator(end()), tmp_arr.Get());

            auto it_e = it_b + (new_capacity - capacity_);
            for (; it_b != it_e; ++it_b)
            {
                Type tmp{};
                *it_b = std::move(tmp);
            }

            data_.swap(tmp_arr);
            current_size_ = new_size;
            capacity_ = new_capacity;
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        // Напишите тело самостоятельно
        if (current_size_ == 0)
        {
            return nullptr;
        }

        return data_.Get();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        if (current_size_ == 0)
        {
            return nullptr;
        }
        return &(data_[current_size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        // Напишите тело самостоятельно
        if (current_size_ == 0)
        {
            return nullptr;
        }

        return &(data_[0]);
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        // Напишите тело самостоятельно
        if (current_size_ == 0)
        {
            return nullptr;
        }
        return &(data_[current_size_]);
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        // Напишите тело самостоятельно
        return begin();
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        // Напишите тело самостоятельно
        return end();
    }

    SimpleVector(const SimpleVector& other) : data_(ArrayPtr<Type>(other.GetSize()))
    {
        // Напишите тело конструктора самостоятельно
        std::copy(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), data_.Get());
        //        std::copy(other.begin(), other.end(), data_.Get());
        current_size_ = other.GetSize();
        capacity_ = current_size_;
    }

    SimpleVector(SimpleVector&& other) : data_(ArrayPtr<Type>(other.GetSize()))
    {
        // Напишите тело конструктора самостоятельно
        std::copy(std::make_move_iterator(other.begin()), std::make_move_iterator(other.end()), data_.Get());
//                std::copy(other.begin(), other.end(), data_.Get());
        current_size_ = std::move(other.current_size_);
        capacity_ = current_size_;

        other.current_size_ = 0;
        other.capacity_ = 0;
    }


    SimpleVector& operator=(const SimpleVector& rhs)
    {
        // Напишите тело конструктора самостоятельно
        if (data_.Get() == rhs.data_.Get())
        {
            return *this;
        }

        auto new_size = rhs.GetSize();
        ArrayPtr<Type> tmp_arr(new_size);
        std::copy(std::make_move_iterator(rhs.begin()), std::make_move_iterator(rhs.end()), tmp_arr.Get());
        data_.swap(tmp_arr);

        current_size_ = new_size;
        capacity_ = new_size;
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item)
    {
        // Напишите тело самостоятельно
        Resize(current_size_ + 1);
        //        memcpy((end() - 1), &item, sizeof(item));

        *(end() - 1) = item;
    }

    void PushBack(Type&& item)
    {
        // Напишите тело самостоятельно
        Resize(current_size_ + 1);
        *(end() - 1) = std::move(item);
    }


    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value)
    {
        // Напишите тело самостоятельно
        auto dist = std::distance(begin(), const_cast<Type*>(pos));

        Resize(current_size_ + 1);

        Type* it_last = const_cast<Type*>(end());
        --it_last;

        Type* it_first = const_cast<Type*>(begin());
        std::advance(it_first, dist);

        std::copy_backward(std::make_move_iterator(it_first), std::make_move_iterator(end() - 1), end());

        *it_first = value;
        return it_first;
    }


    Iterator Insert(ConstIterator pos, Type&& value)
    {
        // Напишите тело самостоятельно
        auto dist = std::distance(begin(), const_cast<Type*>(pos));

        Resize(current_size_ + 1);

        Type* it_last = const_cast<Type*>(end());
        --it_last;

        Type* it_first = const_cast<Type*>(begin());
        std::advance(it_first, dist);

        std::copy_backward(std::make_move_iterator(it_first), std::make_move_iterator(end() - 1), end());

        *it_first = std::move(value);
        return it_first;
    }


    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept
    {
        // Напишите тело самостоятельно
        if (current_size_ > 0)
        {
            --current_size_;
        }
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos)
    {
        if (current_size_ == 0)
        {
            return end();
        }

        // Напишите тело самостоятельно
        if (pos == end() - 1)
        {
            PopBack();
            return end();
        }

        Type* it_first = const_cast<Type*>(pos);
        ++it_first;

        Type* it_dest = const_cast<Type*>(pos);
        std::copy(std::make_move_iterator(it_first), std::make_move_iterator(end()), it_dest);
        --current_size_;

        return it_dest;
    }

    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept
    {
        // Напишите тело самостоятельно
        data_.swap(other.data_);
        std::swap(other.capacity_, capacity_);
        std::swap(other.current_size_, current_size_);
    }

private:
    ArrayPtr<Type> data_;
    size_t capacity_ = 0;
    size_t current_size_ = 0;
    Type default_value = Type{};
};

///  перегрузка операторов----------------------------------------------------------------------------

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !std::lexicographical_compare(rhs.cbegin(), rhs.cend(), lhs.cbegin(), lhs.cend());
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(rhs.cbegin(), rhs.cend(), lhs.cbegin(), lhs.cend());
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !std::lexicographical_compare(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}
