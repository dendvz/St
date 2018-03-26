# include "functions.h"
# include "stegRGB.h"

int main(int argc, char* argv[])
{
	JpegLoader jpegLoader;
	const JpegLoader::ImageInfo* pImageInfo = jpegLoader.Load("C:\\Users\\Anna\\Desktop\\JD.jpg");
	int secret = 0; //секретное число, используется для поиска блока с сообщением
	if (pImageInfo)
	{
		image_buffer = pImageInfo->pData;

		RGB r(pImageInfo->nWidth, pImageInfo->nHeight, pImageInfo->nNumComponent);
		r.into_RGB(pImageInfo->pData, pImageInfo); 
		
		secret = r.find_block_and_hide("Hello", "Secret message");
		std::cout << r.find_block_and_return_mes("Hello", secret) << std::endl;

		r.out_RGB(pImageInfo->pData);
		stbi_write_jpg("C:\\Users\\Anna\\Desktop\\JD1.jpg", pImageInfo->nWidth, pImageInfo->nHeight, pImageInfo->nNumComponent, pImageInfo->pData, 100);
		
	}
	const JpegLoader::ImageInfo* pImageInfo1 = jpegLoader.Load("C:\\Users\\Anna\\Desktop\\JD1.jpg");
	if (pImageInfo1)
	{
		image_buffer = pImageInfo1->pData;
		RGB r(pImageInfo1->nWidth, pImageInfo1->nHeight, pImageInfo1->nNumComponent);
		r.into_RGB(pImageInfo1->pData, pImageInfo1);
		std::cout << r.find_block_and_return_mes("Hello", secret) << std::endl;
	}
	return 0;
}
