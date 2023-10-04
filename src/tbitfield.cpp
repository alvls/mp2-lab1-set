// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include "string"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len)
{
	if (len <= 0)
	{
		throw std::invalid_argument("Length must be greater than zero.");
	}

	BitLen = len;
	MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
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
	{
		throw std::out_of_range("Bit index is out of range.");
	}

	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Bit index is out of range.");
	}

	int bitPosition = n % (sizeof(TELEM) * 8);
	TELEM mask = 1;
	mask = mask << bitPosition;

	return mask;
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Bit index is out of range.");
	}

	const int memIndex = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);
	pMem[memIndex] = pMem[memIndex] | mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Bit index is out of range.");
	}

	const int memIndex = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);
	pMem[memIndex] = pMem[memIndex] & ~mask;
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0 || n >= BitLen)
	{
		throw std::out_of_range("Bit index is out of range.");
	}

	const int memIndex = GetMemIndex(n);
	const TELEM mask = GetMemMask(n);

	return (pMem[memIndex] & mask) ? 1 : 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this == &bf)
	{
		return *this;
	}

	delete[] pMem;
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = bf.pMem[i];
	}

	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение
{
	if (BitLen != bf.BitLen)
	{
		return 0;
	}

	for (int i = 0; i < MemLen; i++)
	{
		if (pMem[i] != bf.pMem[i])
		{
			return 0;
		}
	}

	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
	int resultBitLen, minMemLen;

	if (this == &bf)
	{
		TBitField result(*this);
		return result;
	}
	if (BitLen > bf.BitLen)
	{
		resultBitLen = BitLen;
		minMemLen = bf.MemLen;
	}
	else
	{
		resultBitLen = bf.BitLen;
		minMemLen = MemLen;
	}

	TBitField result(resultBitLen);

	for (int i = 0; i < minMemLen; i++)
	{
		result.pMem[i] = pMem[i] | bf.pMem[i];
	}

	return result;
}

TBitField TBitField::operator&(const TBitField& bf) // операция "и"
{
	int resultBitLen, minMemLen;

	if (this == &bf)
	{
		TBitField result(*this);
		return result;
	}
	if (BitLen > bf.BitLen)
	{
		resultBitLen = BitLen;
		minMemLen = bf.MemLen;
	}
	else
	{
		resultBitLen = bf.BitLen;
		minMemLen = MemLen;
	}

	TBitField result(resultBitLen);

	for (int i = 0; i < minMemLen; i++)
	{
		result.pMem[i] = pMem[i] & bf.pMem[i];
	}

	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen);

	for (int i = 0; i < BitLen; i++)
	{
		if (GetBit(i) == 0)
			result.SetBit(i);
		else
			result.ClrBit(i);
	}

	return result;
}

// ввод/вывод

istream& operator>>(istream& istr, TBitField& bf) // ввод
{
	char ch;
	int index = 0;

	while (istr.get(ch))
	{
		if (ch == '0')
		{
			bf.ClrBit(index);
		}
		else if (ch == '1')
		{
			bf.SetBit(index);
		}
		else
		{
			break;
		}

		index++;

		if (index >= bf.GetLength())
		{
			break;
		}
	}
	return istr;
}

ostream& operator<<(ostream& ostr, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
	{
		ostr << bf.GetBit(i);
	}

	return ostr;
}