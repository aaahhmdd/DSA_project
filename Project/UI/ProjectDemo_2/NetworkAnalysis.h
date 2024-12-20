#ifndef NETWORK_ANALYSIS_H
#define NETWORK_ANALYSIS_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include "User.h"

class NetworkAnalysis {
private:
    std::map<int, User> users;       // Map of user IDs to User objects

public:
    void parse_xml(const std::string& filepath);          // Parse XML file and populate user data
    std::string display_analysis();                             // Display analysis: most influential and active users
    std::string mutual_followers(const std::vector<int>& user_ids); // Find mutual followers among users
    std::string suggest_users_to_follow(int user_id);           // Suggest users for a given user to follow
    std::string search_posts_by_word(const std::string& word);  // Search for posts containing a specific word
    std::string most_active_user();                             // Find the most active user (most connections)

    // Utility function to extract values between XML tags
    std::string extract_value(const std::string& data, const std::string& open_tag, const std::string& close_tag);
};

#endif // NETWORK_ANALYSIS_H
