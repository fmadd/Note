#pragma once
#include <iostream>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <cryptopp/aes.h>
#include <cryptopp/osrng.h>
#include <cryptopp/dh2.h>
#include <cryptopp/dh.h>
#include <cryptopp/secblock.h>
#include <cryptopp/base64.h>
#include <cryptopp/hex.h>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;


void send_crypto_block(tcp::socket &socket, const SecByteBlock &block);

SecByteBlock receive_crypto_block(tcp::socket &socket);

void send_param(tcp::socket &socket, Integer &val);

void
GenerateDHKeys(tcp::socket &socket, DH &dh, AutoSeededRandomPool &rnd, SecByteBlock &privKey, SecByteBlock &pubKey);

CryptoPP::SecByteBlock deriveAESKeyFromDH(SecByteBlock &sharedKey);

std::string encryptAES(const std::string &plaintext, CryptoPP::SecByteBlock key);

std::string decryptAES(const std::string &ciphertext, CryptoPP::SecByteBlock Key);

std::string hashPass(std::string password);
