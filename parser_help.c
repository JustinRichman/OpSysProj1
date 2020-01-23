//COP4610
//Project 1 Starter Code
//example code for initial parsing

//*** if any problems are found with this code,
//*** please report them to the TA


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

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
void ShortResolution( char* arg2);

int main() {
	char* token = NULL;
	char* temp = NULL;

	instruction instr;
	instr.tokens = NULL;
	instr.numTokens = 0;


	while (1) {

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

					addToken(&instr,specialChar);

					start = i + 1;
				}
				// if (token[i]=='$'){
				// 	char env[10];
				// 	strcpy(env, &token[1]);					//copy everything after $
				// 	GetEnv(env);							//use getenv function to return appropriate output
				// }

			}

			if (start < strlen(token)) {
				memcpy(temp, token + start, strlen(token) - start);
				temp[i-start] = '\0';
				addToken(&instr, temp);
			}

			//free and reset variables
			free(token);
			free(temp);

			token = NULL;
			temp = NULL;
		} while ('\n' != getchar());    //until end of line is reached

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		if(strcmp(instr.tokens[0], "echo") == 0) //runs echo
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

		if(strcmp(instr.tokens[0], "cd") == 0) //runs cd
			ShortResolution(instr.tokens[1]);


		addNull(&instr);
	//	printTokens(&instr);
		clearInstruction(&instr);
	}

	return 0;
}

void ShortResolution( char* arg2)
{
	if(strcmp(arg2, "..") == 0)			//checks if second token is '..'
	{
		char buffer[100];
		getcwd(buffer,100);
		chdir("..");

		   if(strlen(buffer)==1)    				 //check if you just have'/'
			 printf("You are at root.\n");
			else
			{
				 getcwd(buffer,100);
				 ChangeDirectory(buffer);
			}
	}

	else if(strcmp(arg2 ,".") == 0)		//checks if second token is '.'
	{
		char buffer[100];
		getcwd(buffer,100);							//just stay at the same directory
	}

	else if(strcmp(arg2 ,"~") == 0)		//checks if second token is '~'
		ChangeDirectory(getenv("HOME"));			//go back to HOME


	else if(strcmp(arg2 ,"/") == 0)		//checks if second token is '\'
		ChangeDirectory("/");			//goes back to root


	// else if(strcmp(instr.tokens[1] ,NULL) == 0)  //THERE IS NO SECOND ARGUMENT
	// {
	// 	printf("No second arg\n");
	// }
	else
	{												//checks if path in second token is valid
		char buffer[100];
		strcpy(buffer,arg2);

		if (arg2[0]=='\\')				//takes the \pathname format
		{
			char newpath[100];
			strcpy(newpath,arg2);
			int i,len=strlen(newpath);				//removes the '\' before the path name

				for(i=1;i<len;i++)
					newpath[i-1]=newpath[i];

			newpath[i-1]='\0';

				if(chdir(newpath)!=0)
					printf("No such file or directory.\n");

				else
				{
					 getcwd(buffer,100);
					 ChangeDirectory(buffer);
				}
		}

		else                                          //checks if second token is a regular directory name without '\'
		{
			if(chdir(buffer)!=0)
				printf("No such file or directory.\n");

			else
			{
				 getcwd(buffer,100);
				 ChangeDirectory(buffer);
			}
		}
	}
}
void ChangeDirectory(char* buf)
{
	//printf("Passing %s to function\n",buf );
	int ret=chdir(buf);
	if(ret==0)
		setenv("PWD",buf,1);			//change PWD to CWD
	else
		printf("No working directory\n");
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
