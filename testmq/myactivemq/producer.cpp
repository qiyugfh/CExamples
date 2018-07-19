#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Long.h>
#include <decaf/util/Date.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <activemq/library/ActiveMQCPP.h>
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
#include <memory>
 
using namespace activemq;
using namespace activemq::core;
using namespace decaf;
using namespace decaf::lang;
using namespace decaf::util;
using namespace decaf::util::concurrent;
using namespace cms;
using namespace std;
 
////////////////////////////////////////////////////////////////////////////////
class SimpleProducer : public Runnable {
private:
 
    Connection* connection;
    Session* session;
    Destination* destination;
    MessageProducer* producer;
    bool useTopic;
    bool clientAck;
    unsigned int numMessages;
    std::string brokerURI;
    std::string destURI;
 
private:
 
    SimpleProducer( const SimpleProducer& );
    SimpleProducer& operator= ( const SimpleProducer& );
 
public:
 
    SimpleProducer( const std::string& brokerURI, unsigned int numMessages,
                    const std::string& destURI, bool useTopic = false, bool clientAck = false ) :
        connection(NULL),
        session(NULL),
        destination(NULL),
        producer(NULL),
        useTopic(useTopic),
        clientAck(clientAck),
        numMessages(numMessages),
        brokerURI(brokerURI),
        destURI(destURI) {
    }
 
    virtual ~SimpleProducer(){
        cleanup();
    }
 
    void close() {
        this->cleanup();
    }
 
    virtual void run() {
   
     printf("begin run ...\n");
     try {
            auto_ptr<ActiveMQConnectionFactory> connectionFactory(new ActiveMQConnectionFactory( brokerURI ) );
           
		   try{
                connection = connectionFactory->createConnection();
                connection->start();
            } catch( CMSException& e ) {
                e.printStackTrace();
                throw e;
            }
 
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
		
            printf("create Producer!\n"); 
            producer = session->createProducer( destination );
            producer->setDeliveryMode( DeliveryMode::PERSISTENT );

            /*
            string threadIdStr = Long::toString( Thread::currentThread()->getId() );
            string text = (string)"Hello world! from thread " + threadIdStr;
 
            for( unsigned int ix=0; ix<numMessages; ++ix )
			{
                TextMessage* message = session->createTextMessage( text );
                message->setIntProperty( "Integer", ix );
                printf( "Sent message #%d from thread %s\n", ix+1, threadIdStr.c_str() );
                producer->send( message );
                delete message;
            }
            */   
 
        }catch ( CMSException& e ) {
            e.printStackTrace();
        }
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
 
private:
 
    void cleanup(){
        try{
            if( destination != NULL ) delete destination;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        destination = NULL;
 
        try
		{
            if( producer != NULL ) delete producer;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        producer = NULL;
 
        try{
            if( session != NULL ) session->close();
            if( connection != NULL ) connection->close();
        }catch ( CMSException& e ) { e.printStackTrace(); }
 
        try{
            if( session != NULL ) delete session;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        session = NULL;
 
        try{
            if( connection != NULL ) delete connection;
        }catch ( CMSException& e ) { e.printStackTrace(); }
        connection = NULL;
    }
};
 
int main(int argc , char* argv[]) 
{
 
    activemq::library::ActiveMQCPP::initializeLibrary();
    std::cout << "=====================================================\n";
    std::cout << "Starting produce message:" << std::endl;
    std::cout << "-----------------------------------------------------\n";
 
    std::string brokerURI ="failover://(tcp://127.0.0.1:61616)";
    unsigned int numMessages = 2000;
    std::string destURI = "test.chen";
 
    bool useTopics = false;
    SimpleProducer producer( brokerURI, numMessages, destURI, useTopics );
    producer.run();
    producer.sendTask();
    producer.close();
 
    std::cout << "-----------------------------------------------------\n";
    std::cout << "Finished test" << std::endl;
    std::cout << "=====================================================\n";
 
    activemq::library::ActiveMQCPP::shutdownLibrary();
}

