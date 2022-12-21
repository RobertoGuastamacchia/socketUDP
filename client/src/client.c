#include "..\..\Functions\functions.h"
int UDPClient();

int main(void) {
	int i = UDPClient();
	system("pause");

}

int UDPClient(){
	WSADATA data;
	int check;
	check=startUP(&data);
	if(check==1){
		int c_socket = -1;
		if(c_socket = createSocket(&c_socket)){ // create the socket
			// Assignment address to the socket
			char domainPort[50];
			char* ip;
			int port;
			//printf("Write 'server' for default configurations, else write any keys.\n");
			//char inputSel[MaxStringOperation]="";
			//fgets(inputSel, sizeof(inputSel), stdin); // input a string

			if(DEBUG==0){
				printf("Inserisci l'server:ip del server\n");
				scanf("%s",&domainPort);
			}else{
				strcpy(domainPort,"srv.di.uniba.it:56700");
			}

			// dns configuration get ip by server name
			char* token = strtok(domainPort,":");
			sprintf(domainPort,"%s",token); //assign server domain name to variable
			token=strtok(NULL,":");
			port=atoi(token); //assign server port to variable
			struct hostent* host = gethostbyname(domainPort);
			if(host==NULL){
				printf("Error on DNS host resolution.");
				return -1;
			}
			struct in_addr* hostAddress = (struct in_addr*)host->h_addr_list[0];
			ip=inet_ntoa(*hostAddress);
			//----------------------------------------------------------------------

			struct sockaddr_in socketAddr;
			configSocket(&socketAddr,ip,port);
			// server connection
			char input[MaxStringOperation];
			char input_string[MaxStringOperation];
			char div[2] = " ";
			printMenu();
			fflush(stdin);
			strcpy(input, "");
			fgets(input, sizeof(input), stdin); // input a string

			while(strcmp(input,"=\n")!=0 && strcmp(input,"=")!=0){
				char *ptr = strtok(input, div);
				int i = 0;
				while(ptr != NULL){ // parsing the string
					if(i==0){
						strcpy(input_string,ptr);
					}
					else{
						strcat(input_string," ");
						strcat(input_string,ptr);
					}
					ptr = strtok(NULL, div);
					i++;
				}
				int string_len = strlen(input_string);
				if (sendMessage(c_socket, input_string,&socketAddr)==-1){ // send message to server
					return -1;
				}
				else{
					char buf[BUFFERSIZE]; // message buffer
					memset(buf, 0, BUFFERSIZE); // set buffer at 0
					printf("Waiting for a response from the server...\n");
					if (reciveMessage(c_socket, buf,&socketAddr) == -1) //recive response from the server
					{
						return -1;
					}
					else{
						printf("Result received from server %s and ip %s: %s = %s\n",parseAddr(inet_ntoa(socketAddr.sin_addr)),inet_ntoa(socketAddr.sin_addr),strtok(input_string,"\n"),strtok(buf,"\n"));
					}
				}
				printMenu();
				fflush(stdin);
				input[MaxStringOperation] = ""; // reset the string
				fgets(input, sizeof(input), stdin);// input a string
			}
			if (sendMessage(c_socket, input,&socketAddr)==-1){ // send message to server
				return -1;
			}
			else{
				printf("Connection closed...\n");
				closesocket(c_socket);
				clearwinsock();
				return 0;
			}
		}
	}
}


void printMenu(){
	printf("-------------- MENU ------------\n");
	printf("1) Add [+ Op1 Op2]\n");
	printf("2) Subtraction [- Op1 Op2]\n");
	printf("3) Division [/ Op1 Op2]\n");
	printf("4) Multiplication [* Op1 Op2]\n");
	printf("5) Exit [=]\n");
	printf("--------------------------------\n");
	printf("Insert operation and operands: \n");
}
