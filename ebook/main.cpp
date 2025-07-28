#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <string.h>

    // константы
        // команды запросов
static const char* read_qr  = "READ";
static const char* cheer_qr = "CHEER";
        // размер буфера чтения команды запросов
static const short bufSz = 16;

class RequestProcessor {
        // макс. число страниц по условию
    static const short pages_num = 1000;
        // словарь "читатель - число страниц"
    std::unordered_map<int,short> users_;
        // словарь-массив "число страниц - число прочитавших такое число"
    int user_counts_[pages_num]={};

public:
    void ProcessRead(int user, short pages) {
        const auto& cit = users_.find(user);
            // not found
        if (cit == users_.cend()) {
            users_[user] = pages;
            ++user_counts_[pages-1];
        }   // found
        else {
            short old_val = cit->second;
            if (old_val < pages) {
                --user_counts_[old_val-1];
                ++user_counts_[pages-1];

                cit->second = pages;
            }
        }
    }

    double ProcessCheer(int user) {
        double ret = 0;
        const auto& cit = users_.find(user);
        if (cit != users_.cend()) {
            if (users_.size() == 1)
                ret = 1;
            else {    
                short u_pages = cit->second;
                int cnt = 0;

                for (int ii=0; ii < u_pages-1; ++ii) 
                    cnt+=user_counts_[ii];

                ret = (double)cnt / (users_.size() - 1);        
            }
        }
        return ret;
    }
};

int main() {
    using namespace std;

    int user = 0, cmd_count;
    cin >> cmd_count;

    RequestProcessor proc;
    char buf[bufSz]={};

    for (int i = 0; i < cmd_count; ++i) {
        memset(buf,0,bufSz);
        cin >> buf;
            // READ
        if (!strcmp(buf,read_qr)) {
            short page;
            cin >> user >> page;           
            proc.ProcessRead(user,page);
        }   // CHEER
        else if (!strcmp(buf,cheer_qr)) {
            cin >> user;
            cout << setprecision(6) << proc.ProcessCheer(user) << '\n';
        }
    }
}
