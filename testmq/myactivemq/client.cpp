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
#include <iostream>

using namespace activemq;
using namespace activemq::core;
using namespace activemq::transport;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;



class SimpleClient : public ExceptionListener, public DefaultTransportListener {
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
    SimpleClient(const SimpleClient &);
    SimpleClient & operator=(const SimpleClient &);

public:

    SimpleClient(const std::string &brokerURI, const std::string &destURI,
                 bool useTopic = false, bool clientAck = false):
        connection(NULL), session(NULL), destination(NULL),
        producer(NULL), consumer(NULL), useTopic(useTopic),
        clientAck(clientAck), brokerURI(brokerURI), destURI(destURI){

    }

    virtual ~SimpleClient(){
        this->cleanup();
    }


    int open(){
        printf("begin open ...\n");
        try {
            ActiveMQConnectionFactory* connectionFactory = new ActiveMQConnectionFactory( brokerURI );
            connection = connectionFactory->createConnection();
            delete connectionFactory;

            ActiveMQConnection* amqConnection = dynamic_cast<ActiveMQConnection*>( connection );
            if( amqConnection != NULL )
            {
                amqConnection->addTransportListener( this );
            }

            connection->start();
            connection->setExceptionListener(this);

            if( clientAck )
            {
                session = connection->createSession( Session::CLIENT_ACKNOWLEDGE );
            } else
            {
                session = connection->createSession( Session::AUTO_ACKNOWLEDGE );
            }

            if( useTopic )
            {
                destination = session->createTopic( destURI );
            } else
            {
                destination = session->createQueue( destURI );
            }

            printf("create producer!\n");
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::PERSISTENT );

            printf("create consumer!\n");
            consumer = session->createConsumer( destination );

        }catch ( CMSException& e ) {
            e.printStackTrace();
            return -1;
        }
        return 0;
    }


    void close(){
        printf("begin close ...\n");
        this->cleanup();
    }


    string recvTask(){
        printf("begin recv ...\n");
        string text = "";
        Message *message = NULL;
        TextMessage *textMessage = NULL;
        try{
            message = consumer->receive();
            textMessage = dynamic_cast<TextMessage *>(message);
            text = textMessage->getText();
        }catch(CMSException &e){
            e.printStackTrace();
        }
        printf("recieve message: %s\n", text.c_str());
        delete textMessage;
        textMessage = NULL;
        return text;
    }


    void sendTask(){
        printf("begin send ...\n");
        string text = "send 11111";
        TextMessage *message = session->createTextMessage(text);
        printf("send message\n");
        producer->send(message);
        delete message;
        message = NULL;
    }



    // If something bad happens you see it here as this class is also been
    // registered as an ExceptionListener with the connection.
    virtual void onException(const CMSException& ex AMQCPP_UNUSED) {
        printf("CMS Exception occurred.  Shutting down client.\n");
        exit(1);
    }

    virtual void onException(const decaf::lang::Exception& ex) {
        printf("Transport Exception occurred: %s \n", ex.getMessage().c_str());
    }

    virtual void transportInterrupted() {
        std::cout << "The Connection's Transport has been Interrupted." << std::endl;
    }

    virtual void transportResumed() {
        std::cout << "The Connection's Transport has been Restored." << std::endl;
    }



private:
    void cleanup(){
        try{
            if( destination != NULL ) delete destination;
        }catch (CMSException& e) {}
        destination = NULL;

        try{
            if( consumer != NULL ) delete consumer;
        }catch (CMSException& e) {}
        consumer = NULL;

        try{
            if( session != NULL ) session->close();
            if( connection != NULL ) connection->close();
        }catch (CMSException& e) {}

        try{
            if( session != NULL ) delete session;
        }catch (CMSException& e) {}
        session = NULL;

        try{
            if( connection != NULL ) delete connection;
        }catch (CMSException& e) {}
        connection = NULL;
    }
};



int main(int argc, char *argv[]){
    activemq::library::ActiveMQCPP::initializeLibrary();

    std::string brokerURI ="failover://(tcp://127.0.0.1:61616)";
    std::string destURI1 = "fanghua1";
    std::string destURI2 = "fanghua2";
    bool useTopic = false;    

    SimpleClient client1(brokerURI, destURI1, useTopic);
    SimpleClient client2(brokerURI, destURI2, useTopic);
    client1.open();
    client2.open();
    client1.sendTask();
    client2.sendTask();
    client1.recvTask();
    //client2.recvTask();
    client1.close();
    client2.close();

    activemq::library::ActiveMQCPP::shutdownLibrary();
    return 0;
}

