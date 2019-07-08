#include <iostream>
#include "fcgio.h"
#include "parser.h"

using namespace std;

int main() {
    // Backup the stdio streambufs
    streambuf * cin_streambuf  = cin.rdbuf();
    streambuf * cout_streambuf = cout.rdbuf();
    streambuf * cerr_streambuf = cerr.rdbuf();

    FCGX_Request request;

    FCGX_Init();
    FCGX_InitRequest(&request, 0, 0);

    while (FCGX_Accept_r(&request) == 0) {
        fcgi_streambuf cin_fcgi_streambuf(request.in);
        fcgi_streambuf cout_fcgi_streambuf(request.out);
        fcgi_streambuf cerr_fcgi_streambuf(request.err);

        cin.rdbuf(&cin_fcgi_streambuf);
        cout.rdbuf(&cout_fcgi_streambuf);
        cerr.rdbuf(&cerr_fcgi_streambuf);

    	string uri = FCGX_GetParam("REQUEST_URI", request.envp);

		cout << "Content-Type: text/plain\r\n" << "\r\n";

		try {
			Parser uriParsed(uri);
			uriParsed.invoke();
		}
		catch (char const* err) {
			cout << err << endl;
			cerr << err << endl;
		}
		catch (...) {
			cout << "fatal error occured." << endl;
			cerr << "fatal error occured." << endl;
		}
    }

    // restore stdio streambufs
    cin.rdbuf(cin_streambuf);
    cout.rdbuf(cout_streambuf);
    cerr.rdbuf(cerr_streambuf);

    return 0;
}
