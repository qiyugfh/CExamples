#include "mqclient.h"



MQClient::MQClient(const std::string &_brokerURI, const std::string &_destURI, bool _useTopic, bool _clientAck):
    connection(NULL), session(NULL), destination(NULL),
    producer(NULL), consumer(NULL), useTopic(_useTopic),
    clientAck(_clientAck), brokerURI(_brokerURI), destURI(_destURI){

}

MQClient::~MQClient(){
    this->cleanup();
}

int MQClient::open(){
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


void MQClient::close(){
    printf("begin close ...\n");
    this->cleanup();
}

#if 1
string MQClient::recvTask(){
    printf("begin recv task...\n");
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


void MQClient::sendTask(const string &msg){
    printf("begin send task ...\n");
    TextMessage *message = session->createTextMessage(msg);
    printf("send message: %s\n", msg.c_str());
    producer->send(message);
    delete message;
    message = NULL;
}
#else
string MQClient::recvTask(){
    printf("begin recv task...\n");
    string text = "";
    Message *message = NULL;
    StreamMessage *streamMessage = NULL;
    try{
        message = consumer->receive();
        streamMessage = dynamic_cast<StreamMessage *>(message);
        text = streamMessage->readString();
    }catch(CMSException &e){
        e.printStackTrace();
    }
    printf("recieve message: %s\n", text.c_str());
    delete streamMessage;
    streamMessage = NULL;
    return text;
}


void MQClient::sendTask(const string &msg){
    printf("begin send task ...\n");
    StreamMessage *message = session->createStreamMessage();
    message->writeString(msg);
    printf("send message: %s\n", msg.c_str());
    producer->send(message);
    delete message;
    message = NULL;
}

#endif

void MQClient::onException(const CMSException& ex AMQCPP_UNUSED) {
    printf("CMS Exception occurred.  Shutting down client.\n");
    exit(1);
}


void MQClient::onException(const decaf::lang::Exception& ex) {
    printf("Transport Exception occurred: %s \n", ex.getMessage().c_str());
}

void MQClient::transportInterrupted() {
    printf("The Connection's Transport has been Interrupted.\n");
}

void MQClient::transportResumed() {
    printf("The Connection's Transport has been Restored.\n");
}

void MQClient::cleanup(){
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
