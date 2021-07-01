#ifndef LIBBITCOIN_THESIS_HTTP_CLIENT_H
#define LIBBITCOIN_THESIS_HTTP_CLIENT_H

#include <iostream>
#include <string>
#include <vector>
#include <curl/curl.h>

class http_client {
public:
    http_client();
    http_client(const std::string& url);

    std::string execute();

private:
    std::string _url;
};


#endif //LIBBITCOIN_THESIS_HTTP_CLIENT_H
