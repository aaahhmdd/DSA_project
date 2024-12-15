class Network_Analysis {
private:

    static bool temp_is_dummy(std::string &temp);
    static std::string trim(std::string&x);
    static std::vector<std::string> divide_string_for_graph(std::string &file);
    static Graph parse(std::string &file);

public:

    static std::string visualize_graph();

    static std::string most_followers();

    static std::string most_connections();

    static std::string mutual_followers(int user1_id, int user2_id) ;

    static std::string user_suggestion(int user_id);

    static std::string post_search(std::string word);

    static void UsingParse(std::string file);
};
