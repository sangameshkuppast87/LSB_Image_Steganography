#ifndef DECODE_H
#define DECODE_H

#include "types.h"

typedef struct _DecodeInfo
{
	/*Source file data*/
	char *src_image_fname;
	FILE* fptr_src_image;

	/* Secret File Info */
        char secret_file_name[20];                                      //Store the secret_file_name
        FILE *fptr_secret;                                              //File pointer for secret_file
        int secret_file_extn_size;
	char secret_file_extn[5];
        int secret_file_size;

	 /* Magic String */
        char magic_string[20];
        int magic_string_length;

}DecodeInfo;


/* Function Read and validate the decode arguments */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Function to do main-decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Function to open source image file */
Status open_image_file(DecodeInfo *decInfo);

/* Function to decode magic string size */
Status decode_magic_string_size(DecodeInfo *decInfo);

/* Function to decode magic string */
Status decode_magic_string(DecodeInfo *decInfo);

/* Function to decode secret file extension size */
Status decode_secret_file_extn_size(DecodeInfo *decInfo);

/* Function to decode file extension*/
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Function to decode secret file size */
Status decode_secret_file_size(DecodeInfo *decInfo);

/* Function to open decoded secret file */
Status open_secret_file(DecodeInfo *decInfo);

/* Function to decode secret file data */
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Function to decode 1-byte (character) data from LCB */
char decode_byte_from_lsb(char *buffer);

/* Function to decode integer value from LSB */
int decode_int_from_lsb(char *int_buffer);

/* Usage printing function */
void usage();

#endif
