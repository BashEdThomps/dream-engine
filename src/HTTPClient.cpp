#include <cstdio>
#include "HTTPClient.h"

namespace Dream {

    HTTPClient::HTTPClient() {

    }

    HTTPClient::~HTTPClient() {

    }

    bool HTTPClient::get(string url, string outputPath) {
      CURL *curl = curl_easy_init();
      CURLcode result;
      if (curl) {
        FILE* fp = fopen(outputPath.c_str(),"wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        result = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        if (result != CURLE_OK) {
           cerr << "HTTPClient: Curl Reported an error while downloading " << url << " to " << outputPath << endl
                << result << endl;
           return false;
        }
      }
      return true;
    }
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}
