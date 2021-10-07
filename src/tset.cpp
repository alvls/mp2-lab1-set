// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
//static TBitField FAKE_BITFIELD(1);
//static TSet FAKE_SET(1);

TSet::TSet(int mp) : MaxPower(mp), BitField(mp) // заполняем максимальную мощность множества и длину битового поля 
{
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength())
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
    return BitField.GetBit(Elem); // получение значения бита
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    BitField.SetBit(Elem); // установить бит
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    BitField.ClrBit(Elem); // очистить бит
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this == &s)
        return *this;
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return BitField == s.BitField; // 1, если равны
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(BitField == s.BitField); // 0, если равны
}

TSet TSet::operator+(const TSet& s) // объединение
{
    TSet res(BitField | s.BitField); // логическое "или"
    return res;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    TSet res(*this);
    res.InsElem(Elem); // включаем элемент в множество, т е устанавливаем бит ч/з логическое "или"
    return res;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    TSet res(*this);
    res.DelElem(Elem); // удаляем элемент из множество, т е очищаем бит ч/з логическое "и"
    return res;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    //TSet res(BitField & s.BitField); 
    //return res;
    return (BitField & s.BitField);
}

TSet TSet::operator~(void) // дополнение
{
    //TSet res(~BitField);
    //return res;
    return (~BitField);
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    char c;
    int tmp;
    do {
        istr >> c;
    } while (c != '{');
    do {
        istr >> tmp;
        s.InsElem(tmp);
        do {
            istr >> c;
        } while (c != ',' && c != '}');
    } while (c != '}');
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    int n = s.GetMaxPower();
    char c = ' ';
    ostr << '{';
    for (int i = 0; i < n; i++) {
        if (s.IsMember(i)) {
            ostr << c << ' ' << i;
            c = ',';
        }
    }
    ostr << '}';
    return ostr;
}
