// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
int BitsInElem = numeric_limits<TELEM>::digits;
TBitField::TBitField(int len)
{
    
    BitLen = len;
    MemLen = BitLen / BitsInElem + 1;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0;
    }
    if (len < 0){
        throw "Len negative";
    }
}
TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++){
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n/BitsInElem;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1<<(n&(BitsInElem-1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > BitLen)
        throw "SetBit out of range";
    if (n < 0)
        throw "SetBit negative";
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > BitLen)
        throw "ClearBit out of range";
    if (n < 0)
        throw "ClearBit negative";
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen)
        throw "GetBit out of range";
    if (n < 0)
        throw "GetBit negative";
    if (pMem[GetMemIndex(n)] & GetMemMask(n)) {
        return 1;
    }
    return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++) {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    TBitField tmp(*this);
    if (tmp == bf)
        return 0;
    return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    TBitField tmp(*this);
    int ma, mi;
    ma = max(BitLen, bf.BitLen);
    mi = min(BitLen, bf.BitLen);
    TBitField ans(ma);
    for (int i = 0; i < ans.BitLen; i++) {
        if (i <= mi) {
            if (tmp.GetBit(i)==1 || bf.GetBit(i)==1)
                ans.SetBit(i);
        }
        
    }
    return ans;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{

    TBitField tmp(*this);
    int ma, mi;
    ma = max(BitLen, bf.BitLen);
    mi = min(BitLen, bf.BitLen);
    TBitField ans(ma);
    for (int i = 0; i < ans.BitLen; i++) {
        if (i <= mi) {
            if (tmp.GetBit(i) ==1 && bf.GetBit(i) == 1)
                ans.SetBit(i);
        }

    }
    return ans;
}
TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(*this);
  
    for (int i = 0; i < BitLen; i++) {
        if (tmp.GetBit(i)==1) {
            tmp.ClrBit(i);
        }
        else {
            tmp.SetBit(i);
        }
    }
    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    cout << "Enter Len\n";
    int size,bit;
    cin >> size;
    TBitField tmp(size);
    cout << "Enter Bitfield\n";
    for (int i = 0; i < size; i++) {
        cin >> bit;
        if (bit == 1)
            tmp.SetBit(i);
        else
            tmp.ClrBit(i);
    }
    bf = tmp;
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
