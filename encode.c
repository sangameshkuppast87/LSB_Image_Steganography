#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Function Definitions */

/*  Get image size

   * Input: Image file ptr
   * Output: width * height * bytes per pixel (3 in our case)
   * Description: In BMP Image, width is stored in offset 18,
   * and height after that. size is 4 bytes
   
*/
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;

	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);

	// Return image capacity
	return width * height * 3;
}

/* 
   * Get File pointers for i/p and o/p files
   * Inputs: Src Image file, Secret file and
   * Stego Image file
   * Output: FILE pointer for above files
   * Return Value: e_success or e_failure, on file errors
*/

Status open_files(EncodeInfo *encInfo)
{
	// Src Image file
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "rb");

	// Do Error handling
	if(encInfo->fptr_src_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname);

		return e_failure;
	}

	// Secret file
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "rb");

	// Do Error handling
	if(encInfo->fptr_secret == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname);

		return e_failure;
	}

	// Stego Image file
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "wb");

	// Do Error handling
	if(encInfo->fptr_stego_image == NULL)
	{
		perror("fopen");
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname);

		return e_failure;
	}

	// No failure return e_success
	return e_success;
}


/* Function to check for operation type */
OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1], "-e") == 0)
		return e_encode;

	else if(strcmp(argv[1], "-d") == 0)
		return e_decode;

	else
		return e_unsupported;
}


/* Function to read and validate arguments passed */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	char* ptr = strstr(argv[2], ".bmp");

	//Checking for ".bmp" extension
	if(ptr== NULL || strcmp(ptr, ".bmp") != 0)				
	{
		printf("Error: The input file is not '.bmp' extension!!!\n");
		return e_failure;
	}
	else
	{
		encInfo->src_image_fname = argv[2];
		//printf("%s\n", encInfo->src_image_fname);
	}

	char* qptr = strstr(argv[3], ".");

	//Checkinhg for secret file extension
	if(qptr == NULL || ((strcmp(qptr, ".txt") != 0) && (strcmp(qptr, ".c") != 0) && (strcmp(qptr, ".sh") != 0)))
	{
		printf("Error: For secret file only '.txt', '.c', '.sh' is allowed!!!\n");
		return e_failure;
	}
	else
	{
		encInfo->secret_fname = argv[3];
	}

	//Checking if the Destination file is given or not
	if(argv[4] == NULL)
	{
		encInfo->stego_image_fname = "stego.bmp";
		return e_success;
	}
	else
	{
		char* rptr = strstr(argv[4], ".bmp");

		//If the destination file is .bmp extension or not
		if(rptr != NULL && ((strcmp(rptr, ".bmp") == 0)))
		{
			encInfo->stego_image_fname = argv[4];
			return e_success;
		}
		else
		{
			printf("Error: Output file should be of '.bmp' extension!!!\n");
			return e_failure;
		}
	}

}


/* Function to do all the encoding process */
Status do_encoding(EncodeInfo *encInfo)
{
	if(open_files(encInfo) == e_failure)
	{
		printf("Error: Failure in opening the files....\n\n");
		return e_failure;
	}
	else
		printf("Info : All the files opened successfully....\n\n");

	if(check_capacity(encInfo) == e_failure)
	{
		printf("Error: Size of secret file is larger than source image file....\n\n");
		return e_failure;
	}
	else
		printf("\nInfo : Checking of image file capacity is done....\n\n");

	if(copy_bmp_header(encInfo->fptr_src_image, encInfo->fptr_stego_image) == e_failure)
	{
		printf("Error: BMP header copying is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : BMP header copied successfully....\n\n");

	if(encode_magic_string_length(encInfo) == e_failure)
	{
		printf("Error: Encoding of Magic String length is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Encoding of Magic String length is successful....\n\n");

	if(encode_magic_string(encInfo) == e_failure)
	{
		printf("Error: Encoding Magic_String is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Magic string encoded successfully....\n\n");


	if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file), encInfo) == e_failure)
	{
		printf("Error: Encoding of secret file extension's size is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Secret file extension size encoded successfully....\n\n");

	if(encode_secret_file_extn(encInfo->extn_secret_file, encInfo) == e_failure)
	{
		printf("Error: Encoding secret file extension is not possible....\n\n");
		return e_failure;
	}
	else
		printf("Info : Secret file extension encoded successfully....\n\n");

	if(encode_secret_file_size(encInfo->secret_file_size, encInfo) == e_failure)
	{
		printf("Error: Encoding of secret file size is not successful....\n\n");
		return e_failure;	
	}
	else
		printf("Info : Secret file size encoded successfully....\n\n");

	if(encode_secret_file_data(encInfo) == e_failure)
	{
		printf("Error: Encoding of secret file data is not successful....\n\n");
		return e_failure;
	}
	else
		printf("Info : Secret file data encoded successfully....\n\n");

	if(copy_remaining_img_data(encInfo) == e_failure)
	{
		printf("Error: Copying of the rest of file data is not successful....\n\n");
		return e_failure;
	}
	else
	{
		printf("Info : Copying the rest of the data is successful....\n\n");
		fclose(encInfo->fptr_src_image);
		fclose(encInfo->fptr_stego_image);
		fclose(encInfo->fptr_secret);
		return e_success;
	}
}


/* Function to check the capacity of the Image file is enough to store the secret file data */
Status check_capacity(EncodeInfo *encInfo)
{
	uint image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image);
	 
	//Storing magic string to the structure
	printf("Enter Magic String : ");
	scanf("%s",encInfo->magic_string);

	encInfo->magic_string_length = strlen(encInfo->magic_string);
	const char* ptr = strstr(encInfo->secret_fname, ".");
	strcpy(encInfo->extn_secret_file, ptr);
	int extension_len = strlen(encInfo->extn_secret_file);
	encInfo->secret_file_size = get_file_size(encInfo->fptr_secret);

	//Calculating total size of the encoding data
	uint Encoding_Data = 54 + (encInfo->magic_string_length + 4 + extension_len + 4 + encInfo->secret_file_size) * 8;

	if(image_capacity > Encoding_Data)
		return e_success;
	else
		return e_failure;
}


