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

    std::cout << "\n  \tWrite email message (To stop entering tap ENTR twice) : \n";

    std::string msg = "";

    while (1) {
        std::cout << "\t\t";
        std::getline(std::cin, tmp_data);

        if(tmp_data == "") {
            std::cout << "\t\t";
            std::getline(std::cin, tmp_data);
            if(tmp_data == "") break;
            else msg += '\n' + tmp_data;
        }
        else msg += tmp_data + '\n';
    }

    std::cout << "\n  \tEnd message writing\n\n";

    std::list<std::string> attachment_list;

    while (1) {
        std::cout << "  \tAdd attachment : ";
        std::getline(std::cin, tmp_data);
        if(tmp_data != "") {
            try {
                attachment_list.push_back(tmp_data);
            }  catch (std::bad_alloc & e) {
                std::cout << "  \t[ERROR #40] Runtime error\n  \t\twhat () : " << e.what() << '\n';
                return 40;
            } catch (std::runtime_error & e) {
                std::cout << "  \t[ERROR #41] Runtime error\n  \t\twhat () : " << e.what() << '\n';
                return 41;
            }
        }
        else break;
    }

    bool debug;

    std::cout << "\n  \tDou you want see debug output [y/n] : ";
    std::getline(std::cin, tmp_data);

    if(tmp_data == "" || tmp_data == "y" || tmp_data == "Y") debug = true;
    else debug = false;

    std::cout << "\n  OK. Let\'s send this message\n\n";

    try {
        email.sent_email(msg, attachment_list, debug);
    }  catch (smtp_err & e) {
        std::cout << "  \t[ERROR #42] Runtime error\n  \t\twhat () : " << e.what() << '\n';
        return 42;
    }

    return 0;
}
