# LSB_Image_Steganography
This project implements Image Steganography using the Least Significant Bit (LSB) method on BMP images. It allows users to hide and extract secret files inside a .bmp image without visible distortion.

🌟 Features

Hide .txt, .c, or .sh files inside BMP images
No visible change to the appearance of the image
Magic-string based secure decoding
Custom output stego image and secret filename support
Preserves original extension of the hidden file during decoding

🧠 How It Works

Each pixel in a BMP image contains R, G, B bytes.
This project modifies the Least Significant Bit (LSB) of these bytes to store hidden data:

Original byte: 11001010
After encoding: 11001011 → Only last bit changed

Because only the last bit of each RGB byte is modified, the visual appearance remains unchanged.

📂 Project File Structure

├── main.c

├── encode.c

├── encode.h

├── decode.c

├── decode.h

├── common.h

├── types.h

└── decoded_secret.txt (sample output)


Compile

gcc main.c encode.c decode.c -o stego

Usage

🔹 Encoding (Hiding a secret file)
./stego -e <source_image.bmp> <secret_file> <optional_output.bmp>

Examples :

./stego -e input.bmp password.txt
./stego -e input.bmp sample.c encrypted.bmp
If output BMP filename is omitted → stego.bmp is created automatically.

🔹 Decoding (Extracting a secret file)

./stego -d <stego_image.bmp> <optional_output_secret_name>

Examples:

./stego -d stego.bmp
./stego -d stego.bmp recovered_msg
If output filename is omitted → decoded_secret + extracted extension is used.

🔐 Security — Magic String Validation

During encoding, the program asks for a magic string (like a password).
During decoding, the same string must be entered — otherwise decoding fails.

🚀 Future Enhancements

-> Support PNG and JPEG formats
-> Encrypt secret data before hiding
-> GUI interface for user-friendly interaction

👨‍💻 Author

Sangamesh Kuppast
