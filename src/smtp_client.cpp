#include "../include/smtp_client.hpp"

smtp_client::smtp_client() : smtp_server(""), sender_email(""), sender_password(""), reply_to_email(""),
    recipient_email(""), message_subject(""), cc_recipient(), bcc_recipient(), fake_recipient() {}

smtp_client::smtp_client(const smtp_client & copy) {
    smtp_server = copy.smtp_server;

    sender_email = copy.sender_email;
    sender_password = copy.sender_password;

    reply_to_email = copy.reply_to_email;

    recipient_email = copy.recipient_email;

    message_subject = copy.message_subject;

    for(auto iter : copy.cc_recipient) cc_recipient.push_back(iter);
    for(auto iter : copy.bcc_recipient) bcc_recipient.push_back(iter);
    for(auto iter : copy.fake_recipient) fake_recipient.push_back(iter);
}

smtp_client::smtp_client(const std::string & sender, const std::string & password, const std::string & smtp) :
                                    smtp_server(""), sender_email(""), sender_password(""), reply_to_email(""), recipient_email(""),
                                    message_subject(""), cc_recipient(), bcc_recipient(), fake_recipient() {
    if(!smtp_email_validate(sender)) throw smtp_err("Can't validate senders email adress");
    if(smtp != "") {
        if(!smtp_email_validate(smtp)) throw smtp_err("Can't validate senders SMTP server(Don't write IP or MAC adress, use domain name)");
        smtp_server = smtp;
    }
    else smtp_server = smtp_get_smtp(sender);
    sender_password = password;
    sender_email = sender;
}

smtp_client::~smtp_client() {
    clear_all();
}

void smtp_client::clear_all() {
    clear_sender();

    clear_recipient();
    clear_cc_recipient();
    clear_bcc_recipient();
    clear_fake_recipient();

    clear_additional();
}

void smtp_client::clear_sender() {
    smtp_server = "";
    sender_email = "";
    sender_password = "";
}

void smtp_client::clear_recipient() {
    recipient_email = "";
}

void smtp_client::clear_cc_recipient() {
    cc_recipient.erase(cc_recipient.begin(), cc_recipient.end());
}

void smtp_client::clear_bcc_recipient() {
    bcc_recipient.erase(bcc_recipient.begin(), bcc_recipient.end());
}

void smtp_client::clear_fake_recipient() {
    fake_recipient.erase(fake_recipient.begin(), fake_recipient.end());
}

void smtp_client::clear_additional() {
    reply_to_email = "";
    message_subject = "";
}

void smtp_client::set_smtp_server(const std::string & smtp) {
    if(!smtp_email_validate(smtp)) throw smtp_err("Can't validate senders SMTP server(Don't write IP or MAC adress, use domain name)");
    smtp_server = smtp;
}

std::string smtp_client::get_smtp_server() const {
    return smtp_server;
}

void smtp_client::set_sender_email(const std::string & sender) {
    if(!smtp_email_validate(sender)) throw smtp_err("Can't validate senders email adress");
    sender_email = sender;
}

std::string smtp_client::get_sender_email() const {
    return sender_email;
}

void smtp_client::set_sender_password(const std::string & password) {
    if(password == "") throw smtp_err("Password can\'t be empty");
    sender_password = password;
}

std::string smtp_client::get_sender_password() const {
    return sender_password;
}

void smtp_client::set_email_to_reply(const std::string & reply) {
    reply_to_email = reply;
}

std::string smtp_client::get_email_to_reply() const {
    return reply_to_email;
}

void smtp_client::set_recipient_email(const std::string & recipient) {
    if(!smtp_email_validate(recipient)) throw smtp_err("Can't validate recipients email adress");
    recipient_email = recipient;
}

std::string smtp_client::get_recipient_email() const {
    return recipient_email;
}

void smtp_client::set_message_subject(const std::string & subject) {
    message_subject = subject;
}

std::string smtp_client::get_message_subject() const {
    return message_subject;
}


std::size_t smtp_client::get_cc_recipient_num() const {
    return cc_recipient.size();
}

void smtp_client::add_cc_recipient(const std::string & cc) {
    if(!smtp_email_validate(cc)) throw smtp_err("Can't validate " + cc);
    cc_recipient.push_back(cc);
}

