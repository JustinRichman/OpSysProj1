//COP4610
//Project 1 Starter Code
//example code for initial parsing

//*** if any problems are found with this code,
//*** please report them to the TA


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

typedef struct
{
	char** tokens;
	int numTokens;
} instruction;

void addToken(instruction* instr_ptr, char* tok);
void printTokens(instruction* instr_ptr);
void clearInstruction(instruction* instr_ptr);
void addNull(instruction* instr_ptr);
char *GetEnv(const char* name);
int setenv(const char *var_name, const char *new_value, int change_flag);
void ChangeDirectory(char* buf);
char* ShortResolution(char* path_str);

int main() {
	char* token = NULL;
	char* temp = NULL;

	instruction instr;
	instr.tokens = NULL;
	instr.numTokens = 0;

	char **args; //for execution

	instruction check;
	check.tokens = NULL;
	check.numTokens = 0;

	int numberOfCommands = 0;
	int forkFlag = 0;

	while (1) {

		forkFlag = 0;

		printf("%s@%s :%s>",getenv("USER"),getenv("MACHINE"),getenv("PWD"));

		// loop reads character sequences separated by whitespace
		do {
			//scans for next token and allocates token var to size of scanned token
			scanf("%ms", &token);
			temp = (char*)malloc((strlen(token) + 1) * sizeof(char));
			int i;
			int start = 0;
			for (i = 0; i < strlen(token); i++) {
				//pull out special characters and make them into a separate token in the instruction
				if (token[i] == '|' || token[i] == '>' || token[i] == '<' || token[i] == '&') {
					if (i-start > 0) {
						memcpy(temp, token + start, i - start);
						temp[i-start] = '\0';
						addToken(&instr, temp);
					}

					char specialChar[2];
					specialChar[0] = token[i];
					specialChar[1] = '\0';

					if(strcmp(specialChar, "|") == 0)
					forkFlag = 1;
					else if(strcmp(specialChar, ">") == 0 && forkFlag == 3) // cmd < fIn > fOut
					forkFlag = 5;
					else if(strcmp(specialChar, "<") == 0 && forkFlag == 2) // cmd > fOut < fIn
					forkFlag = 6;
					else if(strcmp(specialChar, ">") == 0)
					forkFlag = 2;
					else if(strcmp(specialChar, "<") == 0)
					forkFlag = 3;

					addToken(&instr,specialChar);

					start = i + 1;
				}
			}

			if (start < strlen(token)) {
				memcpy(temp, token + start, strlen(token) - start);
				temp[i-start] = '\0';
				temp = strdup(ShortResolution(temp));
				addToken(&instr, temp);
			}

			//free and reset variables
			free(token);
			free(temp);

			token = NULL;
			temp = NULL;
		} while ('\n' != getchar());    //until end of line is reached

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		 int i;
		 for (i = 0; i < instr.numTokens; i++) {
				 if (instr.tokens[i] != NULL){
						 if((i==instr.numTokens-1)&&strcmp(instr.tokens[i],"<")==0) { //checks if < is the last symbol and if it stands alone
										 printf("Invalid syntax.\n");
										 forkFlag = -1;
}
						 else if((i==instr.numTokens-1)&&strcmp(instr.tokens[i],">")==0){  //checks if > is the last symbol and if it stands alone
										 printf("Invalid syntax.\n");
										 forkFlag = -1;
									 }

						 else if((strcmp(instr.tokens[0],"<")==0) )  //checks if < is first and file second
						 {
										 printf("Invalid syntax.\n");
										 forkFlag = -1;
										 break;
						 }
						 else if((strcmp(instr.tokens[0],"|")==0) )  //checks if | is first and file second
						 {
										 printf("Invalid syntax.\n");
										 forkFlag = -1;
										 break;
						 }
						 else if((i==instr.numTokens-1)&&strcmp(instr.tokens[i],"|")==0){  //checks if | is last symbol
										 printf("Invalid syntax.\n");
										 forkFlag = -1;
									 }
						 else if((strcmp(instr.tokens[i],"&")==0)){  //checks if & and | are next to each other
                            if(strcmp(instr.tokens[i+1],"|")==0){
                                if(strcmp(instr.tokens[instr.numTokens-1],"&")==0)
                                    printf("Invalid syntax.\n");
                                else
                                    printf("Invalid syntax.\n");
																		forkFlag = -1;
													  }
                    }
            else if((strcmp(instr.tokens[i],"<")==0)||strcmp(instr.tokens[i],">")==0){  //checks if < or > are with &
                            if(strcmp(instr.tokens[i+1],"&")==0){
                                    printf("Invalid syntax.\n");
																		forkFlag = -1;
                            }
                    }

				 }
		 }




		if(strcmp(instr.tokens[0],"exit") == 0 && forkFlag != -1)
		{
			printf("Exiting Shell\n");
			//waitpid(-1,...,0);			Needs implementation for background
			printf("Commands Executed: %d\nGoodbye!\n", numberOfCommands);
			return 0;
		}
		numberOfCommands++;
		if(strcmp(instr.tokens[0], "echo") == 0 && forkFlag != -1) //runs echo
		{
			int i;
			for(i = 1; i < instr.numTokens; i++)
			{
				if(strstr(instr.tokens[i], "$") != NULL && strlen(instr.tokens[i]) > 1)
				{
					char tmp[35];
					sscanf(instr.tokens[i], "$%s", tmp);
					printf("%s ", getenv(tmp));
				}
				else
					printf("%s ", instr.tokens[i]);
			}
			printf("\n");
		}

		if(strcmp(instr.tokens[0], "cd") == 0 && forkFlag != -1) //runs cd
	{
		if(instr.numTokens > 2)
		{
			printf("cd: Too many arguments.\n");
		}
		else
		{
			if(instr.numTokens < 2)
			{
				chdir(getenv("HOME"));
				setenv("PWD",getenv("HOME"),1);			//change PWD to CWD
			}
			else
			{
				int checkIfgood = chdir(ShortResolution(instr.tokens[1]));
				if(checkIfgood == 0)
					setenv("PWD",ShortResolution(instr.tokens[1]),1);			//change PWD to CWD
				else
					printf("cd: No such directory.\n");

			}
		}
	}


		if((strcmp(instr.tokens[0], "cd")) != 0 && (strcmp(instr.tokens[0], "echo")) != 0 && forkFlag != -1) //Not built-ins
			{
				char *temp_path= strdup(getenv("PATH"));

				int j;

					char * path = strtok(temp_path, ":");
					while(path != NULL)
					{
						addToken(&check, path);
						path = strtok(NULL, ":");
					}


					char* fileCheck; //for pipes
					int futureFile = -1; //for pipes
					for(j = 0; j < check.numTokens; j++){
						fileCheck = strdup(check.tokens[j]);
						strcat(fileCheck, "/");
						strcat(fileCheck, instr.tokens[0]);
						struct stat in = {0};
					 	stat(fileCheck, &in);
						if(S_ISREG(in.st_mode)){
							futureFile = j;
						 	j = check.numTokens+1;
						}
					}


				int i;
				args = (char**) malloc(sizeof(char*));
				args[0] = (char *)malloc((strlen(fileCheck)+1) * sizeof(char));		//making exev args
				strcpy(args[0], fileCheck);

				int checkSpec = 0;
				int background = -1;	//for background
				for(i = 1; i < instr.numTokens; i++){	//cont making execv args[]
					if ( (strcmp(instr.tokens[i], "|") == 0) ||  (strcmp(instr.tokens[i], ">") == 0) ||  (strcmp(instr.tokens[i], "<") == 0)){
						checkSpec = i;
					}
					else if((strcmp(instr.tokens[i], "&") == 0))
					{
						background = i;
					}
					else{
					args = (char**) realloc(args, (i+1) * sizeof(char*));
					args[i] = (char *)malloc((strlen(instr.tokens[i])+1) * sizeof(char));
					strcpy(args[i], instr.tokens[i]);
				}
				}

				args = (char**) realloc(args, (instr.numTokens+1) * sizeof(char*));
				args[instr.numTokens] = (char *)malloc(10 * sizeof(char));
				args[instr.numTokens] = NULL;


					int status;
					pid_t pid = fork();

					if(forkFlag == 0) //No special chars
					{
						if(pid == -1)
						{
							printf("Error -1\n");
							exit(1);
						}
						else if(pid == 0)
						{
							execv(args[0], args);
						}
						else{
							//if(background == -1) //Commented out background try
							waitpid(pid, &status, 0);
							//else if(background > -1){
							//waitpid(pid,&status,WNOHANG);

							}
						}
					else	if(forkFlag == 3){	//INPUT IO
					int fd = open(instr.tokens[2], O_RDWR);

					if(fd<0)
					{
						printf("File does not exist\n" );
						exit(1);
					}

					if(pid == -1)
					{
						printf("Error -1\n");
						exit(1);
					}
					else if(pid == 0)
					{
						close(0);
						dup(fd);
						close(fd);
						execv(args[0], args);
					}
					else{
						waitpid(pid, &status, 0);
						close(fd);
					}
				}
				else if(forkFlag == 2){ 	//OUTPUT IO
					int fd = open(instr.tokens[2], O_CREAT|O_WRONLY|O_TRUNC, 0777);

					if(pid == -1)
					{
						printf("Error -1\n");
						exit(1);
					}
					else if(pid == 0)
					{
						close(1);
						dup(fd);
						close(fd);
						execv(args[0], args);
					}
					else{
						waitpid(pid, &status, 0);
						close(fd);
					}
				}
				else if(forkFlag == 1){ //PIPE


					// // BELOW is our commented out attempt at pipe :(
					//
					// char *fileCheck1;
					// fileCheck1 = strdup(check.tokens[futureFile]);
					// strcat(fileCheck1, "/");
					// char *move = strdup(args[checkSpec+1]);
					// strcat(fileCheck1, move);
					//
					//
					// char **args1;
					// args1 = (char**) malloc(sizeof(char*));
					// args1[0] = (char *)malloc((strlen(fileCheck1)+1) * sizeof(char));
					// //strcpy(args1[0], fileCheck1);
					// args1[0] = strdup(fileCheck1);
					//
					//
					// int n = 1;
					// for(i = checkSpec+2; i < instr.numTokens; i++){
					// 	args1 = (char**) realloc(args1, (n+1) * sizeof(char*));
					// 	args1[i] = (char *)malloc((strlen(instr.tokens[i])+1) * sizeof(char));
					// args1[n] = strdup(instr.tokens[i]);
					// 	n++;
					// }
					//
					//
					// int fd[2];
					// pipe(fd);
					// fd[0] = 3;
					// fd[1] = 4;
					//
					//
					// int status1;
					// pid_t pid1 = fork();
					//
					// if(pid == -1)
					// {
					// 	printf("Error -1\n");
					// 	exit(1);
					// }
					// else if(pid == 0)
					// {
					// 	close(1);
					// 	dup(fd[1]);
					// 	close(fd[0]);
					// 	close(fd[1]);
					// 	execv(args[0], args);
					//
					// 	if(fork() == 0){
					// 		close(0);
					// 		dup(fd[0]);
					// 		close(fd[0]);
					// 		close(fd[1]);
					// 		execv(args[0], args);
					// 	}
					// 	else{
					// 		waitpid(pid, &status, 0);
					// 	}
					// }
					// else{
					// 	waitpid(pid, &status, 0);
					// 	close(fd[0]);
					// 	close(fd[1]);
					// }

				}

			for (i = 0; i < instr.numTokens+1; i++){
				free(args[i]);
			}
			free(args);
			args = NULL;

			addNull(&check);
			clearInstruction(&check);
			}


		addNull(&instr);
	//	printTokens(&instr);
		clearInstruction(&instr);
	}

	return 0;
}

