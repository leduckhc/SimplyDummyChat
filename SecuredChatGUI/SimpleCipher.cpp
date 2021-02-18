#include <string.h>

#include "pch.h"
#include "SimpleCipher.h"
#include <fstream>
#include <assert.h>

#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <string>
#include <cstdint>

using namespace CppCLRWinformsProjekt;


CppCLRWinformsProjekt::AesCipher::AesCipher()
{
	this->SetKeyfile();
}

CppCLRWinformsProjekt::AesCipher::~AesCipher()
{
	delete cipher;
}

void CppCLRWinformsProjekt::AesCipher::SetKeyfile()
{
	std::ifstream fin("keyfile");
	std::string klic;
	fin >> klic;
	if (klic.size() == 16)
	{
		cipher = new AES(klic);
	}
	else {
		cipher = new AES("TADYJEHESLOSIFRY");
	}
}

std::string CppCLRWinformsProjekt::AesCipher::Encode(std::string text)
{
	int sz = text.size();
	int pad_sz = (sz / 16 + 1) * 16;
	
	std::string plainText(text);
	plainText.resize(pad_sz, ' ');

	std::string cipherText = "";
	for (int i = 0; i < pad_sz; i += 16)
		cipherText += cipher->encrypt(plainText.substr(i, 16));
	//const char* cText = cipherText.c_str();
	assert(cipherText.size() % 16 == 0);
	return cipherText;
}

std::string CppCLRWinformsProjekt::AesCipher::Decode(std::string cipherText)
{
	assert(cipherText.size() % 16 == 0);
	std::string plainText = "";
	for (int i = 0; i < cipherText.size(); i += 16)
		plainText += cipher->decrypt(cipherText.substr(i, 16));

	return plainText;
}

/*

SimpleCipher::SimpleCipher()
{
	this->helperArr = new char[ALPHABET_SIZE + 1];
	memset(this->helperArr, 0, ALPHABET_SIZE + 1);
}

SimpleCipher::SimpleCipher(char* key)
{
	//this->key = nullptr;
	this->SetKey(key);
	this->helperArr = new char[ALPHABET_SIZE + 1];
	memset(this->helperArr, 0, ALPHABET_SIZE + 1);
}

CppCLRWinformsProjekt::SimpleCipher::~SimpleCipher()
{
	delete key;
	delete helperArr;
}

const char* SimpleCipher::Encode(const char* plainText)
{
	size_t sz = strlen(plainText);
	if (sz == 0)
		return "";

	char* cipherText = new char[sz + 1];
	strcpy(cipherText, plainText);
	int index;
	for (int i = 0; i < sz; i++)
	{
		if (cipherText[i] >= 'a' && cipherText[i] <= 'z')
		{
			index = cipherText[i] - 'a';
			cipherText[i] = this->helperArr[index]; // is lower
		}

		else if (cipherText[i] >= 'A' && cipherText[i] <= 'Z')
		{
			index = cipherText[i] - 'A';
			cipherText[i] = this->helperArr[index] - 'a' + 'A'; // to upper
		}

		cipherText[i] = cipherText[i] ^ 0x48;
	}
	return cipherText;
}

const char* SimpleCipher::Decode(const char* cipherText)
{
	size_t sz = strlen(cipherText);
	if (sz == 0)
		return "";

	char* plainText = new char[sz + 1];
	strcpy(plainText, cipherText);
	int index;
	for (int i = 0; i < sz; i++)
	{
		plainText[i] = plainText[i] ^ 0x48;


		if (plainText[i] >= 'a' && plainText[i] <= 'z')
		{
			// find the letter plainText[i] in helper 
			index = 0;
			for (; index < ALPHABET_SIZE; ++index)
			{
				if (this->helperArr[index] == plainText[i])
				{
					break;
				}
			}
			plainText[i] = index + 'a'; // is lower in ASCII
		}

		else if (plainText[i] >= 'A' && plainText[i] <= 'Z')
		{
			index = 0;
			for (; index < ALPHABET_SIZE; ++index)
			{
				if ((this->helperArr[index] - 'a' + 'A') == plainText[i])
				{
					break;
				}
			}
			plainText[i] = index + 'A'; // is upper in ASCII
		}
	}
	return plainText;
}

bool Contains(char* text, char c)
{
	auto sz = strlen(text);
	for (int i = 0; i < sz; i++)
	{
		if (text[i] == c)
		{
			return true;
		}
	}
	return false;
}

void SimpleCipher::SetKey(const char* key)
{
	// Copy parameter key to this->key
	if (this->key != nullptr)
		delete this->key;
	auto sz = strlen(key);
	if (sz == 0)
		return;
	this->key = new char[sz + 1];
	memset(this->key, 0, sz + 1);
	for (int i = 0; key[i] != '\0' && i < sz; i++)
	{
		this->key[i] = tolower(key[i]);
	}

	// Set helper arrray
	if (this->helperArr != nullptr)
		delete this->helperArr;
	this->helperArr = new char[ALPHABET_SIZE + 1];
	memset(this->helperArr, 0, ALPHABET_SIZE + 1);
	this->helperArr[0] = this->key[0];
	int k = 1; // actual index of char of key
	char c = 'a'; // actual char from English alphabet
	for (int i = 1; i < ALPHABET_SIZE; i++)
	{
		while (k < sz)
		{
			// If the letter was not used in the helper array, use it
			// otherwise try next character from key
			if (isalpha(this->key[k]) && Contains(this->helperArr, this->key[k]) == false)
			{
				this->helperArr[i] = this->key[k];
				break;
			}
			k++;
		}

		// Already used all characters from key. Use standard alphabet
		if (k >= sz)
		{
			while (c < 'z' && Contains(this->helperArr, c))
			{
				c++;
			}
			this->helperArr[i] = c;
		}
	}
}


CppCLRWinformsProjekt::DesCipher::DesCipher()
{
	std::ifstream fin("keyfile");
	std::string key;
	fin >> key;

	std::string s_text;

	cipher = new DES("", key);
}

void CppCLRWinformsProjekt::DesCipher::SetKey()
{
}




std::string string_to_hex(const std::string& in) {
	std::stringstream ss;

	ss << std::hex << std::setfill('0');
	for (size_t i = 0; in.length() > i; ++i) {
		ss << std::setw(2) << static_cast<unsigned int>(static_cast<unsigned char>(in[i]));
	}

	return ss.str();
}

std::string hex_to_string(const std::string& in) {
	std::string output;

	if ((in.length() % 2) != 0) {
		throw std::runtime_error("String is not valid length ...");
	}

	size_t cnt = in.length() / 2;

	for (size_t i = 0; cnt > i; ++i) {
		uint32_t s = 0;
		std::stringstream ss;
		ss << std::hex << in.substr(i * 2, 2);
		ss >> s;

		output.push_back(static_cast<unsigned char>(s));
	}

	return output;
}


const char* CppCLRWinformsProjekt::DesCipher::Encode(const char* text)
{

	cipher->setPlainText(string_to_hex(std::string(text)));


	return cipher->bin2hex( cipher->getCipherText() ).c_str();
}

const char* CppCLRWinformsProjekt::DesCipher::Decode(const char* text)
{
	//cipher->get
	// DES nema decipher!!!
	return nullptr;
}

*/
