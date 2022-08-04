# OpenCV 模板匹配源码解析

## 假设
- Source/Input - n x n
- Template - m x m
- Result/Output - (n-m+1) x (n-m+1)

## 匹配方法
- `CV_TM_CCORR`或`CV_TM_CCORR_NORMED`
  - 相关匹配法 Cross Correlation
  - 点积:
    - 将每一对像素相乘
    - 求和所有点积
- `CV_TM_CCOEFF`或`CV_TM_CCOEFF_NORMED`
  - 相关系数匹配法 Cross Coefficient
  - 类似于互相关，但用它们的协方差进行了归一化
- `CV_TM_SQDIFF`或`CV_TM_SQDIFF_NORMED`
  - 平方差匹配法 Sum of Square Differences (or SSD)
  - 简单欧氏距离(平方)
    - 将每一对像素进行相减
    - 将每次的相减结果平方
    - 求和所有平方
  - 对于归一化
    - 分母项与上面相同，但除以一个因子，由乘积的平方根计算得到:
      - 模板的和的平方
      - 输入图像的和的平方

[参考链接 1](https://docs.opencv.org/3.4/df/dfb/group__imgproc__object.html#gga3a7850640f1fe1f58fe91a2d7583695dab65c042ed62c9e9e095a1e7e41fe2773)
[参考链接 2](https://stackoverflow.com/questions/48799711/explain-difference-between-opencvs-template-matching-methods-in-non-mathematica)

## 模板匹配
```c++
void cv::matchTemplate(
    InputArray image, 
    InputArray templ, 
    OutputArray result, 
    int method, 
    InputArray mask = noArray()
)		
```
**基本过程：**
1. 使用 Instrumentation framework 测试系统性能（可开启可关闭）
2. 分别获取输入图像的类型、深度和通道数的值
3. 检查 method 的值是否正确
4. 检查输入图像的深度，即每一个像素的位数（bits）是否为8位或32位
5. 检查输入图像的[类型](https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html#af2d2652e552d7de635988f18a84b53e5)是否和模板图像相同
6. 检查输入图像的维度是否超过二维
7. 如果 mask 的值不为空，调用 `cv::matchTemplateMask(_img, _templ, _result, method, _mask)`
8. 判断输入图像与模板图像的大小关系，如果输入图像比匹配图像小，则将原始图像作为模板，原来的模板图像作为搜索图
9. 如果计算机支持 OpenCL 功能，则运行`ocl_matchTemplate()`，该函数的实现在 opencl_kernels_imgproc.hpp 中；如果不支持该功能，则运行常规代码（CPU）
10. 计算相关性矩阵大小，将 correlation size 设置为 result 的大小，并创建 result `_result.create(corrSize, CV_32F)`，创建的 result 中各个位置的值为0
11. 如果该程序在 Intel 处理器上运行，则调用`CV_IPP_RUN_FAST(ipp_matchTemplate(img, templ, result, method))`
12. 计算互相关性（Cross Correlation）`crossCorr(img, templ, result, Point(0,0), 0, 0)`
13. 进行常规模板匹配`common_matchTemplate(img, templ, result, method, cn)`

### CV_INSTRUMENT_REGION
[Instrumentation framework for OpenCV performance test system](https://github.com/opencv/opencv/pull/7101)
- 跟踪OpenCV函数内部的实现执行，并输出到测试日志
- 跟踪在管道中的执行时间权重
- 可以关闭，关闭后不消耗资源

### CV_OCL_RUN
- OpenCL（全称 Open Computing Language，开放运算语言）是第一个面向异构系统通用目的并行编程的开放式、免费标准。
- OpenCL 由两部分组成，一是用于编写 kernels（在 OpenCL 设备上运行的函数）的语言，二是用于定义并控制平台的 API (函数)。
- OpenCL 提供了基于任务和基于数据两种并行计算机制，它极大地扩展了 GPU 的应用范围，使之不再局限于图形领域。
- 所有专门的 ocl 实现都隐藏在通用的 C 算法接口后面。现在函数的执行路径可以在运行时动态选择：CPU 或 OpenCL；这种机制也被称为“透明API”。
- 如果计算机不能使用 OpenCL 功能（没有 GPU 或没有 OpenCL 驱动程序），则运行常规代码（CPU）。
- 也可以在常规代码之间切换，或在代码中使用 OpenCL 版本。
- 如果`setUseOptimized()`或`setUseOpenCL()`设置为 false，则将使用常规代码。
  
[参考链接 1](https://stackoverflow.com/questions/42411157/what-is-the-macro-cv-ocl-run-used-for-in-opencv)
[参考链接 2](https://docs.opencv.org/3.0-rc1/db/dfa/tutorial_transition_guide.html)
[参考链接 3](https://blog.csdn.net/github_38148039/article/details/109469238)

### UMat
- 一个统一的抽象 cv::UMat，允许使用 CPU 或 OpenCL 代码实现相同的 API，而不需要显式调用 OpenCL 加速版本。这些功能如果系统中存在，则使用支持 opencl 的图形处理器，否则会自动切换到 CPU 操作。
- 一般来说，cv::UMat 是 C 类，和 cv::Mat 非常相似。但是实际的 UMat 数据可以位于常规系统内存、专用显存或共享内存中。
- cv::UMat 旨在以一种方便的方式隐藏与 OpenCL 设备的数据交换。

[参考链接 1](https://stackoverflow.com/questions/33602675/what-is-the-difference-between-umat-and-mat-in-opencv)
[参考链接 2](https://docs.opencv.org/3.0-rc1/db/dfa/tutorial_transition_guide.html)

### 使用 Intel® Integrated Performance Primitives (IPP) 进行加速
ipp全称：Intel® Integrated Performance Primitives 英特尔®集成性能原语（Intel®IPP），它是一种软件库，提供了广泛的功能，包括通用信号和图像处理、计算机视觉、数据压缩和字符串操作。

#### CV_IPP_RUN_FAST
如果在英特尔的处理器上使用，OpenCV 就会自动使用一种免费的英特尔集成性能原语库（IPP）的子集，IPP 8.x(IPPICV)。IPPICV 可以在编译阶段链接到 OpenCV，这样一来，会替代相应的低级优化的 C 语言代码（在 cmake 中设置`WITH_IPP=ON/OFF`开启或者关闭这一功能，默认情况为开启）。使用 IPP 获得的速度提升非常可观。

[参考链接](https://blog.csdn.net/github_38148039/article/details/109469238)

#### ipp_matchTemplate
```c++
static bool ipp_matchTemplate(
    Mat& img, 
    Mat& templ, 
    Mat& result, 
    int method
)
```


## 计算互相关性（Cross Correlation）
```c++
void crossCorr(
    const Mat& img, 
    const Mat& _templ, 
    Mat& corr,
    Point anchor, 
    double delta, 
    int borderType 
)
```
**基本过程：**
1. 定义变量
   1. blockScale
   2. minBlockSize
   3. buf
   4. 输入图像、模板图像和 corr 图像的深度
   5. 输入图像、模板图像和 corr 图像的通道数量
2. 检查输入图像、模板图像和 corr 图像的维度
3. 检查模板图像的深度是否超过限制，如果超过，将模板图像的深度转化成 CV_32F 和输入图像深度中的最大值
4. 检查模板图像的深度是否符合要求
5. 检查 corr 图像的大小是否符合要求
6. 获得最大深度值 maxDepth
7. 声明变量
   1. blockSize
   2. dftsize
8. 计算 blockSize 的 width 和 height
9. 调用函数`cv::getOptimalDFTSize`计算 dftsize 的 width 和 height
10. 根据 dftsize 重新计算 blockSize
11. 创建 dftTempl 和 dftImg
12. 如果模板图像的通道数大于1且模板图像的深度不是最大深度，则根据模板大小计算 bufSize
13. 如果输入图像的通道数大于1且输入图像的深度不是最大深度，则重新计算并更新 bufSize 的值
14. 如果 corr 图像的通道数大于1或输入图像的通道数大于1，且 corr 图像的深度不是最大深度，则重新计算并更新 bufSize 的值
15. 根据 bufSize 更新 buf 的大小
16. 计算各模板平面的 DFT
    1.  声明相关变量
        1.  yofs
        2.  src
        3.  dst
        4.  dst1
    2.  如果模板图像的通道数大于1，将 src 设置为 dst1，并调用`mixChannels(&templ, 1, &src, 1, pairs, 1)`将指定的通道合并到第一个通道
    3.  如果 src 不等于 dst1，将 src 设置为 dst1
    4.  如果 dst 的列数大于模板图像的列数，将多出的列的值设为0
    5.  使用`c->apply(dst.data, (int)dst.step, dst.data, (int)dst.step)`将更新后的 dst 应用到之前创建的 DFT2D 的结构上
17. 

### cvRound
将浮点数舍入到最接近的整数。

### 获取 DFT 的最佳大小
DFT - Discrete Fourier Transform
```c++
int cv::getOptimalDFTSize(int vecsize)
```
- 返回给定矢量大小的最佳DFT大小。
- DFT性能不是一个矢量大小的单调函数。因此，当计算两个阵列的卷积或执行一个阵列的光谱分析时，通常有意义的是用0填充输入数据，以获得更大的阵列，可以比原始阵列转换快得多。
  - 大小为2的幂次(2、4、8、16、32，…)的数组处理速度最快。
  - 不过，对于大小为2、3和5的乘积的数组(例如，300 = 5*5*3*2*2)，处理起来也相当高效。
- 函数`cv::getOptimalDFTSize`返回大于或等于 vecsize 的最小数字 N，以便有效地处理大小为N的向量的 DFT。在当前的实现中 N = 2 ^p^ * 3 ^q^ * 5 ^r^ 对于某个整数 p, q, r。
- 如果 vecsize 太大(非常接近 INT_MAX)，函数返回一个负数。

### CV_ELEM_SIZE
计算每个像素点所需的字节总数

### 混合通道 Mix Channels
```c++
void cv::mixChannels(
    const Mat * src,
    size_t nsrcs,
    Mat * dst,
    size_t ndsts,
    const int * fromTo,
    size_t npairs 
)
```
将指定的通道从输入数组复制到输出数组的指定通道。


例子：
```c++
Mat bgra( 100, 100, CV_8UC4, Scalar(255,0,0,255) );
Mat bgr( bgra.rows, bgra.cols, CV_8UC3 );
Mat alpha( bgra.rows, bgra.cols, CV_8UC1 );
// forming an array of matrices is a quite efficient operation,
// because the matrix data is not copied, only the headers
Mat out[] = { bgr, alpha };
// bgra[0] -> bgr[2], bgra[1] -> bgr[1],
// bgra[2] -> bgr[0], bgra[3] -> alpha[0]
int from_to[] = { 0,2, 1,1, 2,0, 3,3 };
mixChannels( &bgra, 1, out, 2, from_to, 4 );
```

### DFT2D apply()
```c++
virtual void cv::hal::DFT2D::apply(
    const uchar * src_data,
    size_t src_step,
    uchar * dst_data,
    size_t dst_step 
)
```

## 常规模板匹配
```c++
static void common_matchTemplate(
    Mat& img,
    Mat& templ,
    Mat& result,
    int method,
    int cn
)
```