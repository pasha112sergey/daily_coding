#include <stdio.h>
#include <unistd.h>
#include "../include/header.h"

int main() {
    progress_bar(N);
}

void progress_bar(int n)
{
    for (int i = 1; i <= n; i++)
    {
        printf("\r");
        printf("%s(%d/%d) ", KNRM, i, n);
        for (int j = 1; j <= i; j++)
            printf("%s#", KBLU);
        for (int j = i+1; j <= n; j++)
            printf("%s-", KMAG);
        fflush(stdout);
        sleep(1);
    }

    printf("\n%sComplete!\n", KNRM);
}