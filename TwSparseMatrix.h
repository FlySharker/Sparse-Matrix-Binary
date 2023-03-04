#pragma once

#include"Twin.h"

template<class T>
class TwSparseMatrix
{
protected:
	Twin<T>* twElems;
	Twin<T>* RowIndex[1000];
	int maxSize, rows, cols, num;
public:
	TwSparseMatrix(int r=5, int c=5,int size=500);
	~TwSparseMatrix();
	int GetRows() const;			// 返回稀疏矩阵行数
	int GetCols() const;			// 返回稀疏矩阵列数
	int GetNum() const;				// 返回稀疏矩阵非零元个数
	TwSparseMatrix(const TwSparseMatrix<T>& copy);	// 复制构造函数
	TwSparseMatrix<T>& operator =(const TwSparseMatrix<T>& copy);// 赋值运算符重载
	void Init(int r, int c, T v);
	void GetElems(int r, int c, T& v);
	void SimpleTranspose(TwSparseMatrix<T>& b);
	void FastTranspose(TwSparseMatrix<T>& b);
};

template<class T>
TwSparseMatrix<T>::TwSparseMatrix(int r, int c, int size)
{
	if (r < 1 || c < 1) throw Error("行数或列数无效!");	// 抛出异常
	maxSize = size;
	rows = r;
	cols = c;
	num = 0;
	twElems = new Twin<T>[1000];
	for (int i = 0; i < 1000; i++) {
		RowIndex[i] = NULL;
	}
}

template<class T>
TwSparseMatrix<T>::~TwSparseMatrix()
{
	if (twElems != NULL) delete[]twElems;
}

template <class T>
int TwSparseMatrix<T>::GetRows() const
// 操作结果：返回稀疏矩阵行数
{
	return rows;					// 返回行数
}

template <class T>
int TwSparseMatrix<T>::GetCols() const
// 操作结果：返回稀疏矩阵列数
{
	return cols;					// 返回列数
}

template <class T>
int TwSparseMatrix<T>::GetNum() const
// 操作结果：返回稀疏矩阵非零元个数
{
	return num;						// 返回非零元个数
}

template <class T>
TwSparseMatrix<T>::TwSparseMatrix(const TwSparseMatrix<T>& copy)
// 操作结果：由稀疏矩阵copy构造新稀疏矩阵——复制构造函数
{
	maxSize = copy.maxSize;							// 最大非零元素个数
	rows = copy.rows;								// 复制行数
	cols = copy.cols;								// 复制列数
	num = copy.num;									// 复制非零元素个数
	twElems = new Twin<T>[maxSize];		            // 为二元组分配存储空间
	cout << "二元组" << endl;
	for (int i = 0; i < num; i++) {
		twElems[i] = copy.twElems[i];
		cout << twElems[i].col << " " << twElems[i].value << endl;
	}	// 复制二元组
	cout << endl;
	cout << "行指针数组" << endl;
	for (int j = 0; j < rows; j++) {
		RowIndex[j] = copy.RowIndex[j];
		cout << RowIndex[j] << endl;
	}
	cout << endl;
}

template <class T>
TwSparseMatrix<T>& TwSparseMatrix<T>::operator =(const TwSparseMatrix<T>& copy)
// 操作结果：将稀疏矩阵copy赋值给当前稀疏矩阵——赋值运算符重载
{
	if (&copy != this) {
		maxSize = copy.maxSize;						// 最大非零元素个数
		if (twElems != NULL) delete[]twElems;	    // 释放存储空间
		twElems = new Twin<T>[30];	                // 分配存储空间
		rows = copy.rows;							// 复制行数
		cols = copy.cols;							// 复制列数
		num = copy.num;								// 复制非零元素个数
		for (int i = 0; i < num; i++) {
			twElems[i] = copy.twElems[i];
		}	// 复制二元组
		for (int j = 0; j < rows; j++) {
			RowIndex[j] = copy.RowIndex[j];
		}
	}
	return *this;
}

template <class T>
void TwSparseMatrix<T>::Init(int r, int c, T v)
{
	if (v != 0) {
		if(RowIndex[r] == NULL) RowIndex[r] = twElems+num;
		twElems[num].col = c;
		twElems[num].value = v;
		num++;
	}
}

