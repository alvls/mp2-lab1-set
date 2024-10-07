#include "tset.h"

// Конструктор с параметром maxElemCount. Инициализирует битовое поле.
TSet::TSet(int maxElemCount) : BitField(1) {
    MaxPower = maxElemCount; // Максимальное количество элементов
    BitField = TBitField(maxElemCount); // Инициализация битового поля
}

// Конструктор копирования. Создает новый набор на основе существующего.
TSet::TSet(const TSet& otherSet) : BitField(otherSet.BitField) {
    MaxPower = otherSet.MaxPower; // Копируем максимальное количество элементов
    BitField = otherSet.BitField; // Копируем битовое поле
}

// Конструктор преобразования типа из TBitField в TSet.
TSet::TSet(const TBitField& bf) : BitField(bf) {
    MaxPower = bf.GetLength(); // Устанавливаем максимальное количество элементов на основе длины битового поля
}

// Преобразование TSet в TBitField.
TSet::operator TBitField() {
    return BitField; // Возвращаем битовое поле
}

// Возвращает максимальное количество элементов в наборе.
int TSet::GetMaxPower() const {
    return MaxPower;
}

// Проверяет, является ли элемент членом множества. Возвращает 1, если член, и 0 в противном случае.
int TSet::IsMember(int elem) const {
    return BitField.GetBit(elem); // Используем битовое поле для проверки принадлежности
}

// Вставляет элемент в множество.
void TSet::InsElem(int elem) {
    BitField.SetBit(elem); // Устанавливаем бит в битовом поле
}

// Удаляет элемент из множества.
void TSet::DelElem(int elem) {
    BitField.ClrBit(elem); // Сбрасываем бит в битовом поле
}

// Операции теории множеств

// Перегрузка оператора присваивания.
TSet& TSet::operator=(const TSet& otherSet) {
    if (this != &otherSet) { // Проверяем самоприсваивание
        MaxPower = otherSet.MaxPower; // Копируем максимальное количество элементов
        BitField = otherSet.BitField; // Копируем битовое поле
    }
    return *this; // Возвращаем текущий объект
}

// Перегрузка оператора сравнения на равенство.
int TSet::operator==(const TSet& otherSet) const {
    return (MaxPower == otherSet.MaxPower) && (BitField == otherSet.BitField); // Сравниваем количество элементов и битовые поля
}

// Перегрузка оператора сравнения на неравенство.
int TSet::operator!=(const TSet& otherSet) const {
    return !(*this == otherSet); // Возвращаем противоположное значение результата сравнения
}

// Перегрузка оператора сложения для объединения двух множеств.
TSet TSet::operator+(const TSet& otherSet) {
    int maxPower = (MaxPower > otherSet.MaxPower) ? MaxPower : otherSet.MaxPower; // Находим максимальное количество элементов
    TSet resultSet(maxPower); // Создаем новое множество
    resultSet.BitField = BitField | otherSet.BitField; // Устанавливаем объединение битовых полей
    return resultSet; // Возвращаем результат
}

// Перегрузка оператора сложения для добавления элемента в множество.
TSet TSet::operator+(int elem) {
    TSet resultSet(MaxPower); // Создаем новое множество
    resultSet.BitField.SetBit(elem); // Устанавливаем бит для нового элемента
    return resultSet; // Возвращаем результат
}

// Перегрузка оператора вычитания для удаления элемента из множества.
TSet TSet::operator-(int elem) {
    TSet resultSet(MaxPower); // Создаем новое множество
    resultSet.BitField.ClrBit(elem); // Сбрасываем бит для элемента
    return resultSet; // Возвращаем результат
}

// Перегрузка оператора умножения для нахождения пересечения двух множеств.
TSet TSet::operator*(const TSet& otherSet) {
    int maxPower = (MaxPower > otherSet.MaxPower) ? MaxPower : otherSet.MaxPower; // Находим максимальное количество элементов
    TSet resultSet(maxPower); // Создаем новое множество
    resultSet.BitField = BitField & otherSet.BitField; // Устанавливаем пересечение битовых полей
    return resultSet; // Возвращаем результат
}

// Перегрузка оператора инверсии для инвертирования битов множества.
TSet TSet::operator~() {
    TSet resultSet(MaxPower); // Создаем новое множество
    resultSet.BitField = ~BitField; // Инвертируем биты битового поля
    return resultSet; // Возвращаем результат
}

// Перегрузка оператора ввода.
std::istream& operator>>(std::istream& inStream, TSet& set) {
    return inStream; // Возвращаем входной поток (добавьте логику для чтения, если нужно)
}

// Перегрузка оператора вывода.
std::ostream& operator<<(std::ostream& outStream, const TSet& set) {
    outStream << set.BitField; // Выводим битовое поле множества
    return outStream; // Возвращаем выходной поток
}
