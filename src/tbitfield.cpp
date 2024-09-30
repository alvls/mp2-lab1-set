// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <bit>
#include <bitset>
#include <iostream>

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
int MagicShiftConstant = 3; ///  std::bit_width(BitsInElem - 1); - doesn work, 8 = 2^3 

/*  
  int  BitLen; // длина битового поля
  TELEM *pMem; // память для представления битового поля
  int  MemLen; // к-во эл-тов Мем для представления бит.поля
  int BitsInElem; // кол-во элементов в ячейке pMem
  int ShiftSize; // смещение для определения ячейки pMem, в котрой располагается полученный номер бита
*/

TBitField::TBitField(int len) : BitLen(len)
{
    BitsInElem = numeric_limits<TELEM>::digits;
    ShiftSize = MagicShiftConstant; ///  std::bit_width(BitsInElem - 1); - doesn work
    MemLen = BitLen / BitsInElem + (BitLen % BitsInElem == 0 ? 0 : 1);
    pMem = new TELEM[MemLen]{ 0 };
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
    : BitsInElem(bf.BitsInElem), ShiftSize(bf.ShiftSize), MemLen(bf.MemLen), BitLen(bf.BitLen)
{
    pMem = new TELEM[MemLen]{ 0 };
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int pos) const  noexcept // индекс Мем для бита n
{
    return (pos >> ShiftSize);
}

TELEM TBitField::GetMemMask(const int pos) const noexcept// битовая маска для бита n
{
    return 1 << (pos & (BitsInElem - 1));
}

int TBitField::GetCell(const int cell_pos) const {
    // 5 4 3 2 1 0
    return pMem[cell_pos];
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return BitLen;
}

void TBitField::SetBit(const int pos) // установить бит
{
    TELEM cell = this->GetMemMask(pos);
    int mem_ind = this->GetMemIndex(pos);
    pMem[mem_ind] |= cell;
}

void TBitField::ClrBit(const int pos) // очистить бит
{
    TELEM cell = ~(this->GetMemMask(pos));
    int mem_ind = this->GetMemIndex(pos);
    pMem[mem_ind] &= cell;
}

int TBitField::GetBit(const int pos) const // получить значение бита
{
    TELEM cell = this->GetMemMask(pos);
    int mem_ind = this->GetMemIndex(pos);
    return ((pMem[mem_ind] & cell) == 0 ? 0 : 1); /// -> (pMem[mem_ind] & cell) ? 1 : 0)
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    BitLen = bf.BitLen;
    BitsInElem = bf.BitsInElem;
    ShiftSize = bf.ShiftSize; 
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen]{ 0 };
    for (int i = 0; i < MemLen; i++)
        pMem[i] = bf.pMem[i];
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение /// bool
{
    if ((BitLen != bf.BitLen) && (MemLen != bf.MemLen))
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
  return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(this->operator==(bf));
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{   
    int max_Mem, min_Mem, i = 0;
    TELEM* max_pMem = NULL;
    TBitField tmo(1);
    if (BitLen > bf.BitLen) {
        TBitField tmp(BitLen);
        max_Mem = MemLen;
        min_Mem = bf.MemLen;
        max_pMem = pMem;
        tmo = tmp;
    }
    else {
        TBitField tmp(bf.BitLen);
        max_Mem = bf.MemLen;
        min_Mem = MemLen;
        max_pMem = bf.pMem;
        tmo = tmp;
    }

    while (i < min_Mem) {
        tmo.pMem[i] = (pMem[i] | bf.pMem[i]);
        i++;
    }
    while (i < max_Mem) {
        tmo.pMem[i] = max_pMem[i];
        i++;
    }
    // max_pMem указывает на память одного из объектов, очищать память по нему не надо
    return tmo;
}



TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{ /// а вот "и" между 101 и 10001 как должно выглядить? как 00001 или 1? 
    int max_Mem, min_Mem, i = 0;
    TELEM* max_pMem = NULL;
    TBitField tmo(1);
    if (BitLen > bf.BitLen) {
        TBitField tmp(BitLen);
        max_Mem = MemLen;
        min_Mem = bf.MemLen;
        max_pMem = pMem;
        tmo = tmp;
    }
    else {
        TBitField tmp(bf.BitLen);
        max_Mem = bf.MemLen;
        min_Mem = MemLen;
        max_pMem = bf.pMem;
        tmo = tmp;
    }

    while (i < min_Mem) {
        tmo.pMem[i] = (pMem[i] & bf.pMem[i]);
        i++;
    }
    while (i < max_Mem) {
        tmo.pMem[i] = 0;
        i++;
    }
    // max_pMem указывает на память одного из объектов, очищать память по нему не надо
    return tmo;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmo(BitLen);
    for (int i = 0; i < MemLen; i++)
        tmo.pMem[i] = (~pMem[i]);
    return tmo;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    string bit_field;
    istr >> bit_field;
    if (bf.BitLen != bit_field.length()) throw std::string("different lengths");
    for (int i = 0; i < bit_field.length(); i++) {
        if (bit_field[bit_field.length() - i - 1] == '1') bf.SetBit(i);
        else if (bit_field[bit_field.length() - i - 1] != '0') {
            throw std::string("Incorrect bit field input");
            for (int idk = 0; idk < bf.BitLen; idk++) 
                bf.ClrBit(idk);
        }
    }
    return istr;
}

///del in release
/*
ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    std::string field = "";
    int i = bf.MemLen - 1;
    for (; i > 0; i--) {
        field += dec_to_bin(bf.GetCell(i));
        //while (field.length() < bf.BitsInElem)
        //   field = '0' + field; 
        field +=' ';
        ostr << field;
        field = "";
    }

    for (int j = 0; j < (bf.BitLen % bf.BitsInElem); j++)
        ostr << bf.GetBit(j + i * bf.BitsInElem);
    //std::cout << field;
    return ostr;
}
*/

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
    std::string field = "";
    for (int i = 0; i < bf.BitLen; i++)
        field = (bf.GetBit(i) ? '1' : '0') + field;
    return (ostr << field);
}

std::string dec_to_bin(int num) 
{
    char tmp = 0;
    std::string binNum = "";

    while (num > 0) {
        tmp = num % 2;
        binNum += (tmp == 0 ? '0' : '1'); /// ну это переделать надо
        num /= 2;
    }

    return binNum;
}

int bin_to_dec(string bin_num) 
{
    int dec_num = 0, i = 0, powered = 1;
    while (i < bin_num.length()) {
        dec_num += (bin_num[i++] == '1' ? 1 : 0) * powered;
        powered *= 2;
    }
    return dec_num;


}