std::string smtp_client::get_cc_recipient(const std::size_t & num) const {
    if(num >= cc_recipient.size()) throw smtp_err("CC_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = cc_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    return * iter;
}

void smtp_client::remove_cc_recipient(const std::size_t & num) {
    if(num >= cc_recipient.size()) throw smtp_err("CC_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = cc_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    cc_recipient.erase(iter);
}

void smtp_client::remove_cc_recipient(const std::string & cc) {
    auto iter = cc_recipient.begin();
    for(std::size_t i{0}; i < cc_recipient.size(); i++) {
        if((*iter) == cc) {
            cc_recipient.erase(iter);
            return;
        }
        iter++;
    }
    throw smtp_err("CC_RECIPIENTS list doesnt conatin \'" + cc + '\'');
}

std::list<std::string> & smtp_client::get_cc_recipient_list() const {
    return const_cast<std::list<std::string> &>(cc_recipient);
}


std::size_t smtp_client::get_bcc_recipient_num() const {
    return bcc_recipient.size();
}

void smtp_client::add_bcc_recipient(const std::string & bcc) {
    if(!smtp_email_validate(bcc)) throw smtp_err("Can't validate " + bcc);
    bcc_recipient.push_back(bcc);
}

std::string smtp_client::get_bcc_recipient(const std::size_t & num) const {
    if(num >= bcc_recipient.size()) throw smtp_err("BCC_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = bcc_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    return * iter;
}

void smtp_client::remove_bcc_recipient(const std::size_t & num) {
    if(num >= bcc_recipient.size()) throw smtp_err("BCC_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = bcc_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    bcc_recipient.erase(iter);
}

void smtp_client::remove_bcc_recipient(const std::string & bcc) {
    auto iter = bcc_recipient.begin();
    for(std::size_t i{0}; i < bcc_recipient.size(); i++) {
        if((*iter) == bcc) {
            bcc_recipient.erase(iter);
            return;
        }
        iter++;
    }
    throw smtp_err("BCC_RECIPIENTS list doesnt conatin \'" + bcc + '\'');
}

std::list<std::string> & smtp_client::get_bcc_recipient_list() const {
    return const_cast<std::list<std::string> &>(bcc_recipient);
}


std::size_t smtp_client::get_fake_recipient_num() const {
    return fake_recipient.size();
}

void smtp_client::add_fake_recipient(const std::string & fake) {
    fake_recipient.push_back(fake);
}

std::string smtp_client::get_fake_recipient(const std::size_t & num) const {
    if(num >= fake_recipient.size()) throw smtp_err("FAKE_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = fake_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    return * iter;
}

void smtp_client::remove_fake_recipient(const std::size_t & num) {
    if(num >= fake_recipient.size()) throw smtp_err("FAKE_RECIPIENTS list doesn't contain element with " + std::to_string(num) + " index");
    auto iter = fake_recipient.begin();
    for(std::size_t i{0}; i < num; i++) iter++;
    fake_recipient.erase(iter);
}

void smtp_client::remove_fake_recipient(const std::string & fake) {
    auto iter = fake_recipient.begin();
    for(std::size_t i{0}; i < fake_recipient.size(); i++) {
        if((*iter) == fake) {
            fake_recipient.erase(iter);
            return;
        }
        iter++;
    }
    throw smtp_err("FAKE_RECIPIENTS list doesnt conatin \'" + fake + '\'');
}

std::list<std::string> & smtp_client::get_fake_recipient_list() const {
    return const_cast<std::list<std::string> &>(fake_recipient);
}

bool smtp_client::sent_email(const std::string & msg, const std::list<std::string> & attachment, const bool & debug) {
    if(debug) std::clog << "[DEBUG] Message sending start\n\n";

    if(debug) std::clog << "[DEBUG] Message analysis :\n";

    if(debug) std::clog << "\tSender's : ";
    if(sender_email == "") {
        if(debug) std::clog << "UNDEFINED\n";
        throw smtp_err("You need enter senders email adress");
    }
    if(debug) std::clog << sender_email << '\n';

    if(debug) std::clog << "\tSender's password : ";
    if(sender_password == "") {
        if(debug) std::clog << "UNDEFINED\n";
        throw smtp_err("You need enter your email's passowrd");
    }
    if(debug) std::clog << sender_password << '\n';

    if(debug) {
        std::clog << "\tEmail to reply : ";
        if(reply_to_email == "") std::clog << "UNDEFINED\n";
        else std::clog << reply_to_email << '\n';
    }

    if(debug) {
        std::clog << "\tMessage subject : ";
        if(message_subject == "") std::clog << "UNDEFINED\n";
        else std::clog << "\'" << message_subject << "\'\n";
    }

    if(debug) std::clog << "\tRecipient : ";
    if(recipient_email == "") {
        if(debug) std::clog << "UNDEFINED\n";
        throw smtp_err("You need to ender general recipient's email adress");
    }
    if(debug) std::clog << recipient_email << '\n';

    if(debug) {
        auto iter = cc_recipient.begin();
        for(std::size_t i{0}; i < cc_recipient.size(); i++) {
            std::clog << "\tCc_recipien #" << i + 1 << " : " << *iter << '\n';
            iter++;
        }

        iter = bcc_recipient.begin();
        for(std::size_t i{0}; i < bcc_recipient.size(); i++) {
            std::clog << "\tBcc_recipien #" << i + 1 << " : " << *iter << '\n';
            iter++;
        }

        iter = fake_recipient.begin();
        for(std::size_t i{0}; i < fake_recipient.size(); i++) {
            std::clog << "\tFake_recipien #" << i + 1 << " : " << *iter << '\n';
            iter++;
        }

        std::clog << '\n';
    }

    if(message_subject == "") std::clog << "[WARNING] Messages without subject can be a spam\n\n";

    if(debug) std::clog << "[DEBUG] Messages length is " << msg.length() << " characters\n\n";

    if(debug) std::clog << "[DEBUG] Attachments analysis\n";

    if(attachment.size() == 0) {
        if(debug) std::clog << "\tThe message hasn't got attachments\n\n";
    }
    else {
        std::ifstream fin; auto iter = attachment.begin();
        for(std::size_t i{0}; i < attachment.size(); i++) {
            if(debug) std::clog << "\t#" << i + 1 << ' ' << *iter;

            fin.open(*iter);

            if(fin.is_open()) {
                if(debug) std::clog << " exist\n";
            }
            else {
                if(debug) std::clog << " doesn't exist\n";
                throw smtp_err("\'" + *iter + "\' attachment doesn't exist");
            }

            fin.close();

            iter++;
        }
        if(debug) std::clog << ' ';
    }

    std::string smtp_header = "", tmp = "";

    if(debug) std::clog << "[DEBUG] SMTP header building\n";

    tmp = "Date: " + smtp_get_time() + "\r\n"; smtp_header += tmp;
    if(debug) std::clog << '\t' << tmp;

    tmp = "To : <" + recipient_email + ">\r\n"; smtp_header += tmp;
    if(debug) std::clog << '\t' << tmp;

    tmp = "";

    for(std::size_t i{0}; i < sender_email.length(); i++) {
        if(sender_email.at(i) == ' ') break;
        tmp += sender_email.at(i);
    }

    tmp = "From: <" + tmp + '>' + sender_email.substr(tmp.length()) + "\r\n"; smtp_header += tmp;
    if(debug) std::clog << '\t' << tmp;

    auto iter = cc_recipient.begin();
    for(std::size_t i{0}; i < cc_recipient.size(); i++) {
        tmp = "Cc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug) std::clog << '\t' << tmp;
        iter++;
    }

    iter = fake_recipient.begin();
    for(std::size_t i{0}; i < fake_recipient.size(); i++) {
        tmp = "Cc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug) std::clog << '\t' << tmp;
        iter++;
    }

    iter = bcc_recipient.begin();
    for(std::size_t i{0}; i < bcc_recipient.size(); i++) {
        tmp = "Bcc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug) std::clog << '\t' << tmp;
        iter++;
    }

    tmp = "Message-ID: <" + smtp_gen_msg_id() + ">\r\n"; smtp_header += tmp;
    if(debug) std::clog << '\t' << tmp;

    tmp = "Subject: " + message_subject + "\r\n"; smtp_header += tmp;
    if(debug) std::clog << '\t' << tmp;

    if(reply_to_email != "") {
        tmp = "Reply-To: <" + reply_to_email + ">\r\n"; smtp_header += tmp;
        if(debug) std::clog << '\t' << tmp;
    }

    if(debug) std::clog << "\n[DEBUG] Start work with CURL\n\n";

    CURL * email_msg;

    if(debug) std::clog << "[DEBUG] CURL init : ";

    email_msg = curl_easy_init();

    if(email_msg) {
        if(debug) std::clog << "DONE\n\n";

        curl_mimepart * part = nullptr;

        curl_slist * header = nullptr;
        curl_slist * slist = nullptr;
        curl_slist * rec = nullptr;

        curl_mime * mime = nullptr;
        curl_mime * alt = nullptr;

        CURLcode err = CURLE_OK;

        if(debug) std::clog << "[DEBUG] Login to SMTP server " << smtp_get_login(sender_email) << "\n\n";

        curl_easy_setopt(email_msg, CURLOPT_USERNAME, smtp_get_login(sender_email).c_str());
        curl_easy_setopt(email_msg, CURLOPT_PASSWORD, sender_password.c_str());

        if(debug) std::clog << "[DEBUG] SMTP server is " << smtp_get_smtp(sender_email) << "\n\n";

        curl_easy_setopt(email_msg, CURLOPT_URL, smtp_get_smtp(sender_email).c_str());

        #ifdef  SKIP_PEER_VERIFICATION
            if(debug) std::clog << "[DEBUG] Skip peer verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYPEER , 0L);
        #endif

        #ifdef  SKIP_HOSTNAME_VERIFICATION
            if(debug) std::clog << "[DEBUG] Skip hostname verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYHOST , 0L);
        #endif

        tmp = "";

        for(std::size_t i{0}; i < sender_email.length(); i++) {
            if(sender_email.at(i) == ' ') break;
            tmp += sender_email.at(i);
        }

        if(debug) std::clog << "[DEBUG] Sender's email " << tmp << "\n\n";

        curl_easy_setopt(email_msg, CURLOPT_MAIL_FROM, tmp.c_str());

        tmp = '<' + recipient_email + '>';
        rec = curl_slist_append(rec, tmp.c_str());

        iter = cc_recipient.begin();
        for(std::size_t i{0}; i < cc_recipient.size(); i++) {
            tmp = '<' + *iter + '>';
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        iter = bcc_recipient.begin();
        for(std::size_t i{0}; i < bcc_recipient.size(); i++) {
            tmp = '<' + *iter + '>';
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        curl_easy_setopt(email_msg, CURLOPT_MAIL_RCPT, rec);

        for(std::size_t i{0}; i < smtp_header.length(); i++) header = curl_slist_append(header, &smtp_header.at(i));

        curl_easy_setopt(email_msg, CURLOPT_HTTPHEADER, header);

        if(debug) std::clog << "[DEBUG] Mime init\n\n";

        mime = curl_mime_init(email_msg);
        alt = curl_mime_init(email_msg);

        part = curl_mime_addpart(alt);
        curl_mime_data(part, msg.c_str(), CURL_ZERO_TERMINATED);

        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");

        slist = curl_slist_append(nullptr, "Content-Disposition: inline");
        curl_mime_headers(part, header, 1);

        if(attachment.size() != 0) {
            auto a_iter = attachment.begin();

            for(std::size_t i{0}; i < attachment.size(); i++) {
                part = curl_mime_addpart(mime);
                curl_mime_filedata(part, (*a_iter).c_str());

                curl_easy_setopt(email_msg, CURLOPT_MIMEPOST, mime);

                a_iter++;
            }
        }
        else curl_easy_setopt(email_msg, CURLOPT_MIMEPOST, mime);

        if(debug) {
            std::clog << "[DEBUG] CURL debug is ON\n\n";
            curl_easy_setopt(email_msg, CURLOPT_VERBOSE, 1L);
        }

        if(debug) std::clog << "[DEBUG] CURL_DEBUG ____________________\n\n";

        err = curl_easy_perform(email_msg);

        if(debug) std::clog << "\n[DEBUG] CURL_DEBUG ____________________\n\n";

        if(err != CURLE_OK) {
            if(debug) std::clog << "[DEBUG] CURL error : " << err << "\n\n";
            throw smtp_err("CURL sending error : " + std::string(curl_easy_strerror(err)));
        }

        if(debug) std::clog << "[DEBUG] Memory free\n";

        curl_slist_free_all(rec);
        curl_slist_free_all(header);

        curl_easy_cleanup(email_msg);
    }
    else {
        if(debug) std::clog << "ERROR\n\n";
        throw smtp_err("Can't init CURL. Check your internet connection");
    }

    return true;
}

std::string smtp_client::smtp_get_time() const {
    std::time_t t = time(nullptr);

    std::string buf = ctime(&t), smtp_time = buf.substr(0, 3) + ", ";

    smtp_time += buf.substr(11, 2);

    if(smtp_time.at(smtp_time.length() - 1) != ' ') smtp_time += ' ';

    smtp_time += buf.substr(4,3) + " " + buf.substr(20,4) + " " + buf.substr(11,8);

    return smtp_time;
}

std::string smtp_client::smtp_gen_msg_id() const {
    std::string msg_id = smtp_get_time();

    while(msg_id.find(' ') != std::string::npos) msg_id.erase(msg_id.find(' '));
    while(msg_id.find(',') != std::string::npos) msg_id.erase(msg_id.find(','));
    while(msg_id.find(':') != std::string::npos) msg_id.erase(msg_id.find(':'));

    msg_id += "ht80ew"; srand(static_cast<unsigned int>(time(nullptr)));

    for(std::size_t i{0}; i < 10; i++) std::random_shuffle(msg_id.begin(), msg_id.end());

    return msg_id + static_cast<char>((rand() % 24) + 66) + "@zosima.com";
}

std::string smtp_client::smtp_get_smtp(const std::string & adress) const {
    std::string tmp = "";
    for(std::size_t i{0}; i < adress.length(); i++) {
        if(adress.at(i) != ' ') tmp += adress.at(i);
        else break;
    }

    if(!smtp_email_validate(tmp)) throw smtp_err("Can't validate senders SMTP server. You need enter email adress before name!");

    if(tmp.find("@yandex.ru") != std::string::npos) return "smtps://smtp.yandex.ru";
    if(tmp.find("@gmail.com") != std::string::npos) return "smtps://smtp.gmail.com";
    if(tmp.find("@google.com") != std::string::npos) return "smtps://smtp.gmail.com";
    if(tmp.find("@rambler.ru") != std::string::npos) return "smtps://smtp.rambler.ru";
    if(tmp.find("@mail.ru") != std::string::npos) return "smtps://smtp.mail.ru";
    if(tmp.find("@outlook.com") != std::string::npos) return "smtps://smtp-mail.outlook.com";

    throw smtp_err("Can't find suitable SMTP server in the base");
}

std::string smtp_client::smtp_get_login(const std::string & adress) const {
    std::string tmp = "";
    for(std::size_t i{0}; i < adress.length(); i++) {
        if(adress.at(i) != ' ') tmp += adress.at(i);
        else break;
    }

    if(!smtp_email_validate(tmp)) throw smtp_err("Can't validate senders email adress");

    if(tmp.substr(tmp.length() - 10) == "@yandex.ru") return tmp.substr(0, tmp.length() - 10);

    return tmp;
}

bool smtp_client::smtp_email_validate(const std::string & adress) const {
    if(adress.find('@') == std::string::npos) return false;

    std::string tmp = "";

    for(std::size_t i{0}; i < adress.length(); i++) {
        if(adress.at(i) == ' ') break;
        tmp += adress.at(i);
    }

    std::size_t buf = 0;
    for(std::size_t i{0}; i < tmp.length(); i++) {
        if(tmp.at(i) != '@') buf++;
        else break;
    }

    if(buf == 0) return false;

    tmp = tmp.substr(buf + 1);

    if(tmp.length() == 0) return false;

    for(std::size_t i{0}; i < tmp.length(); i++) {
        if(tmp.at(i) == '.') {
            if(tmp.substr(i + 1).length() == 0) return false;
        }
    }

    return true;
}

