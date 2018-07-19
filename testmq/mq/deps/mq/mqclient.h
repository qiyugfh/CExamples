#ifndef _MQ_CLIENT_H_
#define _MQ_CLIENT_H_

#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/core/ActiveMQConnection.h>
#include <activemq/transport/DefaultTransportListener.h>
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Integer.h>
#include <activemq/util/Config.h>
#include <decaf/util/Date.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>


using namespace activemq;
using namespace activemq::core;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;



class MQClient : public ExceptionListener, public DefaultTransportListener {
private:
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;
    MessageConsumer* consumer;
    bool useTopic;
    bool clientAck;
    std::string brokerURI;
    std::string destURI;

private:
    MQClient(const MQClient &);
    MQClient & operator=(const MQClient &);

public:

    MQClient(const std::string &_brokerURI, const std::string &_destURI,
             bool _useTopic = false, bool _clientAck = false);

    virtual ~MQClient();

    int open();
    void close();
    string recvTask();
    void sendTask(const string &msg);

    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException(const CMSException& ex AMQCPP_UNUSED);
    virtual void onException(const decaf::lang::Exception& ex);
    virtual void transportInterrupted();
    virtual void transportResumed();



private:
    void cleanup();
};


#endif // _MQ_CLIENT_H_


