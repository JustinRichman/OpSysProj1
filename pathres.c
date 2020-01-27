#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* PathResolution(char* path_str);

int main()
{
  char* entered;
  while(1)
  {
    printf(">>>");
    scanf("%ms", &entered);
    if(strcmp(entered, "exit") == 0)
      break;
    char* newpath;
    printf("Original: %s\n", entered);
    newpath = PathResolution(entered);
    printf("Post-Res: %s\n", newpath);
    //free(newpath);
  }


  return 0;
}

char* PathResolution(char* path_str) // . (beginning), ..(beginning), ./.. (anywhere), ~
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
    int j;
    for(j = 0; j < strlen(edit_path_str) - 2; j++)
		{
			edit_path_str[j] = edit_path_str[j+2];
		}

    tmp[indexSlash] = '\0';
    strcat(tmp,edit_path_str);
    edit_path_str = strdup(tmp);
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

	return edit_path_str;
}
