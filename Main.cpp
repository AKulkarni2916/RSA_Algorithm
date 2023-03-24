#include <iostream>

#include "RSA.h"

using namespace FT42;

int main()
{
	std::string PT;
	std::list <iamtcs::BigInt> CipherTXT;
	iamtcs::BigInt* Encryption_Key1_Ptr, * Encryption_Key2_Ptr;
	std::string PlainTXT;

	std::cout << "\nEnter String to Encrypt\t";
	std::cin >> PT;

	Sender Sender_Obj(PT);

	std::cout << "\n Available Data is - \n";
	Sender_Obj.Print();

	CipherTXT = Sender_Obj.Create_Cipher_Text();

	std::cout << "\nCreated Cipher Text is = ";
	Sender_Obj.Print_Cipher_Text(CipherTXT);

	Encryption_Key1_Ptr = Sender_Obj.get_Encryption_Key1();
	Encryption_Key2_Ptr = Sender_Obj.get_Encryption_Key2();

	Receiver Receiver_Obj(CipherTXT);
	
	PlainTXT = Receiver_Obj.Create_Plain_Text(Encryption_Key1_Ptr, Encryption_Key2_Ptr);

	std::cout << "\n\nCreated Plain Text is " << PlainTXT << std::endl;

	return 0;
}