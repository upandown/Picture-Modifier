#pragma OPENCL EXTENSION cl_nv_printf:enables


__OVERLOADABLE__ double abs(double a) {
	if (a < 0)
		return -a;
	else
		return a;

}

__kernel  void rotatePic(__global double *pixel,__global double *rPic, double width,
	double height, double rWidth, double rHeight, double angle) {
	double PI = 3.14159265358979323846;


	angle = angle / 180.0f * PI;
	double cosa = cos(angle);
	double sina = sin(angle);

	int x = get_global_id(0);
	int y = get_global_id(1);


	////后向映射寻找原图中的位置

	int Rx = (y - rHeight / 2.0f)*cosa - (x - rWidth / 2.0f)*sina + height / 2.0f;
	int Ry = (y - rHeight / 2.0f)*sina + (x - rWidth / 2.0f)*cosa + width / 2.0f;


	double element = 0; //像素值
	double Wx[4], Wy[4];  //xy的权重

	double a = -0.5;
	int pn = (int)Rx;  //获得整数部分
	double u = Rx - pn;  //获得小数部分
	double np[4];
	np[0] = 1 + u;   //>1
	np[1] = u;        //<1
	np[2] = 1 - u;     //<1
	np[3] = 2 - u;   //>1

	Wx[0] = a * pow((float)abs(np[0]), 3.0f) - 5 * a * pow((float)abs(np[0]), 2.0f) + 8 * a * abs(np[0]) - 4 * a;
	Wx[1] = (a + 2) * pow((float)abs(np[1]), 3.0f) - (a + 3) * pow((float)abs(np[1]), 2.0f) + 1;
	Wx[2] = (a + 2) * pow((float)abs(np[2]), 3.0f) - (a + 3) * pow((float)abs(np[2]), 2.0f) + 1;
	Wx[3] = a * pow((float)abs(np[3]), 3.0f) - 5 * a * pow((float)abs(np[3]), 2.0f) + 8 * a * abs(np[3]) - 4 * a;

	pn = (int)Ry;  //获得整数部分
	u = Ry - pn;  //获得小数部分
	np[0] = 1 + u;   //>1
	np[1] = u;        //<1
	np[2] = 1 - u;     //<1
	np[3] = 2 - u;   //>1
	Wy[0] = a * pow((float)abs(np[0]), 3.0f) - 5 * a * pow((float)abs(np[0]), 2.0f) + 8 * a * abs(np[0]) - 4 * a;
	Wy[1] = (a + 2) * pow((float)abs(np[1]), 3.0f) - (a + 3) * pow((float)abs(np[1]), 2.0f) + 1;
	Wy[2] = (a + 2) * pow((float)abs(np[2]), 3.0f) - (a + 3) * pow((float)abs(np[2]), 2.0f) + 1;
	Wy[3] = a * pow((float)abs(np[3]), 3.0f) - 5 * a * pow((float)abs(np[3]), 2.0f) + 8 * a * abs(np[3]) - 4 * a;



	//求出这个像素的大小
	if (x >= 0 && x < rWidth  && y >= 0 && y < rHeight) {
		for (int a = 0; a < 4; a++) {
			for (int b = 0; b < 4; b++) {// a00 * x0y0 + a10*x
				if (Rx + a - 1 < 0 || Rx + a - 1 >= height || Ry + b - 1 < 0 || Ry + b - 1 >= width)    //防止越界 超出原图像
					rPic[y * (int)rWidth + x] = 0;
				else
				{
					int X = (Rx + a - 1) * width;
					int Y = Ry + b - 1;
					element += pixel[ X + Y] * Wx[a] * Wy[b];
				}
			}
		}
		rPic[y * (int)rWidth + x] = element;
	}
}