#include <stdio.h>




struct bmpImage{
	// Data read from the header of the BMP file
	unsigned char header[54]; // Each BMP file begins by a 54-bytes header
	unsigned int dataPos;     // Position in the file where the actual data begins
	unsigned int width, height;
	unsigned int imageSize;   // = width*height*3
	// Actual RGB data
	unsigned char * data;


	bmpImage()
	{
		data = NULL;
	}

	bmpImage(const char* filepath)
	{
		load(filepath);
	}


	int load(const char* filepath)
	{
		// Open the file
		FILE * file = fopen(filepath,"rb");
		if (!file){printf("Image could not be opened\n"); return 0;}

		// If not 54 bytes read : problem
		if ( fread(header, 1, 54, file)!=54 )
		{ 
	    	printf("Not a correct BMP file\n");
			return 0;
		}

	    if ( header[0]!='B' || header[1]!='M' )
	    {
	    	printf("Not a correct BMP file\n");
	    	return 0;
		}
	    
	    dataPos    = *(int*)&(header[0x0A]);
		imageSize  = *(int*)&(header[0x22]);
		width      = *(int*)&(header[0x12]);
		height     = *(int*)&(header[0x16]);

		// Some BMP files are misformatted, guess missing information
		if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
		if (dataPos==0)      dataPos=54; // The BMP header is done that way

		// Create a buffer
		data = new unsigned char [imageSize];

		// Read the actual data from the file into the buffer
		fread(data,1,imageSize,file);

		//Everything is in memory now, the file can be closed
		fclose(file);
	}
};

