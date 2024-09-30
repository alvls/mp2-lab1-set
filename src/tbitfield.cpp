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

TBitField::TBitField(int len) : BitLen(len)
{
    if (len < 0) { throw std::invalid_argument("Cannot create bit field with length less than 0"); }

    MemLen = (len + BitsInElement - 1) / BitsInElement;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = 0u;
    }
}

TBitField::TBitField(const TBitField &bf) : MemLen(bf.MemLen), BitLen(bf.BitLen) // конструктор копирования
{
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n / BitsInElement;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0) throw std::invalid_argument("Negative index");
    if (n >= BitLen) throw std::invalid_argument("Too large index");
    pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0) throw std::invalid_argument("Negative index");
    if (n >= BitLen) throw std::invalid_argument("Too large index");
    pMem[GetMemIndex(n)] &= ~(1 << n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0) throw std::invalid_argument("Negative index");
    if (n >= BitLen) throw std::invalid_argument("Too large index");
    return (bool)(pMem[GetMemIndex(n)] & GetMemMask(n));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    for (int i = 0; i < MemLen; i++) {
        pMem[i] = bf.pMem[i];
    }
    return (*this);
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return false;
    return true;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen) return true;

    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return true;
    return false;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    if (BitLen > bf.BitLen) {
        TBitField res(*this);
        for (int i = 0; i < bf.MemLen; i++)
            res.pMem[i] |= bf.pMem[i];
        return res;
    }
    else {
        TBitField res(bf);
        for (int i = 0; i < MemLen; i++)
            res.pMem[i] |= pMem[i];
        return res;
    }
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen > bf.BitLen) {
        TBitField res(bf);
        for (int i = 0; i < bf.MemLen; i++)
            res.pMem[i] &= pMem[i];
        return res;
    }
    else {
        TBitField res(*this);
        for (int i = 0; i < MemLen; i++)
            res.pMem[i] &= bf.pMem[i];
        return res;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res(*this);
    for (int i = 0; i < MemLen; i++)
        res.pMem[i] = ~pMem[i];
    if (BitLen % BitsInElement != 0) {
        TELEM el = ~0u;
        int nuzh = BitLen % BitsInElement;
        el = el << nuzh;
        el = ~el;
        res.pMem[MemLen - 1] &= el;
    }
    return res;
}

// ввод/вывод TODO

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    // input format : 001000100101001 - string consisting of 0 and 1
    //                0-index       last index
    string s;
    istr >> s;
    for (int i = 0; i < s.length() && i < bf.BitLen; i++) {
        if (s[i] == '1') bf.SetBit(i);
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    // output format : 001000100101001 - string consisting of 0 and 1
    //                 0-index       last index
    string s;
    for (int i = 0; i < bf.BitLen; i++) {
        if (bf.GetBit(i)) s += '1';
        else s += '0';
    }
    ostr << s;
    return ostr;
}
