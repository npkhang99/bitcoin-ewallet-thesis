#include "http_client.h"

http_client::http_client() {}

http_client::http_client(const std::string& url, request_types request_type)
        : _url(url), request_type(request_type) {}

static size_t call_back(char* data, size_t size, size_t nmemb, void* userp) {
    ((std::string*) userp)->append(data);
    return size * nmemb;
}

void http_client::set_data(const std::string& data) {
    _data = data;
}

std::string http_client::execute() {
    std::string response;

    switch (request_type) {
        case GET:
            response = execute_get();
            break;
        case POST:
            response = execute_post();
            break;
        default:
            break;
    }

    return response;
}

std::string http_client::execute_get() {
    CURL* curl_handle = curl_easy_init();
    CURLcode code;
    std::string response;

    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, call_back);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);
        code = curl_easy_perform(curl_handle);
        if (code != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(code) << std::endl;
        }
        curl_easy_cleanup(curl_handle);
    }

    return response;
}

std::string http_client::execute_post() {
    CURL* curl_handle = curl_easy_init();
    CURLcode code;
    std::string response;

    if (curl_handle) {
        curl_easy_setopt(curl_handle, CURLOPT_URL, _url.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, _data.c_str());
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, call_back);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &response);

        code = curl_easy_perform(curl_handle);
        if (code != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: "
                      << curl_easy_strerror(code) << std::endl;
        }

        curl_easy_cleanup(curl_handle);
    }

    return response;
}
