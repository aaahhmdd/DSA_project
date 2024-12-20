#ifndef NETWORK_ANALYSIS_H
#define NETWORK_ANALYSIS_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <algorithm>
#include "User.h"  // Include User class

class NetworkAnalysis {
private:
    std::map<int, User> users;       // Map of user IDs to User objects

public:
    void parse_xml(const std::string& filepath);
    void display_analysis();
    void mutual_followers(const std::vector<int>& user_ids);
    void suggest_users_to_follow(int user_id);
    void search_posts_by_word(const std::string& word);
    void most_active_user();

    std::string extract_value(const std::string& data, const std::string& open_tag, const std::string& close_tag);
};

#endif
