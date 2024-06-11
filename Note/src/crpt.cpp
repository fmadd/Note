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

void recive_param(tcp::socket& socket, Integer& val){
    SecByteBlock block =  receive_crypto_block( socket);
    val.Decode(block.BytePtr(), block.SizeInBytes());
}

void send_param(tcp::socket& socket, Integer& val){
    size_t size = val.MinEncodedSize();
    SecByteBlock block(size);
    val.Encode(block.BytePtr(), size);
    send_crypto_block(socket, block);

}

void GenerateDHKeys(tcp::socket& socket, DH& dh, AutoSeededRandomPool& rnd, SecByteBlock& privKey, SecByteBlock& pubKey) {
    Integer p, q, g;
    recive_param(socket,p);
    recive_param(socket,g);
    q = (p - 1) / 2;

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

    ECB_Mode<AES>::Encryption cfbEncryption(key, key.size());
    string cipher, encoded;
    StringSource ss1( plaintext, true,
                      new StreamTransformationFilter( cfbEncryption,
                                                      new StringSink( cipher )
                      )
    );

    StringSource ss2( cipher, true,
                      new HexEncoder(
                              new StringSink( encoded )
                      )
    );
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
    return recovered;
}



std::string hashPass(std::string password){


    SHA256 hash;
    string hashed_password;

    hash.Update(reinterpret_cast<const CryptoPP::byte*>(password.data()), password.size());
    hashed_password.resize(hash.DigestSize());

    hash.Final(reinterpret_cast<CryptoPP::byte*>(&hashed_password[0]));

    string encoded;
    HexEncoder encoder(new StringSink(encoded));
    encoder.Put(reinterpret_cast<const CryptoPP::byte*>(hashed_password.data()), hashed_password.size());
    encoder.MessageEnd();
    return encoded;
}
