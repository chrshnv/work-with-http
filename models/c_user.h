#ifndef C_USER_H
#define C_USER_H
#include <string>

#include "../utils/json.h"

namespace ns {
    class c_user {
    public:
        c_user() = default;

        c_user(const std::string& username, const std::string& password) {
            this->username = username;
            this->password = password;
        }

        std::string username;
        std::string password;
    };

    inline void to_json(nlohmann::ordered_json &j, const c_user& c) {
        j = { { "username", c.username }, { "password", c.password } };
    }

    inline void from_json(const nlohmann::ordered_json& j, c_user& c) {
        j.at("username").get_to(c.username);
        j.at("password").get_to(c.password);
    }
}

#endif //C_USER_H
