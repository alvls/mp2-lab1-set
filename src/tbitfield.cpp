// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cstring>

using namespace std;

TBitField::TBitField(int len)
{
    if (len <= 0)
        throw invalid_argument("Bit field length must be positive");
    BitLen = len;
    MemLen = (len + 31) >> 5;  // len / 32, так как 1 элемент pMem хранит 32 бита
    pMem = new TELEM[MemLen];
    std::memset(pMem, 0, MemLen * sizeof(TELEM));  // Инициализируем массив нулями
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    BitLen = bf.BitLen;
    MemLen = bf.MemLen;
    pMem = new TELEM[MemLen];
    memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));  // Копируем данные
}

TBitField::~TBitField()
{
    delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;  // Индекс элемента массива (n / 32)
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);  // Маска для бита n % 32
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    int index = GetMemIndex(n);
    pMem[index] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    int index = GetMemIndex(n);
    pMem[index] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0 || n >= BitLen)
        throw out_of_range("Bit index out of range");
    int index = GetMemIndex(n);
    return (pMem[index] & GetMemMask(n)) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (this != &bf)
    {
        if (MemLen != bf.MemLen)
        {
            delete[] pMem;
            MemLen = bf.MemLen;
            pMem = new TELEM[MemLen];
        }
        BitLen = bf.BitLen;
        memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));  // Копируем данные
    }
    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (BitLen != bf.BitLen)
        return 0;
    for (int i = 0; i < MemLen; i++)
        if (pMem[i] != bf.pMem[i])
            return 0;
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
    return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(len);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i];
    for (int i = 0; i < bf.MemLen; i++)
        result.pMem[i] |= bf.pMem[i];
    return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = (BitLen > bf.BitLen) ? BitLen : bf.BitLen;
    TBitField result(len);
    for (int i = 0; i < MemLen; i++)
        result.pMem[i] = pMem[i] & bf.pMem[i];
    return result;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField result(BitLen);
    for (int i = 0; i < BitLen; i++)
    {
        if (GetBit(i))  // Если бит был установлен
            result.ClrBit(i);  // Очищаем его
        else
            result.SetBit(i);  // Иначе устанавливаем
    }
    return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ch;
    for (int i = 0; i < bf.GetLength(); i++)
    {
        istr >> ch;
        if (ch == '1')
            bf.SetBit(i);  // Устанавливаем бит, если введена единица
        else if (ch == '0')
            bf.ClrBit(i);  // Очищаем бит, если введен ноль
        else
            throw invalid_argument("Invalid character when entering bit field");
    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    for (int i = 0; i < bf.GetLength(); i++)
        ostr << bf.GetBit(i);  // Выводим каждый бит битового поля
    return ostr;
}