#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
	int sockfd = 0;
	struct sockaddr_in serv_addr;
	int set, gre;
	char * line = NULL;
	size_t len = 0;
	int nread;
	double rez;
	double n, p;
	if(argc != 2)
	{
		printf("\n Usage: %s <ip of server> \n",argv[0]);
		return 1;
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Error : Could not create socket \n");
		return 1;
	}
	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5001);

	if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
	{
		printf("\n inet_pton error occured\n");
		return 1;
	}

	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	printf("-------Uspesno ste se konekotvali na server!-------\n Posaljite poruku serveru npr: 3 + 2 ili upisite 'izadji' za izlaz\n\n\nPoruka: ");

	set = 0;
	while (!set)
	{
		nread=getline(&line, &len, stdin);
		nread--;
		line[nread] ='\0';
		if (strcmp(line,"izadji") == 0)
		{
			set = 1;
			printf("Prekinuta konekcija sa serverom!\n");
			return -1;
		}
		write (sockfd, line, strlen(line));
		n = read(sockfd,&rez,sizeof(rez));
		p = read(sockfd,&gre, sizeof(gre));
		if(gre == 0)
		{
			printf("Rezultat: %.2f\nPoruka: ",rez);
		}
		else 
		{
			printf("Pogresno unet znak !\nDozvoljene operacije su: [ + - * / ] !!!\nPoruka: ");
		}	
	}
	close(sockfd);
	return 0;
}
