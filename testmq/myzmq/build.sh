#!/bin/bash

g++ -g -Wall  -I/opt/zeromq/include -L/opt/zeromq/lib -lzmq hwserver.cpp -o hwserver

g++ -g -Wall  -I/opt/zeromq/include -L/opt/zeromq/lib -lzmq hwclient.cpp -o hwclient


