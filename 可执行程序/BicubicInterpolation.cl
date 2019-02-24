#pragma OPENCL EXTENSION cl_nv_printf:enables


__OVERLOADABLE__ double abs(double a) {
	if (a < 0)
		return -a;
	else
		return a;

}

__kernel void extendPic(__global double *pixel,__global double *bigPic, double width, double height, double factor) {

	//int y = blockDim.y * blockIdx.y + threadIdx.y;
	//int x = blockDim.x * blockIdx.x + threadIdx.x;  //计算索引


	int x = get_global_id(0);
	int y = get_global_id(1);


	double bWidth = factor * width;
	double bHeight = factor * height;

	if (x >= 2 && x < bWidth - 4 && y >= 2 && y < bHeight - 4) {
		double bI = (double)y * 1 / factor; //放大前的位置 y
		double bJ = (double)x * 1 / factor; // x

		double Wx[4], Wy[4];  //xy的权重

		double a = -0.5;
		int pn = (int)bI;  //获得整数部分
		double u = bI - pn;  //获得小数部分
		double np[4];
		np[0] = 1 + u;   //>1
		np[1] = u;        //<1
		np[2] = 1 - u;     //<1
		np[3] = 2 - u;   //>1

		Wx[0] = a * pow((float)abs(np[0]), 3.0f) - 5 * a * pow((float)abs(np[0]), 2.0f) + 8 * a * abs(np[0]) - 4 * a;
		Wx[1] = (a + 2) * pow((float)abs(np[1]), 3.0f) - (a + 3) * pow((float)abs(np[1]), 2.0f) + 1;
		Wx[2] = (a + 2) * pow((float)abs(np[2]), 3.0f) - (a + 3) * pow((float)abs(np[2]), 2.0f) + 1;
		Wx[3] = a * pow((float)abs(np[3]), 3.0f) - 5 * a * pow((float)abs(np[3]), 2.0f) + 8 * a * abs(np[3]) - 4 * a;

		 pn = (int)bJ;  //获得整数部分
		 u = bJ - pn;  //获得小数部分
		np[0] = 1 + u;   //>1
		np[1] = u;        //<1
		np[2] = 1 - u;     //<1
		np[3] = 2 - u;   //>1
		Wy[0] = a * pow((float)abs(np[0]), 3.0f) - 5 * a * pow((float)abs(np[0]), 2.0f) + 8 * a * abs(np[0]) - 4 * a;
		Wy[1] = (a + 2) * pow((float)abs(np[1]), 3.0f) - (a + 3) * pow((float)abs(np[1]), 2.0f) + 1;
		Wy[2] = (a + 2) * pow((float)abs(np[2]), 3.0f) - (a + 3) * pow((float)abs(np[2]), 2.0f) + 1;
		Wy[3] = a * pow((float)abs(np[3]), 3.0f) - 5 * a * pow((float)abs(np[3]), 2.0f) + 8 * a * abs(np[3]) - 4 * a;


		double element = 0;

		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
				/*element += (Vec3f)(dealPic.at<cv::Vec3b>((int)bI + a - 1, (int)bJ + b - 1)) * Wx[a] * Wy[b];*/
			/*	if ((bJ + (b - 1)) >= 0 && (bJ + (b - 1)) < width  && (bI + a - 1) >= 0 && (bI + a - 1) < height)*/
				element += pixel[(int)(bI + a - 1) * (int)width + (int)(bJ + (b - 1))] * Wx[a] * Wy[b];
			}
		}

		bigPic[(int)(y * bWidth + x)] = element;

	}
	/*else
		bigPic[int(y * bWidth + x)] = 0;*/

}

