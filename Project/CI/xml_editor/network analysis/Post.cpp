#include "Post.h"

void Post::set_body(const std::string& b) {
    body = b;
}

void Post::add_topic(const std::string& t) {
    topics.push_back(t);
}
