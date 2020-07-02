#include "smtp.hpp"

std::string buf;

smtp::smtp() : sender(""), passwd(""), msg_theme(""), recipient ("") {}

smtp::~smtp() {
    clear();
}

void smtp::clear() {
    buf = sender = passwd = recipient = msg_theme = "";

    cc_recipients.clear();
    bcc_recipients.clear();
}

void smtp::set_sender(const std::string & name) {
    sender = name;
}

inline std::string smtp::get_sender() const {
    return sender;
}

void smtp::set_passwd(const std::string & pas) {
    passwd = pas;
}

inline std::string smtp::get_passwd() const {
    return passwd;
}

void smtp::set_msg_theme(const std::string & subject) {
    msg_theme = subject;
}

inline std::string smtp::get_msg_theme() const {
    return msg_theme;
}

void smtp::set_recipient(const std::string & msg_to) {
    recipient = msg_to;
}

inline std::string smtp::get_recipient() const {
    return recipient;
}

bool smtp::add_cc_recipient(const std::string & cc) {
    if(cc_recipients.size() >= cc_recipients.max_size()) return false;
    cc_recipients.push_back(cc); return true;
}

std::string smtp::get_cc_recipient(const std::size_t & num) const {
    if(num >= cc_recipients.size()) throw std::out_of_range("Message don't contain recipient with this num");

    auto iter = cc_recipients.begin();

    for(std::size_t i{0}; i < num; i++) iter++;

    return *iter;
}

bool smtp::remove_cc_recipient(const std::string & cc) {
    auto iter = cc_recipients.begin();

    for(std::size_t i{0}; i < cc_recipients.size(); i++) {
        if(*iter == cc) {
            cc_recipients.erase(iter);
            return true;
        }

        iter++;
    }

    return false;
}

bool smtp::remove_cc_recipient(const std::size_t & num) {
    if(num >= cc_recipients.size()) return false;

    auto iter = cc_recipients.begin();

    for(std::size_t i{0}; i < num; i++) iter++;

    cc_recipients.erase(iter);

    return true;
}

std::size_t smtp::get_cc_recipients_num() const {
    return cc_recipients.size();
}

void smtp::clear_cc_recipients() {
    cc_recipients.clear();
}

bool smtp::add_bcc_recipient(const std::string & bcc) {
    if(bcc_recipients.size() >= bcc_recipients.max_size()) return false;
    bcc_recipients.push_back(bcc); return true;
}

std::string smtp::get_bcc_recipient(const std::size_t & num) const {
    if(num >= bcc_recipients.size()) throw std::out_of_range("Message don't contain recipient with this num");

    auto iter = bcc_recipients.begin();

    for(std::size_t i{0}; i < num; i++) iter++;

    return *iter;
}

bool smtp::remove_bcc_recipient(const std::string & bcc) {
    auto iter = bcc_recipients.begin();

    for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
        if(*iter == bcc) {
            bcc_recipients.erase(iter);
            return true;
        }

        iter++;
    }

    return false;
}

bool smtp::remove_bcc_recipient(const std::size_t & num) {
    if(num >= bcc_recipients.size()) return false;

    auto iter = bcc_recipients.begin();

    for(std::size_t i{0}; i < num; i++) iter++;

    bcc_recipients.erase(iter);

    return true;
}

std::size_t smtp::get_bcc_recipients_num() const {
    return bcc_recipients.size();
}

void smtp::clear_bcc_recipients() {
    bcc_recipients.clear();
}