template <class T>
void TwSparseMatrix<T>::GetElems(int rr, int cc, T& v)
{
	if (RowIndex[rr] == NULL) {
		v = 0;
		return;
	}
	else {
		if (rr != rows-1) {
			int o = 1, u = 1;
			while (RowIndex[rr + o] == NULL) o++;
			while (RowIndex[rr] + u != RowIndex[rr + o]) u++;
			while (u) {
				if ((RowIndex[rr] + u-1)->col == cc) {
					v = (RowIndex[rr] + u-1)->value;
					return;
				}
				u--;
			}
			v = 0;
		}
		else {
			int t=0;
			while (RowIndex[rr] + t != twElems + num) {
				if ((RowIndex[rr] + t)->col == cc) {
					v = (RowIndex[rr] + t)->value;
					return;
				}
				t++;
			}
			v = 0;
		}
	}
}

template<class T>
void TwSparseMatrix<T>::SimpleTranspose(TwSparseMatrix<T>& b) 
{
	b.rows = cols;
	b.cols = rows;
	b.num = num;
	b.maxSize = maxSize;
	delete[]b.twElems;
	b.twElems = new Twin<T>[b.maxSize];
	for (int i = 0; i < 100; i++) b.RowIndex[i] = NULL;

	if (b.num > 0) {						
		int r = 0;
		int k = 0;
		for (int col = 0; col < cols; col++) {//在二元组表中遍历列，即转置后的行
			for (int j = 0; j < num; j++) {
				if (twElems[j].col == col) {
					for (int i = rows - 1; i >= 0; i--) {//找对应二元组的行
						int t = 0;
						if (RowIndex[i] + t == NULL) continue;
						while (RowIndex[i] + t != twElems + j && RowIndex[i] + t != twElems + num) t++;
						if (RowIndex[i] + t == twElems + j) {
							r = i;
							break;
						}
					}
					b.twElems[k].col = r;//将旧二元组转移到转置二元组中，并调整行指针数组中各指针的指向
					b.twElems[k].value = twElems[j].value;
					if (b.RowIndex[twElems[j].col] == NULL) b.RowIndex[twElems[j].col] = b.twElems + k;
					k++;
				}
			}
		}
	}
}

template<class T>
void TwSparseMatrix<T>::FastTranspose(TwSparseMatrix<T>& b)
{
	b.rows = cols;
	b.cols = rows;
	b.num = num;
	b.maxSize = maxSize;
	delete[]b.twElems;
	b.twElems = new Twin<T>[b.maxSize];
	for (int i = 0; i < 100; i++) b.RowIndex[i] = NULL;
	int* cNum = new int[cols + 1];
	int* cPos = new int[cols + 1];

	if (b.num > 0) {
		int r = 0;
		int k = 0;
		for (int col = 0; col < cols; col++) cNum[col] = 0;	// 初始化cNum
		for (int i = 0; i < num; i++) ++cNum[twElems[i].col];		// 统计原矩阵中每一列的非零元个数	
		cPos[0] = 0;						// 第一列的第一个非零元在b存储的起始位置
		for (int col = 1; col < cols; col++) cPos[col] = cPos[col - 1] + cNum[col - 1];	// 循环求每一列的第一个非零元在b存储的起始位置
			
		for (int j = 0; j < num; j++) {//在二元组表中遍历列，即转置后的行
				for (int i = rows - 1; i >= 0; i--) {//找对应二元组的行
					int t = 0;
					if (RowIndex[i] + t == NULL) continue;
					while (RowIndex[i] + t != twElems + j && RowIndex[i] + t != twElems + num) t++;
					if (RowIndex[i] + t == twElems + j) {
						r = i;
						break;
					}
				}
				k = cPos[twElems[j].col];
				b.twElems[k].col = r;//将旧二元组转移到转置二元组中，并调整行指针数组中各指针的指向
				b.twElems[k].value = twElems[j].value;
				if (b.RowIndex[twElems[j].col] == NULL) b.RowIndex[twElems[j].col] = b.twElems + k;
				++cPos[twElems[j].col];
		}
	}
}