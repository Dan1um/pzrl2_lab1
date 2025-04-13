#pragma once
#include <algorithm>
#include <string>
using ValueType = double;
class Vector
{
public:
    Vector() = default;
    Vector(const ValueType* rawArray, const size_t size, float coef = 2.0f) : _data(new ValueType[size]), _size(size), _capacity(size), _multiplicativeCoef(coef){
        std::copy(rawArray, rawArray + size, _data);
    }
    explicit Vector(const Vector& other) : _data(new ValueType[other._size]), _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef){
        std::copy(other._data, other._data + other._size, _data);
    }
    Vector& operator=(const Vector& other){
        if (this != &other){
            delete[] _data;
            _data = new ValueType[other._size];
            _size = other._size;
            _capacity = other._size;
            _multiplicativeCoef = other._multiplicativeCoef;
            std::copy(other._data, other._data + other._size, _data);
        }
        return *this;
    }
    explicit Vector(Vector&& other) noexcept : _data(other._data), _size(other._size), _capacity(other._capacity), _multiplicativeCoef(other._multiplicativeCoef) {
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }
    Vector& operator=(Vector&& other) noexcept{
        if (this != &other){
            delete[] _data;
            _data = other._data;
            _size = other._size;
            _capacity = other._capacity;
            _multiplicativeCoef = other._multiplicativeCoef;
            other._data = nullptr;
            other._capacity = 0;
            other._size = 0;
        }
        return *this;
    }
    ~Vector(){
        delete[] _data;
    }

    void pushBack(const ValueType& value){
        reserve(++_size);
        _data[++_size] = value;
    }
    void pushFront(const ValueType& value){
        reserve(++_size);
        std::memmove(_data + 1, _data, _size * sizeof(ValueType));
        _data[0] = value;
        ++_size;
    }

    void insert(const ValueType& value, size_t pos){
        if (pos > _size) pos = _size;
        reserve(++_size);
        std::memmove(_data + pos + 1, _data + pos, (_size - pos) * sizeof(ValueType));
        _data[pos] = value;
        ++_size;
    }
	
    void insert(const ValueType* values, size_t size, size_t pos){
        if (pos > _size) pos = _size;
        reserve(_size + size);
        std::memmove(_data + pos + size, _data + pos, (_size - pos) * sizeof(ValueType));
        std::copy(values, values + 1, _data + pos);
        _size += size;
    }

    void insert(const Vector& vector, size_t pos){
        insert(vector._data, vector._size, pos);
    }

    void popBack(){
        --_size;
    }

    void popFront(){
        std::memmove(_data, _data + 1, (_size - 1) * sizeof(ValueType));
        --_size;
    }

    void erase(size_t pos, size_t count = 1){
        if (pos >= _size) return;
        if (pos + count > _size) count = _size - pos;
        std::memmove(_data + pos, _data + pos + count, (_size - pos - count) * sizeof(ValueType));
        _size -= count;
    }

    void eraseBetween(size_t beginPos, size_t endPos){
        if (beginPos >= _size || beginPos >= endPos) return;
        if (endPos > _size) endPos = _size;
        erase(beginPos, endPos - beginPos);
    }

    size_t size() const { return _size; }

    size_t capacity() const { return _capacity; };

    double loadFactor() const{
        if (_capacity == 0) return 0.0;
        return static_cast<double>(_size) / _capacity;
    }

    ValueType& operator[](size_t idx){ return _data[idx]; }

    const ValueType& operator[](size_t idx) const { return _data[idx]; };

    long long find(const ValueType& value) const {
        for (size_t i = 0; i < _size; ++i){
            if (_data[i] == value) return static_cast<long long>(i);
        }
        return -1;
    }
   
    void reserve(size_t capacity){
        if(capacity > _capacity){
            size_t newCapacity = std::max(static_cast<size_t>(_capacity * _multiplicativeCoef), capacity);
            ValueType* newData = new ValueType[newCapacity];
            std::copy(_data, _data+_size, newData);
            delete[] _data;
            _data = newData;
            _capacity = newCapacity;
        }
    }

    void shrinkToFit(){
        if (_capacity > _size){
            ValueType* newData = new ValueType[_size];
            std::copy(_data, _data+_size, newData);
            delete[] _data;
            _data = newData;
            _capacity = _size;
        }
    }
	
    class Iterator
    {
        ValueType* _ptr;
    public:
        explicit Iterator(ValueType* ptr);
        ValueType& operator*();
        const ValueType& operator*() const;
        ValueType* operator->();
        const ValueType* operator->() const;
        Iterator operator++();
        Iterator operator++(int);
        bool operator==(const Iterator& other) const;
        bool operator!=(const Iterator& other) const;
    };

    Iterator begin() { return Iterator(_data); }

    Iterator end() { return Iterator(_data + _size); };
private:
    ValueType* _data = nullptr; //! "сырой" указатель на данные
    size_t _size = 0; //! текущий размер
    size_t _capacity = 0; //! максимальный размер (количество элементов, под которое выделена память)
    float _multiplicativeCoef = 2.0f; //! коэффициент увеличения _capacity вектора при _size == _capacity
};