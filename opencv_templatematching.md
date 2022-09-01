# OpenCV 模板匹配源码解析

[source code](https://github.com/opencv/opencv/blob/4.x/modules/imgproc/src/templmatch.cpp)

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
**基本过程：**
1. 判断输入图像的通道数不是1，如果是，则返回 false
2. 判断模板函数的大小是否与图像大小相当，如果是，则返回 false
3. 根据匹配方法调用对应的函数

## 计算互相关性（Cross Correlation）- 快速傅里叶变换（Fast Fourier Transform）
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

[如何理解傅里叶变换](https://github.com/JasmineCAicai/Unique-Graphics/blob/master/FourierTransform.pdf)

kernel - template image

**主要过程：**
1. 计算出合适的 dft 后的矩阵大小
2. 调整图像大小，填充图像边缘
3. 将核部分进行 dft 操作
4. 将原图像进行 dft 操作，并在频域空间上将原图像与模板图像相乘，计算相关性
5. 将图像还原到原来的大小（inverse DFT）

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
17. 定义变量
    1.  tileCountX
    2.  tileCountY
    3.  tileCount
    4.  wholeSize
    5.  roiofs
    6.  img0
18. 判断边界类型，如果`borderType & BORDER_ISOLATED`为0，则调整子矩阵的大小并定位其在父矩阵中的位置
19. 定义变量
    1.  cF
    2.  cR
    3.  f
    4.  f_inv
20. 按块计算相关性
    1.  将 dftIm 的值填充为0
    2.  定义变量
        1.  x，y
        2.  bsz，dsz
        3.  x0，y0
        4.  x1，y1
        5.  x2，y2
        6.  src0，dst，dst1，cdst
    3.  如果输入图像的通道数大于1，则混合通道
    4.  如果 src 不等于 dftImg，则将 src 设置为 dftImg
    5.  如果裁剪后的图像更小，调用`copyMakeBorder(dst1, dst, y1-y0, dst.rows-dst1.rows-(y1-y0), x1-x0, dst.cols-dst1.cols-(x1-x0), borderType)`进行边界填充
    6.  如果 bsz 的高等于块的高，则`cF->apply(dftImg.data, (int)dftImg.step, dftImg.data, (int)dftImg.step)`；反之对 dftImg 执行正向傅里叶变换
    7.  定义 dftTempl1
    8.  调用`mulSpectrums(dftImg, dftTempl1, dftImg, 0, true)`对频域空间上的元素相乘
    9.  如果 bsz 的高等于块的高，则`cR->apply(dftImg.data, (int)dftImg.step, dftImg.data, (int)dftImg.step)`；反之对 dftImg 执行反向傅里叶变换
    10. 重新定义 src
    11. 如果 corr 图像的通道数大于1，在 corr 图像的深度不是最大深度时更新 src，接着混合通道；反之如果当前所在通道为输入图像的第一通道，将 src 转换为 cdst，否则
        1.  如果 corr 图像的深度不是最大深度，更新 src
        2.  调用`add(src, cdst, cdst)`使 src 和 cdst 的元素分别相加

- 对 dft 的优化：使用 tile
  - 由于`nonzeroRows != 0`被传递给前向转换调用（forward transform calls），并且 A 和 B 分别被复制到 tempA 和 tempB 的左上角，因此不需要清除整个 tempA 和 tempB。只需要清除`tempA.cols - A.cols` (`tempB.cols - B.cols`)矩阵的最右列。
  - 这种基于 dft 的卷积不需要应用到整个大数组，特别是如果 B 明显小于 A 或反之亦然。相反，可以用分部分来计算卷积。为此，需要将输出数组C拆分为多个 tile。
  - 对于每个 tile，估计 A 和 B 的哪些部分需要在这个 tile 中计算卷积。如果 C 中的 tiles 太小，由于重复的工作，速度会下降很多。在最终的情况下，当 C 中的每个 tile 都是单个像素时，该算法就等价于朴素的卷积算法。
  - 如果磁贴太大，临时数组 tempA 和 tempB 就会变得太大，而且由于糟糕的缓存局部性，速度也会变慢。所以，在这两者之间存在一个最佳的贴图大小。
- 如果 C 语言中不同的平铺块可以并行计算，因此，卷积是由部分完成的，那么循环就可以被线程化。

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

### 边界类型 BORDER_ISOLATED 16

### 调整ROI
```c++
void cv::Mat::locateROI(
    Size & wholeSize,
    Point & ofs 
) const
```
- 定位父矩阵中的矩阵标头。
- 在使用 Mat::row, Mat::col, Mat::rowRange, Mat::colRange 和其他方法从矩阵中提取子矩阵之后，生成的子矩阵只指向原始大矩阵的一部分。但是，每个子矩阵包含信息(由 datastart 和 dataend 字段表示)，这些信息有助于重建原始矩阵的大小和提取的子矩阵在原始矩阵中的位置。locateROI 方法就是这么做的。
```c++
Mat& cv::Mat::adjustROI(
    int dtop,
    int dbottom,
    int dleft,
    int dright 
)
```
- 调整子矩阵的大小和在父矩阵中的位置。
- 该方法是 Mat::locateROI 的附赠方法。
- 这些函数的典型用途是确定子矩阵在父矩阵中的位置，然后以某种方式移动位置。
- 通常，当需要考虑 ROI 之外的像素时，过滤操作可能需要它。当所有方法参数都为正时，ROI 需要向各个方向增长指定的数量。
  - 例如:`A.adjustROI(2, 2, 2, 2);`
  - 在这个例子中，矩阵的大小在每个方向上增加了4个元素
  - 矩阵向左移动2个元素，向上移动2个元素，这为5x5内核的过滤带来了所有必要的像素。
- adjustROI 强制在父矩阵内调整 ROI，即调整 ROI 的边界受父矩阵边界的约束。
  - 例如，如果子矩阵 A 位于父矩阵的第一行，调用`A.adjustroi(2,2,2,2)`那么 A 不会向上增加。


[参考链接 locateROI](https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html#a40b5b3371a9c2a4b2b8ce0c8068d7c96)


[参考链接 adjustROI](https://docs.opencv.org/3.4/d3/d63/classcv_1_1Mat.html#a2fece3507ee7e1284deee6da99e76b9b)

### CV_HAL_DFT_IS_INPLACE 1024
### CV_HAL_DFT_INVERSE 1
### CV_HAL_DFT_SCALE 2
### CV_HAL_DFT_IS_INPLACE | CV_HAL_DFT_INVERSE | CV_HAL_DFT_SCALE 1027

### 填充图片边界
```c++
void cv::copyMakeBorder(
    InputArray src,
    OutputArray dst,
    int top,
    int bottom,
    int left,
    int right,
    int borderType,
    const Scalar & value = Scalar() 
)
```
- 在图像周围形成边框。
- 该函数将源图像复制到目标图像的中间。复制源图像的左边、右边、上面和下面的区域将被外推像素填充。这并不是基于它的过滤函数所做的(它们动态地推断像素)，而是其他更复杂的函数，包括用户自己的函数，可能会做的来简化图像边界处理。
- 当源图像是大图像的一部分(ROI)时，函数会尝试使用 ROI 外的像素来形成边界。要禁用此特性并始终进行外推，就像如果 src 不是 ROI 一样，请使用borderType | BORDER_ISOLATED。

[参考链接](https://docs.opencv.org/3.4/d2/de8/group__core__array.html#ga2ac1049c2c3dd25c2b41bffe17658a36)


### dft
```c++
void cv::dft(
    InputArray src,
    OutputArray dst,
    int flags = 0,
    int nonzeroRows = 0
)
```
[参考链接](https://docs.opencv.org/4.x/d2/de8/group__core__array.html#gadd6cf9baf2b8b704a11b5f04aaf4f39d)


### 傅里叶光谱元素相乘
```c++
void cv::mulSpectrums(
    InputArray a,
    InputArray b,
    OutputArray c,
    int flags,
    bool conjB = false
)
```
- 执行两个傅里叶光谱的每个元素相乘。
- mulSpectrums 函数执行两个 ccs 填充的或复杂的矩阵的每个元素的乘法，这些矩阵是一个实的或复杂的傅里叶变换的结果。
- 该函数与 dft 和 idft 一起，可用于快速计算两个数组的卷积(传递 conjB=false)或相关性(传递 conjB=true)。当数组是复数数组时，只需将它们(每个元素)与第二数组元素的可选共轭相乘。当数组是真实的时，它们被假定为 ccs 封装(详见 dft)。

[参考链接](https://docs.opencv.org/3.4/d2/de8/group__core__array.html#ga3ab38646463c59bf0ce962a9d51db64f)

### 元素相加
```c++
void cv::add(
    InputArray src1,
    InputArray src2,
    OutputArray dst,
    InputArray mask = noArray(),
    int dtype = -1
)
```
- 计算两个数组或一个数组和一个标量的每个元素的和。
- 函数 add 计算:
  - 当两个输入数组具有相同的大小和相同的通道数时，两个数组的和:
𝚍𝚜𝚝(I)=𝚜𝚊𝚝𝚞𝚛𝚊𝚝𝚎(𝚜𝚛𝚌𝟷(I)+𝚜𝚛𝚌𝟸(I))𝚒𝚏 𝚖𝚊𝚜𝚔(I)≠0
  - 当 src2 由 scalar 构造或具有与 src1.channels() 相同数量的元素时，数组和标量的和:
𝚍𝚜𝚝(I)=𝚜𝚊𝚝𝚞𝚛𝚊𝚝𝚎(𝚜𝚛𝚌𝟷(I)+𝚜𝚛𝚌𝟸)𝚒𝚏 𝚖𝚊𝚜𝚔(I)≠0
  - 当由 scalar 构造的 src1 或与 src2.channels() 具有相同数量的元素时，标量和数组的和:
𝚍𝚜𝚝(I)=𝚜𝚊𝚝𝚞𝚛𝚊𝚝𝚎(𝚜𝚛𝚌𝟷+𝚜𝚛𝚌𝟸(I))𝚒𝚏 𝚖𝚊𝚜𝚔(I)≠0
  - 其中 I 是数组元素的多维索引。对于多通道阵列，每个通道独立处理。
- 输入数组和输出数组都可以具有相同或不同的深度。例如，可以将16位无符号数组添加到8位有符号数组中，并将其和存储为32位浮点数组。输出数组的深度由 dtype 参数决定。在上面的第二种和第三种情况下，以及在第一种情况下，当`src1.depth() == src2.depth()`时，dtype 可以设置为默认的-1。在本例中，输出数组将具有与输入数组相同的深度，无论是 src1、src2 还是两者都是。
  - 两个图像应该具有相同的深度和类型，或者第二个图像可以只是一个标量值。
- 当输出阵列的深度为 CV_32S 时，不应用饱和。在溢出的情况下，用户甚至可能会得到不正确的符号的结果。
- OpenCV 加法和 Numpy 加法之间是有区别的。OpenCV 加法是一个饱和操作，而 Numpy 加法是一个模操作。例如：
    ```c++
    >>> x = np.uint8([250])
    >>> y = np.uint8([10])
    >>> print( cv.add(x,y) ) # 250+10 = 260 => 255
    [[255]]
    >>> print( x+y )          # 250+10 = 260 % 256 = 4
    [4]
    ```

[参考链接 1](https://docs.opencv.org/3.4/d0/d86/tutorial_py_image_arithmetics.html)


[参考链接 2](https://docs.opencv.org/3.4/d2/de8/group__core__array.html#ga10ac1bfb180e2cfda1701d06c24fdbd6)


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
**基本过程：**
1. 定义变量
   1. numType
   2. isNormed
   3. invArea
   4. sum, sqsum
   5. templMean, templSdv
   6. q0, q1, q2, q3
   7. templNorm, templSum2
2. 如果方法为 CCOEFF，调用`integral(img, sum, CV_64F)`计算输入图像的积分，并计算模板图像的平均值；反之
   1. 计算输入图像的积分和平方积分
   2. 计算模板图像的平均值和标准差
   3. 计算方差
   4. 如果方差小于一定值，且方法为CCOEFF_NORMED，将 result 的各个位置上的数值全部设置为1，并返回
   5. 计算平方的均值 templSum2
   6. 如果方法不是 CCOEFF，则将模板平均值设置为0，并将 templNorm 设置为平方的均值
   7. `templSum2 /= invArea;`
   8. `templNorm = std::sqrt(templNorm);`
   9. `templNorm /= std::sqrt(invArea);`
   10. 定义变量 q0, q1, q2, q3
3. 定义变量
   1. p0, p1, p2, p3
   2. sumstep
   3. sqstep
   4. i, j, k
4. 遍历之前定义的 result 并计算结果
   1. 定义变量
      1. rrow
      2. idx, idx2
      3. num
      4. wndMean2, wndSum2
   2. 如果方法为 CCOEFF
      1. 根据通道数量，分别
         1. 计算待检测的区域的积分
         2. `wndMean2 += t*t;`
         3. `num -= t*templMean[k];`
      2. `wndMean2 *= invArea;`
   3. 如果方法为 Normed 或 SQDIFF
      1. 根据通道数量，分别
         1. 计算待检测的区域的平方积分（？
         2. `wndSum2 += t;`
      2. 如果方法为 SQDIFF，根据公式，计算`num = wndSum2 - 2*num + templSum2`，并更新`num = MAX(num, 0.)`
   4. 如果方法为 Normed
      1. 定义变量`diff2 = MAX(wndSum2 - wndMean2, 0)`
      2. 如果 diff2 的值小于一定值，为了避免舍入误差，`t = 0`；反之`t = std::sqrt(diff2)*templNorm`
      3. 如果`fabs(num) < t`，则`num /= t`；如果`fabs(num) < t*1.125`，则`num = num > 0 ? 1 : -1`；否则`num = method != CV_TM_SQDIFF_NORMED ? 0 : 1`
   5. 在 result 的对应位置上更新计算好的结果
5. 结束计算

### 计算图像积分 integral()
- src：输入图像为内联公式，8位或浮点(32f或64f)
- sum：内联公式的积分图像，32位整数或浮点数(32f或64f)
- sqsum：平方像素值的积分图像;它是内联公式，双精度浮点(64f)数组
- tilted：对旋转45度的图像积分;它是具有与sum相同数据类型的内联公式数组
- sdepth：期望的积分深度和倾斜积分图像CV_32S, CV_32F，或CV_64F
- sqdepth：平方像素值的积分图像的期望深度CV_32F或CV_64F

该函数为源图像计算一个或多个积分图像，如下所示:


![sum](https://latex.codecogs.com/png.latex?%5Ctexttt%7Bsum%7D%20%28X%2CY%29%20%3D%20%20%5Csum%20_%7Bx%3CX%2Cy%3CY%7D%20%20%5Ctexttt%7Bimage%7D%20%28x%2Cy%29 "sum")


![sqsum](https://latex.codecogs.com/png.latex?%5Ctexttt%7Bsqsum%7D%20%28X%2CY%29%20%3D%20%20%5Csum%20_%7Bx%3CX%2Cy%3CY%7D%20%20%5Ctexttt%7Bimage%7D%20%28x%2Cy%29%5E2 "sqsum")


![tilted](https://latex.codecogs.com/png.latex?%5Ctexttt%7Btilted%7D%20%28X%2CY%29%20%3D%20%20%5Csum%20_%7By%3CY%2Cabs%28x-X%2B1%29%20%5Cleq%20Y-y-1%7D%20%20%5Ctexttt%7Bimage%7D%20%28x%2Cy%29 "tilted")


使用这些积分图像，可以在恒定的时间内计算图像上特定的垂直或旋转矩形区域的总和、平均值和标准偏差，例如:


![formula](https://latex.codecogs.com/png.latex?%5Csum%20_%7Bx_1%20%5Cleq%20x%20%3C%20x_2%2C%20%20%5C%2C%20y_1%20%20%5Cleq%20y%20%3C%20y_2%7D%20%20%5Ctexttt%7Bimage%7D%20%28x%2Cy%29%20%3D%20%20%5Ctexttt%7Bsum%7D%20%28x_2%2Cy_2%29-%20%5Ctexttt%7Bsum%7D%20%28x_1%2Cy_2%29-%20%5Ctexttt%7Bsum%7D%20%28x_2%2Cy_1%29%2B%20%5Ctexttt%7Bsum%7D%20%28x_1%2Cy_1%29 "formula")


例如，它可以使用可变窗口大小进行快速模糊或快速块关联。对于多通道图像，每个通道的和是独立累加的。


![Image Integral](https://docs.opencv.org/3.4.7/integral.png "Image Integral")


[参考链接 1](https://docs.rs/opencv/0.19.2/opencv/imgproc/fn.integral.html)


[参考链接 2](https://aishack.in/tutorials/integral-images-opencv/)

### 计算平均值和标准差
```c++
void cv::meanStdDev(
    InputArray src,
    OutputArray mean,
    OutputArray stddev,
    InputArray mask = noArray()
)
```
- 计算数组元素的平均值和标准差
- 函数cv::meanStdDev独立计算每个通道数组元素的平均值和标准差M，并通过输出参数返回:
  ![meanStdDev](https://github.com/JasmineCAicai/Unique-Graphics/blob/master/meanStdDev.png "meanStdDev")
- 当所有掩码元素都为0时，函数返回`mean=stddev=Scalar::all(0)`
- 计算出的标准差只是完全归一化协方差矩阵的对角线
  - 如果需要完整的矩阵，可以将多通道数组 M x N 重塑为单通道数组 M*N x mtx.channels()
  - 仅当矩阵是连续的时候才可能
  - 然后将该矩阵传递给 calcCovarMatrix


[参考链接](https://docs.opencv.org/3.4/d2/de8/group__core__array.html#ga846c858f4004d59493d7c6a4354b301d)

### DBL_EPSILON 2.2204460492503131e-16
