#pragma once

#include "AES.h"
#include "DES.h"

namespace CppCLRWinformsProjekt
{

	ref class ICipher
	{
	public:
		virtual std::string Encode(std::string text) = 0;
		virtual std::string Decode(std::string text) = 0;
	};

	ref class AesCipher : public ICipher
	{
	public:
		AesCipher();
		~AesCipher();

		void SetKeyfile();
		std::string Encode (std::string text) override;
		std::string Decode(std::string text) override;
	private:
		AES* cipher;
	};



	/*



	// interface for any encryption algorithm
	ref class ICipher
	{
	public:
		virtual const char* Encode(const char* text) = 0;
		virtual const char* Decode(const  char* text) = 0;
	};


	ref class SimpleCipher : public ICipher
	{
# define ALPHABET_SIZE 26



	public:
		SimpleCipher();
		SimpleCipher( char* key);
		~SimpleCipher();

		void SetKey(const char* key);
		const char* Encode(const  char* text) override;
		const char* Decode(const char* text) override;
	private:
		char* key;
		char* helperArr;
		unsigned char kk;
	};


	ref class DesCipher : public ICipher
	{
	public:
		DesCipher();
		~DesCipher() { delete cipher; };
		void SetKey();
		const char* Encode(const char* text) override;
		const char* Decode(const char* text) override;
	private:
		DES* cipher;
	};

	*/

}
