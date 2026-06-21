#include<stdio.h>
#include<string.h>
#include"decode.h"

/* Function to read and validate the decode arguments from the command line */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
	/* Checking for .bmp extension of source image file */
	char* ptr = strstr(argv[2], ".bmp");
	if(ptr== NULL || strcmp(ptr, ".bmp") != 0)
	{
		printf("Error: The input file is not '.bmp' extension....\n\n");
		return e_failure;
	}
	else
	{
		decInfo->src_image_fname = argv[2];
	}

	/* Checking if the output secret file name is passed or not */
	if(argv[3] == NULL)
	{
		strcpy(decInfo->secret_file_name,"decoded_secret");
	}
	else
	{
		/* Checking the secret file name is passed with or without the extension */
		char *qptr = strstr(argv[3], ".");
		if(qptr == NULL)
		{
			strcpy(decInfo->secret_file_name, argv[3]);
		}
		else
		{
			printf("Error : Output secret file is passed with extension....\n\n");
			printf("\tSould not pass the Output secret file with any extension....\n\n");
			return e_failure;
		}
	}

	return e_success;
}


/* Main Do-decoding function */
Status do_decoding(DecodeInfo *decInfo)
{
	if(open_image_file(decInfo) == e_failure)
	{
		printf("Error: The image is file is not opened successfully....\n\n");
		return e_failure;
	}
	else
		printf("Info : The image is file is opened successfully....\n\n");

	if(decode_magic_string_size(decInfo) == e_failure)
	{
		printf("Error: Decoding of magic string size is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Decoding of magic string size is done successfully....\n\n");

	if(decode_magic_string(decInfo) == e_failure)
	{
		printf("Error: Decoding of magic string is not successful....\n\n");
		return e_failure;
	}
	else
		printf("\nInfo : Decoding of magic string is done successfully....\n\n");

	if(decode_secret_file_extn_size(decInfo) == e_failure)
	{
		printf("Error: Decoding of size of secret file extension is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Decoding of size of secret file extension is done successfully....\n\n");

	if(decode_secret_file_extn(decInfo) == e_failure)
	{
		printf("Error: Decoding of secret file extension is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Decoding of secret file extension is done successfully....\n\n");

	if(decode_secret_file_size(decInfo) == e_failure)
	{
		printf("Error: Decoding of secret file size is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Decoding of secret file size is done successfully....\n\n");

	if(open_secret_file(decInfo) == e_failure)
	{
		printf("Error: The secret file is not opened successfully....\n\n");
		fclose(decInfo->fptr_src_image);
		return e_failure;
	}
	else
		printf("Info : The secret file is opened successfully....\n\n");

	if(decode_secret_file_data(decInfo) == e_failure)
	{
		printf("Error: Decoding of secret file data is not successfully....\n\n");
		return e_failure;
	}
	else
	{
		fclose(decInfo->fptr_src_image);
		fclose(decInfo->fptr_secret);
		printf("Info : Decoding of secret file data is done successfully....\n\n");
		return e_success;
	}

}


/* Function to open the source image file */
Status open_image_file(DecodeInfo *decInfo)
{
	// Src Image file
	decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");

	// Doing Error handling
	if (decInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->src_image_fname);

		return e_failure;
	}
	fseek(decInfo->fptr_src_image, 54, SEEK_SET);
	return e_success;
}


/* Function to decode magic string size */
Status decode_magic_string_size(DecodeInfo *decInfo)
{
	char size_buffer[32];
	fread(size_buffer, sizeof(char), 32, decInfo->fptr_src_image);
	decInfo->magic_string_length = decode_int_from_lsb(size_buffer);
	
	return e_success;
}


/* Function to decode integer value from the LSB */
int decode_int_from_lsb(char *int_buffer)
{
	int size = 0;
	for(int i = 0; i < 32; i++)
	{
		size = size | (int_buffer[i] & 1) << (31 - i);
	}
	return size;
}


/* Fucntion to decode magic string */
Status decode_magic_string(DecodeInfo *decInfo)
{
	char buffer[8], enc_magic_string[20];
	int i;
	for(i = 0; i < decInfo->magic_string_length; i++)
	{
		fread(buffer, sizeof(char), 8, decInfo->fptr_src_image);
		decInfo->magic_string[i] = decode_byte_from_lsb(buffer);
	}
	decInfo->magic_string[i] = '\0';
	printf("Enter magic_string for decoding : ");
	scanf("%s", enc_magic_string);
	if(strcmp(decInfo->magic_string, enc_magic_string) == 0)
		return e_success;
	else
		return e_failure;

}


/* Function to decode one byte of data from LSB */
char decode_byte_from_lsb(char *buffer)
{
	char ch = 0;
	for(int i = 0; i < 8;i++)
	{
		ch = ch | (buffer[i] & 1) << (7 - i);
	}
	
	return ch;
}


/* Function to decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
	char size_buffer[32];
	fread(size_buffer, 1, 32, decInfo->fptr_src_image);
	decInfo->secret_file_extn_size = decode_int_from_lsb(size_buffer);
	
	return e_success;
}


/* Function to decode secret file extension */
Status decode_secret_file_extn(DecodeInfo *decInfo)
{
	char byte_buffer[8];
	int i;
	for(i = 0; i < decInfo->secret_file_extn_size; i++)
	{
		fread(byte_buffer, 1, 8, decInfo->fptr_src_image);
		decInfo->secret_file_extn[i] = decode_byte_from_lsb(byte_buffer);
	}
	decInfo->secret_file_extn[i] = '\0';

	/* Concatinating the secret file with the extension decode from the sorce image file */
	strcat(decInfo->secret_file_name, decInfo->secret_file_extn);
	return e_success;
}


/* Function to decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo)
{
	char size_buffer[32];
	fread(size_buffer, 1, 32, decInfo->fptr_src_image);
	decInfo->secret_file_size = decode_int_from_lsb(size_buffer);

	return e_success;
}


/* Function to open secret file */
Status open_secret_file(DecodeInfo *decInfo)
{
	decInfo->fptr_secret = fopen(decInfo->secret_file_name, "wb");

	// Do Error handling
	if (decInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->secret_file_name);
		return e_failure;
	}

	return e_success;
}


/* Function to decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo)
{
	char buffer[8];

	/* Creating buffer to store the data from the source image file */
	char secret_data[decInfo->secret_file_size];
	for(int i = 0;i < decInfo->secret_file_size; i++)
	{
		fread(buffer, 1, 8, decInfo->fptr_src_image);
		secret_data[i] = decode_byte_from_lsb(buffer);
	}
	fwrite(secret_data, 1, sizeof(secret_data), decInfo->fptr_secret);

	return e_success;
}

void usage()
{
	printf("Error: Unsupported operation types!!!\n");
	printf("Usage:\n");
	printf("1. Encoding -> ./a.out -e <source.bmp file> <secret_file> <optional - output.bmp file>\n");
	printf("1. Decoding -> ./a.out -d <source.bmp file> <optional - output.txt file>\n");
}