bool smtp::sent_email(const std::string & msg, const bool & debug_out) {
    if(debug_out) std::clog << "[DEBUG] Start sending a message\n\n";

    if(sender == "") {
        if(debug_out) std::clog << "[DEBUG] The sender is not specified\n";
        throw smtp_err("You can't send an email without the sender");
        return false;
    }

    if(debug_out) std::clog << "[DEBUG] The sender is : " << sender << "\n";

    if(passwd == "") {
        if(debug_out) std::clog << "[DEBUG] The password is not specified\n";
        throw smtp_err("You can't send an email without your email account password");
        return false;
    }

    if(debug_out) {
        std::clog << "[DEBUG] The password is : ";
        for(std::size_t i{0}; i < passwd.length(); i++) std::clog << '*';
        std::clog << "\n\n";
    }

    if(recipient == "") {
        if(debug_out) std::clog << "[DEBUG] The recipient is not specified\n";
        throw smtp_err("You can't send email without main recipient");
        return false;
    }

    if(debug_out) std::clog << "[DEBUG] The recipient is : " << recipient << "\n\n";

    if(debug_out) {
        std::clog << "[DEBUG] CC_RECIPIENTS ANALYSIS :\n";

        auto iter = cc_recipients.begin();

        std::clog << "\tYou have " << cc_recipients.size() << " CC recipients\n";
        for(std::size_t i{0}; i < cc_recipients.size(); i++) {
            std::clog << "\t\t" << i + 1 << " - " << *iter << "\n";
            iter++;
        }

        std::clog << "\n";
    }

    if(debug_out) {
        std::clog << "[DEBUG] BCC_RECIPIENTS ANALYSIS :\n";

        auto iter = bcc_recipients.begin();

        std::clog << "\tYou have " << bcc_recipients.size() << " BCC recipients\n";
        for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
            std::clog << "\t\t" << i + 1 << " - " << *iter << "\n";
            iter++;
        }

        std::clog << "\n";
    }

    if(msg_theme == "") {
        if(debug_out) std::clog << "[WARNING] Messages without subject can be SPAM\n\n";
    }

    if(debug_out) std::clog << "[DEBUG] Subject is : " << msg_theme << "\n\n";

    if(debug_out) std::clog << "[DEBUG] Message text _______________________\n\n" << msg << "\n\n[DEBUG] End text ___________________________\n\n";

    if(debug_out) std::clog << "[DEBUG] Start smtp header build\n\n";

    std::string smtp_header = "" , tmp = "";

    if(debug_out) std::clog << "[DEBUG] SMTP header\n";

    tmp = "Date: " + get_smtp_date() + " \r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "To: <" + recipient + ">\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "";

    for(std::size_t i{0}; i < sender.length(); i++) {
        if(sender.at(i) != ' ') tmp += sender.at(i);
        else break;
    }

    tmp = "From: <" + tmp + ">" + sender.substr(tmp.length()) + "\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    auto iter = cc_recipients.begin();

    for(std::size_t i{0}; i < cc_recipients.size(); i++) {
        tmp = "Cc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug_out) std::clog << "\t" << tmp;
        iter++;
    }

    iter = bcc_recipients.begin();

    for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
        tmp = "Bcc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug_out) std::clog << "\t" << tmp;
        iter++;
    }

    tmp = "Message-ID: <" + gen_smtp_msg_id() + ">\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "Subject: " + msg_theme + "\r\n\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    smtp_header += msg + "\r\n\r\n";

    if(debug_out) std::clog << "[DEBUG] Start work with smtp server\n\n";

    buf = smtp_header;

    CURL * email_msg;
    CURLcode err = CURLE_OK;

    curl_slist * rec = nullptr;
    upload_status uploud_stx;

    uploud_stx.lines_read = 0;

    if(debug_out) std::clog << "[DEBUG] CURL intit\n\n";

    email_msg = curl_easy_init();

    if(email_msg) {
        if(debug_out) std::clog << "[DEBUG] LOGIN to enter SMTP is : " << smart_smtp_login(sender) << "\n\n";
        curl_easy_setopt(email_msg, CURLOPT_USERNAME, smart_smtp_login(sender).c_str());

        if(debug_out) {
            std::clog << "[DEBUG] PASSWORD to enter SMTP is : ";
            for(std::size_t i{0}; i < passwd.length(); i++) std::clog << '*';
            std::clog << "\n\n";
        }
        curl_easy_setopt(email_msg, CURLOPT_PASSWORD, passwd.c_str());

        if(debug_out) std::clog << "[DEBUG] SMTP server is : " << smart_smtp_server(sender) << "\n\n";
        curl_easy_setopt(email_msg, CURLOPT_URL, smart_smtp_server(sender).c_str());

        #ifdef  SKIP_PEER_VERIFICATION
            if(debug_out) std::clog << "[DEBUG] Skip peer verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYPEER , 0L);
        #endif

        #ifdef  SKIP_HOSTNAME_VERIFICATION
            if(debug_out) std::clog << "[DEBUG] Skip hostname verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYHOST , 0L);
        #endif

        tmp = "";

        for(std::size_t i{0}; i < sender.length(); i++) {
            if(sender.at(i) == ' ') break;
            tmp += sender.at(i);
        }

        if(debug_out) std::clog << "[DEBUG] Setting sender : " << tmp << "\n\n";
        tmp = "<" + tmp + ">";

        curl_easy_setopt(email_msg, CURLOPT_MAIL_FROM, tmp.c_str());

        if(debug_out) std::clog << "[DEBUG] Adding recipients\n";

        tmp = "<" + recipient + ">";
        if(debug_out) std::clog << "\t" << tmp << "\n";
        rec = curl_slist_append(rec, tmp.c_str());

        iter = cc_recipients.begin();
        for(std::size_t i{0}; i < cc_recipients.size(); i++) {
            tmp = "<" + *iter + ">";
            if(debug_out) std::clog << "\t" << tmp << " - CC\n";
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        iter = bcc_recipients.begin();
        for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
            tmp = "<" + *iter + ">";
            if(debug_out) std::clog << "\t" << tmp << " - BCC\n";
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        if(debug_out) std::clog << "\n[DEBUG] Setting email check\n\n";
        curl_easy_setopt(email_msg, CURLOPT_MAIL_RCPT, rec);

        if(debug_out) std::clog << "[DEBUG] Upload good info\n\n";
        curl_easy_setopt(email_msg, CURLOPT_READFUNCTION, this->payload_source);

        if(debug_out) std::clog << "[DEBUG] Save server messages is ON\n\n";
        curl_easy_setopt(email_msg, CURLOPT_READDATA, &uploud_stx);

        if(debug_out) std::clog << "[DEBUG] Preparation\n\n";
        curl_easy_setopt(email_msg, CURLOPT_UPLOAD, 1L);

        if(debug_out) std::clog << "[DEBUG] Enable CURL debug\n\n";
        if(debug_out) curl_easy_setopt(email_msg, CURLOPT_VERBOSE, 1L);

        if(debug_out) std::clog << "[DEBUG] CURL DEBUG ____________________\n\n";

        err = curl_easy_perform(email_msg);

        if(debug_out) std::clog << "\n[DEBUG] CURL DEBUG ____________________\n\n";

        if(err != CURLE_OK) {
            if(debug_out) std::clog << "[DEBUG] CURL err : " << err << "\n\n";
            throw smtp_err("Error in CURL sending");
            return false;
        }

        if(debug_out) std::clog << "[DEBUG] Memory clear\n\n";

        curl_slist_free_all(rec);
        curl_easy_cleanup(email_msg);

        if(debug_out) std::clog << "[DEBUG] DONE!\n\n";
    }
    else {
        if(debug_out) std::clog << "[DEBUG] Cannot init CURL";
        throw smtp_err("Cannot init CURL, Search your internet connection");
        return false;
    }

    return true;
}

