// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <stdexcept>
#include <algorithm>
#include <limits>

static constexpr size_t maxElem = numeric_limits<TELEM>::max();
static constexpr size_t bitsInElem = numeric_limits<TELEM>::digits;

TBitField::TBitField(size_t len)
    : BitLen(len)
    , MemLen((len / bitsInElem) + ((len % bitsInElem) != 0))
{
    pMem = new TELEM[MemLen]();
}

TBitField::TBitField(const TBitField& bf)
    : BitLen(bf.BitLen)
    , MemLen(bf.MemLen)
{
    pMem = new TELEM[MemLen];
    copy(bf.pMem, bf.pMem + bf.MemLen, pMem);
}

TBitField::~TBitField() { delete[] pMem; }

size_t TBitField::GetMemIndex(const size_t n) const { return n / bitsInElem; } // индекс Мем для бита n


TELEM TBitField::GetMemMask(const size_t n) const // битовая маска для бита n
{
    return 1 << (n & (bitsInElem - 1));
}

inline void TBitField::check_index(const size_t n) const // проверка индекса
{
    if (n >= BitLen) { throw exception("incorrect index"); }
}

// доступ к битам битового поля

size_t TBitField::GetLength(void) const { return BitLen; } // получить длину (к-во битов)

void TBitField::SetBit(const size_t n) // установить бит
{
    check_index(n); //проверяем существование индекса
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const size_t n) // очистить бит
{
    check_index(n);
    pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

bool TBitField::GetBit(const size_t n) const // получить значение бита
{
    check_index(n);
    return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
    if (this == &bf) { return *this; }

    TBitField tmp(bf);
    swap(*this, tmp);
    return *this;
}

bool TBitField::operator==(const TBitField& bf) const noexcept // сравнение
{
    if (BitLen != bf.BitLen) { return false; }

    for (size_t i = 0; i < MemLen; i++) { if (pMem[i] != bf.pMem[i]) { return false; } }
    
    return true;
}

bool TBitField::operator!=(const TBitField& bf) const noexcept { return !(*this == bf); } // сравнение

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    const bool more = BitLen > bf.BitLen;
    const TBitField* rhs = more ? &bf : this;
    TBitField result = more ? TBitField(*this) : TBitField(bf);

    for (size_t i = 0; i < result.MemLen; i++) { result.pMem[i] |= rhs->pMem[i]; }

    return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
    TBitField result(max(BitLen, bf.BitLen));
    const size_t less = min(MemLen, bf.MemLen);

    for (size_t i = 0; i < less; i++) { result.pMem[i] = pMem[i] & bf.pMem[i]; }

    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);

    for (size_t i = 0; i < MemLen - 1; i++) { result.pMem[i] = ~pMem[i]; }

    result.pMem[MemLen - 1] = pMem[MemLen - 1] ^ (maxElem >> (bitsInElem - (BitLen & (bitsInElem - 1))));

    return result;
}

// ввод/вывод

void swap(TBitField& first, TBitField& second)
{
    swap(first.BitLen, second.BitLen);
    swap(first.MemLen, second.MemLen);
    swap(first.pMem, second.pMem);
}

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
    char c;
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        istr >> c;
        if (c == '0') { bf.ClrBit(i); }

        else { bf.SetBit(i); }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    for (size_t i = 0; i < bf.BitLen; i++)
    {
        ostr << bf.GetBit(i) ? '1' : '0';
    }
    return ostr;
}
