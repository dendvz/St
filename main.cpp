# include "functions.h"
# include "stegRGB.h"

int main(int argc, char* argv[])
{
	JpegLoader jpegLoader;
	if (argc != 2)
	{
		std::cerr << "syntax: " << argv[0] << " <file>" << std::endl;
		exit(1);
	}
	const JpegLoader::ImageInfo* pImageInfo = jpegLoader.Load(argv[1]);
	int secret = 0; //секретное число, используется для поиска блока с сообщением
	if (pImageInfo)
	{
		std::cout << "nWidth = " << pImageInfo->nWidth
			  << " nHeight = " << pImageInfo->nHeight 
			  << " nNumComponent = " << int(pImageInfo->nNumComponent) << std::endl;
/*
		image_buffer = pImageInfo->pData;

		RGB r(pImageInfo->nWidth, pImageInfo->nHeight, pImageInfo->nNumComponent);
		r.into_RGB(pImageInfo->pData, pImageInfo); 
		
		secret = r.find_block_and_hide("Hello", "Secret message");
		std::cout << r.find_block_and_return_mes("Hello", secret) << std::endl;

		r.out_RGB(pImageInfo->pData);
*/
		stbi_write_jpg("out.jpg", pImageInfo->nWidth, pImageInfo->nHeight, pImageInfo->nNumComponent, pImageInfo->pData, 100);
		return 0;
		
	}
	const JpegLoader::ImageInfo* pImageInfo1 = jpegLoader.Load("out.jpg");
	if (pImageInfo1)
	{
		image_buffer = pImageInfo1->pData;
		RGB r(pImageInfo1->nWidth, pImageInfo1->nHeight, pImageInfo1->nNumComponent);
		r.into_RGB(pImageInfo1->pData, pImageInfo1);
		std::cout << r.find_block_and_return_mes("Hello", secret) << std::endl;
	}
	return 0;
}
