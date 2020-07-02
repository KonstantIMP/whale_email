#include <iostream>
#include "smtp.hpp"

using namespace std;

static const char *b[] = {"abcdefg", nullptr};

int main()
{
    smtp a;
    a.set_sender("KonstantinFromIT@yandex.ru Konstantin");
    a.set_passwd("Zebra30z2004");
    a.set_recipient("Kuzka30z@yandex.ru");
    a.set_msg_theme("Olga is marry");

    a.add_cc_recipient("mihedovkos@gmail.com");
    a.add_bcc_recipient("hixat46411@kartk5.com");

    std::list<std::string> f;
    f.push_back("a.txt");

    const char**cpp;

    //for(cpp = b; *cpp; cpp++) std::cout << *cpp;

    if(a.sent_email("I will go to your party, but i love you", f, true)) std::cout << "Yes";

    return 0;
}
