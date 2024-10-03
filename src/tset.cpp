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

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
}

// конструктор копирования
TSet::TSet(const TSet &s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    BitField = s.BitField;
    MaxPower = s.MaxPower;
    return (*this);
}

int TSet::operator==(const TSet &s) const // сравнение
{
    return BitField == s.BitField;
    if (MaxPower > s.MaxPower) {
        TSet tmp(MaxPower);
        TBitField bf(MaxPower);
        bf = bf | s.BitField;
        tmp.BitField = bf & BitField;
        return tmp.BitField == BitField;
    }
    else {
        TSet tmp(s.MaxPower);
        TBitField bf(s.MaxPower);
        bf = bf | BitField;
        tmp.BitField = bf & (s.BitField);
        return tmp.BitField == s.BitField;
    }
}

int TSet::operator!=(const TSet &s) const // сравнение
{
    return BitField != s.BitField;
    if (MaxPower > s.MaxPower) {
        TSet tmp(MaxPower);
        TBitField bf(MaxPower);
        bf = bf | s.BitField;
        tmp.BitField = bf & BitField;
        return tmp.BitField != BitField;
    }
    else {
        TSet tmp(s.MaxPower);
        TBitField bf(s.MaxPower);
        bf = bf | BitField;
        tmp.BitField = bf & (s.BitField);
        return tmp.BitField != s.BitField;
    }
}

TSet TSet::operator+(const TSet &s) // объединение
{
    TSet tmp(max(s.MaxPower, MaxPower));
    tmp.BitField = BitField | s.BitField;
    return tmp;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet tmp(*this);
    tmp.BitField.SetBit(Elem);
    return tmp;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet tmp(*this);
    tmp.BitField.ClrBit(Elem);
    return tmp;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    // TODO пересечение размеров 10 и 7 должно иметь размер 7
    if (MaxPower > s.MaxPower) {
        TSet tmp(MaxPower);
        TBitField bf(MaxPower);
        bf = bf | s.BitField;
        tmp.BitField = BitField & bf;
        return tmp;
    }
    else {
        TSet tmp(s.MaxPower);
        TBitField bf(s.MaxPower);
        bf = bf | BitField;
        tmp.BitField = bf & (s.BitField);
        return tmp;
    }
}

TSet TSet::operator~(void) // дополнение
{
    TSet tmp(*this);
    tmp.BitField = ~(tmp.BitField);
    return tmp;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    // input bitfield
    istr >> s.BitField;
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    // output bitfield
    ostr << s.BitField;

    return ostr;
}
