#include <stdio.h>
#include <stdlib.h>

int main()
{
    int n_processes, n_resources;
    printf("enter the number of processes and resources: ");
    scanf("%d %d", &n_processes, &n_resources);

    int available[n_resources];
    int max[n_processes][n_resources];
    int need[n_processes][n_resources];
    int allocation[n_processes][n_resources];

    int done[n_processes];
    int safe_sequence[n_processes];
    for (int i = 0; i < n_processes; i++) {
        done[i] = 0;
    }

    printf("enter the available resources:\n");
    for (int i = 0; i < n_resources; i++) {
        scanf("%d", &available[i]);
    }

    printf("enter the allocated resources: ");
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &allocation[i][j]);
        }
    }

    printf("enter the maximum resources: ");
    for (int i = 0; i < n_processes; i++) {
        for (int j = 0; j < n_resources; j++) {
            scanf("%d", &max[i][j]);
        }
    }

    // calculate the need matrix
    for (int i = 0; i < n_processes; i++) { 
		for (int j = 0; j < n_resources; j++) 
			need[i][j] = max[i][j] - allocation[i][j]; 
	}

    // check for safe sequence
    int count = 0;
    for (int i = 0; i < n_processes; i++) {
        if (done[i]) continue;

        int j = 0;
        for (j = 0; j < n_resources; j++) {
            if (need[i][j] > available[j]) {
                break;
            }
        }
        if (j == n_resources) {
            safe_sequence[count++] = i;
            done[i] = 1;
            for (int k = 0; k < n_resources; k++) {
                available[k] += allocation[i][k];
            }
            i = -1;
        }
    }

    if (count == n_processes) {
        printf("safe sequence is: ");
        for (int i = 0; i < n_processes; i++) {
            printf("%d ", safe_sequence[i]);
        }
        printf("\n");
    }
    else printf("unsafe state, deadlock possible.\n");
}