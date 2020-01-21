#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void doprocessing (int sock)
{
	double n, p;
	char *prvi, *znak, *drugi;
	double rez;
	double br1, br2;
	int set = 0;
	int gre = 0;
	char buffer[256];
	while(!set)
	{
		bzero(buffer,256);
		n = read(sock,buffer,255);
		if (n < 0)
			printf("Error reading from port");
		prvi = strtok(buffer," ");
    		znak = strtok(NULL," ");
     		drugi = strtok(NULL," ");
		br1 = atof(prvi);
		br2 = atof(drugi);
     		if (strcmp(znak,"+") == 0)
		{
     			rez = br1+br2;
			printf("Primljeno: %.2f + %.2f \nRezultat: %.2f \n\n", br1, br2, rez); 
			gre = 0;
		}
     		else if (strcmp(znak,"-") == 0)
		{
     			rez = br1-br2;
			printf("Primljeno: %.2f - %.2f \nRezultat: %.2f \n\n", br1, br2, rez);
			gre = 0;
		}
     		else if (strcmp(znak,"*") == 0)
		{     		
			rez = br1*br2;
			printf("Primljeno: %.2f * %.2f \nRezultat: %.2f \n\n", br1, br2, rez); 
			gre = 0;
		}
     		else if (strcmp(znak,"/") == 0)
		{     		
			rez = br1/br2;
			printf("Primljeno: %.2f / %.2f \nRezultat: %.2f \n\n", br1, br2, rez); 
			gre = 0;
		}
     		else
     		{
     			printf("Klijent je uneo pogresan znak...\n\n");
			gre = 1;
     		}	
     		n = write(sock,&rez,sizeof(rez));
		p = write(sock,&gre, sizeof(gre));
     		if (n < 0)
			printf("ERROR writing to socket");	
	}
	close(sock);
}

int main( int argc, char *argv[] )
{
	int sockfd, newsockfd, portno, clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int  n;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("ERROR opening socket");
		exit(1);
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = 5001;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
	{
		perror("ERROR on binding");
		exit(1);
	}
	printf("Server je pokrenut! Cekaju se klijenti...\n");
	listen(sockfd,5);
	clilen = sizeof(cli_addr);
	while (1)
	{ 
		newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
		printf("Klijent se konektovao...\n\n");
		if (newsockfd < 0)
		{
			perror("ERROR on accept");
			exit(1);
		}

		int  pid = fork();
		if (pid < 0)
		{
			perror("ERROR on fork");
			exit(1);
		}
		if (pid == 0) 
		{
			doprocessing(newsockfd);
			close(sockfd);		
			exit(0);
		}
		else
		{
			close(newsockfd);
		}
	}
}
