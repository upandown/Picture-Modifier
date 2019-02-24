#pragma OPENCL EXTENSION cl_nv_printf:enables
__kernel void GayssianFilterCL(__global double *pixel, __global  double* wight, double Width, double Height, int filterSize)
{
	int skipSize = filterSize / 2; //�����������

	int x = get_global_id(0);
	int y = get_global_id(1);

	if (x >= skipSize && x < Width - 1 - filterSize && y >= skipSize && y < Height - 1 - filterSize) { //��ֹԽ��

		//double * filterArray = (double*)malloc(sizeof(double) * filterSize * filterSize);  //�洢������ڵ�����ֵ
		double filterArray[9];
		int c_cout = 0;
		for (int k = skipSize, count_i = 0; count_i < filterSize; count_i++) {
			// i -1 j -1    i - 0, j - 1 , i +1, j -1 
			// 0 1 2 3 4 5 p[5]
			//�����ص��������
			for (int p = skipSize, count_j = 0; count_j < filterSize; count_j++) {
				filterArray[c_cout] = pixel[(y - k) * (int)Width + x - p];
				p--;
				c_cout++;
			}
			k--;
		}
		double value = 0;
		//��Ȩ���
		for (int i = 0; i < filterSize* filterSize; i++)
			value += filterArray[i] * wight[i];

		pixel[y * (int)Width + x] = value;
		/*free(filterArray);*/

	}
}