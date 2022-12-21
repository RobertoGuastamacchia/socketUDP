#include "..\..\Functions\functions.h"
#define SERVER_IP "127.0.0.1"

int main() {
	int port = PORT;
		if (port < 0) {
			printf("Port %s is not valid.\n");
			return 0;
		}else{
			WSADATA data;
			int check;
			check=startUP(&data);
			if(check==1){
				int socket =-1;
				if( socket = createSocket(&socket)){ // create the sochet
					// Assignment address to the socket
					struct sockaddr_in echoServAddr;
					struct sockaddr_in echoClntAddr;
					configSocket(&echoServAddr,SERVER_IP,port);
					int ck = 1;
					if (bind(socket, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) < 0) { // bind() the socket
						printf("Error - bind() failed.\n");
						closesocket(socket);
						clearwinsock();
						return -1;
					}else{
						printf("Success - bind().\n");
						while (1) {
							int err = 0;
							char buf[BUFFERSIZE]; // buffer for the input message
							memset(buf, 0, BUFFERSIZE);
							printf("Waiting client message...\n");
							char outputString[MaxStringOperation];
							while (err==0) {
								if (reciveMessage(socket, buf,&echoClntAddr) == 1) // recive message
								{
									printf("Recived request %s from: client %s and ip: %s\n",strtok(buf,"\n"),parseAddr(inet_ntoa(echoClntAddr.sin_addr)),inet_ntoa(echoClntAddr.sin_addr));
									if(strcmp(buf,"=\n")!=0 && strcmp(buf,"=")!=0){
										elaborateCalc(buf,outputString); // elaborate the input string
										if (sendMessage(socket, outputString,&echoClntAddr)==-1){ // send message to client
											err=1;
											//closesocket(socket);
											//clearwinsock();
											continue;
										}else{
											memset(buf, 0, BUFFERSIZE);
										}
									}
									else{
										printf("Client connection closing...\n");
										closesocket(socket);
										socket = createSocket(&socket);
										bind(socket, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr));
										//clearwinsock();
										err=1;
									}
								}
								else{
									err = 1;
								}
							}
							if (socket==-1) {
								printf("Error - association client-server failed.\n");
								closesocket(socket);
								clearwinsock();
							}
							err = 0;
						}
					}
				}
			}
		}
		system("PAUSE");
}


int add (int op1,int op2){
	int res = 0;
	res = op1+op2;
	return res;
}
int mult (int op1,int op2){
	int res = 0;
	res = op1*op2;
	return res;
}
int sub (int op1,int op2){
	int res = 0;
	res = op1-op2;
	return res;
}
int division (int op1,int op2){
	int res = 0;
	res = op1/op2;
	return res;
}
void elaborateCalc(char* buf,char* outputString){
	float output;
	char op[1];
	int op1;
	int op2;
	char *ptr = strtok(buf, " ");
	int i = 0;
	while(ptr != NULL){
		if(i==0){
			strcpy(op,ptr);
		}
		if(i==1){
			op1=atoi(ptr);
		}
		if(i==2){
			op2=atoi(ptr);
		}
		ptr = strtok(NULL, " ");
		i++;
	}
	if(strcmp(op,"+")==0){
		output=add(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"-")==0){
		output=sub(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"*")==0){
		output=mult(op1,op2);
		itoa(output,outputString,10);
	}
	else if(strcmp(op,"/")==0){
		if(op2 != 0){
			output=division(op1,op2);
			itoa(output,outputString,10);
		}else{
			strcpy(outputString,"Error, No valid operation , op2 < 0.");
		}
	}
	else{
		strcpy(outputString,"Error, No valid operator");
	}
}

