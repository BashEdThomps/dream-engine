#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <iostream>

extern "C" {
    #include <curl/curl.h>
}

size_t write_data(void*, size_t, size_t, FILE*);

namespace Dream {
    using namespace std;
    class HTTPClient {
    public:
      HTTPClient();
      ~HTTPClient();
      bool get(string,string);
    };
} //End of Dream

#endif // HTTPCLIENT_H
