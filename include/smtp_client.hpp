#ifndef SMTP_CLIENT_HPP
#define SMTP_CLIENT_HPP

#include <curl/curl.h>

#include <exception>
#include <algorithm>

#include <iostream>
#include <fstream>

#include <cstddef>
#include <string>

#include <ctime>

#include <list>

class smtp_err : public std::exception {
public:
    smtp_err(const std::string & err) : err_msg(err) {}

    const char * what() const noexcept {return err_msg.c_str();}

private:
    std::string err_msg;
};

class smtp_client {
public:
    smtp_client();
    smtp_client(const smtp_client &);

    smtp_client(const std::string &, const std::string &, const std::string & = "");

    ~smtp_client();

    void clear_all();

    void clear_sender();

    void clear_recipient();
    void clear_cc_recipient();
    void clear_bcc_recipient();
    void clear_fake_recipient();

    void clear_additional();

    void set_smtp_server(const std::string &);
    std::string get_smtp_server() const;

    void set_sender_email(const std::string &);
    std::string get_sender_email() const;

    void set_sender_password(const std::string &);
    std::string get_sender_password() const;

    void set_email_to_reply(const std::string &);
    std::string get_email_to_reply() const;

    void set_recipient_email(const std::string &);
    std::string get_recipient_email() const;

    void set_message_subject(const std::string &);
    std::string get_message_subject() const;


    std::size_t get_cc_recipient_num() const;

    void add_cc_recipient(const std::string &);
    std::string get_cc_recipient(const std::size_t &) const;

    void remove_cc_recipient(const std::size_t &);
    void remove_cc_recipient(const std::string &);

    std::list<std::string> & get_cc_recipient_list() const;


    std::size_t get_bcc_recipient_num() const;

    void add_bcc_recipient(const std::string &);
    std::string get_bcc_recipient(const std::size_t &) const;

    void remove_bcc_recipient(const std::size_t &);
    void remove_bcc_recipient(const std::string &);

    std::list<std::string> & get_bcc_recipient_list() const;


    std::size_t get_fake_recipient_num() const;

    void add_fake_recipient(const std::string &);
    std::string get_fake_recipient(const std::size_t &) const;

    void remove_fake_recipient(const std::size_t &);
    void remove_fake_recipient(const std::string &);

    std::list<std::string> & get_fake_recipient_list() const;


    bool sent_email(const std::string &, const std::list<std::string> &, const bool &);

protected:
    std::string smtp_get_time() const;

    std::string smtp_gen_msg_id() const;

    std::string smtp_get_smtp(const std::string &) const;

    std::string smtp_get_login(const std::string &) const;

    bool smtp_email_validate(const std::string &) const;

private:
    std::string smtp_server;

    std::string sender_email;
    std::string sender_password;

    std::string reply_to_email;

    std::string recipient_email;

    std::string message_subject;

    std::list<std::string> cc_recipient;
    std::list<std::string> bcc_recipient;
    std::list<std::string> fake_recipient;
};

#endif // SMTP_CLIENT_HPP
