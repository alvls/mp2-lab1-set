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

TSet::TSet(int mp) : BitField(mp)
{
    MaxPower = mp;
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField)
{
    MaxPower = s.MaxPower;
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf)
{
    MaxPower = bf.GetLength();
}

TSet::operator TBitField()
{
    return BitField;
}

// получить макс. к-во эл-тов
int TSet::GetMaxPower(void) const
{
    return MaxPower;
}

// элемент множества?
int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower) throw "Element out of range";
    return BitField.GetBit(Elem);
}

// включение элемента множества
void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) throw "Element out of range";
    BitField.SetBit(Elem);
}

// исключение элемента множества
void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower) throw "Element out of range";
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

// присваивание
TSet& TSet::operator=(const TSet &s)
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return *this;
}

// сравнение
int TSet::operator==(const TSet &s) const
{
    return (BitField == s.BitField);
}

// сравнение
int TSet::operator!=(const TSet &s) const
{
    return !(*this == s);
}

// объединение
TSet TSet::operator+(const TSet &s)
{
    TBitField resultBitField = BitField | s.BitField;
    return TSet(resultBitField);
}

// объединение с элементом
TSet TSet::operator+(const int Elem)
{
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

// разность с элементом
TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

// пересечение
TSet TSet::operator*(const TSet &s)
{
    TBitField resultBitField = BitField & s.BitField;
    return TSet(resultBitField);
}

// дополнение
TSet TSet::operator~(void)
{
    return TSet(~BitField);
}

// перегрузка ввода/вывода

// ввод
istream &operator>>(istream &istr, TSet &s) {
    int elem;
    while (istr >> elem) {
        if (elem >= 0 && elem < s.GetMaxPower()) {
            s.InsElem(elem);
        }
    }
    return istr;
}

// вывод
ostream& operator<<(ostream &ostr, const TSet &s) {
    for (int i = 0; i < s.GetMaxPower(); i++) {
        if (s.IsMember(i)) {
            ostr << i << " ";
        }
    }
    return ostr;
}
