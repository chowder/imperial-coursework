#ifndef IMAP_H
#define IMAP_H
#include "imaputils.hpp"
#include <libetpan/libetpan.h>
#include <string>
#include <functional>
#include <iostream>
#include <sstream>

namespace IMAP {

// Forward declaration
class Session;

class Message {
	Session* session;
	uint32_t uid;
	std::string body = "";
	std::string from = "";
	std::string subject = "";

	void getMessageData();
	std::string parseFrom(clist* from_lst);

public:
	Message(Session* session, uint32_t uid);

	// Get the body of the message. You may chose to either include the headers or not.
	std::string getBody();

	// Get one of the descriptor fields (subject, from, ...)
	std::string getField(std::string fieldname);

	// Remove this mail from its mailbox
	void deleteFromMailbox();
};

class Session {
	std::string mb;
	Message** messages;
	std::function<void()> updateUI;
	struct mailimap* imap = mailimap_new(0, NULL);

public:
	Session(std::function<void()> updateUI): updateUI(updateUI){};

	uint32_t getMessageCount();

	// Get all messages in the INBOX mailbox terminated by a nullptr (like we did in class)
	Message** getMessages();

	// Connect to the specified server (143 is the standard unencrypte imap port)
	void connect(std::string const& server, size_t port = 143);

	// Log in to the server (connect first, then log in)
	void login(std::string const& userid, std::string const& password);

	// Select a mailbox (only one can be selected at any given time) this can only
	// be performed after login
	void selectMailbox(std::string const& mailbox);

	// Get UID of a message
	uint32_t getMailUID(struct mailimap_msg_att* msg_att);

	friend Message;

	~Session();
};
}

#endif /* IMAP_H */
