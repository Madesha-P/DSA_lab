#include <stdio.h>
#include <stdlib.h>
// using gets methode 
int findmatch(char str[20], char pat[20], char rep[20], char ans[40]);

void main() {
char str[20], pat[20], rep[20], ans[40];
    int flag;

    printf("Enter the main String :\n");
    gets(str);
    
    printf("Enter the pattern String :\n");
    gets(pat);

    printf("Enter the replacement String :\n");
    gets(rep);
    


    flag = findmatch(str, pat, rep, ans);
    
    if (flag) {
        printf("Pattern found & Resultant String is : %s\n", ans);
    } else {
        printf("\n
            Pattern not found\n");
    }
}

int findmatch(char str[20], char pat[20], char rep[20], char ans[40]) {
    int i, j, m, c, flag, k;
    i = j = m = c = flag = 0;

    while (str[c] != '\0') {
        if (str[m] == pat[i]) {
            i++;
            m++;

            if (pat[i] == '\0') {
                flag = 1;
                
                for (k = 0; rep[k] != '\0'; k++, j++) {
                    ans[j] = rep[k];
                }
                
                c = m;
                i = 0;
            }
        } else {
            ans[j] = str[c];
            j++;
            c++;
            m = c;
            i = 0;
        }
    }
  
    ans[j] = '\0';
    return flag;
}