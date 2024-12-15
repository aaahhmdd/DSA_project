// author : hussam & mohamed 

#include <iostream> // Add any other necessary includes
using namespace std;

bool Network_Analysis::temp_is_dummy(string &temp) {
    bool is_dummy = 1;
    for (auto ch: temp)
        is_dummy = is_dummy && (ch == '\n' || ch == ' ');

    return is_dummy;
}

string Network_Analysis::trim(string&x) {
    string y = "";
    int start = 0;
    for (char c: x) {
        if (c != '\n' && c != ' ')
            break;

        start++;
    }
    int end = x.size() - 1;
    for (int i = x.size() - 1; i >= 0; i--) {
        if (x[i] != '\n' && x[i] != ' ')
            break;

        end--;
    }

    for (int i = start; i <= end; i++) {
        y += x[i];
    }
    return y;
}

vector<string> Network_Analysis::divide_string_for_graph(string &file) {
    vector<string> divided_file;
    string temp = "";
    bool start = true;
    bool tag;
    int index = 0;
    while (index < file.size()) {
        if (start) {
            if (file[index] == '<')
                tag = true;
            else
                tag = false;

            start = false;
        }
        temp += file[index];
        if (tag) {
            if (file[index] == '>') {
                divided_file.push_back(temp);
                temp = "";
                start = true;
            }
        }
        else {
            if (file[index + 1] == '<') {
                if (!temp_is_dummy(temp)) {
                    divided_file.push_back(trim(temp));
                }
                temp = "";
                start = true;
            }
        }
        index++;
    }
    return divided_file;
}

Graph Network_Analysis::parse(string &file) {
    Graph gg;
    vector<string> divided_file = divide_string_for_graph(file);
    User* obj_ptr;
    Post* post_ptr;
    vector<string> topics;
    vector<Post> posts;
    bool inside_followers = false;
    for(int i=0;i<divided_file.size();i++){
        string s = divided_file[i];
        if(s=="<user>")
            obj_ptr = new User;


        if(s=="<id>") {
            if (!inside_followers)
                obj_ptr->set_user_id(stoi(divided_file[i + 1]));
            else {
                gg.add_follower(obj_ptr->get_user_id(), stoi(divided_file[i + 1]));
            }
        }

        if(s=="<name>")
            obj_ptr->set_name(divided_file[i+1]);

        if(s == "<followers>")
            inside_followers = true;

        if(s == "</followers>")
            inside_followers = false;

        if(s=="<post>"){
            post_ptr = new Post;
            if(divided_file[i+1]!="<body>")
                post_ptr->set_body(divided_file[i+1]);
        }

        if(s=="<body>")
            post_ptr->set_body(divided_file[i+1]);
        if(s=="<topic>")
            topics.push_back(divided_file[i+1]);


        if(s=="</post>"){
            posts.push_back(*post_ptr);
            delete post_ptr;
        }

        if(s=="</topics>"){
            post_ptr->set_topics(topics);
            topics.clear();
        }


        if(s=="</posts>") {
            obj_ptr->set_posts(posts);
            posts.clear();
        }

        if(s=="</user>"){
            gg.add_user(obj_ptr->get_user_id(),*obj_ptr);
            delete obj_ptr;
        }
    }
    g = gg;
    return g;
}

void Network_Analysis::UsingParse(string file){
    g=parse(file);
}


string convertToPath(string x) {
    string new_x = "";
    bool in = false;
    bool need = false;
    for (int i = 0; i < x.size(); i++) {
        if (x[i] == '\\') {
            if (need)
                new_x += '\"';

            new_x += '\\';

            int j = i + 1;
            bool space = false;
            while (j < x.size() && x[j] != '\\') {
                if (x[j] == ' ')
                    space = true;

                j++;
            }
            if (space) {
                new_x += '\"';
                need = true;
            } else
                need = false;
        } else
            new_x += x[i];
    }
    return new_x;
}

string Network_Analysis::visualize_graph() {
    string s = "";
    s += "digraph test{\n";

    map<int, User> users = g.get_users();
    map<int, vector<int>> followersOfUser = g.get_followersOfUsers();
    for (auto x: users) {
        s+= "\"";
        s += x.second.get_name();
        s+="\"";
        s += "  ->  { ";
        int num = 0;
        for (auto id: followersOfUser[x.first]) {
            s += "\"";
            s += users[id].get_name();
            s += "\"";
            if (num != followersOfUser[x.first].size() - 1)
                s += " ";

            num++;
        }
        s += " }\n";
    }

    s += "\n}";
    filesystem::path currentPath = filesystem::current_path();
    filesystem::path filePath = currentPath / "graph.dot";

    File::outputFile(filePath.string(), s);
    system((convertToPath(currentPath.string()) + "\\graphlib\\bin\\dot -Tpng -O graph.dot").c_str());

    return (currentPath /"graph.dot.png").string();
}



string Network_Analysis::most_followers() {
    vector<User> users = g.most_followers();
    string s = "";
    int i = 1;
    for (auto &u: users) {
        s += to_string(i) + ") " + "User with id : " + to_string(u.get_user_id()) + " and name : ";
        s += u.get_name();
        s += '\n';
        i++;
    }
    return s;
}

string Network_Analysis::most_connections() {
    vector<User> users = g.most_connections();
    string s = "";
    int i = 1;
    for (auto &u: users) {
        s += to_string(i) + ") " + "User with id : " + to_string(u.get_user_id()) + " and name : ";
        s += u.get_name();
        s += '\n';
        i++;
    }
    return s;
}

string Network_Analysis::mutual_followers(int user1_id, int user2_id) {
    User user1 = g.get_user_by_id(user1_id);
    User user2 = g.get_user_by_id(user2_id);
    vector<User> users = g.mutual_followers(user1,user2);
    string s = "";
    if(users.empty())
        return "There are no mutual followers";
    int i = 1;
    set<int>ss;
    for (auto &u: users) {

        if (!ss.count(u.get_user_id())) {
            s += to_string(i) + ") " + "User with id : " + to_string(u.get_user_id()) + " and name : ";
            s += u.get_name();
            s += '\n';
            i++;
        }

        ss.insert(u.get_user_id());
    }
    return s;
}
string Network_Analysis::user_suggestion(int user_id) {
    User user = g.get_user_by_id(user_id);
    string s = "";
    vector<User> users = g.user_suggestion(user);
    if(users.empty())
        return "There are no suggestions";
    int i = 1;
    set<int>ss;
    for (auto &u: users) {

        if (!ss.count(u.get_user_id())) {
            s += to_string(i) + ") " + "User with id : " + to_string(u.get_user_id()) + " and name : ";
            s += u.get_name();
            s += '\n';
            i++;
        }

        ss.insert(u.get_user_id());
    }
    return s;
}

string Network_Analysis::post_search(string word) {
    vector<User> users = g.Search_in_post(word);
    string s = "";
    int i = 1;
    for (auto &u: users) {
        s += to_string(i) + ") " + "User with id : " + to_string(u.get_user_id()) + " and name : ";
        s += u.get_name();
        s += " posted about the word ( " + word + " ).";
        s += '\n';
        i++;
    }
    return s;
}