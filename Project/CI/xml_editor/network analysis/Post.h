#ifndef POST_H
#define POST_H

#include <string>
#include <vector>

class Post {
public:
    std::string body;                // The content of the post
    std::vector<std::string> topics; // List of topics associated with the post

    void set_body(const std::string& b);         // Set the body of the post
    void add_topic(const std::string& t);        // Add a topic to the post
};

#endif
