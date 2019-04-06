//class udp_client_server;
#include "udp_header.h"
#include <string.h>
#include <unistd.h>

namespace udp_client_server{

/** initialise UDP server object 
* function initialises UDP server object and makes it ready to receive messages
* server address and port specified in constructor
* function calls connect() to connect socket to specified address
* \note
* \warning
*  We'll see
* \exception udp_client_server_runtime_error
*/

udp_server::udp_server(const std::string& addr, int port)
	: f_port(port)
	, f_addr(addr)
	{
	char dec_port[16];
	snprintf(dec_port, sizeof(dec_port), "%d", f_port);
	dec_port[sizeof(dec_port) / sizeof(dec_port[0]) - 1] = '\0';

	struct addrinfo hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family =	AF_UNSPEC;
	hints.ai_socktype =	SOCK_DGRAM;
	hints.ai_protocol = 	IPPROTO_UDP;

	int r(getaddrinfo(addr.c_str(), dec_port, &hints, &f_addrinfo));

	if (r!=0 || f_addrinfo == NULL){
		throw udp_client_server_runtime_error(("invalid address or port for socket:\"" + addr + ":" + dec_port + "\"").c_str());
	}
	f_socket = socket(f_addrinfo->ai_family, SOCK_DGRAM | SOCK_CLOEXEC, IPPROTO_UDP);
	if(f_socket == -1)
	{
		freeaddrinfo(f_addrinfo);
		throw udp_client_server_runtime_error((" could not create UDP socket for: \"" + addr + ":" + dec_port + "\"").c_str());
	}

	r = bind(f_socket, f_addrinfo->ai_addr, f_addrinfo->ai_addrlen);

	if (r!=0){
		freeaddrinfo(f_addrinfo);
		close(f_socket);
		throw udp_client_server_runtime_error(("could not bind  UDP socket with: \"" + addr + ":" + dec_port + "\"").c_str());
	}
}


/** \brief Clean up the UDP server.
*
* This function frees the address info structures and closes the socket
*/

udp_server::~udp_server(){
	freeaddrinfo(f_addrinfo);
	close(f_socket);
}

/** \brief The socket used by this UDP server
*
* Function returns socket identifier.
* \return The socket of this UDP server
*/

int udp_server::get_socket() const{
	return f_socket;
}

/** \brief The port used by this UDP server.
*
* This function returns the port attached to UDP server
* Copy of port specified in the constructor.
* \return Port of UDP server
*/

int udp_server::get_port() const{
	return f_port;
}

/** \brief Return the address of UDP server.
*
* \return The address as passed to constructor
*/

std::string udp_server::get_addr() const{
	return f_addr;
}

/** \brief Wait on a message.
* Function waits until a message is received on server.
* Can only return from function if a message is received.
* The socket type can be changed by making it unblocked.
* \param[in] msg The buffer where the message is saved
* \param[in] max_size Maximum size the message can have.
* 
* \return The number of bytes read or -1 if error occurs.
*/

int udp_server::recv(char *msg, size_t max_size){
	return::recv(f_socket, msg, max_size, 0);
}

/** Wait for data to come in.
*
* This function waits for a given amount of time for data to come in.
* If no data comes in max_wait_ms, function returns -1 and errno set to EAGAIN.
*
* The socket is blocking socket.
* \param[in] msg Buffer where the message is saved.
* \param[in] max_size Size of \p msg buffer in bytes.
* \param[in] max_wait_ms Max time to wait for a message, in ms.
*
* \return -1 if error occurs or the function times out; otherwise, the number of bytes received.
*/

int udp_server::timed_recv(char *msg, size_t max_size, int max_wait_ms) {
		fd_set s;
		FD_ZERO(&s);
		FD_SET(f_socket, &s);
		struct timeval timeout;
		timeout.tv_sec = max_wait_ms / 1000;
		timeout.tv_usec = (max_wait_ms % 1000) * 1000;
		int retval = select(f_socket + 1, &s, &s, &s, &timeout);

		if (retval==-1){
			// select() set errno accordingly;
			return -1;
		}
		
		if (retval>0){
			// socket has data
			return::recv(f_socket, msg, max_size, 0);
		}

		// socket has no data
		errno = EAGAIN;
		return -1;
}

} // namespace udp_client_server
// vim: ts=4 sw=4 et






