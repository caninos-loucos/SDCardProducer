#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static char buffer[512];

int main(int argc, char *argv[])
{
	FILE *input = NULL;
	FILE *output = NULL;
	size_t len, bytes_read, i, j;
	
	if (argc != 4)
	{
		printf("Sample usage: 'bin2cpp in.bin out.h var_name'\n");
		return 0;
	}
	
	printf("Input file: %s\n", argv[1]);
	printf("Output file: %s\n", argv[2]);
	printf("Variable name: %s\n", argv[3]);
	
	input = fopen(argv[1], "rb");
	
	if (!input)
	{
		printf("Could not open file: %s\n", argv[1]);
		return 0;
	} 
	
	output = fopen(argv[2], "w");
	
	if (!output)
	{
		printf("Could not create file: %s\n", argv[2]);
		fclose(input);
		return 0;
	} 
	
	fprintf(output, "const unsigned char hex_%s[] =\n{\n", argv[3]);
	
	len = j = 0;
	
	do
	{
		bytes_read = fread(buffer, 1, sizeof(buffer), input);
		
		for (i = 0; i < bytes_read; i++)
		{
			if (j == 8)
			{
				fprintf(output, "\n");
				j = 0;
			}
			
			if (j == 0) {
				fprintf(output, "\t");
			}
			
			j++;
		
			fprintf(output, "0x%02x, ", (uint32_t)(buffer[i]) & 0xFF);
		}
		
		len += bytes_read;
		
	} while (bytes_read != 0);
	
	fprintf(output, "\n};\n\n");
	
	//fprintf(output, "const int hex_%s_len = %u;\n\n", argv[3], len);
	
	fclose(input);
	fclose(output);
	
	return 0;
}

