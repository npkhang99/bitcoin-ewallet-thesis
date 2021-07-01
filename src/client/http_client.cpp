#include "http_client.h"

http_client::http_client() {}

http_client::http_client(const std::string& url)
        : _url(url) {}

static size_t call_back(char* data, size_t size, size_t nmemb, void* userp) {
    ((std::string*) userp)->append(data);
    return size * nmemb;
}

std::string http_client::execute() {
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
