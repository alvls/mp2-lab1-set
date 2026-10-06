// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <string>

//Количество битов в одном элементе TELEM (для unsigned int = 32)
static const int BITS_PER_ELEM = sizeof(TELEM) * 8;

//конструкторы / деструктор

TBitField::TBitField(int len)
{
    if (len < 0)
        throw std::invalid_argument("TBitField: negative length");
    BitLen = len;
    MemLen = (len + BITS_PER_ELEM - 1) / BITS_PER_ELEM;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i)
            pMem[i] = 0;
    }
    else {
        pMem = nullptr;
    }
}

TBitField::TBitField(const TBitField& bf) //конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i)
            pMem[i] = bf.pMem[i];
    }
    else {
        pMem = nullptr;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

//вспомогательные

int TBitField::GetMemIndex(const int n) const // индекс в pМем для бита n
{
    return n / BITS_PER_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return static_cast<TELEM>(1) << (n % BITS_PER_ELEM);
}

//доступ к битам

int TBitField::GetLength(void) const
{
    return BitLen;
}

void TBitField::SetBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n)
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index out of range");
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const
{
    if (n < 0 || n >= BitLen)
        throw std::out_of_range("bit index out of range");
    return (pMem[GetMemIndex(n)] & GetMemMask(n)) ? 1 : 0;
}

//битовые операции

TBitField& TBitField::operator=(const TBitField& bf)
{
    if (this == &bf) return *this;
    delete[] pMem;
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    if (MemLen > 0) {
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; ++i)
            pMem[i] = bf.pMem[i];
    }
    else {
        pMem = nullptr;
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const
{
    if (BitLen != bf.BitLen) return 0;
    for (int i = 0; i < MemLen; ++i) {
        if (pMem[i] != bf.pMem[i]) return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxLen);
    int minMemLen = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minMemLen; ++i)
        result.pMem[i] = pMem[i] | bf.pMem[i];
    if (MemLen > bf.MemLen) {
        for (int i = minMemLen; i < MemLen; ++i)
            result.pMem[i] = pMem[i];
    }
    else {
        for (int i = minMemLen; i < bf.MemLen; ++i)
            result.pMem[i] = bf.pMem[i];
    }
    int bitsInLast = maxLen % BITS_PER_ELEM;
    if (bitsInLast != 0 && result.MemLen > 0) {
        TELEM mask = (static_cast<TELEM>(1) << bitsInLast) - 1;
        result.pMem[result.MemLen - 1] &= mask;
    }
    return result;
}

TBitField TBitField::operator&(const TBitField& bf)
{
    int maxLen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField res(maxLen);
    int minWords = (MemLen < bf.MemLen) ? MemLen : bf.MemLen;
    for (int i = 0; i < minWords; ++i)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void)
{
    TBitField result(BitLen);
    for (int i = 0; i < MemLen; ++i)
        result.pMem[i] = ~pMem[i];
    int bitsInLast = BitLen % BITS_PER_ELEM;
    if (bitsInLast != 0 && result.MemLen > 0) {
        TELEM mask = (static_cast<TELEM>(1) << bitsInLast) - 1;
        result.pMem[result.MemLen - 1] &= mask;
    }
    return result;
}

//ввод/вывод

istream& operator>>(istream& istr, TBitField& bf)
{
    string s;
    istr >> s;
    for (int i = 0; i < bf.BitLen; ++i)
        bf.ClrBit(i);
    int n = (int)s.length();
    for (int i = 0; i < n && i < bf.BitLen; ++i) {
        if (s[i] == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf)
{
    for (int i = bf.BitLen - 1; i >= 0; --i)
        ostr << (bf.GetBit(i) ? '1' : '0');
    return ostr;
}