char* ShortResolution(char* path_str)						// I know you said to replace all strdups with malloc but i had troubles freeing the pointers.
{
	if(strlen(path_str) == 1) // is '/' and is root
	{
    if(strcmp(path_str, "/") == 0)
		  return path_str;
	}

	char* edit_path_str = strdup(path_str);

	if(edit_path_str[0] == '~')
	{
		char tmpHome[256];
		strcpy(tmpHome,getenv("HOME"));
		int i;
		for(i = 0; i < strlen(edit_path_str) - 1; i++)
		{
			edit_path_str[i] = edit_path_str[i+1];
		}
		edit_path_str[strlen(edit_path_str)-1] = '\0';
		strcat(tmpHome,edit_path_str);
		edit_path_str = strdup(tmpHome);
		//free(tmpHome);
	}

	if(edit_path_str[1] == '.' && edit_path_str[0] == '.') // .. at beginning
	{
    char tmp[256];
		strcpy(tmp,getenv("PWD")); // /home/majors/gurley/cop4610/proj1
		int i = strlen(tmp) -1;
    int indexSlash;
    for(i; i > -1; i--)
    {
      if(tmp[i] == '/')
      {
        indexSlash = i;
        break;
      }
    }
    if(strlen(edit_path_str) > 3)
    {
      int j;
      for(j = 0; j < strlen(edit_path_str) - 2; j++)
      {
        edit_path_str[j] = edit_path_str[j+2];
      }
      tmp[indexSlash] = '\0';
      strcat(tmp,edit_path_str);
      edit_path_str = strdup(tmp);
      edit_path_str[strlen(edit_path_str)-2] = '\0';
    }
    else
    {
      tmp[indexSlash] = '\0';
      edit_path_str = strdup(tmp);
    }

	}

  if(edit_path_str[0] == '.' && edit_path_str[1] != '.') // . at the beginning
	{
		char tmpPWD[256];
		strcpy(tmpPWD,getenv("PWD"));
		int i;
		for(i = 0; i < strlen(edit_path_str) - 1; i++)
		{
			edit_path_str[i] = edit_path_str[i+1];
		}
		edit_path_str[strlen(edit_path_str)-1] = '\0';
		strcat(tmpPWD,edit_path_str);
		edit_path_str = strdup(tmpPWD);
		//free(tmpPWD);
	}

  int iterator;
  for(iterator = 1; iterator < strlen(edit_path_str); iterator++)     //checking for . or .. anywhere     ex: ~/
  {
    if(edit_path_str[iterator] == '.' && edit_path_str[iterator-1] == '.')
    {
      int tmpDist;
      int endslash = iterator - 2;
      int q = endslash - 1;
      for(q; q < 1000000; q--)
      {
        if(edit_path_str[q] == '/')
        {
          tmpDist = endslash - q + 3;
          int i;
          for(i = q; i ; i++)
          {
            edit_path_str[i] = edit_path_str[i+tmpDist];
            if(edit_path_str[i+tmpDist+1] == '\0')
            {
              edit_path_str[i+1] = '\0';
              break;
            }
          }
          break;
        }
      }
    }
  }
//    *** This breaks the shell if uncommented but should do path res if somehting like cd proj1 -> cd $PWD/proj1
	// if(edit_path_str[0] != '/')
  // {
  //   char* frontPWD = (char*)malloc((strlen(getenv("PWD")) + strlen(edit_path_str) +2) * sizeof(char));
	// 	strcpy(frontPWD, getenv("PWD"));
	// 	strcat(frontPWD, "/");
	// 	strcat(frontPWD, edit_path_str);
	// 	edit_path_str = frontPWD;
	// 	free(frontPWD);
  // }

	return edit_path_str;
}

