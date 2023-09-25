// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <algorithm>
#include <iterator>
#include <exception>
// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(size_t len)
{
    BitLen = len;
    MemLen = ((BitLen / BIT_IN_ELEM) + (BitLen % BIT_IN_ELEM != 0));
    pMem = new TELEM[MemLen]();

}

TBitField::TBitField(const TBitField& bf) : TBitField(bf.BitLen) // конструктор копирования
{
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BIT_IN_ELEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1U << (n % BIT_IN_ELEM);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n > BitLen || n < 0)
        throw exception("out of range");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n > BitLen || n < 0)
        throw exception("out of range");
    pMem[GetMemIndex(n)] &= ~(GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n > BitLen || n < 0)
        throw exception("out of range");
    return (pMem[GetMemIndex(n)] >> (n % BIT_IN_ELEM)) & 1;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    BitLen = bf.BitLen;
    if (MemLen != bf.MemLen)
    {
        MemLen = bf.MemLen;
        TELEM* f = new TELEM[MemLen];
        delete[] pMem;
        pMem = f;
    }
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField& bf) const noexcept // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    const TBitField* less = MemLen > bf.MemLen ? &bf : this;
    TBitField res(MemLen > bf.MemLen ? *this : bf);
    for (int i = 0; i < less->MemLen; i++)
        res.pMem[i] |= less->pMem[i];
    return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    int len = max(BitLen, bf.BitLen);
    const int minlen = min(MemLen, bf.MemLen);
    TBitField res(len);
    for (int i = 0; i < minlen; i++)
        res.pMem[i] = pMem[i] & bf.pMem[i];
    return res;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(BitLen);
    for (int i = 0; i < BitLen; i++)
        GetBit(i) == 0 ? res.SetBit(i) : res.ClrBit(i);
    /*for (int i = 0; i < MemLen - 1; i++)
        res.pMem[i] = ~pMem[i];                        почему то не работает((
    res.pMem[MemLen - 1] = pMem[MemLen - 1] ^ (TELEM_MAX >> (BIT_IN_ELEM - (BitLen & (BIT_IN_ELEM - 1))));*/
    return res;
}

// ввод/вывод

istream& operator>>(istream& is, TBitField& bf) // ввод
{
    char ch;
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        is >> ch;
        switch (ch)
        {
        case '0':
            bf.ClrBit(i);
            break;
        case '1':
            bf.SetBit(i);
            break;
        default:
            continue;
        }
    }
    return is;
}

ostream& operator<<(ostream& os, const TBitField& bf) // вывод
{
    for (int i = 0; i < bf.BitLen; i++)
        os << (bf.GetBit(i) ? 1 : 0);
    return os;
}