/* Function to get the file size of secret file */
uint get_file_size(FILE* fptr)
{
	uint size;
	fseek(fptr, 0, SEEK_END);
	size = ftell(fptr);
	rewind(fptr);
	return size;
}


/* Function to copy BMP header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	rewind(fptr_src_image);
	char buffer[54];
	int size1 = fread(buffer, sizeof(char), 54, fptr_src_image);
	int size2 = fwrite(buffer, sizeof(char), 54, fptr_dest_image);
	if(size1 == 54 && size2 == 54)	
		return e_success;
	else
		return e_failure;
}


/* Function to encode magic string length */
Status encode_magic_string_length(EncodeInfo* encInfo)
{
	char buffer[32];
	fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);
	encode_int_to_lsb(encInfo->magic_string_length, buffer);
	fwrite(buffer, sizeof(char), 32, encInfo->fptr_stego_image);
	return e_success;
}


/* Function to encode magic string */
Status encode_magic_string(EncodeInfo* encInfo)
{
	if(encode_data_to_image(encInfo->magic_string, strlen(encInfo->magic_string), encInfo) == e_success)
		return e_success;
	else
		return e_failure;
}


/* Function to encode data to image */
Status encode_data_to_image(const char* data, int size, EncodeInfo* encInfo)
{
	for(int i = 0; i < size; i++)
	{
		unsigned char buffer[8];
		fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);
		encode_byte_to_lsb(data[i], buffer);
		fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
	}
	return e_success;
}


/* Function to encode byte(character value) to LSB */
Status encode_byte_to_lsb(char data, unsigned char* image_buffer)
{
	for (int i = 0; i < 8; i++)
	{
		int bit_pos = 7 - i;						            // reverse the order
		unsigned char bit = (data >> bit_pos) & 1;			    // extract bit from MSB side
		image_buffer[i] = (image_buffer[i] & 0xFE) | bit;		// clear LSB and insert bit
	}
	return e_success;
}


/* Function to encode secret file extension size */
Status encode_secret_file_extn_size(int size, EncodeInfo *encInfo)
{
	char buffer[32];
	fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);
	encode_int_to_lsb(size, buffer);
	fwrite(buffer, sizeof(char), 32, encInfo->fptr_stego_image);
	return e_success;
}


/* Function to encode size(integer value) to LSB */ 
Status encode_int_to_lsb(int size, char* buffer)
{
	for(int i = 0; i < 32; i++)
	{
		int bit_pos = 31 - i;
		unsigned char bit = (size >> bit_pos) & 1;
		buffer[i] = (buffer[i] & 0xFE) | bit;
	}
	return e_success;
}


/* Function to encode secret file extension */
Status encode_secret_file_extn(const char *file_extn, EncodeInfo *encInfo)
{
	int size = strlen(encInfo->extn_secret_file);
	for(int i = 0; i < size; i++)
	{
		unsigned char buffer[8];
		fread(buffer, sizeof(buffer), 1, encInfo->fptr_src_image);
		encode_byte_to_lsb(file_extn[i], buffer);
		fwrite(buffer, sizeof(buffer), 1, encInfo->fptr_stego_image);
	}
	return e_success;
}


/* Function to encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo)
{
	char buffer[32];
	fread(buffer, sizeof(char), 32, encInfo->fptr_src_image);
	encode_int_to_lsb(file_size, buffer);
	fwrite(buffer, sizeof(char), 32, encInfo->fptr_stego_image);
	return e_success;
}


/* Function to encode secret file data */
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	unsigned char buffer[8];
	int ch;

	/* Getting the data from the secret file data character by character */
	while((ch = fgetc(encInfo->fptr_secret)) != EOF)				
	{
		fread(buffer, sizeof(char), 8, encInfo->fptr_src_image);
		encode_byte_to_lsb(ch, buffer);
		fwrite(buffer, sizeof(char), 8, encInfo->fptr_stego_image);
	}
	return e_success;
}


/* Function to copy the remaining data from the source image file to the Destination image file */
Status copy_remaining_img_data(EncodeInfo* encInfo)
{
	int ch;
	while((ch = fgetc(encInfo->fptr_src_image)) != EOF)
	{
		fputc(ch, encInfo->fptr_stego_image);
	}
	return e_success;
}
