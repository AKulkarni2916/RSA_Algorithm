#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <list>

#include "bigint.h"

namespace FT42
{
	class Sender
	{
		iamtcs::BigInt Encryption_Key1;		//N
		iamtcs::BigInt Encryption_Key2;		//D
		iamtcs::BigInt Encryption_Key3;		//E

		std::string Plain_Text;				//PT
		
		void Create_Encryption_Key1();
		void Create_Encryption_Key2();
		void Create_Encryption_Key3();

	public:
		iamtcs::BigInt Public_Key1;			//P
		iamtcs::BigInt Public_Key2;			//Q

		std::list <iamtcs::BigInt> Create_Cipher_Text();

		Sender(long long int P, long long int Q, std::string Text)
		{
			Public_Key1.assign(P);
			Public_Key2.assign(Q);

			if (Public_Key1.isPrime() && Public_Key2.isPrime())
			{
				Plain_Text = Text;

				Create_Encryption_Key1();		//N
				Create_Encryption_Key2();		//D
				Create_Encryption_Key3();		//E
			}
			else
			{
				std::cout << "Received non Prime numbers\nExiting\n";
				exit(1);
			}
		}

		Sender(std::string Text) :Public_Key1("48112959837082048697", 20, 10), Public_Key2("71755440315342536873", 20, 10)
		{
			Plain_Text = Text;

			Create_Encryption_Key1();		//N
			Create_Encryption_Key2();		//D
			Create_Encryption_Key3();		//E
		}

		void Print_Encryption_Key1();
		void Print_Encryption_Key2();
		void Print_Encryption_Key3();

		iamtcs::BigInt* get_Encryption_Key1();		//D
		iamtcs::BigInt* get_Encryption_Key2();		//E

		void Print();
		void Print_Cipher_Text(std::list <iamtcs::BigInt>);
	};

	class Receiver
	{
		std::list <iamtcs::BigInt> Cipher_Text;
		void Print_Cipher_Text(std::list <iamtcs::BigInt> Cipher_Text);

	public:

		Receiver(std::list <iamtcs::BigInt> Received_List)
		{
			Cipher_Text = Received_List;
		}

		std::string Create_Plain_Text(iamtcs::BigInt*, iamtcs::BigInt*);
	};
};