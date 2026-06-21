/*
Name: Sangamesh I Kuppast
Date: 18/11/2025
Description: LSB IMAGE STEGANOGRAPHY 

	-> The project hides secret data inside a BMP image using the Least Significant Bit (LSB) technique.
    -> The program embeds file contents (text, source code, etc.) into selected pixel bytes of a .bmp image without visibly altering the image.	
    -> The project supports two operations — encoding (-e) and decoding (-d).
    -> Encoding embeds a secret file into a BMP image.
    -> Decoding extracts the previously hidden file from a BMP image.

*/

#include <stdio.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char* argv[])
{
	/* Creating structure objects for Encoding and Decoding */
	EncodeInfo encInfo;
	DecodeInfo decInfo;
	if(argc > 1)
	{
		int Res = check_operation_type(argv);
		if(Res == e_encode)
		{
			if(argc >= 4 && argc <= 5)
			{
				if(read_and_validate_encode_args(argv, &encInfo) == e_success)
				{
					printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< ENCODING STARTED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					if(do_encoding(&encInfo) == e_success)
					{
						printf("Info : Encoding of secret file in '.bmp' file is successful....\n\n");
						printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< ENCODING COMPLETED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}

					else
					{
						printf("Info : Encoding of secret file in '.bmp' file is not successful....\n\n");
						printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< ENCODING FAILED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}
				}
			}
			else
			{
				printf("Error : Invalid Number of arguments for E_encode....\n\n");
			}
		}
		else if(Res == e_decode)
		{
			if(argc == 3 || argc == 4)
			{
				if(read_and_validate_decode_args(argv, &decInfo) == e_success)
				{
					printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< DECODING STARTED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					if(do_decoding(&decInfo) == e_success)
					{
						printf("Info : Decoding of secret file is done successfully....\n\n");
						printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< DECODING COMPLETED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}
					else
					{
						printf("Info : Decoding of secret file is un-successful....\n\n");
						printf("\n<<<<<<<<<<<<<<<<<<<<<<<<<<< DECODING FAILED >>>>>>>>>>>>>>>>>>>>>>>>>>>\n\n");
					}
				}
			}
			else
			{
				printf("Error : Invalid Number of arguments for E_decode....\n\n");
			}
		}
		else
		{
			usage();
		}
	}
	else
	{
		usage();
	}

	return 0;
}
