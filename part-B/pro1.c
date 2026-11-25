/* Standard library headers used for input/output and program termination. */
#include <stdio.h>
#include <stdlib.h>

/* Custom string operations. */

/* Return the number of characters in str1, excluding the null terminator. */
int str_len(char str1[])
{
	int i = 0;
	while (str1[i] != '\0')
		i++;
	return i;
}

/* Append str2 to the end of str1. */
void str_cat(char str1[], char str2[])
{
	int i = str_len(str1), j = 0;
	while (str2[j] != '\0')
	{
		str1[i] = str2[j];
		i++, j++;
	}
	str1[i] = '\0';
}

/* Compare str1 and str2; return zero when they are equal. */
int str_cmp(char str1[], char str2[])
{
	int i = 0;
	while (str1[i] != '\0' && str2[i] != '\0') {
		if (str1[i] != str2[i]) {
			return str1[i] - str2[i];
		}
		i++;
	}
	return str1[i] - str2[i];
}

/* Reverse str1 in place by swapping characters from both ends. */
void str_rev(char str1[])
{
	int i = 0, j = str_len(str1) - 1;
	char temp;
	while (i < j) {
		temp = str1[i];
		str1[i] = str1[j];
		str1[j] = temp;
		i++, j--;
	}
}

/* Display the string-operation menu and process the selected option. */
void main()
{
	int choice;
	char str1[100], str2[100];
	while (choice<6) {
		printf("\n1 = String length\n2 = String concatenation\n3 = String comparison\n4 = String reversal\n5 = Exit\nEnter your choice: ");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			/* Read a string and display its length. */
			printf("Enter string to find length: ");
			gets(str1);
			printf("String length = %d\n", str_len(str1));
			break;
		case 2:
			/* Read two strings, append the second to the first, and display the result. */
			printf("Enter String-1:\n");
			gets(str1);
			printf("Enter String-2:\n");
			gets(str2);
			str_cat(str1, str2);
			printf("Concatenated string = %s\n", str1);
			break;
		case 3:
			/* Read two strings and report whether they are equal. */
			printf("Enter String-1:\n");
			gets(str1);
			printf("Enter String-2:\n");
			gets(str2);
			if (str_cmp(str1, str2) == 0) {
				printf("Both strings are the same.\n");
			} else {
				printf("Both strings are not the same.\n");
			}
			break;
		case 4:
			/* Read a string, reverse it in place, and display the result. */
			printf("Enter a string to reverse: ");
			gets(str1);
			str_rev(str1);
			printf("Reversed string = %s\n", str1);
			break;
		/* Exit the program when option 5 is selected. */
		case 5: exit(0);
		default: printf("Invalid choice.\n");
			break;
		}
	}
}