#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <set>
#include "Post.h"  // Include Post class

class User {
public:
    int id;                          // Unique ID of the user
    std::string name;                // Name of the user
    std::vector<Post> posts;         // List of posts made by the user
    std::set<int> followers;         // Set of follower IDs
    std::set<int> following;         // Set of IDs the user is following

    void add_post(const Post& post);         // Add a post to the user's list of posts
    void add_follower(int follower_id);     // Add a follower to the user's followers set
    void add_following(int following_id);   // Add a user to the current user's following set
};

#endif
