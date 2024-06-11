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
#include "socket_service.hpp"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;


void send_crypto_block(socket_service &socket, const SecByteBlock &block) {
    size_t size = block.size();
    socket.writeInSocket(buffer(&size, sizeof(size)));
    socket.writeInSocket(buffer(block.data(), size));
}

SecByteBlock receive_crypto_block(socket_service &socket) {
    size_t size = socket.read_filesize();
    SecByteBlock block(size);
    socket.read_buffer(buffer(block.data(), size));

    Integer a;
    a.Decode(block.BytePtr(), block.SizeInBytes());

    return block;
}

void send_param(socket_service &socket, Integer &val) {
    size_t size = val.MinEncodedSize();
    SecByteBlock block(size);
    val.Encode(block.BytePtr(), size);
    send_crypto_block(socket, block);

}

void
GenerateDHKeys(socket_service &socket, DH &dh, AutoSeededRandomPool &rnd, SecByteBlock &privKey, SecByteBlock &pubKey) {
    dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, 512);

    Integer p = dh.GetGroupParameters().GetModulus();
    Integer q = (p - 1) / 2;
    Integer g = dh.GetGroupParameters().GetGenerator();
    send_param(socket, p);
    send_param(socket, g);

    dh.AccessGroupParameters().Initialize(p, q, g);
    privKey.resize(dh.PrivateKeyLength());
    pubKey.resize(dh.PublicKeyLength());
    dh.GenerateKeyPair(rnd, privKey, pubKey);
}


CryptoPP::SecByteBlock deriveAESKeyFromDH(SecByteBlock &sharedKey) {
    SecByteBlock key(SHA256::DIGESTSIZE);
    SHA256().CalculateDigest(key, sharedKey, sharedKey.size());

    return key;
}


std::string encryptAES(const std::string &plaintext, CryptoPP::SecByteBlock key) {
//
    ECB_Mode<AES>::Encryption cfbEncryption(key, key.size());
    string cipher, encoded;
    StringSource ss1(plaintext, true,
                     new StreamTransformationFilter(cfbEncryption,
                                                    new StringSink(cipher)
                     )
    );

    StringSource ss2(cipher, true,
                     new HexEncoder(
                             new StringSink(encoded)
                     )
    );
    cout << "cipher text: " << encoded << endl;
    return cipher;

}

std::string decryptAES(const std::string &ciphertext, CryptoPP::SecByteBlock Key) {
    ECB_Mode<AES>::Decryption aesDecryption(Key, Key.size());
    string recovered;

    StringSource ss3(ciphertext, true,
                     new StreamTransformationFilter(aesDecryption,
                                                    new StringSink(recovered)
                     )
    );

    cout << "recovered text: " << recovered << endl;
    return recovered;
}

std::string hashPass(std::string password) {


    SHA256 hash;
    string hashed_password;

    hash.Update(reinterpret_cast<const CryptoPP::byte *>(password.data()), password.size());
    hashed_password.resize(hash.DigestSize());

    hash.Final(reinterpret_cast<CryptoPP::byte *>(&hashed_password[0]));

    string encoded;
    HexEncoder encoder(new StringSink(encoded));
    encoder.Put(reinterpret_cast<const CryptoPP::byte *>(hashed_password.data()), hashed_password.size());
    encoder.MessageEnd();


    return encoded;
}

CryptoPP::SecByteBlock sharing_key(socket_service &socket) {
    DH dh;
    AutoSeededRandomPool rnd;
    dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, 512);

    SecByteBlock privKey, pubKey, otherPubKey;
    GenerateDHKeys(socket, dh, rnd, privKey, pubKey);
    otherPubKey = receive_crypto_block(socket);
    send_crypto_block(socket, pubKey);
    SecByteBlock sharedKey(dh.AgreedValueLength());

    std::cout << "suc generated key" << endl;
    return deriveAESKeyFromDH(sharedKey);
}
