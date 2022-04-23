// Noah Green
// CS 344: Assignment 1

// citation: https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C
// citation: found several examples that were used to assist at: https://github.com/gaspardpetit/base64/
// citation: was able to figure out the base64 algorithm using: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <inttypes.h>


// Check that uint8_t type exists
#ifndef UINT8_MAX
#error "No support for uint8_t"
#endif


static char const alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"			// Sets the alphabet index
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/=";


int main(int argc, char *argv[])
{
  size_t count = 0;
  uint8_t in[3];
  uint8_t out[4];
  FILE* input_fd = stdin;
  FILE* output = stdout;

  // check if too many arguments have been passed
  if(argc > 2) {
    printf("Please specify a path to a valid file or\n");
    printf("leave the argument blank.\n");
    exit(1);
  }

  // If more than 1 argument is passed (and must be less than 3, see above), compare str argument with "-"
  if(argc > 1){
    if(strcmp(argv[1], "-") < 0 || strcmp(argv[1], "-") > 0) {
      input_fd = fopen(argv[1], "rb");  // else, set the file descriptor to the str argument file
      if (input_fd == NULL){
		    fprintf(stderr, "Could not open %s\n", argv[1]);	// if file descriptor is NULL, pass error and exit
		    exit(1);
      }
    }
  }


    while(1) {
      size_t in_array_bytes = sizeof(in) / sizeof(*in);		// find # of data items to be read
      size_t out_array_bytes = sizeof(out) / sizeof(*out);    // find # of data items to be written
      size_t tobe_read = fread(in, sizeof(*in), in_array_bytes, input_fd);		// read


      if(tobe_read == 0) {			    	// if a 0 is returned, file may be at an end
        if(feof(input_fd)) {				// check if input_fd has ended
          break;
          }
        }
      if(tobe_read == 3){						                  // if fread(3) returns 3 bytes
    	out[0] = alphabet[in[0] >> 2];			          // perform base64 algorithm
        out[1] = alphabet[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        out[2] = alphabet[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
        out[3] = alphabet[in[2] & 0x3f];
      } else if(tobe_read == 2) {			                // if fread(3) returns 2 bytes
        out[0] = alphabet[in[0] >> 2];
        out[1] = alphabet[((in[0] & 0x03) << 4) | (in[1] >> 4)];
        out[2] = alphabet[((in[1] & 0x0f) << 2)];
        out[3] = alphabet[64];				               // since 3 bytes were not read, sets a "=" for padding for last index
      } else if(tobe_read == 1) {			              // if fread(3) returns 1 bytes
        out[0] = alphabet[in[0] >> 2];
        out[1] = alphabet[((in[0] & 0x03) << 4)];
        out[2] = alphabet[64];				            // since 3 bytes were not read, sets a "=" for padding for last 2 indices
        out[3] = alphabet[64];
            }
      fwrite(out, sizeof(*out), out_array_bytes, output);         	// write result to output

      count += 4;          			// increase count by number of indices written to
      if(count == 76) {				// after 76 characters are written, print new line
        printf("\n");
        count = 0;					// reset count
        }
      
    }
  printf("\n");							// new line at end of run
}
