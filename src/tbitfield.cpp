// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cmath>

TBitField::TBitField(int len)
{
    if (len < 0) throw "Negative length";
    BitLen = len;
    MemLen = ceil((double) len / (sizeof(TELEM) * 8));
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = 0;
}

// конструктор копирования
TBitField::TBitField(const TBitField &bf)
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete [] pMem;
}

// индекс Мем для бита n
int TBitField::GetMemIndex(const int n) const
{
    return n / (sizeof(TELEM) * 8);
}

// битовая маска для бита n
TELEM TBitField::GetMemMask(const int n) const
{
    return 1 << (n % (sizeof(TELEM) * 8));
}

// получить длину (к-во битов)
int TBitField::GetLength(void) const
{
    return BitLen;
}

// установить бит
void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen) throw "Dit index out of range";
    int index = n / (sizeof(TELEM) * 8);
    int offset = n % (sizeof(TELEM) * 8);
    pMem[index] |= 1 << offset;
}

// очистить бит
void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen) throw "Dit index out of range";
    int index = n / (sizeof(TELEM) * 8);
    int offset = n % (sizeof(TELEM) * 8);
    pMem[index] &= ~(1 << offset);
}

// получить значение бита
int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen) throw "Dit index out of range";
    int index = n / (sizeof(TELEM) * 8);
    int offset = n % (sizeof(TELEM) * 8);
    return (pMem[index] & (1 << offset)) != 0;
}

// присваивание
TBitField& TBitField::operator=(const TBitField &bf)
{
    if (this != &bf)
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen;i++)
            pMem[i] = bf.pMem[i];
    }
    return *this;
}

// сравнение
int TBitField::operator==(const TBitField &bf) const
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i]) return 0;
    return 1;
}

// сравнение
int TBitField::operator!=(const TBitField &bf) const
{
    return !(*this == bf);
}

// операция "или"
TBitField TBitField::operator|(const TBitField &bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] |= bf.pMem[i];
    return result;
}

// операция "и"
TBitField TBitField::operator&(const TBitField &bf)
{
    int minLen = (BitLen < bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(minLen);
    for (int i = 0; i < result.MemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];
    return result;
}

// отрицание
TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = ~pMem[i];
    return result;
}

// ввод
istream &operator>>(istream &istr, TBitField &bf) {
    int bit;
    while (istr >> bit) {
        if (bit >= 0 && bit < bf.GetLength()) {
            bf.SetBit(bit);
        }
    }
    return istr;
}

// вывод
ostream &operator<<(ostream &ostr, const TBitField &bf) {
    for (int i = 0; i < bf.GetLength(); i++) {
        if (bf.GetBit(i)) {
            ostr << i << " ";
        }
    }
    return ostr;
}
