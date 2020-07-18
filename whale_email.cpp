#include "whale_email.hpp"
#include "defines.hpp"

void print_hello(std::ostream & os) {
    os << "                                                                        \n";
    os << "            _             _                                      _    _ \n";
    os << "   _ _ _   | |_    ___   | |   ___        ___    _____    ___   |_|  | |\n";
    os << "  | | | |  |   |  | .'|  | |  | -_|      | -_|  |     |  | .'|  | |  | |\n";
    os << "  |_____|  |_|_|  |__,|  |_|  |___|      |___|  |_|_|_|  |__,|  |_|  |_|\n";
    os << "                                                                      \n";
    os << "  Version : " << str_version << "\t\t\t         .\n";
    os << "  \t\t\t\t\t             \":\"\n";
    os << "  Developer : KonstantIMP\t\t           ___:____     |\"\\/\"|\n";
    os << "  Feedback : mihedovkos@gmail.com\t         ,'        `.    \\  /\n";
    os << "  \t\t\t\t\t         |  O        \\___/  |\n";
    os << "  Use 'whale -h' to see help\t\t       ~^~^~^~^~^~^~^~^~^~^~^~^~\n\n";
}

void print_help(std::ostream & os) {
    os << "  WHALE : version " << str_version << "\n\n";

    os << "  whale is an application to invisible email sending\n\n";

    os << "  HELP MANUAL :\n\n";
    os << "  [USAGE]\n  \twhale [\'-h\' if you want to see this message]\n\n";

    os << "  \twhale uses stdin to get all usefull info for work\n\n";
}
