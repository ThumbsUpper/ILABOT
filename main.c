#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

typedef struct {
	int x;
	int y;
} Vector2;
typedef struct {
	int x;
	int y;
	int z;
} Vector3;

int readBmpPixels(FILE *file, uint32_t pixelOffset, Vector2 imgSize, uint8_t *buffer) {
	size_t rowBytes = (size_t)imgSize.x * 3;
	size_t rowStride = (rowBytes + 3) & ~(size_t)3;
	uint8_t *row = malloc(rowStride);
	if (row == NULL) { return -1; }
	if (fseek(file, (long)pixelOffset, SEEK_SET) != 0) {
		free(row);
		return -1;
	}
	for (int pixelRow = 0; pixelRow < imgSize.y; ++pixelRow) {
		if (fread(row, 1, rowStride, file) != rowStride) {
			free(row);
			return -1;
		}
		memcpy(buffer + (size_t)pixelRow * rowBytes, row, rowBytes);
	}
	free(row);
	return 0;
}

void checkImages(Vector3 *img, Vector3 *imgcheck, uint8_t *differience, Vector2 imgSize) {
	size_t pixelTotal = (size_t)imgSize.x * imgSize.y;
	for (size_t i = 0; i < pixelTotal; ++i) {
		differience[i] = img[i].x != imgcheck[i].x ||
						 img[i].y != imgcheck[i].y ||
						 img[i].z != imgcheck[i].z ? 255 : 0;
	}
}

int main(void) {

	// file opeining code
	FILE *img = fopen("current.bmp", "r");
	FILE *imgcheck = fopen("prev.bmp", "r");
	FILE *imgOutput = fopen("imgOutput.ppm", "wb");
	FILE *imgcheckOutput = fopen("imgcheckOutput.ppm", "wb");
	FILE *differienceOutput = fopen("differienceOutput.pgm", "wb");

	if (img == NULL || imgcheck == NULL || imgOutput == NULL || imgcheckOutput == NULL || differienceOutput == NULL) { printf("File not successfully opened\n"); return 1; }

	// Image Processing Code
	// Load Image into Buffer
	uint8_t imgHeaderBuffer[0x20];
	size_t bytesRead = fread(imgHeaderBuffer, 1, 0x20, img);
	if (bytesRead == 0) { printf("Error reading from file\n"); return 1; }
	
	// Define Image Specifications
	Vector2 imgSize;
	uint32_t startimg = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];

	//Send img to be put into a buffer
	uint8_t imgBuffer;
	if ((imgBuffer = malloc(sizeof(img) - startimg)) == 0) { printf("Could not allocate space\n"); return 1;}
	if (!readBmpPixels(img, startimg, imgSize, imgBuffer)) { printf("Error reading bmp file\n"); return 1; }

	// Image Check Processing Code
	// Load Image Check into Buffer
	bytesRead = fread(imgHeaderBuffer, 1, 0x20, imgcheck);
	if (bytesRead == 0) { printf("Error reading from file\n"); return 1; }
	
	// Define Image Specifications
	Vector2 imgcheckSize;
	uint32_t startimgcheck = *(uint32_t*)&imgHeaderBuffer[0x0A];
	imgcheckSize.x = *(uint32_t*)&imgHeaderBuffer[0x12];
	imgcheckSize.y = *(uint32_t*)&imgHeaderBuffer[0x16];

	//Send img to be put into a buffer
	uint8_t imgcheckBuffer;
	if ((imgcheckBuffer = malloc(sizeof(imgcheck) - startimgcheck)) == 0) { printf("Could not allocate space\n"); return 1;}
	if (!readBmpPixels(imgcheck, startimgcheck, imgcheckSize, imgcheckBuffer)) { printf("Error reading bmp file\n"); return 1; }

	
	uint8_t *differience = malloc(imgSize.x * imgSize.y);
	checkImages(img, imgcheck, differience, imgSize);
	fprintf(differienceOutput, "P5\n%d %d\n255\n", imgSize.x, imgSize.y);
	for (int i = 0; i < imgSize.x * imgSize.y; ++i) {
		uint8_t *differienceLoc = &differience[imgSize.x*imgSize.y-1-i];
		if (fwrite(differienceLoc, 1, 1, differienceOutput) != 1) {
	    perror("Writing PGM pixels");
	    return 1;
		}
	}

	// close the files
	free(imgBuffer);
	free(imgcheckBuffer);
	free(differience);
    fclose(imgcheck);
    fclose(img);
    fclose(imgOutput);
    fclose(imgcheckOutput);
	fclose(differienceOutput);
    return 0;
}