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
	if (len < 0)
		throw std::invalid_argument("length is wrong.");
	BitLen = len;
	MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = TELEM(0);
}

TBitField::TBitField(const TBitField& bf): BitLen(bf.BitLen), MemLen(bf.MemLen) // конструктор копирования
{
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n >= BitLen) && (n < 0))
		throw std::out_of_range("index is out of range.");
	return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n >= BitLen) && (n < 0))
		throw std::out_of_range("index is out of range.");
	TELEM mask;
	mask = 1 << (n % (sizeof(TELEM) * 8));
	mask &= pMem[n / (sizeof(TELEM) * 8)];
	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("index is out of range.");
	TELEM tmp = (1 << n);
	pMem[GetMemIndex(n)] |= tmp;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("index is out of range.");
	TELEM tmp = ~(1 << n);
	pMem[GetMemIndex(n)] &= tmp;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n >= BitLen || n < 0)
		throw std::out_of_range("index is out of range.");
	if (GetMemMask(n) > 0)
		return 1;
	else return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (&bf != this)
	{
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
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
	if (*this == bf)
		return 0;
	else 
		return 1;
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	const TBitField& mxlen = (BitLen > bf.BitLen) ? *this : bf;
	const TBitField& mnlen = (BitLen > bf.BitLen) ? bf : *this;
	int lenght = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
	int diff = abs(MemLen - bf.MemLen);
	TBitField res(__max(BitLen, bf.BitLen));
	for (int i = 0; i < diff; i++)
		res.pMem[i] = mxlen.pMem[i];
	for (int i = 0; i < lenght; i++)
		res.pMem[i + diff] = mxlen.pMem[i + diff] | mnlen.pMem[i];
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	const TBitField& mxlen = (BitLen > bf.BitLen) ? *this : bf;
	const TBitField& mnlen = (BitLen > bf.BitLen) ? bf : *this;
	int lenght = (BitLen > bf.BitLen) ? bf.MemLen : MemLen;
	int diff = abs(MemLen - bf.MemLen);
	TBitField res(__max(BitLen, bf.BitLen));
	for (int i = 0; i < diff; ++i)
		res.pMem[i] = TELEM(0);
	for (int i = 0; i < lenght; ++i)
		res.pMem[i + diff] = mxlen.pMem[i + diff] & mnlen.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);
	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = ~pMem[i];
	int extraBits = BitLen % (sizeof(TELEM) * 8);
	if (extraBits != 0)
	{
		TELEM mask = (1 << extraBits) - 1;
		result.pMem[MemLen - 1] &= mask;
	}
	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	char ch;
	int index = 0;
	for (int i = 0; i < bf.GetLength(); i++)
		bf.ClrBit(i);
	while (istr >> ch)
	{
		if (ch == '1')
		{
			if (index < bf.GetLength())
				bf.SetBit(index);
		}
		else if (ch != '0')
			throw std::invalid_argument("invalid bit character.");
		index++;
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		ostr << bf.GetBit(i);
	return ostr;
}