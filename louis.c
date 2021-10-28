#include <stdio.h>

int	main()
{
	char *louis = "louis";
	
	*(louis)++;
	printf("louis: %c\n", *louis);
	return (0);
}
