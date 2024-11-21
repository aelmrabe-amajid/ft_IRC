#include "ErrorLog.hpp"

int main() {
    Message::setErrorsDatabase();

    std::string errorMsg = Message::getError("john", 431);
    std::cout << errorMsg;

    std::string joinErrorMsg = Message::getJoinError("john", "#channel", 403);
    std::cout << joinErrorMsg;

    return 0;
}