#include "whale_email.hpp"
#include "smtp_client.hpp"

int main(int argc, char * argv[]) {
    print_hello(std::cout);

    if(argc > 1) {
        for(std::size_t i{1}; i < static_cast<std::size_t>(argc); i++) {
            if(std::string(argv[i]) == "-h") {
                print_help(std::cout);
                return 0;
            }
        }
        std::cout << "[ERROR #10] Incorrect command line arguments\n";
        std::cout << "\twhat () : WHALE can recieve only \'-h\' argument and it doesn\'t able to find it\n";
        return 10;
    }

    return 0;
}
