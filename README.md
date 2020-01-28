Operating Systems Project 1- Shell Implementation

The purpose of this project is to design and implement a basic shell interface that supports i/o redirection, pipes, background processing,
and a series of basic built in functions as specified.

		File listing
			The only file used to execute our program is:
			1) shell.c

		Make file description
			To compile shell.c use the following commands:
			-call make
			-creates an executable called shell

		Documentation of Group Member Contribution
			Group Members include : Tyler Gurley, Justin Richman, and Samantha Chaves. The work distribution is as follows:

			1)Prompt
				All of us worked on Prompt

			2)Shortcut Resolution
				Tyler and Samantha worked together to complete Shortcut Resolution

			3)$PATH Resolution
				Samantha completed $PATH Resolution

			4)Execution
				Justin completed execution

			5)I/O Redirection
				Justin and Samantha worked together to complete piping
				Samantha completed the error checking for i/o redirection

			6)Pipes
				Justin and Samantha worked together to complete piping

			7)Background Processing
				Samantha completed the error checking for invalid syntax

			8)Built-Ins
				Tyler completed all built-ins except for jobs

		Program Bugs
			With the time we had a lotted for the program, we were unable to execute pipes correctly. The reason being, we had a segmentation fault that was undetectable. This segmentation fault was preventing us from checking if the *commented* code we have written for pipes was correct or not. We did however, complete the error checking for invalid syntax.

			It should also be noted that shell.c does not check for both input and output redirection within one command (e.g CMD < FILE_IN > FILE_OUT vice versa). We did however, complete the error checking for invalid syntax.

			For background processing we only completed the error checking for invalid syntax.
