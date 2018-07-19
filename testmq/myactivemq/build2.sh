#!/bin/bash


g++ client.cpp -o client -I/usr/local/ActiveMQ-CPP/  -I/usr/local/ActiveMQ-CPP/include/activemq-cpp-3.9.4  -I/usr/local/apr/include/apr-1  -L/usr/local/ActiveMQ-CPP/lib   -lactivemq-cpp -lpthread

g++ producer.cpp -o send  -I/usr/local/ActiveMQ-CPP/  -I/usr/local/ActiveMQ-CPP/include/activemq-cpp-3.9.4  -I/usr/local/apr/include/apr-1  -L/usr/local/ActiveMQ-CPP/lib   -lactivemq-cpp -lpthread
g++ consumer.cpp -o consumer  -I/usr/local/ActiveMQ-CPP/  -I/usr/local/ActiveMQ-CPP/include/activemq-cpp-3.9.4   -I/usr/local/apr/include/apr-1 -L/usr/local/ActiveMQ-CPP/lib  -lactivemq-cpp -lpthread
