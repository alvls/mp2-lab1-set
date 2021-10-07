// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле
#include <exception>
#include <cmath>
#include "tbitfield.h"

// Fake variables used as placeholders in tests
// Поддельные переменные, используемые в тестах в качестве заполнителей
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len){
    if (len >= 0) {
        BitLen = len;
        MemLen = (len + 8 * sizeof(TELEM) - 1) >> (int)(log2(8 * sizeof(TELEM)) + 0.1);
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = 0;
        }
    }
    else { throw "ERROR";}
}

// конструктор копирования
TBitField::TBitField(const TBitField &bf) {
    MemLen = bf.MemLen;
    BitLen = bf.BitLen;
    pMem = new TELEM[MemLen];
    if (pMem != NULL) {
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
}
//деструктор 
TBitField::~TBitField() {
    if (pMem != NULL) { delete[] pMem;}
}
// индекс Мем для бита n
int TBitField::GetMemIndex(const int n) const {
    if ((n < 0) || (n > BitLen - 1)) { throw "ERROR"; }
    else {
        const int MemIndex = n / (sizeof(TELEM) * 8);
        return MemIndex;
    }
}
// битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const {
    TELEM MemMask = 1;
    return (MemMask << (n % (sizeof(TELEM) * 8)));
}

// доступ к битам битового поля
// получить длину (к-во битов)
int TBitField::GetLength(void) const {
    return BitLen;
}

// установить бит
void TBitField::SetBit(const int n) {   
    // n - номер 
    if ((n >= 0) && (n < BitLen)){
        pMem[GetMemIndex(n)] |=  GetMemMask(n); // устанавливаем бит
    }
    else { throw "ERROR";}
}
// очистить бит
void TBitField::ClrBit(const int n) {
    if ((n >= 0) && (n < BitLen)){
        pMem[GetMemIndex(n)] &=~GetMemMask(n); 
    }
    else { throw "ERROR";}
}
// получить значение бита
int TBitField::GetBit(const int n) const {
    if ((n >= 0) && (n < BitLen)){
        return pMem[GetMemIndex(n)] & GetMemMask(n);
    }
    else { throw "ERROR"; }
}

// битовые операции

// присваивание
TBitField& TBitField::operator=(const TBitField &bf) {
    BitLen = bf.BitLen; //BitLen - длина битового поля - макс. кол-во битов
    if (MemLen != bf.MemLen) {
        MemLen = bf.MemLen;  
    }
    if (pMem != NULL) {
        delete[] pMem;
    }
    pMem = new TELEM[MemLen];
    if (pMem != NULL) {
        for (int i = 0; i < MemLen; i++) {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}
// сравнение
int TBitField::operator==(const TBitField &bf) const {
    int Bool = 1;
    if (BitLen != bf.BitLen) { Bool = 0;}
    else {
        for (int i = 0; i < MemLen; i++) {
            if (pMem[i] != bf.pMem[i]){
                Bool = 0;
                break;
            }
        }
    } 
    return Bool;
}
// сравнение
int TBitField::operator!=(const TBitField &bf) const {
    return !(*this == bf);
}
// операция "или"
TBitField TBitField::operator|(const TBitField &bf) {
    int i;
    int len = BitLen;
    if (bf.BitLen > len) {
        len = bf.BitLen;
    }
    TBitField temp(len);
    for (i = 0; i < MemLen; i++) {
        temp.pMem[i] = pMem[i];
    }
    for (i = 0; i < bf.MemLen; i++) {
        temp.pMem[i] |= bf.pMem[i];
    }
    return temp;
}
// операция "и"
TBitField TBitField::operator&(const TBitField &bf) {
    int len = max(BitLen, bf.BitLen);
    TBitField result(len);
    for (int i = 0; i < MemLen; i++) {
        result.pMem[i] = pMem[i];
    }
    for (int i = 0; i < bf.MemLen; i++) {
        result.pMem[i] &= bf.pMem[i];
    }
    return result;
}
// отрицание
TBitField TBitField::operator~(void){
    TBitField res(*this);
    for (int i = 0; i < BitLen; i++){
        res.pMem[GetMemIndex(i)] ^= res.GetMemMask(i);
    }
    return res;
}

// ввод/вывод
// ввод
istream &operator>>(istream &istr, TBitField &bf) {
    int k;
    for (int i = 0; i < bf.BitLen; i++) {
        istr >> k;
        while ((k != 1) && (k != 0)) {
            istr >> k;
        }
        if (k == 1) {
            bf.SetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}
// вывод
ostream &operator<<(ostream &ostr, const TBitField &bf){
    for (int i = 0; i < bf.BitLen; i++){
        if ((bf.pMem[(int)(i / (sizeof(TELEM) * 8))] & bf.GetMemMask(i)) == bf.GetMemMask(i)){ ostr << "1"; }
        else { ostr << "0";}
    }
    return ostr;
}
