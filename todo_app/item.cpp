#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include <algorithm>

using namespace std;
using namespace chrono;

class Item {
    private:
        string title;
        string description;
        const system_clock::time_point post_date;
        system_clock::time_point complete_by;
        vector<string> flairs;

        bool completed;

    public: 
        Item(const string &tit,
            const system_clock::time_point &complete_by_date,
            const string &desc = "",
             const vector<string>& fl = vector<string>(),
             bool is_completed = false) 
            : title(tit),
            description(desc),
            post_date(system_clock::now()),
            complete_by(complete_by_date),
            flairs(fl),
            completed(is_completed) {}
        
        string getTitle() const { return title; }
        void setTitle(const string &t) { title = t; }

        string getDescription() const { return description; }
        void setDescription(const string &d) { description = d; }

        const system_clock::time_point& getPostDate() const { return post_date; }

        const system_clock::time_point& getCompleteBy() const {return complete_by;}
        void setCompleteBy(const system_clock::time_point& tp) {complete_by = tp;}

        const vector<string>& getFlairs() { return flairs; }
        void addFlair(const string& flair) { flairs.push_back(flair); }
        void removeFlair(const string& flair) {
            flairs.erase(remove(flairs.begin(), flairs.end(), flair), flairs.end());
        }

        void setCompleted(bool c) { completed = c;}
        bool getCompleted() const { return completed; }
    
};