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

TSet::TSet(int mp) : BitField(mp), MaxPower(mp) {}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()) {}

TSet::operator TBitField() { return BitField; }

int TSet::GetMaxPower(void) const { return MaxPower; } // получить макс. к-во эл-тов 

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem >= BitField.GetLength() || Elem < 0)
        throw "ERROR";
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem >= BitField.GetLength() || Elem < 0)
        throw "ERROR";
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem >= BitField.GetLength() || Elem < 0)
        throw "ERROR";
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (&s != this)
    {
        MaxPower = s.MaxPower;     // максимальная мощность множества
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    if ((MaxPower == s.MaxPower) && (BitField == s.BitField))
        return 1;
    else
        return 0;
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    if (MaxPower != s.MaxPower || BitField != s.BitField)
        return 1;
    else
        return 0;
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TBitField tmp = BitField | s.BitField;
    TSet out(tmp);
    return out;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem > MaxPower - 1 || Elem < 0)
        throw "ERROR";
    TSet out(*this);
    out.BitField.SetBit(Elem);
    return out;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem > MaxPower - 1 || Elem < 0)
        throw "ERROR";
    TSet out(*this);
    out.BitField.ClrBit(Elem);
    return out;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    TBitField tmp = BitField & s.BitField;
    TSet out(tmp);
    return out;
}

TSet TSet::operator~(void) // дополнение
{
    TSet out(~BitField);
    return out;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    ostr << s.BitField;
    return ostr;
}