char *GetEnv(const char* name)
{
   printf("%s : %s\n",name,getenv(name));

   if(getenv(name)==NULL)
   		printf("Command not found.\n");
}

//reallocates instruction array to hold another token
//allocates for new token within instruction array
void addToken(instruction* instr_ptr, char* tok)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**) malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**) realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	//allocate char array for new token in new slot
	instr_ptr->tokens[instr_ptr->numTokens] = (char *)malloc((strlen(tok)+1) * sizeof(char));
	strcpy(instr_ptr->tokens[instr_ptr->numTokens], tok);

	instr_ptr->numTokens++;
}

void addNull(instruction* instr_ptr)
{
	//extend token array to accomodate an additional token
	if (instr_ptr->numTokens == 0)
		instr_ptr->tokens = (char**)malloc(sizeof(char*));
	else
		instr_ptr->tokens = (char**)realloc(instr_ptr->tokens, (instr_ptr->numTokens+1) * sizeof(char*));

	instr_ptr->tokens[instr_ptr->numTokens] = (char*) NULL;
	instr_ptr->numTokens++;
}

void printTokens(instruction* instr_ptr)
{
	int i;
	printf("Tokens:\n");
	for (i = 0; i < instr_ptr->numTokens; i++) {
		if ((instr_ptr->tokens)[i] != NULL)
			printf("%s [%d]\n", (instr_ptr->tokens)[i],i);
	}
}

void clearInstruction(instruction* instr_ptr)
{
	int i;
	for (i = 0; i < instr_ptr->numTokens; i++)
		free(instr_ptr->tokens[i]);

	free(instr_ptr->tokens);

	instr_ptr->tokens = NULL;
	instr_ptr->numTokens = 0;
}