bool smtp::sent_email(const std::string & msg, const std::list<std::string> & files, const bool & debug_out) {
    if(debug_out) std::clog << "[DEBUG] Message sent started!\n\n";

    if(sender == "") {
        if(debug_out) std::clog << "[DEBUG] Sender undefined\n";
        throw smtp_err("You cannot send message without sender");
        return false;
    }

    if(debug_out) std::clog << "[DEBUG] Sender is : " << sender << "\n";

    if(passwd == "") {
        if(debug_out) std::clog << "[DEBUG] Password undefined\n";
        throw smtp_err("You cannot send message without your email account password");
        return false;
    }

    if(debug_out) {
        std::clog << "[DEBUG] Password is : ";
        for(std::size_t i{0}; i < passwd.length(); i++) std::clog << '*';
        std::clog << "\n\n";
    }

    if(recipient == "") {
        if(debug_out) std::clog << "[DEBUG] Recipient undefined\n";
        throw smtp_err("You cannot send message without Recipient");
        return false;
    }

    if(debug_out) std::clog << "[DEBUG] Recipient is : " << recipient << "\n\n";

    if(debug_out) {
        std::clog << "[DEBUG] CC_RECIPIENTS ANALYSIS :\n";

        auto iter = cc_recipients.begin();

        std::clog << "\tYou have " << cc_recipients.size() << " CC recipients\n";
        for(std::size_t i{0}; i < cc_recipients.size(); i++) {
            std::clog << "\t\t" << i + 1 << " - " << *iter << "\n";
            iter++;
        }

        std::clog << "\n";
    }

    if(debug_out) {
        std::clog << "[DEBUG] BCC_RECIPIENTS ANALYSIS :\n";

        auto iter = bcc_recipients.begin();

        std::clog << "\tYou have " << bcc_recipients.size() << " BCC recipients\n";
        for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
            std::clog << "\t\t" << i + 1 << " - " << *iter << "\n";
            iter++;
        }

        std::clog << "\n";
    }

    if(msg_theme == "") {
        if(debug_out) std::clog << "[WARNING] Messages without subject can be SPAM\n\n";
    }

    if(debug_out) std::clog << "[DEBUG] Subject is : " << msg_theme << "\n\n";

    if(debug_out) std::clog << "[DEBUG] Message text _______________________\n\n" << msg << "\n\n[DEBUG] End text ___________________________\n\n";

    if(debug_out) std::clog << "[DEBUG] Attachments ANALYSIS\n";

    std::list<std::string> exist;

    auto iter = files.begin();

    for(std::size_t i{0}; i < files.size(); i++) {
        if(debug_out) std::clog << "\t{" << *iter << "} - ";
        std::ifstream fin(*iter);
        if(fin.is_open()) {
            exist.push_back(*iter);
            if(debug_out) std::clog << "exist\n";
        }
        else if(debug_out) std::clog << "doesn\'t exist";
        fin.close(); iter++;
    }

    std::string smtp_header = "", tmp = "";

    if(debug_out) std::clog << "\n[DEBUG] Start SMTP header build\n";

    tmp = "Date: " + get_smtp_date() + " \r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "To: <" + recipient + ">\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "";

    for(std::size_t i{0}; i < sender.length(); i++) {
        if(sender.at(i) != ' ') tmp += sender.at(i);
        else break;
    }

    tmp = "From: <" + tmp + ">" + sender.substr(tmp.length()) + "\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    iter = cc_recipients.begin();

    for(std::size_t i{0}; i < cc_recipients.size(); i++) {
        tmp = "Cc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug_out) std::clog << "\t" << tmp;
        iter++;
    }

    iter = bcc_recipients.begin();

    for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
        tmp = "Bcc: <" + *iter + ">\r\n"; smtp_header += tmp;
        if(debug_out) std::clog << "\t" << tmp;
        iter++;
    }

    tmp = "Message-ID: <" + gen_smtp_msg_id() + ">\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    tmp = "Subject: " + msg_theme + "\r\n\r\n"; smtp_header += tmp;
    if(debug_out) std::clog << "\t" << tmp;

    CURL * email_msg;
    CURLcode err = CURLE_OK;

    if(debug_out) std::clog << "[DEBUG] CURL intit\n\n";

    email_msg = curl_easy_init();

    if(email_msg) {
        curl_slist * header = nullptr;
        curl_slist * slist = nullptr;
        curl_slist * rec = nullptr;

        curl_mimepart * part= nullptr;

        curl_mime * mime = nullptr;
        curl_mime * alt = nullptr;

        if(debug_out) std::clog << "[DEBUG] LOGIN to enter SMTP is : " << smart_smtp_login(sender) << "\n\n";
        curl_easy_setopt(email_msg, CURLOPT_USERNAME, smart_smtp_login(sender).c_str());

        if(debug_out) {
            std::clog << "[DEBUG] PASSWORD to enter SMTP is : ";
            for(std::size_t i{0}; i < passwd.length(); i++) std::clog << '*';
            std::clog << "\n\n";
        }
        curl_easy_setopt(email_msg, CURLOPT_PASSWORD, passwd.c_str());

        if(debug_out) std::clog << "[DEBUG] SMTP server is : " << smart_smtp_server(sender) << "\n\n";
        curl_easy_setopt(email_msg, CURLOPT_URL, smart_smtp_server(sender).c_str());

        #ifdef  SKIP_PEER_VERIFICATION
            if(debug_out) std::clog << "[DEBUG] Skip peer verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYPEER , 0L);
        #endif

        #ifdef  SKIP_HOSTNAME_VERIFICATION
            if(debug_out) std::clog << "[DEBUG] Skip hostname verification\n\n";
            curl_easy_setopt(mail_message, CURLOPT_SSL_VERIFYHOST , 0L);
        #endif

        tmp = "";

        for(std::size_t i{0}; i < sender.length(); i++) {
            if(sender.at(i) == ' ') break;
            tmp += sender.at(i);
        }

        if(debug_out) std::clog << "[DEBUG] Setting sender : " << tmp << "\n\n";
        tmp = "<" + tmp + ">";

        curl_easy_setopt(email_msg, CURLOPT_MAIL_FROM, tmp.c_str());

        if(debug_out) std::clog << "[DEBUG] Adding recipients\n";

        tmp = "<" + recipient + ">";
        if(debug_out) std::clog << "\t" << tmp << "\n";
        rec = curl_slist_append(rec, tmp.c_str());

        iter = cc_recipients.begin();
        for(std::size_t i{0}; i < cc_recipients.size(); i++) {
            tmp = "<" + *iter + ">";
            if(debug_out) std::clog << "\t" << tmp << " - CC\n";
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        iter = bcc_recipients.begin();
        for(std::size_t i{0}; i < bcc_recipients.size(); i++) {
            tmp = "<" + *iter + ">";
            if(debug_out) std::clog << "\t" << tmp << " - BCC\n";
            rec = curl_slist_append(rec, tmp.c_str());
            iter++;
        }

        if(debug_out) std::clog << "\n[DEBUG] Setting email check\n\n";
        curl_easy_setopt(email_msg, CURLOPT_MAIL_RCPT, rec);

        if(debug_out) std::clog << "[DEBUG] Adding header to CURL\n\n";
        for(std::size_t i{0}; i < smtp_header.size(); i++) header = curl_slist_append(header, &smtp_header.at(i));
        curl_easy_setopt(email_msg, CURLOPT_HTTPHEADER, header);

        if(debug_out) std::clog << "[DEBUG] MIME init\n\n";
        mime = curl_mime_init(email_msg);
        alt = curl_mime_init(email_msg);

        if(debug_out) std::clog << "[DEBUG] Adding text to email\n\n";
        part = curl_mime_addpart(alt);
        curl_mime_data(part, msg.c_str(), CURL_ZERO_TERMINATED);

        if(debug_out) std::clog << "[DEBUG] Setting MIME type\n\n";
        part = curl_mime_addpart(mime);
        curl_mime_subparts(part, alt);
        curl_mime_type(part, "multipart/alternative");
        slist = curl_slist_append(NULL, "Content-Disposition: inline");
        curl_mime_headers(part, slist, 1);

        if(debug_out) std::clog << "[DEBUG] Adding attachments\n";
        iter = exist.begin();
        for(std::size_t i{0}; i < exist.size(); i++) {
            if(debug_out) std::clog << "\t" << i + 1 << " - " << *iter << "\n";
            part = curl_mime_addpart(mime);
            curl_mime_filedata(part, (*iter).c_str());
            curl_easy_setopt(email_msg, CURLOPT_MIMEPOST, mime);
        }

        if(debug_out) std::clog << "\n[DEBUG] CURL debug messages is ON\n\n";
        if(debug_out) curl_easy_setopt(email_msg, CURLOPT_VERBOSE, 1L);

        if(debug_out) std::clog << "[DEBUG] CURL DEBUG _________________\n\n";

        err = curl_easy_perform(email_msg);

        if(debug_out) std::clog << "\n[DEBUG] CURL DEBUG _________________\n\n";

        if(err != CURLE_OK) {
            if(debug_out) std::clog << "[DEBUG] CURL err : " << err << "\n\n";
            throw smtp_err("Error in CURL sending");
            return false;
        }

        if(debug_out) std::clog << "[DEBUG] Memory free\n\n";

        curl_slist_free_all(rec);
        curl_slist_free_all(header);

        curl_easy_cleanup(email_msg);

        curl_mime_free(mime);
    }
    else {
        if(debug_out) std::clog << "[DEBUG] Cannot init CURL";
        throw smtp_err("Cannot init CURL, Search your internet connection");
        return false;
    }

    return true;
}

