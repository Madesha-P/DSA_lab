#include <stdio.h>
//Tower of hanai
// Parameter order: n, Source (S), Destination (D), Transfer (T)
void TOH(int n, char S, char D, char T) {
    if (n == 1) {
        printf("Move disk 1 from %c to %c\n", S, D);
        return; 
    }

    TOH(n - 1, S, T, D);
    printf("Move disk %d from %c to %c\n", n, S, D);
    TOH(n - 1, T, D, S);
}

int main() {
    int disks = 3; // Matches the variable name passed below
    TOH(disks, 'A', 'C', 'B');
    return 0;
}