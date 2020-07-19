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

    smtp_client email;

    std::string tmp_data = "";

    std::cout << "  Hello! I see You want sent an email. Let\'s start :\n\n";

    std::cout << "  \tEnter your email adress : ";
    std::getline(std::cin, tmp_data);

    try {
        email.set_sender_email(tmp_data);
    }  catch (smtp_err & e) {
        std::cout << "  \t[ERROR #140] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
        return 140;
    }

    std::cout << "  \tHow do you want to introduce yourself : ";
    std::getline(std::cin, tmp_data);

    if(tmp_data != "") {
        try {
            email.set_sender_email(email.get_sender_email() + " " + tmp_data);
        }  catch (smtp_err & e) {
            std::cout << "  \t[ERROR #141] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
            return 141;
        }
    }
    else std::cout << "\n  \tOK. It was, is and will be a secret\n\n";

    std::cout << "  \tEnter your\'s email password : ";
    std::getline(std::cin, tmp_data);

    try {
        email.set_sender_password(tmp_data);
    }  catch (smtp_err & e) {
        std::cout << "  \t[ERROR #142] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
        return 142;
    }

    std::cout << "\n  \tEnter message\'s subject : ";
    std::getline(std::cin, tmp_data);

    if(tmp_data == "") std::cout << "  \t[WARNING] Messages without subject can be a SPAM\n";
    else email.set_message_subject(tmp_data);

    std::cout << "\n  \tEnter email adress to reply : ";
    std::getline(std::cin, tmp_data);

    if(tmp_data != "") {
        try {
            email.set_email_to_reply(tmp_data);
        }  catch (smtp_err & e) {
            std::cout << "  \t[ERROR #143] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
            return 143;
        }
    }
    else std::cout << "\n  \tOK. It will be " << email.get_sender_email() << '\n';

    std::cout << "\n  \tEnter recipient's email adress : ";
    std::getline(std::cin, tmp_data);

    try {
        email.set_recipient_email(tmp_data);
    }  catch (smtp_err & e) {
        std::cout << "  \t[ERROR #144] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
        return 144;
    }

    std::cout << '\n';

    while (1) {
        std::cout << "  \tAdd cc_recipient : ";
        std::getline(std::cin, tmp_data);

        if(tmp_data != "") {
            try {
                email.add_cc_recipient(tmp_data);
            }  catch (smtp_err & e) {
                std::cout << "  \t[ERROR #145] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
                return 145;
            }
        }
        else break;
    }

    std::cout << '\n';

    while (1) {
        std::cout << "  \tAdd bcc_recipient : ";
        std::getline(std::cin, tmp_data);

        if(tmp_data != "") {
            try {
                email.add_bcc_recipient(tmp_data);
            }  catch (smtp_err & e) {
                std::cout << "  \t[ERROR #146] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
                return 146;
            }
        }
        else break;
    }

    std::cout << '\n';

    while (1) {
        std::cout << "  \tAdd fake_recipient : ";
        std::getline(std::cin, tmp_data);

        if(tmp_data != "") {
            try {
                email.add_fake_recipient(tmp_data);
            }  catch (smtp_err & e) {
                std::cout << "  \t[ERROR #147] Incorrect input or runtime error\n  \t\twhat () : " << e.what() << '\n';
                return 147;
            }
        }
        else break;
    }

    return 0;
}
