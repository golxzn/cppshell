#include "deps.hpp"

#include "permissions.hpp"

namespace security {

bool Permissions::read() const {
    return perms[p_read_position];
}

bool Permissions::write() const {
    return perms[p_write_position];
}

bool Permissions::execute() const {
    return perms[p_execute_position];
}

void Permissions::read(bool allow) {
    perms.set(p_read_position, allow);
}

void Permissions::write(bool allow) {
    perms.set(p_write_position, allow);
}

void Permissions::execute(bool allow) {
    perms.set(p_execute_position, allow);
}

Permissions::operator std::string() const {
    std::string strperms('-', 3);
    if(read()) {
        strperms[0] = 'r';
    }
    if(write()) {
        strperms[0] = 'w';
    }
    if(execute()) {
        strperms[0] = 'e';
    }
    return strperms;
}

} // namespace security

namespace std {

string to_string(const security::Permissions &permissions) {
    return std::string(permissions);
}

ostream &operator<<(ostream &out, const security::Permissions &permissions) {
    return (out << std::string(permissions));
}

} // namespace std