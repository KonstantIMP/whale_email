#ifndef SMTP_HPP
#define SMTP_HPP

#include <curl/curl.h>

#include <exception>
#include <stdexcept>

#include <algorithm>
#include <iostream>
#include <fstream>

#include <cstddef>
#include <cstring>
#include <string>
#include <list>

#include <ctime>

class smtp_err : public std::exception {
public:
    smtp_err(const std::string & msg) : m_error(msg) {}

    const char * what() const noexcept { return m_error.c_str(); }

private:
    std::string m_error;

};

class smtp {
public:
    smtp();
    ~smtp();

    void clear();

    void set_sender(const std::string &);
    std::string get_sender() const;

    void set_passwd(const std::string &);
    std::string get_passwd() const;

    void set_msg_theme(const std::string &);
    std::string get_msg_theme() const;

    void set_recipient(const std::string &);
    std::string get_recipient() const;

    bool add_cc_recipient(const std::string &);
    std::string get_cc_recipient(const std::size_t &) const;

    bool remove_cc_recipient(const std::string &);
    bool remove_cc_recipient(const std::size_t &);

    std::size_t get_cc_recipients_num() const;

    void clear_cc_recipients();

    bool add_bcc_recipient(const std::string &);
    std::string get_bcc_recipient(const std::size_t &) const;

    bool remove_bcc_recipient(const std::string &);
    bool remove_bcc_recipient(const std::size_t &);

    std::size_t get_bcc_recipients_num() const;

    void clear_bcc_recipients();

    bool sent_email(const std::string &, const bool & = false);
    bool sent_email(const std::string &, const std::list<std::string> &, const bool & = false);

protected:
    std::string get_smtp_date() const;

    std::string gen_smtp_msg_id() const;

    std::string smart_smtp_login(const std::string &) const;

    std::string smart_smtp_server(const std::string &) const;

    static std::size_t payload_source(void *, std::size_t, std::size_t, void *);

private:
    struct upload_status {
      int lines_read;
    };

    std::string sender;
    std::string passwd;

    std::string msg_theme;

    std::string recipient;

    std::list<std::string> cc_recipients;
    std::list<std::string> bcc_recipients;
};

#endif // SMTP_HPP
