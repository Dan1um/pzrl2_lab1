#include "vector.h"
#include <stdexcept>
#include <algorithm>
#include <cstring>

Vector::Vector() = default;

Vector::Vector(const ValueType* rawArray, const size_t size, float coef) : _data(new ValueType[size]), _size(size), _capacity(size), _multiplicativeCoef(coef){
    std::copy(rawArray, rawArray + size, _data);
}
Vector::Vector(const Vector& other) : _data(new ValueType[other._size]), _size(other._size), _capacity(other._size), _multiplicativeCoef(other._multiplicativeCoef){
    std::copy(other._data, other._data + other._size, _data);
}
Vector& Vector::operator=(const Vector& other) {
    if (this != &other) {
        delete[] _data;
        _data = new ValueType[other._size];
        _size = other._size;
        _capacity = other._size;
        _multiplicativeCoef = other._multiplicativeCoef;
        std::copy(other._data, other._data + other._size, _data);
    }
    return *this;
}
Vector::Vector(Vector&& other) noexcept : _data(other._data), _size(other._size), _capacity(other._capacity), _multiplicativeCoef(other._multiplicativeCoef) {
    other._data = nullptr;
    other._size = 0;
    other._capacity = 0;
}
Vector& Vector::operator=(Vector&& other) noexcept{
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
Vector::~Vector(){
    delete[] _data;
}

void Vector::pushBack(const ValueType& value){
    reserve(_size + 1);
    _data[_size++] = value;
}
void Vector::pushFront(const ValueType& value){
    reserve(_size + 1);
    std::memmove(_data + 1, _data, _size * sizeof(ValueType));
    _data[0] = value;
    ++_size;
}

void Vector::insert(const ValueType& value, size_t pos){
    if (pos > _size) pos = _size;
    reserve(_size + 1);
    std::memmove(_data + pos + 1, _data + pos, (_size - pos) * sizeof(ValueType));
    _data[pos] = value;
    ++_size;
}
	
void Vector::insert(const ValueType* values, size_t size, size_t pos){
    if (pos > _size) pos = _size;
    reserve(_size + size);
    std::memmove(_data + pos + size, _data + pos, (_size - pos) * sizeof(ValueType));
    std::copy(values, values + size, _data + pos);
    _size += size;
}

void Vector::insert(const Vector& vector, size_t pos){
    insert(vector._data, vector._size, pos);
}

void Vector::popBack(){
    if(_size == 0){
        throw std::out_of_range("Пустой вектор");
    }
    --_size;
}

void Vector::popFront(){
    if(_size == 0){
        throw std::out_of_range("Пустой вектор");
    }
    std::memmove(_data, _data + 1, (_size - 1) * sizeof(ValueType));
    --_size;    
}

void Vector::erase(size_t pos, size_t count){
    if (pos >= _size) return;
    if (pos + count > _size) count = _size - pos;
    std::memmove(_data + pos, _data + pos + count, (_size - pos - count) * sizeof(ValueType));
    _size -= count;
}

void Vector::eraseBetween(size_t beginPos, size_t endPos){
    if (beginPos >= _size || beginPos >= endPos) return;
    if (endPos > _size) endPos = _size;
    erase(beginPos, endPos - beginPos);
}

size_t Vector::size() const { return _size; }

size_t Vector::capacity() const { return _capacity; }

double Vector::loadFactor() const{
    if (_capacity == 0) return 0.0;
    return static_cast<double>(_size) / _capacity;
}

ValueType& Vector::operator[](size_t idx){
    if (idx >= _size) throw std::out_of_range("Выход за границы оператора []");
    return _data[idx]; 
}
const ValueType& Vector::operator[](size_t idx) const {
    if (idx >= _size) throw std::out_of_range("Выход за границы оператора []");
    return _data[idx];
}

long long Vector::find(const ValueType& value) const {
    for (size_t i = 0; i < _size; ++i){
        if (_data[i] == value) return static_cast<long long>(i);
    }
    return -1;
}
   
void Vector::reserve(size_t capacity){
    if(capacity > _capacity){
        size_t expanded = capacity;
        if(_capacity > 0){
            size_t newCapacity = static_cast<size_t>(capacity * _multiplicativeCoef);
            if (newCapacity > expanded){
                expanded == newCapacity;
            }
        }
        ValueType* newData = new ValueType[expanded];
        std::copy(_data, _data+_size, newData);
        delete[] _data;
        _data = newData;
        _capacity = expanded;
    }
}

void Vector::shrinkToFit(){
    if (_size == 0){
        delete[] _data;
        _data = nullptr;
        _capacity = 0;
    }
    else if (_capacity > _size){
        ValueType* newData = new ValueType[_size];
        std::copy(_data, _data+_size, newData);
        delete[] _data;
        _data = newData;
        _capacity = _size;
    }
}

Vector::Iterator::Iterator(ValueType* ptr) : _ptr(ptr) {}

ValueType& Vector::Iterator::operator*() {return *_ptr;}
const ValueType& Vector::Iterator::operator*() const {return *_ptr;}

ValueType* Vector::Iterator::operator->() {return _ptr;}
const ValueType* Vector::Iterator::operator->() const {return _ptr;}

Vector::Iterator Vector::Iterator::operator++() {++_ptr; return *this;}
Vector::Iterator Vector::Iterator::operator++(int) { Iterator tmp = *this; ++_ptr; return tmp; }

bool Vector::Iterator::operator==(const Iterator& other) const { return _ptr == other._ptr; }
bool Vector::Iterator::operator!=(const Iterator& other) const { return _ptr != other._ptr; }

Vector::Iterator Vector::begin() { return Iterator(_data); }
Vector::Iterator Vector::end() { return Iterator(_data + _size); };