std::string smtp::get_smtp_date() const {
    std::time_t t = time(nullptr);

    std::string buf = ctime(&t), output = buf.substr(0, 3) + ", ";

    output += buf.substr(11, 2);

    if(output.at(output.length() - 1) != ' ') output += ' ';

    output += buf.substr(4,3) + " " + buf.substr(20,4) + " " + buf.substr(11,8);

    return output;
}

std::string smtp::gen_smtp_msg_id() const {
    std::string result = get_smtp_date();

    for(std::size_t i{0}; i < result.size(); i++) {
        if(result.at(i) == ' ' || result.at(i) == ',' || result.at(i) == ':') {
            result.erase(i, 1);
            continue;
        }
    }

    result += "htw80e";

    for(std::size_t i{0}; i < 5; i++) std::random_shuffle(result.begin(), result.end());

    srand(time(nullptr));

    result = (char)((rand() % 24) + 66) + result + "@zosima.com";

    for(std::size_t i{0}; i < result.size(); i++) {
        if(result.at(i) == ' ' || result.at(i) == ',' || result.at(i) == ':') {
            result.erase(i, 1);
            continue;
        }
    }

    return result;
}

std::string smtp::smart_smtp_login(const std::string & login) const {
    std::string buf = "";

    for(std::size_t i{0}; i < login.length(); i++) {
        if(login.at(i) == ' ') break;
        buf += login.at(i);
    }

    if(buf.find("@") == std::string::npos) {
        throw smtp_err("Cannot find domen name for SMTP-server. You must enter email before real name!");
    }

    if(buf.substr(buf.length() - 9, 9) == "yandex.ru")
        return buf.substr(0, buf.length()-10);

    return buf;
}

