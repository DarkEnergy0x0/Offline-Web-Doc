#include <iostream>
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <regex>
#include <iomanip>


using std::cout; using std::endl;
using std::ifstream; using std::ofstream; using std::ios;
using std::string;
using std::vector; using std::map;
using std::regex_search; using std::regex; using std::cmatch; using std::smatch;
using std::exception;
using std::setbase; 


void getAllfiles(vector<string>&, const string&);
void getAllfiles(vector<string>& files, const string& entryDir) {
    cout << "[*]\tScanning dir: " << entryDir << endl;
    for (auto& i : std::filesystem::directory_iterator(entryDir)) {
        auto fp = i.path().generic_string();
        if (i.is_regular_file()) {
            cout << "Found file: " << fp << endl;
            files.emplace_back(std::move(fp));
        } else if (i.is_directory()) {
            cout << "Found dir: " << fp << endl;
            getAllfiles(files, fp);
        } else {
            cout << "Unknown type of file: " << fp << endl;
        }
    }
}

map<string, vector<unsigned char>> readAllFiles(vector<string>&);
map<string, vector<unsigned char>> readAllFiles(vector<string>& files) {
    cout << "[*]\tReading files..." << endl;
    map<string, vector<unsigned char>> ret{};
    for (auto& file : files) {
        cout << "Reading file: " << file << endl;
        ifstream f{file,  ios::in | ios::binary};
        if (f.is_open()) {
            char buf[255]{};
            vector<unsigned char> chars{};
            while (!f.eof()) {
                f.read(buf, sizeof(buf));
                int count = f.gcount();
                for (int i=0; i < count; ++i) {
                    chars.emplace_back((unsigned char) buf[i]);
                }
            }
            ret.emplace(std::move(file), std::move(chars));
            f.close();
        } else {
            cout << "Read file failed：" << file << endl;
            continue;
        }
    }
    return ret;
}


void genH(map<string, vector<unsigned char>>&, string&, const string&);
void genH(map<string, vector<unsigned char>>& dts,string& entryDir, const string& dataFName) {
    cout << "[*]\tGenerating header file: " << dataFName << "..." << endl;
    ofstream f{dataFName, ios::out};
    if (!f) {
        throw "Open file failed: " + dataFName;
    }
    // Get the basename for file
    // regex reg{".+\\\\(.+)"};
    smatch m{};
        regex rootDirPatt{R"_(^.+?(\/.+))_"};
        f << "#include <map>\n#include <vector>\n#include <string>\nusing namespace std;\n";
        f << "static const map<string, vector<unsigned char>> __datas__{\n";
        for (auto& i : dts) {
            string pathWithRoot = std::regex_replace(i.first, rootDirPatt, "$1");
            cout << "Generating byte array for: " << pathWithRoot << endl;
            f << "{\"" << pathWithRoot << "\",";
            f << "{";
            for (auto& j : i.second) {
                const auto bufferSize = snprintf(nullptr, 0, "%#X", j);
                char _t[bufferSize+1];
                snprintf(&_t[0], bufferSize+1, "%#X", j);
                // printf("%#X", j);
                f.write(&_t[0], bufferSize);
                f.put(',');
                f.flush();
            }
            f << "}";
            f << "},";
            f.flush();
        }
        f << "\n};";
        f.close();
}

int main() {
    string dataFName = "datas_hpp";
    string entry = "dist/";
    vector<string> files{};
    
    getAllfiles(files, entry);
    auto total = readAllFiles(files);
    genH(total, entry, dataFName);

    return EXIT_SUCCESS;
}