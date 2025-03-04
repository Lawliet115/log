#pragma once
#include <iostream>
#include <memory>

#include "formatter/default_formatter.h"
#include "formatter/effective_formatter.h"
#include "crypt/aes_crypt.h"
#include "proto/effective_msg.pb.h"
#include "utils/sys_util.h"
#include "utils/file_util.h"

#include "log_handle.h"
#include "sinks/sink.h"
#include "sinks/console_sink.h"
#include "log_common.h"
#include "internal_log.h"
#include "mmap/mmap.h"
#include "context/context.h"
#include "compress/zstd_compress.h"
#include "compress/zlib_compress.h"  


int main(){
    
    auto ecdh_key = logger::crypt::GenECDHKey();
    auto client_pri = std::get<0>(ecdh_key);
    auto client_pub = std::get<1>(ecdh_key);
    std::cout << "client_pri: " << client_pri << std::endl;
    std::cout << "client_pub: " << client_pub << std::endl;
  
    auto aes_key = logger::crypt::AESCrypt::GenerateKey();
    std::cout << "aes_key: " << aes_key << std::endl;
  
    auto ecdh_key2 = logger::crypt::GenECDHKey();
    auto server_pri = std::get<0>(ecdh_key2);
    auto server_pub = std::get<1>(ecdh_key2);
    std::cout << "server_pri: " << server_pri << " size " << server_pri.size() << std::endl;
    std::cout << "server_pub: " << server_pub << " size " << server_pub.size() << std::endl;
  
    auto client_pri_hex = logger::crypt::BinaryKeyToHex(client_pri);
    std::cout << "client_pri_hex: " << client_pri_hex << std::endl;
    auto client_pub_hex = logger::crypt::BinaryKeyToHex(client_pub);
    std::cout << "client_pub_hex: " << client_pub_hex << std::endl;
  
    auto server_pri_hex = logger::crypt::BinaryKeyToHex(server_pri);
    std::cout << "server_pri_hex: " << server_pri_hex << std::endl;
    auto server_pub_hex = logger::crypt::BinaryKeyToHex(server_pub);
    std::cout << "server_pub_hex: " << server_pub_hex << std::endl;
  
    auto client_shared_key = logger::crypt::GenECDHSharedSecret(client_pri, server_pub);
    std::cout << "client_shared_key: " << client_shared_key << std::endl;
  
    auto server_shared_key = logger::crypt::GenECDHSharedSecret(server_pri, client_pub);
    std::cout << "server_shared_key: " << server_shared_key << std::endl;
  
    auto shared_key_hex = logger::crypt::BinaryKeyToHex(client_shared_key);
    std::cout << "shared_key_hex: " << shared_key_hex << std::endl;
  
    logger::crypt::AESCrypt aes(client_shared_key);
    std::string input = "hello, world!";
    std::string output;
    output.clear();

    std::cout << "iv: " << aes.GetIv_() << std::endl;

    aes.Encrypt(input.data(), input.size(), output);
    output = logger::crypt::BinaryKeyToHex(output);
    std::cout << "crypted: " << output << std::endl;

    output = logger::crypt::HexKeyToBinary(output);
    std::string decrypted = aes.Decrypt(output.data(), output.size());
    std::cout << "decrypted: " << decrypted << std::endl;

    return 0;
}