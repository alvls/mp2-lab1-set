// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(mp) // конструктор инициализатор
{
    if (mp < 0)
        throw std::invalid_argument("The maximum power of the set must be non-negative");
    MaxPower = mp;
}

TSet::TSet(const TSet &s) : MaxPower(s.MaxPower), BitField(s.BitField) // конструктор копирования
{
}

TSet::TSet(const TBitField &bf) : MaxPower(bf.GetLength()), BitField(bf) // конструктор преобразования типа
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить максимальное количество элементов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества? (Elem - индекс элемента в битовом поле; Элемент - целое число, которому соответствует индекс в битовом поле)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element is out of range");
    return BitField.GetBit(Elem); // 1 - если бит установлен, 0 - если бит не установлен
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element is out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element is out of range");
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet &TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s)
    {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение
{
    if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
        return 1;
    return 0;
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TBitField bf = BitField | s.BitField;
    return TSet(bf);
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element is out of range");
    TSet result(*this);   // создание копии текущего множества
    result.InsElem(Elem); // включение элемента в множество
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("Element is out of range");
    TSet result(*this);   // создание копии текущего множества
    result.DelElem(Elem); // исключение элемента из множества
    return result;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int maxpow = (MaxPower > s.MaxPower) ? MaxPower : s.MaxPower;
    int minpow = (MaxPower < s.MaxPower) ? MaxPower : s.MaxPower;
    TSet result(maxpow);
    for (int i = 0; i < minpow; i++)
        if (BitField.GetBit(i) && s.BitField.GetBit(i))
            result.BitField.SetBit(i);
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TBitField bf = ~BitField;
    return TSet(bf);
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream &operator<<(ostream &ostr, const TSet &s) // вывод
{
    ostr << "{";
    bool first = true;
    for (int i = 0; i < s.GetMaxPower(); i++)
        if (s.IsMember(i))
        {
            if (!first)
                ostr << ", ";
            ostr << i;
            first = false;
        }
    ostr << "}";
    return ostr;
}
