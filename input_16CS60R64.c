#include<stdio.h>
#include<unistd.h>
#include<termios.h>
#include<string.h>

int getch(){                                          //getch() function to get the key value as soon as it is pressed
	int ch;
	struct termios new, old;
	tcgetattr(STDIN_FILENO, &old);
	memcpy(&new, &old, sizeof(new));
	new.c_lflag &= ~(ICANON);
	tcsetattr(STDIN_FILENO, TCSANOW, &new);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &old);
	return ch;
}

int eatinput(){                                      //analogous funtion to getchar()
	static char buffer[4096];                    //buffer to keep the input untill '\n' is pressed
	static int i = 0, len = 0;
	int ch, esc = 0;
	char *getCommand(char [], char);
	if(len == 0){
		do{
			switch(ch = getch()){
				case '\n': buffer[i++] = ch;
					   len = i;
					   i = 0;
					   continue;
				case 11: 
				case 12:   printf("\033[1D");printf(" ");printf("\033[1D");
					   printf("\033[1D");printf(" ");printf("\033[1D");
					   break;
				case 27:   esc  = 1;
					   printf("\033[1D");printf(" ");printf("\033[1D");
					   printf("\033[1D");printf(" ");printf("\033[1D");
					   break;
				case 127:  printf("\033[1D");printf(" ");printf("\033[1D");
					   printf("\033[1D");printf(" ");printf("\033[1D");
					   if(i > 0){
					   	printf("\033[1D");printf(" ");printf("\033[1D");
					   	buffer[i--] = '\0';
					   }
					   break;
				default:   buffer[i++] = ch;
					 
			}
			if(esc == 1){                              //If an Escape sequence is pressed
				while((ch = getch()) != 65 && ch != 66 && ch != 67 && ch != 68 && ch != 70 && ch != 72 && ch != 126){
					printf("\033[1D");printf(" ");printf("\033[1D");
				}
				printf("\033[1D");printf(" ");printf("\033[1D");
				if(ch == 65 || ch == 66){
					while(i > 0){
						printf("\033[1D");printf(" ");printf("\033[1D");
						buffer[i--] = '\0';
					}
					buffer[i] = '\0';
					getCommand(buffer, ch);   //gets the command from the file hist.log according to the option 'ch',
					printf("%s", buffer);     //strores it in the buffer ro allow the additional editing.
					i = strlen(buffer);
				}
				esc = 0;
			}
		}while(ch != '\n');
		return buffer[i++];
	}
	else{
		if((ch = buffer[i]) != '\n'){
			buffer[i++] = '\0';
			return ch;
		}
		else{
			buffer[i] = '\0';
			i = len = 0;
			return ch;
		}
	}
}
