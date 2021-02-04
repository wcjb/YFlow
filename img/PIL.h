/**
 * @file PIL.h
 * @brief 整合stb三个图片处理头文件，提供高级API
 * @author 殉道者 (wcjbyjx@gmail.com)
 * @version 0.0.1
 * @date 2021-01-27
 * @copyright Copyright © 2014-2021 weichijunbo.
 * @attention       目前在使用时发现，读取图片并进行resize（缩小，降低分辨率）再保存，图片
 * 大小会增大，具体原因不明。
 * *********************************************************************************
 * @par ChangeLog:
 * <table>
 * <tr><th>Date       <th>Version <th>Author  <th>Description
 * <tr><td>2021-01-27 <td>0.0.1    <td>殉道者     <td>内容
 * </table>
 * *********************************************************************************
 */
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image_resize.h"
#include <iostream>
using namespace std;
///图片加载、缩放即保存功能来源https://github.com/nothings/stb
// int main() {
//     std::cout << "Hello, STB_Image" << std::endl;

//     string inputPath = "C:\\Users\\wcjb\\Documents\\Dataset\\hg.jpg";
//     int iw, ih, n;
    
//     // 加载图片获取宽、高、颜色通道信息
//     unsigned char *idata = stbi_load(inputPath.c_str(), &iw, &ih, &n, 0);

//     int ow = iw / 2;
//     int oh = ih / 2;
//     auto *odata = (unsigned char *) malloc(ow * oh * n);

//     // 改变图片尺寸
//     stbir_resize(idata, iw, ih, 0, odata, ow, oh, 0, STBIR_TYPE_UINT8, n, STBIR_ALPHA_CHANNEL_NONE, 0,
//                  STBIR_EDGE_CLAMP, STBIR_EDGE_CLAMP,
//                  STBIR_FILTER_BOX, STBIR_FILTER_BOX,
//                  STBIR_COLORSPACE_SRGB, nullptr
//     );

//     string outputPath = "C:\\Users\\wcjb\\Documents\\Dataset\\hg_out.jpg";
//     // 写入图片
//     stbi_write_png(outputPath.c_str(), ow, oh, n, odata, 0);

//     stbi_image_free(idata);
//     stbi_image_free(odata);
//     return 0;
// }