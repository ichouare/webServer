#include "./webserver.hpp"

int main()
{
//     std::string requese = "GET /hello.htm HTTP/1.1 User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)
// Host: www.tutorialspoint.com
// Accept-Language: en-us
// Accept-Encoding: gzip, deflate
// Connection: Keep-Alive";

    Requese *req = new Requese ("GET /file1.html HTTP/1.1\r\nUser-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\nHost: www.tutorialspoint.com:156\r\nAccept-Language: en-us\r\nAccept-Encoding: gzip, deflate \r\nConnection: Keep-Alive\r\nDate: 2012-04-02\r\nContent-Type: multipart/form-data; boundary=---------------------------1234567890123456789012345678\r\nContent-Length: 0\r\n\r\n");
        std::cout << "here" << std::endl;
    Response res(req->status_response_code, req->Initial_Request_Line, req->response_items);
    res.build_response();

}