#include <doctest.h>
#include "../src/crpt.h"
using boost::asio::ip::tcp;
using namespace boost::asio;
using namespace std;
//TEST_CASE("send_crypto_block") {
//boost::asio::io_service io_service;
//tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
//tcp::socket socket(io_service);
//
//// Создать тестовый блок данных
//SecByteBlock block = "Hello, world!";
//
//// Отправить блок данных через сокет
//send_crypto_block(socket, block);
//
//// Убедиться, что блок данных успешно отправлен через сокет
//CHECK(socket.send_buffer_size() == block.size() + sizeof(size_t));
//}
//
//TEST_CASE("receive_crypto_block") {
//// Создать фиктивный сокет
//boost::asio::io_service io_service;
//        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
//        tcp::socket socket(io_service);
//
//// Создать тестовый блок данных
//SecByteBlock block = "Hello, world!";
//size_t size = block.size();
//
//// Отправить размер блока данных через сокет
//write(socket, buffer(&size, sizeof(size)));
//
//// Отправить блок данных через сокет
//write(socket, buffer(block.data(), size));
//
//// Получить блок данных через сокет
//SecByteBlock received_block = receive_crypto_block(socket);
//
//// Убедиться, что полученный блок данных совпадает с тестовым блоком данных
//CHECK(received_block == block);
//}

TEST_CASE("send_param") {
// Создать фиктивный сокет
boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
        tcp::socket socket(io_service);

// Создать тестовое значение
Integer val(12345);

// Отправить значение параметра через сокет
send_param(socket, val);

// Убедиться, что значение параметра успешно отправлено через сокет
CHECK(socket.send_buffer_size() == val.MinEncodedSize() + sizeof(size_t));
}

TEST_CASE("GenerateDHKeys") {
// Создать фиктивный сокет
boost::asio::io_service io_service;
        tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
        tcp::socket socket(io_service);

// Создать объект DH
DH dh;

// Создать объект AutoSeededRandomPool
AutoSeededRandomPool rnd;

// Создать тестовые приватный и публичный ключи
SecByteBlock privKey;
SecByteBlock pubKey;

// Сгенерировать ключи DH
GenerateDHKeys(socket, dh, rnd, privKey, pubKey);

// Убедиться, что сгенерированы приватный и публичный ключи
CHECK(privKey.size() == dh.PrivateKeyLength());
CHECK(pubKey.size() == dh.PublicKeyLength());
}

TEST_CASE("deriveAESKeyFromDH") {
// Создать тестовый общий ключ
SecByteBlock sharedKey = "Shared key";

// Вывести ключ AES
SecByteBlock key = deriveAESKeyFromDH(sharedKey);

// Убедиться, что ключ AES успешно выведен
CHECK(key.size() == SHA256::DIGESTSIZE);
}

TEST_CASE("encryptAES") {
// Создать тестовой текст
string plaintext = "Hello, world!";

// Создать ключ AES
SecByteBlock key = deriveAESKeyFromDH(sharedKey);

// Зашифровать текст с помощью AES
string ciphertext = encryptAES(plaintext, key);

// Убедиться, что текст успешно зашифрован
CHECK(ciphertext.size() > 0);
}

TEST_CASE("decryptAES") {
// Создать зашифрованный текст
string ciphertext = "U29tZSB0ZXh0IHdlIHdhbnQgdG8gZGVjcnlwdA==";

// Создать ключ AES
SecByteBlock key = deriveAESKeyFromDH(sharedKey);

// Расшифровать текст с помощью AES
string plaintext = decryptAES(ciphertext, key);

// Убедиться, что текст успешно расшифрован
CHECK(plaintext == "Some text we want to decrypt");
}

TEST_CASE("hashPass") {
// Создать тестовый пароль
string password = "password";

// Хешировать пароль
string hashed_password = hashPass(password);

// Убедиться, что пароль успешно хеширован
CHECK(hashed_password.size() > 0);
}