std::string smtp::smart_smtp_server(const std::string & login) const {
    if(login.find("@yandex.ru") != std::string::npos) return "smtps://smtp.yandex.ru";
    if(login.find("@gmail.ru") != std::string::npos) return "smtps://smtp.gmail.com";
    if(login.find("@google.ru") != std::string::npos) return "smtps://smtp.gmail.com";
    if(login.find("@rambler.ru") != std::string::npos) return "smtps://smtp.rambler.ru";
    if(login.find("@mail.ru") != std::string::npos) return "smtps://smtp.mail.ru";
    if(login.find("@outlook.com") != std::string::npos) return "smtps://smtp-mail.outlook.com";

    throw smtp_err("Now we can't do MX to find SMTP server, which is'n in the base");
}

std::size_t smtp::payload_source(void * ptr, std::size_t size, std::size_t nmemb, void * userp) {
    upload_status * upload_ctx = (struct upload_status *)userp;

    if((size == 0) || (nmemb == 0) || ((size*nmemb) < 1)) return 0;

    static const char *payload_text[] = {buf.c_str(), NULL};
    const char * data = payload_text[upload_ctx->lines_read];

    if(data) {
      std::size_t len = strlen(data);
      memcpy(ptr, data, len);
      upload_ctx->lines_read++;

      return len;
    }

    return 0;
}
