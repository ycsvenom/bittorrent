#pragma once
#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <lib/network/network.hpp>

IpAddress reserve_address();
int make_server(IpAddress addr);
int accept_client(int server_sockfd);

#endif