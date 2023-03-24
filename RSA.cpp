#include <iostream>
#include <vector>
#include <mpir.h>

#include "bigint.h"
#include "RSA.h"

using namespace FT42;

void Sender::Create_Encryption_Key1()
{
	iamtcs::BigInt Temp;

	Public_Key1.multiply(Public_Key2, &Temp);

	Encryption_Key1.assign(Temp);
}

/*
isPrime - Returns 0 if number is not prime
		else returnes the data means value of BigInt Object

compare - Returns 0 if both same
		else returns 1
*/

void Sender::Create_Encryption_Key2()
{
	const iamtcs::BigInt One(1), Zero(0);
	iamtcs::BigInt Phi, Temp, Temp1, Temp2, Remainder, divider = 2;

	std::list <iamtcs::BigInt> FactorList;

	int iCounter, iRetVal1, iRetVal2, iTemp1, iTemp2 = 0;
	bool match;

	Public_Key1.subtract(One, &Temp1);	//Temp1 = P - 1;
	Public_Key2.subtract(One, &Temp2);	//Temp2 = Q - 1;

	Temp1.multiply(Temp2, &Phi);		//Phi = (P - 1) * (Q - 1)

	do
	{
		Phi.modulo(divider, &Remainder);
		iTemp1 = Remainder.compare(Zero);

		if (!iTemp1)
		{
			FactorList.push_back(divider);
			iTemp2++;
		}

		divider.add(One);

	} while (iTemp2 < 10);

	for (int iCounter = 2; iCounter <= 10; iCounter++)
	{
		match = false;
		auto it = FactorList.begin();

		while ( it != FactorList.end() )
		{
			Temp.assign(iCounter);
			iRetVal1 = Temp.compare(*it);

			if( ! ( Temp.compare(*it) ) )
			{
				match = true;
				break;
			}

			it++;
		}

		iRetVal2 = Temp.isPrime();

		if (!match && iRetVal2)
		{
			Encryption_Key2.assign(Temp);
			return;
		}
	}


}

void FT42::Sender::Create_Encryption_Key3()
{
	iamtcs::BigInt Phi;
	iamtcs::BigInt Temp1, Temp2;		//Temp1 = P - 1 & Temp2 = Q - 1;
	iamtcs::BigInt Temp3, Temp4;		//Calculation Purpose
	iamtcs::BigInt One(1), Zero(0);		//Compare Purpose
	iamtcs::BigInt Quo, Rem;

	Public_Key1.subtract(One, &Temp1);	//Temp1 = P - 1;
	Public_Key2.subtract(One, &Temp2);	//Temp2 = Q - 1;

	Temp1.multiply(Temp2, &Phi);		//Phi = (P - 1) * (Q - 1)
	Temp3.assign(1);					//Temp3 = 1 - not for addition to increment
	
	Temp4.assign(Phi);

	while (1)
	{
		Temp4.multiply(Temp3);			//directly not multiplying by Phi as don't want to modify it
		Temp4.add(One);

		Temp4.divideBy(Encryption_Key2, &Quo, &Rem);

		if (!(Rem.compare(Zero)))
		{
			Encryption_Key3.assign(Quo);
			break;
		}
		else
		{
			Temp3.add(One);
		}

		Temp4.assign(Phi);
	}
}

std::list <iamtcs::BigInt> FT42::Sender::Create_Cipher_Text()
{
	std::list <iamtcs::BigInt> CipherText;
	std::vector<int> iVec;
	iamtcs::BigInt Temp1, Temp2;
	int len = Plain_Text.length();
	int iTemp;

	for (int iCounter = 0; iCounter < len; iCounter++)
	{
		iTemp = Plain_Text[iCounter];

		iVec.push_back(iTemp);
	}

	for (int iCounter = 0; iCounter < len; iCounter++)
	{
		Temp1.assign(iVec[iCounter]);
		Temp1.powmod(Encryption_Key3, Encryption_Key1, &Temp2);
		CipherText.push_back(Temp2);
	}

	return CipherText;
}


iamtcs::BigInt* FT42::Sender::get_Encryption_Key1()
{
	iamtcs::BigInt* Encryption_Key1_Ptr = new iamtcs::BigInt(Encryption_Key1);

	return Encryption_Key1_Ptr;
}

iamtcs::BigInt* FT42::Sender::get_Encryption_Key2()
{
	iamtcs::BigInt* Encryption_Key2_Ptr = new iamtcs::BigInt(Encryption_Key2);

	return Encryption_Key2_Ptr;
}

void Sender::Print()
{
	unsigned int len;
	char* str = nullptr;

	this->Public_Key1.toStr(str, len, 10);
	std::cout << "iPublic_Key1 = " << str << std::endl;
	str = nullptr;

	this->Public_Key2.toStr(str, len, 10);
	std::cout << "iPublic_Key2 = " << str << std::endl;
	str = nullptr;

	this->Encryption_Key1.toStr(str, len, 10);
	std::cout << "Encryption_Key1 (N) = " << str << std::endl;
	str = nullptr;

	this->Encryption_Key2.toStr(str, len, 10);
	std::cout << "Encryption_Key2 (E) = " << str << std::endl;
	str = nullptr;

	this->Encryption_Key3.toStr(str, len, 10);
	std::cout << "Encryption_Key3 (D) = " << str << std::endl;
	str = nullptr;

	std::cout << "Received Plain Text is = " << Plain_Text << std::endl;
}

void FT42::Sender::Print_Cipher_Text(std::list <iamtcs::BigInt> Cipher_Text)
{
	unsigned int len;
	char* str = nullptr;
	auto it = Cipher_Text.begin();

	while (it != Cipher_Text.end())
	{
		it->toStr(str, len, 10);
		std::cout << str;
		str = nullptr;

		it++;
	}
}

std::string FT42::Receiver::Create_Plain_Text (
												iamtcs::BigInt* Encryption_Key1_Ptr, 
												iamtcs::BigInt* Encryption_Key2_Ptr
											  )
{
	int count = Cipher_Text.size();
	unsigned int len;
	char* str = nullptr;
	int num, iCounter;
	char TXT[100];
	iamtcs::BigInt Temp3(*Encryption_Key1_Ptr), Temp4(*Encryption_Key2_Ptr);

	delete Encryption_Key1_Ptr;
	delete Encryption_Key2_Ptr;

	auto it = Cipher_Text.begin();
	iamtcs::BigInt Temp1, Temp2;

	for (iCounter = 0; iCounter < count; iCounter++)
	{
		Temp1.assign(*it);

		Temp4.toStr(str, len, 10);

		num = std::stoll(str);

		Temp1.powmod(num, Temp3);

		Temp1.toStr(str, len, 10);

		num = std::stoll(str);	

		TXT[iCounter] = num;

		it++;
	}

	TXT[iCounter] = '\0';

	std::string PlainTXT(TXT);

	return PlainTXT;
}