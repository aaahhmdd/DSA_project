#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <queue>
#include <algorithm>

// Class to represent a Post made by a user
class Post {
public:
    std::string body;                // The content of the post
    std::vector<std::string> topics; // List of topics associated with the post

    // Set the body of the post
    void set_body(const std::string& b) { body = b; }

    // Add a topic to the post
    void add_topic(const std::string& t) { topics.push_back(t); }
};

// Class to represent a User in the social network
class User {
public:
    int id;                          // Unique ID of the user
    std::string name;                // Name of the user
    std::vector<Post> posts;         // List of posts made by the user
    std::set<int> followers;         // Set of follower IDs (ensures uniqueness)
    std::set<int> following;         // Set of IDs the user is following

    // Add a post to the user's list of posts
    void add_post(const Post& post) { posts.push_back(post); }

    // Add a follower to the user's followers set
    void add_follower(int follower_id) { followers.insert(follower_id); }

    // Add a user the current user is following
    void add_following(int following_id) { following.insert(following_id); }
};

// Class to analyze and manage the social network data
class NetworkAnalysis {
private:
    std::map<int, User> users;       // Map of user IDs to User objects

public:
    void parse_xml(const std::string& filepath);          // Parse XML file and populate user data
    void display_analysis();                             // Display analysis: most influential and active users
    void mutual_followers(const std::vector<int>& user_ids); // Find mutual followers among users
    void suggest_users_to_follow(int user_id);           // Suggest users for a given user to follow
    void search_posts_by_word(const std::string& word);  // Search for posts containing a specific word
    void most_active_user();                             // Find the most active user (most connections)

    // Utility function to extract values between XML tags
    std::string extract_value(const std::string& data, const std::string& open_tag, const std::string& close_tag);
};

// Function to extract a value between XML tags
std::string NetworkAnalysis::extract_value(const std::string& data, const std::string& open_tag, const std::string& close_tag) {
    size_t start = data.find(open_tag);
    if (start == std::string::npos) return "";
    start += open_tag.length();
    size_t end = data.find(close_tag, start);
    if (end == std::string::npos) return "";
    return data.substr(start, end - start);
}

// Function to parse the XML file and extract user, post, and follower data
void NetworkAnalysis::parse_xml(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error opening file." << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();   // Read the file into a string stream
    std::string xml_data = buffer.str();

    size_t user_pos = 0;
    while ((user_pos = xml_data.find("<user>", user_pos)) != std::string::npos) {
        size_t user_end = xml_data.find("</user>", user_pos);
        std::string user_block = xml_data.substr(user_pos, user_end - user_pos);

        User user;
        user.id = std::stoi(extract_value(user_block, "<id>", "</id>"));
        user.name = extract_value(user_block, "<name>", "</name>");

        // Parse posts made by the user
        size_t post_pos = 0;
        while ((post_pos = user_block.find("<post>", post_pos)) != std::string::npos) {
            size_t post_end = user_block.find("</post>", post_pos);
            std::string post_block = user_block.substr(post_pos, post_end - post_pos);

            Post post;
            post.set_body(extract_value(post_block, "<body>", "</body>"));

            // Parse topics within a post
            size_t topic_pos = 0;
            while ((topic_pos = post_block.find("<topic>", topic_pos)) != std::string::npos) {
                size_t topic_end = post_block.find("</topic>", topic_pos);
                std::string topic = post_block.substr(topic_pos + 7, topic_end - topic_pos - 7);
                post.add_topic(topic);
                topic_pos = topic_end;
            }

            user.add_post(post);
            post_pos = post_end;
        }

        // Parse followers for the user
        size_t follower_pos = 0;
        while ((follower_pos = user_block.find("<follower>", follower_pos)) != std::string::npos) {
            size_t follower_end = user_block.find("</follower>", follower_pos);
            int follower_id = std::stoi(extract_value(user_block.substr(follower_pos, follower_end - follower_pos), "<id>", "</id>"));
            user.add_follower(follower_id);
            users[follower_id].add_following(user.id); // Add the following relationship
            follower_pos = follower_end;
        }

        users[user.id] = user; // Add the user to the users map
        user_pos = user_end;
    }
}

// Display the most influential and most active users
void NetworkAnalysis::display_analysis() {
    // Find the most influential user (user with the most followers)
    int max_followers = 0, influencer_id = -1;
    for (const auto& [id, user] : users) {
        if (user.followers.size() > max_followers) {
            max_followers = user.followers.size();
            influencer_id = id;
        }
    }
    std::cout << "Most Influential User: " << users[influencer_id].name << " with " << max_followers << " followers." << std::endl;

    // Find the most active user (most connections)
    most_active_user();
}

// Find and display the most active user (followers + following connections)
void NetworkAnalysis::most_active_user() {
    int max_connections = 0, active_user_id = -1;
    for (const auto& [id, user] : users) {
        int total_connections = user.followers.size() + user.following.size();
        if (total_connections > max_connections) {
            max_connections = total_connections;
            active_user_id = id;
        }
    }
    std::cout << "Most Active User: " << users[active_user_id].name << " with " << max_connections << " connections." << std::endl;
}

// Find mutual followers among a list of users
void NetworkAnalysis::mutual_followers(const std::vector<int>& user_ids) {
    if (user_ids.empty()) return;
    std::set<int> mutual_followers = users[user_ids[0]].followers;

    for (size_t i = 1; i < user_ids.size(); ++i) {
        std::set<int> current_followers = users[user_ids[i]].followers;
        std::set<int> intersection;
        std::set_intersection(mutual_followers.begin(), mutual_followers.end(),
                              current_followers.begin(), current_followers.end(),
                              std::inserter(intersection, intersection.begin()));
        mutual_followers = intersection;
    }

    std::cout << "Mutual Followers: ";
    for (int id : mutual_followers) std::cout << id << " ";
    std::cout << std::endl;
}

// Suggest users for a given user to follow based on mutual followers
void NetworkAnalysis::suggest_users_to_follow(int user_id) {
    if (!users.count(user_id)) return;
    std::set<int> suggestions;
    for (int follower_id : users[user_id].followers) {
        for (int fof : users[follower_id].followers) {
            if (fof != user_id && !users[user_id].followers.count(fof)) {
                suggestions.insert(fof);
            }
        }
    }

    std::cout << "Suggested Users to Follow: ";
    for (int id : suggestions) std::cout << users[id].name << " (ID: " << id << ") ";
    std::cout << std::endl;
}

// Search posts for a specific word or topic
void NetworkAnalysis::search_posts_by_word(const std::string& word) {
    for (const auto& [id, user] : users) {
        for (const auto& post : user.posts) {
            if (post.body.find(word) != std::string::npos ||
                std::find(post.topics.begin(), post.topics.end(), word) != post.topics.end()) {
                std::cout << "User: " << user.name << " -> Post: " << post.body << std::endl;
            }
        }
    }
}

int main() {
    NetworkAnalysis analysis;
    std::string filepath = "sample.xml";

    analysis.parse_xml(filepath);
    analysis.display_analysis();
    analysis.mutual_followers({1, 2, 3});
    analysis.suggest_users_to_follow(1);
    analysis.search_posts_by_word("dolor");

    return 0;
}
