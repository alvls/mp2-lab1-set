// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"
#include <stdexcept>

// -------------------- конструкторы --------------------

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) {}

TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) {}

TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) {}

TSet::operator TBitField() { return BitField; }

// -------------------- доступ --------------------

int TSet::GetMaxPower(void) const { return MaxPower; }

int TSet::IsMember(const int Elem) const
{
    if (Elem < 0 || Elem >= MaxPower) return 0;
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::InsElem: element out of range");
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem)
{
    if (Elem < 0 || Elem >= MaxPower)
        throw std::out_of_range("TSet::DelElem: element out of range");
    BitField.ClrBit(Elem);
}

// -------------------- операции --------------------

TSet& TSet::operator=(const TSet& s)
{
    if (this == &s) return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const { return BitField == s.BitField; }
int TSet::operator!=(const TSet& s) const { return BitField != s.BitField; }

TSet TSet::operator+(const int Elem)
{
    TSet result(*this);
    result.InsElem(Elem);   // теперь бросает при выходе за диапазон
    return result;
}

TSet TSet::operator-(const int Elem)
{
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator+(const TSet& s)   // объединение
{
    return TSet(BitField | s.BitField);
}

TSet TSet::operator*(const TSet& s)   // пересечение
{
    int n = (BitField.GetLength() > s.BitField.GetLength())
        ? BitField.GetLength() : s.BitField.GetLength();
    TBitField a(n), b(n);
    for (int i = 0; i < BitField.GetLength(); ++i)
        if (BitField.GetBit(i)) a.SetBit(i);
    for (int i = 0; i < s.BitField.GetLength(); ++i)
        if (s.BitField.GetBit(i)) b.SetBit(i);
    return TSet(a & b);
}

TSet TSet::operator~(void)            // дополнение
{
    return TSet(~BitField);
}

// -------------------- ввод/вывод --------------------

istream& operator>>(istream& istr, TSet& s)
{
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s)
{
    ostr << s.BitField;
    return ostr;
}
