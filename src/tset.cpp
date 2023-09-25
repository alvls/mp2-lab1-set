// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(size_t mp)
    : MaxPower(mp),
    BitField(mp) {}

// конструктор копирования
TSet::TSet(const TSet& s)
    : MaxPower(s.MaxPower)
    , BitField(s.BitField) {}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf)
    : MaxPower(bf.GetLength())
    , BitField(bf) {}

TSet::operator TBitField() { return BitField; }

size_t TSet::GetMaxPower(void) const { return MaxPower; } // получить макс. к-во эл-тов

bool TSet::IsMember(const size_t elem) const { return BitField.GetBit(elem); } // элемент множества?

void TSet::InsElem(const size_t elem) { BitField.SetBit(elem); } // включение элемента множества

void TSet::DelElem(const size_t elem) { BitField.ClrBit(elem); } // исключение элемента множества

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s) { return *this; }

    TSet tmp(s);
    swap(*this, tmp);
    return *this;
}

bool TSet::operator==(const TSet& s) const { return MaxPower == s.MaxPower && BitField == s.BitField; } // сравнение

bool TSet::operator!=(const TSet& s) const { return !(*this == s); } // сравнение

TSet TSet::operator+(const TSet& s) { return TSet(BitField | s.BitField); } // объединение

TSet TSet::operator*(const TSet& s) { return TSet(BitField & s.BitField); } // пересечение

TSet TSet::operator+(const size_t Elem) // объединение с элементом
{
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}

TSet TSet::operator-(const size_t Elem) // разность с элементом
{
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}

TSet TSet::operator~(void) { return TSet(~BitField); } // дополнение

// перегрузка ввода/вывода

void swap(TSet& first, TSet& second)
{
    swap(first.MaxPower, second.MaxPower);
    swap(first.BitField, second.BitField);
}

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
