#include "incs/httplib.h"
#include "datas_hpp"
#include <map>
#include <regex>

enum OutputType {
    info,
    warn,
    succ,
    fail
};

void putMsg(string, OutputType);
void putMsg(string s, OutputType ot) {
    stringstream msg{};
    switch (ot) {
        case info:
            msg << "[*]";
            break;
        case warn:
            msg << "[!]";
            break;
        case succ:
            msg << "[+]";
            break;
        case fail:
            msg << "[x]";
            break;
        default:
            msg << "[?]";
            break;
    }
    msg << " ";
    msg << s;
    cout << msg.str() << endl;
}

const map<string, string> MimeTypes {
    {"js", "application/javascript"},
    {"html", "text/html; charset=utf-8"},
    {"css", "text/css"},
    {"ico", "image/vnd.microsoft.icon"},
    {"png", "image/png"},
    {"jpg", "image/jpeg"},
    {"text", "text/plain"},
    {"svg", "image/svg+xml"}
};

const regex mimeTypePatt{R"_(.+\.(.+))_"};

string getMimeType(const string&);
string getMimeType(const string& s1) {
    smatch matt{};
    map<string, string>::const_iterator iter{};
    if (regex_search(s1, matt, mimeTypePatt)) {
        auto fExt = matt.str(1);
        if ((iter = MimeTypes.find(fExt)) != MimeTypes.end()) {
            return iter->second;
        }
    }
    return MimeTypes.find("text")->second;
}

int main() {
    httplib::Server srv{};
    auto port = srv.bind_to_any_port("127.0.0.1");
    srv.Get(R"_(.*)_", [&] (const httplib::Request& req, httplib::Response& res) {

        map<string, vector<unsigned char>>::const_iterator page{};
        ;
        // Set index page
        string reqURL{ req.path == "/" ? "/index.html" : req.path};

        if ((page = __datas__.find(reqURL)) != __datas__.end()) {
            auto mimeType = getMimeType(page->first);
            res.set_content((const char*)page->second.data(), page->second.size(), mimeType.c_str());
            res.status = 200;
            return;
        } else {
            putMsg("Unknown URL:" + reqURL, warn);
        }
        res.status = 404;
    });

    string url{};
    url.assign("http://127.0.0.1:" + to_string(port));
    putMsg(string{"Doc server have started at: http://127.0.0.1:" + to_string(port)}, info);
    putMsg("Press <Ctrl-c> or close this window to exit.", info);
    cout << endl << endl;
    system(("start " + url).c_str());
    srv.listen_after_bind();
    return EXIT_SUCCESS;
}