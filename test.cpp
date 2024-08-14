#include	<unistd.h>
#include	<stdio.h>
#include 	<poll.h>
#include 	<string>



int main ()
{

	char name[255];

	write(1,"enter name :",12);
	read(0,name,sizeof(name));

	printf("hello  ==> %s ", name);



	return (0);
}
