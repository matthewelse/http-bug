#include "mbed.h"
#include "easy-connect.h"
#include "http_request.h"

#define TARGET_URL "http://192.168.0.14"

void dump_response(HttpResponse* res) {
    printf("Status: %d - %s\n", res->get_status_code(), res->get_status_message().c_str());
 
    printf("Headers:\n");
    for (size_t ix = 0; ix < res->get_headers_length(); ix++) {
        printf("\t%s: %s\n", res->get_headers_fields()[ix]->c_str(), res->get_headers_values()[ix]->c_str());
    }
    printf("\nBody (%d bytes):\n\n%s\n", res->get_body_length(), res->get_body_as_string().c_str());
}

int main() {
    // Connect to the network (see mbed_app.json for the connectivity method used)
    NetworkInterface *network = easy_connect(true);
    if (!network) {
        printf("Cannot connect to the network, see serial output");
        return 1;
    }

    HttpRequest* get_req = new HttpRequest(network, HTTP_GET, TARGET_URL);
 
    // By default the body is automatically parsed and stored in a string, this is memory heavy.
    // To receive chunked response, pass in a callback as third parameter to 'send'.
    HttpResponse* get_res = get_req->send();
    if (!get_res) {
        printf("HttpRequest failed (error code %d)\n", get_req->get_error());
        return 1;
    }

    printf("Expecting some JSON in the response, we'll probably only see the 200 OK.\n");

    printf("\n----- HTTP GET response -----\n");
    dump_response(get_res);

    delete get_req;

    Thread::wait(osWaitForever);
}
