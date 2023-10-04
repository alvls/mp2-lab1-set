// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

// Fake variables used as placeholders in tests
//static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
    BitLen = len;
    MemLen = BitLen % 32 == 0 ? BitLen / 32 : (BitLen % 32) + 1;
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++)
    {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        return -1;
    return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    if (n < 0 || n >= BitLen)
    {
        return 0;
    }
    TELEM mask = 1;
    mask <<= (n % (sizeof(TELEM) * 8));
    return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит                      1110        0001
{
    if (n < 0 || n >= BitLen)
        throw 1;
    {
        int index = GetMemIndex(n);
        TELEM mask = GetMemMask(n);
        pMem[index] |= mask;
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw 1;
    int index = GetMemIndex(n);
    TELEM mask = ~GetMemMask(n);
    pMem[index] &= mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw 1;
    {
        int index = GetMemIndex(n);
        TELEM mask = GetMemMask(n);
        return (pMem[index] & mask) ? 1 : 0;
    }
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this != &bf)
    {
        delete[] pMem;
        BitLen = bf.BitLen;
        MemLen = bf.MemLen;
        pMem = new TELEM[MemLen];
        for (int i = 0; i < MemLen; i++)
        {
            pMem[i] = bf.pMem[i];
        }
    }
    return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
    {
        if (pMem[i] != bf.pMem[i])
            return 0;
    }
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(operator==(bf));
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    const TBitField* less = MemLen > bf.MemLen ? &bf : this;
    TBitField res(MemLen > bf.MemLen ? *this : bf);
    for (int i = 0; i < less->MemLen; i++)
        res.pMem[i] |= less->pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"    11101   1010    
{
    int maxlen = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(maxlen);
    for (int i = 0; i < result.MemLen; i++)
    {
        result.pMem[i] = pMem[i] & bf.pMem[i];
    }
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
        GetBit(i) == 0 ? res.SetBit(i) : res.ClrBit(i);

    return res;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char bit;
    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> bit;
        if (bit == '0')
            bf.ClrBit(i);
        else if (bit == '1')
            bf.SetBit(i);
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
    {
        if (bf.GetBit(i))
            ostr << '1';
        else
            ostr << '0';
    }
    return ostr;
}
