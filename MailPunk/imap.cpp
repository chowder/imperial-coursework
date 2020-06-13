#include "imap.hpp"

using namespace std;

IMAP::Message::Message(Session* session, uint32_t uid):
  session(session), uid(uid){
    getMessageData();
}


void IMAP::Message::getMessageData(){
  // Create fetch type
  struct mailimap_set* set = mailimap_set_new_single(uid);
  struct mailimap_fetch_type* fetch_type = mailimap_fetch_type_new_fetch_att_list_empty();

  struct mailimap_fetch_att* env_att = mailimap_fetch_att_new_envelope();
  mailimap_fetch_type_new_fetch_att_list_add(fetch_type, env_att);

  struct mailimap_section* section = mailimap_section_new(NULL);
  struct mailimap_fetch_att* body_att = mailimap_fetch_att_new_body_peek_section(section);
  mailimap_fetch_type_new_fetch_att_list_add(fetch_type, body_att);

  // Create fetch result
  clist* fetch_result;
  check_error(mailimap_uid_fetch(session->imap, set, fetch_type, &fetch_result),
              "Could not fetch");
  auto msg_att = (struct mailimap_msg_att*) clist_content(clist_begin(fetch_result));

  // Iterate of each result of one given message
  for (clistiter* cur = clist_begin(msg_att->att_list); cur != NULL; cur = clist_next(cur)){
    auto item = (struct mailimap_msg_att_item*) clist_content(cur);
    if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC)
      continue;
    // Envelope data
    if (item->att_data.att_static->att_type == MAILIMAP_MSG_ATT_ENVELOPE) {
      if (item->att_data.att_static->att_data.att_env->env_subject)
        subject = item->att_data.att_static->att_data.att_env->env_subject;
      if (item->att_data.att_static->att_data.att_env->env_from->frm_list)
    		parseFrom(item->att_data.att_static->att_data.att_env->env_from->frm_list);
    }
    // Body data
    if (item->att_data.att_static->att_type == MAILIMAP_MSG_ATT_BODY_SECTION)
      body = item->att_data.att_static->att_data.att_body_section->sec_body_part;
  }
  // Free things
  mailimap_fetch_list_free(fetch_result);
  mailimap_fetch_type_free(fetch_type);
  mailimap_set_free(set);
}


string IMAP::Message::parseFrom(clist* from_lst){
  for (clistiter* it = clist_begin(from_lst); it != NULL; it = clist_next(it)){
    auto address = (mailimap_address*) clist_content(it);
    if (address->ad_mailbox_name && address->ad_host_name){
      from += address->ad_mailbox_name;
      from += '@';
      from += address->ad_host_name;
    } else
      from += "<UNKNOWN>";
    from += "; ";
  }
}


string IMAP::Message::getBody(){
  return body;
}


string IMAP::Message::getField(string fieldname){
  if (fieldname == "From")
    return from;
  if (fieldname == "Subject")
    return subject;
  return NULL;
}


void IMAP::Message::deleteFromMailbox(){
  struct mailimap_set* set = mailimap_set_new_single(uid);
  struct mailimap_flag_list* flag_list = mailimap_flag_list_new_empty();
  mailimap_flag_list_add(flag_list, mailimap_flag_new_deleted());
  struct mailimap_store_att_flags* att_flags = mailimap_store_att_flags_new_set_flags(flag_list);
  mailimap_uid_store(session->imap, set, att_flags);
  mailimap_expunge(session->imap);
  // Free things
  mailimap_set_free(set);
  mailimap_store_att_flags_free(att_flags);
  // Update UI
  session->updateUI();
  // Delete all messages
  for (int i = 0; session->messages[i]; i++){
    if (this == session->messages[i])
      continue;
    delete session->messages[i];
  }
  delete [] session->messages;
  delete this;
}


void IMAP::Session::connect(string const& server, size_t port){
  check_error(mailimap_socket_connect(imap, server.c_str(), port),
              "Could not connect to server");
}


void IMAP::Session::login(string const& userid, string const& password){
  check_error(mailimap_login(imap, userid.c_str(), password.c_str()),
              "Could not login");
}


void IMAP::Session::selectMailbox(std::string const& mailbox){
  mb = mailbox;
  check_error(mailimap_select(imap, mailbox.c_str()),
              "Could not select mailbox");
}


IMAP::Message** IMAP::Session::getMessages(){
  // Check if mailbox is empty
  uint32_t num_messages = getMessageCount();
  if (num_messages == 0){
    messages = new Message*[1];
    messages[0] = nullptr;
    return messages;
  }
  // Create variables
  clist* fetch_result;
  clistiter* cur;
  auto set = (mailimap_set*) mailimap_set_new_interval(1, 0);
  auto fetch_type = (mailimap_fetch_type*) mailimap_fetch_type_new_fetch_att_list_empty();
  auto fetch_att = (mailimap_fetch_att*) mailimap_fetch_att_new_uid();
  mailimap_fetch_type_new_fetch_att_list_add(fetch_type, fetch_att);

  // Fetch messages
  check_error(mailimap_fetch(imap, set, fetch_type, &fetch_result),
              "Could not fetch");
  messages = new Message*[num_messages + 1];

  // Create messages
  int count = 0;
  for (cur = clist_begin(fetch_result); cur != NULL; cur = clist_next(cur)) {
    auto msg_att = (struct mailimap_msg_att*) clist_content(cur);
    uint32_t uid = getMailUID(msg_att);
    messages[count++] = new Message(this, uid);
  }
  messages[count] = nullptr;

  // Free stuff
  mailimap_fetch_list_free(fetch_result);
  mailimap_fetch_type_free(fetch_type);
  mailimap_set_free(set);
  return messages;
}


uint32_t IMAP::Session::getMessageCount(){
  struct mailimap_status_att_list* att_list;
  struct mailimap_mailbox_data_status* status;
  att_list = mailimap_status_att_list_new_empty();
  check_error(mailimap_status_att_list_add(att_list, MAILIMAP_STATUS_ATT_MESSAGES),
              "MAILIMAP_ERROR_MEMORY");
  check_error(mailimap_status(imap, mb.c_str(), att_list, &status),
              "MAILIMAP_ERROR_STATUS");
  clistiter* cur;
  for (cur = clist_begin(status->st_info_list); cur != NULL; cur = clist_next(cur)){
    auto status_info = (struct mailimap_status_info*) clist_content(cur);
    switch (status_info->st_att){
      case MAILIMAP_STATUS_ATT_MESSAGES:
        uint32_t num = status_info->st_value;
        mailimap_status_att_list_free(att_list);
        mailimap_mailbox_data_status_free(status);
        return num;
    }
  }
}


uint32_t IMAP::Session::getMailUID(struct mailimap_msg_att* msg_att){
  clistiter* cur;
  for (cur = clist_begin(msg_att->att_list); cur != NULL; cur = clist_next(cur)){
    auto item = (struct mailimap_msg_att_item*) clist_content(cur);
    if (item->att_type != MAILIMAP_MSG_ATT_ITEM_STATIC)
      continue;
    if (item->att_data.att_static->att_type != MAILIMAP_MSG_ATT_UID)
      continue;
    return item->att_data.att_static->att_data.att_uid;
  }
  return 0;
}


IMAP::Session::~Session(){
  mailimap_free(imap);
  for (int i = 0; messages[i]; i++)
    delete messages[i];
  delete [] messages;
}
