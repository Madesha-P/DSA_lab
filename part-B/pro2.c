#include <stdio.h>
void main()
{
	int i, j, key, a[50], n;
	printf("enter number of array elements:\n");
	scanf("%d", &n);
	printf("enter array elements:\n");
	for (i = 0; i < n; i++)
		scanf("%d", &a[i]);
	for (i = 1; i < n; i++)
	{
		key = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			j--;
		}
		a[j + 1] = key;
	}
	printf("sorted array:\n");
	for (i = 0; i < n; i++)
		printf("%d\n", a[i]);
}