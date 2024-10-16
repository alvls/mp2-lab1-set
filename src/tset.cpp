#include "tset.h"
#include <stdexcept>

// Конструктор с параметром MaxPower
TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {
    if (mp < 0) {
        throw std::invalid_argument("Max power must be non-negative");
    }
}

// Конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

// Конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

// Преобразование в битовое поле
TSet::operator TBitField() {
    return BitField;
}

// Получение максимального количества элементов
int TSet::GetMaxPower(void) const {
    return MaxPower;
}

// Проверка, является ли элемент членом множества
int TSet::IsMember(const int Elem) const {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of bounds");
    }
    return BitField.GetBit(Elem);
}

// Включение элемента в множество
void TSet::InsElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of bounds");
    }
    BitField.SetBit(Elem);
}

// Исключение элемента из множества
void TSet::DelElem(const int Elem) {
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of bounds");
    }
    BitField.ClrBit(Elem);
}

// Операции над множествами

TSet& TSet::operator=(const TSet& s) { // Присваивание
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const { // Сравнение
    return BitField == s.BitField;
}

int TSet::operator!=(const TSet& s) const { // Сравнение
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) { // Объединение
    int maxPower = std::max(MaxPower, s.MaxPower);
    TSet temp(maxPower);
    temp.BitField = BitField | s.BitField; // Объединяем битовые поля
    return temp;
}

TSet TSet::operator+(const int Elem) { // Объединение с элементом
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of bounds");
    }
    TSet temp(*this);
    temp.InsElem(Elem); // Включаем элемент в новое множество
    return temp;
}

TSet TSet::operator-(const int Elem) { // Разность с элементом
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element out of bounds");
    }
    TSet temp(*this);
    temp.DelElem(Elem); // Исключаем элемент из нового множества
    return temp;
}

TSet TSet::operator*(const TSet& s) {
    int maxSize = std::max(MaxPower, s.MaxPower);
    TSet result(maxSize);
    for (int i = 0; i < maxSize; ++i) {
        if (i < MaxPower && i < s.MaxPower && IsMember(i) && s.IsMember(i)) {
            result.InsElem(i);
        }
    }
    return result;
}

TSet TSet::operator~(void) { // Дополнение
    TSet result(MaxPower);
    for (int i = 0; i < MaxPower; ++i) {
        if (!IsMember(i)) {
            result.InsElem(i); // Включаем элемент, если он отсутствует в исходном множестве
        }
    }
    return result;
}

// Перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) { // Ввод
    int temp;
    s.DelElem(temp); // Очищаем текущее множество
    while (istr >> temp) {
        if (temp < 0 || temp >= s.GetMaxPower()) {
            throw std::out_of_range("Input element out of bounds");
        }
        s.InsElem(temp); // Включаем новый элемент
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) { // Вывод
    for (int i = 0; i < s.GetMaxPower(); ++i) {
        if (s.IsMember(i)) {
            ostr << i << " "; // Выводим элементы
        }
    }
    return ostr;
}