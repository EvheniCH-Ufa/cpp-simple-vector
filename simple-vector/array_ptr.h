
/* Прошу прощения, в прошлый раз случайно нажал на кнопку "Отправить на проверку" на телефоне */
/* Сейчас все замечания исправлены */

#pragma once

#include <cassert>
#include <cstdlib>
#include <utility>



template <typename Type>
class ArrayPtr {
public:
    // Инициализирует ArrayPtr нулевым указателем
    ArrayPtr() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPtr(size_t size) {
        // Реализуйте конструктор самостоятельно
        delete[] raw_ptr_;
        raw_ptr_ = size ? new Type[size] : nullptr;
    }

    // Конструктор из сырого указателя, хранящего адрес массива в куче либо nullptr
    explicit ArrayPtr(Type* raw_ptr) noexcept {
        delete[] raw_ptr_;
        raw_ptr_ = raw_ptr;
    }


    explicit ArrayPtr(ArrayPtr& arr) noexcept {
        // Реализуйте конструктор самостоятельно   или в списке инициализации... это новое
        std::swap(raw_ptr_, arr.raw_ptr_);
    }

    // Запрещаем копирование
    ArrayPtr(const ArrayPtr&) = delete;

    ~ArrayPtr() {
        // Напишите деструктор самостоятельно
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }

    // Запрещаем присваивание
    ArrayPtr& operator=(const ArrayPtr&) = delete;

    ArrayPtr& operator=(ArrayPtr& arr)
    {
        if (raw_ptr_ != arr.raw_ptr_)
        {
            std::swap(raw_ptr_, arr.raw_ptr_);
        }
        return this;
    }

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        Type* tmp;
        std::exchange(tmp, raw_ptr_); //new
        raw_ptr_ = nullptr;
        return tmp;
    }

    // Возвращает ссылку на элемент массива с индексом index
    Type& operator[](size_t index) noexcept {
        // Реализуйте операцию самостоятельно
        return raw_ptr_[index];
    }

    // Возвращает константную ссылку на элемент массива с индексом index
    const Type& operator[](size_t index) const noexcept {
        // Реализуйте операцию самостоятельно
        return raw_ptr_[index];
    }

    // Возвращает true, если указатель ненулевой, и false в противном случае
    explicit operator bool() const {
        // Заглушка. Реализуйте операцию самостоятельно
        return !(raw_ptr_ == nullptr);
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        // Заглушка. Реализуйте метод самостоятельно
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPtr& other) noexcept {
        // Реализуйте метод самостоятельно
        std::swap(other.raw_ptr_, raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};
