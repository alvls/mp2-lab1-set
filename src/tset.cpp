// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
////////////////static const int FAKE_INT = -1;
////////////////static TBitField FAKE_BITFIELD(1);
////////////////static TSet FAKE_SET(1);

//конструктор
TSet::TSet(int mp) : MaxPower(mp), BitField(mp){
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField){
}

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : BitField(bf), MaxPower(bf.GetLength()){
}


TSet::operator TBitField(){
    TBitField temp(this->BitField);
    return temp;
}
// получить макс. к-во эл-тов
int TSet::GetMaxPower(void) const {
    return MaxPower;
}

// элемент множества?
int TSet::IsMember(const int Elem) const{
    return BitField.GetBit(Elem);
}

// включение элемента множества
void TSet::InsElem(const int Elem) {
    BitField.SetBit(Elem);
}
// исключение элемента множества
void TSet::DelElem(const int Elem) {
    BitField.ClrBit(Elem);
}


// теоретико-множественные операции
// присваивание
TSet& TSet::operator=(const TSet& s) {
    if (this == &s) {
        return *this;
    }
    MaxPower = s.MaxPower;
    BitField = s.BitField;
    return *this;
}
// сравнение
int TSet::operator==(const TSet& s) const {
    return BitField == s.BitField;
}

// сравнение
int TSet::operator!=(const TSet& s) const {
    return !(BitField == s.BitField);
}

// объединение
TSet TSet::operator+(const TSet& s) {
    TSet res(BitField | s.BitField);
    return res;
}
// объединение с элементом
TSet TSet::operator+(const int Elem) {
    TSet res(*this);
    res.InsElem(Elem);
    return res;
}
// разность с элементом
TSet TSet::operator-(const int Elem) {
    TSet res(*this);
    res.DelElem(Elem);
    return res;
}
// пересечение
TSet TSet::operator*(const TSet& s) {
    TSet res(BitField & s.BitField);
    return res;
}
// дополнение
TSet TSet::operator~(void) {
    TSet res(~BitField);
    return res;
}

// перегрузка ввода/вывода
// ввод
istream &operator>>(istream &istr, TSet &s) {
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
// вывод
ostream& operator<<(ostream &ostr, const TSet &s) {
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
