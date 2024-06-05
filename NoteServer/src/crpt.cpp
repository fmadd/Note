#include "crpt.h"

using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
using namespace CryptoPP;


void send_crypto_block(tcp::socket& socket, const SecByteBlock& block) {
    size_t size = block.size();
    write(socket, buffer(&size, sizeof(size)));
    write(socket, buffer(block.data(), size));
}
SecByteBlock receive_crypto_block(tcp::socket& socket) {
    size_t size;
    read(socket, buffer(&size, sizeof(size)));

    SecByteBlock block(size);
    read(socket, buffer(block.data(), size));

    Integer a;
    a.Decode(block.BytePtr(), block.SizeInBytes());

    return block;
}
void send_param(tcp::socket& socket, Integer& val){
    size_t size = val.MinEncodedSize();
    SecByteBlock block(size);
    val.Encode(block.BytePtr(), size);
    send_crypto_block(socket, block);

}
void GenerateDHKeys(tcp::socket& socket, DH& dh, AutoSeededRandomPool& rnd, SecByteBlock& privKey, SecByteBlock& pubKey) {
    dh.AccessGroupParameters().GenerateRandomWithKeySize(rnd, 512);

    Integer p = dh.GetGroupParameters().GetModulus();
    Integer q = (p - 1) / 2;
    Integer g = dh.GetGroupParameters().GetGenerator();
    send_param(socket,  p);
    send_param(socket, g);

    dh.AccessGroupParameters().Initialize(p, q, g);
    privKey.resize(dh.PrivateKeyLength());
    pubKey.resize(dh.PublicKeyLength());
    dh.GenerateKeyPair(rnd, privKey, pubKey);
}


CryptoPP::SecByteBlock deriveAESKeyFromDH(SecByteBlock& sharedKey) {
    SecByteBlock key(SHA256::DIGESTSIZE);
    SHA256().CalculateDigest(key, sharedKey, sharedKey.size());

    return key;
}


std::string encryptAES(const std::string &plaintext, CryptoPP::SecByteBlock key) {
//
    ECB_Mode<AES>::Encryption cfbEncryption(key, key.size());
    string cipher, encoded;
    StringSource ss1( plaintext, true,
                      new StreamTransformationFilter( cfbEncryption,
                                                      new StringSink( cipher )
                      ) // StreamTransformationFilter
    ); // StringSource

    StringSource ss2( cipher, true,
                      new HexEncoder(
                              new StringSink( encoded )
                      ) // HexEncoder
    ); // StringSource
    cout << "cipher text: " << encoded << endl;
    return cipher;

}

std::string decryptAES(const std::string &ciphertext, CryptoPP::SecByteBlock Key) {
    ECB_Mode<AES>::Decryption aesDecryption(  Key,  Key.size());
    string recovered ;

    StringSource ss3( ciphertext, true,
                      new StreamTransformationFilter( aesDecryption,
                                                      new StringSink( recovered )
                      ) // StreamTransformationFilter
    ); // StringSource

    cout << "recovered text: " << recovered << endl;
    return recovered;
}
std::string hashPass(std::string password){


    SHA256 hash;
    string hashed_password;

    hash.Update(reinterpret_cast<const CryptoPP::byte*>(password.data()), password.size());
    hashed_password.resize(hash.DigestSize());

    hash.Final(reinterpret_cast<CryptoPP::byte*>(&hashed_password[0]));

    // Преобразуем хешированный пароль в строку в шестнадцатеричном формате.
    string encoded;
    HexEncoder encoder(new StringSink(encoded));
    encoder.Put(reinterpret_cast<const CryptoPP::byte*>(hashed_password.data()), hashed_password.size());
    encoder.MessageEnd();

    // Выводим хешированный пароль.
    cout << "Хешированный пароль: " << encoded << endl;

    return encoded;
}
