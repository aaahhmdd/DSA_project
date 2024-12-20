#include "User.h"

void User::add_post(const Post& post) {
    posts.push_back(post);
}

void User::add_follower(int follower_id) {
    followers.insert(follower_id);
}

void User::add_following(int following_id) {
    following.insert(following_id);
}
