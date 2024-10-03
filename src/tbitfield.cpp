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

TBitField::TBitField(int len)
{
    if (len < 1) throw("Error wrong length");
    BitLen = len;
    if ((BitLen & (BitsInElem - 1)) == 0) {
        MemLen = len >> ShiftSize;
    }
    else {
        MemLen = (len >> ShiftSize) + 1;
    } 
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i].bit0 = 0;
        pMem[i].bit1 = 0;
        pMem[i].bit2 = 0;
        pMem[i].bit3 = 0;
        pMem[i].bit4 = 0;
        pMem[i].bit5 = 0;
        pMem[i].bit6 = 0;
        pMem[i].bit7 = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i].bit0 = bf.pMem[i].bit0;
        pMem[i].bit1 = bf.pMem[i].bit1;
        pMem[i].bit2 = bf.pMem[i].bit2;
        pMem[i].bit3 = bf.pMem[i].bit3;
        pMem[i].bit4 = bf.pMem[i].bit4;
        pMem[i].bit5 = bf.pMem[i].bit5;
        pMem[i].bit6 = bf.pMem[i].bit6;
        pMem[i].bit7 = bf.pMem[i].bit7;
    }
}

TBitField::~TBitField()
{
    delete[] pMem;
    pMem = nullptr;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if ((n & (BitsInElem - 1)) == 0) {
        return n >> ShiftSize;
    }
    else {
        return (n >> ShiftSize) + 1;
    }
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    TELEM mask_n; mask_n.bit0 = 0; mask_n.bit1 = 0; mask_n.bit2 = 0; mask_n.bit3 = 0; mask_n.bit4 = 0; mask_n.bit5 = 0; mask_n.bit6 = 0; mask_n.bit7 = 0;
    if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit0 = pMem[n >> ShiftSize].bit0;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit1 = pMem[n >> ShiftSize].bit1;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit2 = pMem[n >> ShiftSize].bit2;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit3 = pMem[n >> ShiftSize].bit3;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit4 = pMem[n >> ShiftSize].bit4;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit5 = pMem[n >> ShiftSize].bit5;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit6 = pMem[n >> ShiftSize].bit6;
    }
    else if ((n & (BitsInElem - 1)) == 0) {
        mask_n.bit7 = pMem[n >> ShiftSize].bit7;
    }
    return mask_n;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n > BitLen) throw("Error, wrong bit");
    if ((n & (BitsInElem - 1)) == 0) {
        pMem[n >> ShiftSize].bit0 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 1) {
        pMem[n >> ShiftSize].bit1 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 2) {
        pMem[n >> ShiftSize].bit2 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 3) {
        pMem[n >> ShiftSize].bit3 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 4) {
        pMem[n >> ShiftSize].bit4 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 5) {
        pMem[n >> ShiftSize].bit5 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 6) {
        pMem[n >> ShiftSize].bit6 = 1;
    }
    else if ((n & (BitsInElem - 1)) == 7) {
        pMem[n >> ShiftSize].bit7 = 1;
    }
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n > BitLen) throw("Error, wrong bit");
    if ((n & (BitsInElem - 1)) == 0) {
        pMem[n >> ShiftSize].bit0 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 1) {
        pMem[n >> ShiftSize].bit1 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 2) {
        pMem[n >> ShiftSize].bit2 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 3) {
        pMem[n >> ShiftSize].bit3 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 4) {
        pMem[n >> ShiftSize].bit4 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 5) {
        pMem[n >> ShiftSize].bit5 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 6) {
        pMem[n >> ShiftSize].bit6 = 0;
    }
    else if ((n & (BitsInElem - 1)) == 7) {
        pMem[n >> ShiftSize].bit7 = 0;
    }
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n > BitLen) throw("Error, wrong bit");
    int result;
    if ((n & (BitsInElem - 1)) == 0) {
        result = pMem[n >> ShiftSize].bit0;
    }
    else if ((n & (BitsInElem - 1)) == 1) {
        result = pMem[n >> ShiftSize].bit1;
    }
    else if ((n & (BitsInElem - 1)) == 2) {
        result = pMem[n >> ShiftSize].bit2;
    }
    else if ((n & (BitsInElem - 1)) == 3) {
        result = pMem[n >> ShiftSize].bit3;
    }
    else if ((n & (BitsInElem - 1)) == 4) {
        result = pMem[n >> ShiftSize].bit4;
    }
    else if ((n & (BitsInElem - 1)) == 5) {
        result = pMem[n >> ShiftSize].bit5;
    }
    else if ((n & (BitsInElem - 1)) == 6) {
        result = pMem[n >> ShiftSize].bit6;
    }
    else { 
        result = pMem[n >> ShiftSize].bit7;
    }
    return result;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    delete[] pMem;
    pMem = new TELEM[MemLen];
    for (int i = 0; i < MemLen; i++) {
        pMem[i].bit0 = bf.pMem[i].bit0;
        pMem[i].bit1 = bf.pMem[i].bit1;
        pMem[i].bit2 = bf.pMem[i].bit2;
        pMem[i].bit3 = bf.pMem[i].bit3;
        pMem[i].bit4 = bf.pMem[i].bit4;
        pMem[i].bit5 = bf.pMem[i].bit5;
        pMem[i].bit6 = bf.pMem[i].bit6;
        pMem[i].bit7 = bf.pMem[i].bit7;
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    int result_comparison = 0;
    if (BitLen == bf.BitLen) {
        if (MemLen == bf.MemLen) {
            result_comparison = 1;
            for (int i = 0; i < BitLen; i++) {
                if (GetBit(i) != bf.GetBit(i)) {
                    result_comparison = 0;
                    break;
                }

            }
        }
    }
    return result_comparison;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    int result_comparison = 0;
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) != bf.GetBit(i)) {
            result_comparison = 1;
            break;

        }
    }
    return result_comparison;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{

    int MemLenMax = bf.MemLen;
    int MemLenMin = MemLen;
    int BitLenMax = bf.BitLen;
    if (BitLen > bf.BitLen) {
        BitLenMax = BitLen;
    }
    if (MemLen >= bf.MemLen) {
        MemLenMax = MemLen;
        MemLenMin = bf.MemLen;
    }
    TBitField res_oper_or(MemLenMax);
    res_oper_or.BitLen = BitLenMax;
    for (int i = 0; i < MemLenMin; i++) {
        res_oper_or.pMem[i].bit0 = pMem[i].bit0 | bf.pMem[i].bit0;
        res_oper_or.pMem[i].bit1 = pMem[i].bit1 | bf.pMem[i].bit1;
        res_oper_or.pMem[i].bit2 = pMem[i].bit2 | bf.pMem[i].bit2;
        res_oper_or.pMem[i].bit3 = pMem[i].bit3 | bf.pMem[i].bit3;
        res_oper_or.pMem[i].bit4 = pMem[i].bit4 | bf.pMem[i].bit4;
        res_oper_or.pMem[i].bit5 = pMem[i].bit5 | bf.pMem[i].bit5;
        res_oper_or.pMem[i].bit6 = pMem[i].bit6 | bf.pMem[i].bit6;
        res_oper_or.pMem[i].bit7 = pMem[i].bit7 | bf.pMem[i].bit7;
    }
    if (MemLen > bf.MemLen) {
        for (int i = MemLenMin; i < MemLenMax; i++) {
            res_oper_or.pMem[i].bit0 = pMem[i].bit0;
            res_oper_or.pMem[i].bit1 = pMem[i].bit1;
            res_oper_or.pMem[i].bit2 = pMem[i].bit2;
            res_oper_or.pMem[i].bit3 = pMem[i].bit3;
            res_oper_or.pMem[i].bit4 = pMem[i].bit4;
            res_oper_or.pMem[i].bit5 = pMem[i].bit5;
            res_oper_or.pMem[i].bit6 = pMem[i].bit6;
            res_oper_or.pMem[i].bit7 = pMem[i].bit7;
        }
    }
    else {
        for (int i = MemLenMin; i < MemLenMax; i++) {
            res_oper_or.pMem[i].bit0 = bf.pMem[i].bit0;
            res_oper_or.pMem[i].bit1 = bf.pMem[i].bit1;
            res_oper_or.pMem[i].bit2 = bf.pMem[i].bit2;
            res_oper_or.pMem[i].bit3 = bf.pMem[i].bit3;
            res_oper_or.pMem[i].bit4 = bf.pMem[i].bit4;
            res_oper_or.pMem[i].bit5 = bf.pMem[i].bit5;
            res_oper_or.pMem[i].bit6 = bf.pMem[i].bit6;
            res_oper_or.pMem[i].bit7 = bf.pMem[i].bit7;
        }
    }
    return res_oper_or;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    if (BitLen > bf.BitLen) {
        TBitField res_oper_and(BitLen);
        for (int i = 0; i < bf.MemLen; i++) {
            res_oper_and.pMem[i].bit0 = pMem[i].bit0 & bf.pMem[i].bit0;
            res_oper_and.pMem[i].bit1 = pMem[i].bit1 & bf.pMem[i].bit1;
            res_oper_and.pMem[i].bit2 = pMem[i].bit2 & bf.pMem[i].bit2;
            res_oper_and.pMem[i].bit3 = pMem[i].bit3 & bf.pMem[i].bit3;
            res_oper_and.pMem[i].bit4 = pMem[i].bit4 & bf.pMem[i].bit4;
            res_oper_and.pMem[i].bit5 = pMem[i].bit5 & bf.pMem[i].bit5;
            res_oper_and.pMem[i].bit6 = pMem[i].bit6 & bf.pMem[i].bit6;
            res_oper_and.pMem[i].bit7 = pMem[i].bit7 & bf.pMem[i].bit7;
        }
        return res_oper_and;
    }
    else {
        TBitField res_oper_and(bf.BitLen);
        for (int i = 0; i < MemLen; i++) {
            res_oper_and.pMem[i].bit0 = pMem[i].bit0 & bf.pMem[i].bit0;
            res_oper_and.pMem[i].bit1 = pMem[i].bit1 & bf.pMem[i].bit1;
            res_oper_and.pMem[i].bit2 = pMem[i].bit2 & bf.pMem[i].bit2;
            res_oper_and.pMem[i].bit3 = pMem[i].bit3 & bf.pMem[i].bit3;
            res_oper_and.pMem[i].bit4 = pMem[i].bit4 & bf.pMem[i].bit4;
            res_oper_and.pMem[i].bit5 = pMem[i].bit5 & bf.pMem[i].bit5;
            res_oper_and.pMem[i].bit6 = pMem[i].bit6 & bf.pMem[i].bit6;
            res_oper_and.pMem[i].bit7 = pMem[i].bit7 & bf.pMem[i].bit7;
        }
        return res_oper_and;
    }
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField res_oper_denial(BitLen);
    res_oper_denial.MemLen = MemLen;
    for (int i = 0; i < BitLen; i++) {
        if (GetBit(i) == 0) {
            res_oper_denial.SetBit(i);
        }
        else {
            res_oper_denial.ClrBit(i);
        }
    }
    return res_oper_denial;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{

    for (int i = 0; i < bf.BitLen; i++) {
        int n;
        istr >> n;
        if (n == 1) {
            bf.GetBit(i);
        }
        else {
            bf.ClrBit(i);
        }
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.MemLen; i++) {
        ostr << bf.GetBit(i);
    }
    return ostr;
}
