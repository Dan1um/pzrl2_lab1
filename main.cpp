#include "Vector.h"
#include <iostream>
#include <cassert>

// Утилита для печати содержимого вектора
void printVector(const Vector& v, const std::string& label = "Vector") {
    std::cout << label << ": [ ";
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "] (size = " << v.size() << ", capacity = " << v.capacity() << ")\n";
}

void testConstructorsAndAssignment() {
    std::cout << "--- testConstructorsAndAssignment ---\n";

    // Тест конструктора с массивом
    double arr[] = {1.0, 2.0, 3.0};
    Vector v1(arr, 3);
    assert(v1.size() == 3);
    printVector(v1, "v1");

    // Копирование
    Vector v2(v1);
    assert(v2.size() == 3);
    printVector(v2, "v2 (copy of v1)");

    // Присваивание копированием
    Vector v3;
    v3 = v1;
    assert(v3.size() == 3);
    printVector(v3, "v3 (assigned v1)");

    // Перемещение
    Vector v4(std::move(v1));
    printVector(v4, "v4 (moved from v1)");

    // Присваивание перемещением
    Vector v5;
    v5 = std::move(v2);
    printVector(v5, "v5 (moved from v2)");
}

void testPushPop() {
    std::cout << "--- testPushPop ---\n";

    Vector v;
    v.pushBack(10.0);
    v.pushBack(20.0);
    v.pushFront(5.0);
    printVector(v, "After pushFront + pushBack");

    v.popBack();
    v.popFront();
    printVector(v, "After popBack + popFront");
}

void testInsertErase() {
    std::cout << "--- testInsertErase ---\n";

    Vector v;
    v.pushBack(1.0);
    v.pushBack(2.0);
    v.pushBack(4.0);
    v.insert(3.0, 2); // вставка в середину
    v.insert(0.0, 0); // вставка в начало
    printVector(v, "After insert");

    v.erase(1, 2); // удаление элементов
    printVector(v, "After erase(1, 2)");

    v.eraseBetween(0, 2);
    printVector(v, "After eraseBetween(0, 2)");
}

void testFindAndAccess() {
    std::cout << "--- testFindAndAccess ---\n";

    Vector v;
    for (int i = 0; i < 5; ++i) v.pushBack(i * 10.0);

    assert(v[2] == 20.0);
    long long pos = v.find(30.0);
    std::cout << "Find 30 -> index: " << pos << "\n";

    pos = v.find(100.0);
    std::cout << "Find 100 -> index: " << pos << "\n";

    printVector(v);
}

void testIterator() {
    std::cout << "--- testIterator ---\n";

    Vector v;
    for (int i = 0; i < 5; ++i) v.pushBack(i);

    double sum = 0;
    for (auto it = v.begin(); it != v.end(); ++it) {
        sum += *it;
    }

    std::cout << "Sum using iterator: " << sum << "\n";
    assert(sum == 0 + 1 + 2 + 3 + 4);
}

int main() {
    std::cout << "LOL";
    testConstructorsAndAssignment();
    testPushPop();
    testInsertErase();
    testFindAndAccess();
    testIterator();

    std::cout << "✅ Все тесты прошли успешно.\n";
    return 0;
}
