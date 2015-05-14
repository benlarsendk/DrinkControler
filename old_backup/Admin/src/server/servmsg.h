#ifndef SERVMSG
#define SERVMSG


#include <iostream>
#include <queue>
#include <stdlib.h>
using namespace std;

/* ENUM */
enum
{
    CHECKNAMEDRINK = 0,
    GETINGREDIENTSNAME = 1,
    CREATEDRINK = 2,
    GETDRINKSNAME = 3,
    GETDRINK = 4,
    CHANGEDRINK = 5,
    DELETEDRINK = 6,
    CHECKNAMEINGREDIENT = 7,
    CHECKCONTAINER = 8,
    CREATEINGREDIENT = 9,
    GETINGREDIENTADDRESS = 10,
    CHANGEINGREDIENTADDR = 11,
    DELETEINGREDIENT = 12,
    CLEAN = 13


};



class Message
{
public:
  virtual ~Message() {}
};




struct Item
{
    unsigned long id_;
    Message * msg_;
};


class MsgQueue
{
public:
    MsgQueue()
    {
    max_ = 20;
    }
  MsgQueue(unsigned long maxSize)
  {
    max_ = maxSize;
  }

  void send(unsigned long id, Message* msg = NULL)
  {
    Item element;						// Opret element
    element.id_ = id;					// Giv variablerne
    element.msg_ = msg;

    pthread_mutex_lock(&mtx);			// Tag mutexen
    while (container_.size() == max_)	// Hvis køen er fuld
      pthread_cond_wait(&signal,&mtx);	// Så smid mutexen og afvent signal
    container_.push(element);			// indsæt data i køen
    pthread_mutex_unlock(&mtx);			// Smid låsen
    pthread_cond_signal(&signal);		// Signaler vi har et element
  }

  Message* receive(unsigned long & id)
  {
    pthread_mutex_lock(&mtx);			// Tager mutexen
    while (container_.empty())			// Hvis tom
      pthread_cond_wait(&signal,&mtx);	// Smid mutex og afvent

    Item ocal = container_.front();		// get item til lokalt element
    container_.pop();
    pthread_mutex_unlock(&mtx);			// Smid låsen
    pthread_cond_signal(&signal);		// Signaler vi er done

    id = local.id_;						// returner id
    return local.msg_;					// returnet msg
  }

  ~MsgQueue() {}

private:
  queue<Item> container_;
  int max_;
  pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
  pthread_cond_t signal = PTHREAD_COND_INITIALIZER;
};

//Beskeder

struct Car : public Message
{
  MsgQueue* queue_;
};

struct Guard : public Message
{
  MsgQueue* queue_;
};

#endif //SERVMSG
