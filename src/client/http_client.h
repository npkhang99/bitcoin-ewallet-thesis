#ifndef LIBBITCOIN_THESIS_HTTP_CLIENT_H
#define LIBBITCOIN_THESIS_HTTP_CLIENT_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <curl/curl.h>

enum request_types {
    GET,
    POST
};

/// Basic HTTP client
class http_client {
public:
    http_client();
    http_client(const std::string& url, request_types request_type = GET);

    void set_data(const std::string& data);

    std::string execute();

private:
    std::string _url;
    std::string _data;
    request_types request_type;

    std::string execute_get();
    std::string execute_post();
};


#endif //LIBBITCOIN_THESIS_HTTP_CLIENT_H
