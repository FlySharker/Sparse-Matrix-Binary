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
	int GetRows() const;			// ����ϡ���������
	int GetCols() const;			// ����ϡ���������
	int GetNum() const;				// ����ϡ��������Ԫ����
	TwSparseMatrix(const TwSparseMatrix<T>& copy);	// ���ƹ��캯��
	TwSparseMatrix<T>& operator =(const TwSparseMatrix<T>& copy);// ��ֵ���������
	void Init(int r, int c, T v);
	void GetElems(int r, int c, T& v);
	void SimpleTranspose(TwSparseMatrix<T>& b);
	void FastTranspose(TwSparseMatrix<T>& b);
};

template<class T>
TwSparseMatrix<T>::TwSparseMatrix(int r, int c, int size)
{
	if (r < 1 || c < 1) throw Error("������������Ч!");	// �׳��쳣
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
// �������������ϡ���������
{
	return rows;					// ��������
}

template <class T>
int TwSparseMatrix<T>::GetCols() const
// �������������ϡ���������
{
	return cols;					// ��������
}

template <class T>
int TwSparseMatrix<T>::GetNum() const
// �������������ϡ��������Ԫ����
{
	return num;						// ���ط���Ԫ����
}

template <class T>
TwSparseMatrix<T>::TwSparseMatrix(const TwSparseMatrix<T>& copy)
// �����������ϡ�����copy������ϡ����󡪡����ƹ��캯��
{
	maxSize = copy.maxSize;							// ������Ԫ�ظ���
	rows = copy.rows;								// ��������
	cols = copy.cols;								// ��������
	num = copy.num;									// ���Ʒ���Ԫ�ظ���
	twElems = new Twin<T>[maxSize];		            // Ϊ��Ԫ�����洢�ռ�
	cout << "��Ԫ��" << endl;
	for (int i = 0; i < num; i++) {
		twElems[i] = copy.twElems[i];
		cout << twElems[i].col << " " << twElems[i].value << endl;
	}	// ���ƶ�Ԫ��
	cout << endl;
	cout << "��ָ������" << endl;
	for (int j = 0; j < rows; j++) {
		RowIndex[j] = copy.RowIndex[j];
		cout << RowIndex[j] << endl;
	}
	cout << endl;
}

template <class T>
TwSparseMatrix<T>& TwSparseMatrix<T>::operator =(const TwSparseMatrix<T>& copy)
// �����������ϡ�����copy��ֵ����ǰϡ����󡪡���ֵ���������
{
	if (&copy != this) {
		maxSize = copy.maxSize;						// ������Ԫ�ظ���
		if (twElems != NULL) delete[]twElems;	    // �ͷŴ洢�ռ�
		twElems = new Twin<T>[30];	                // ����洢�ռ�
		rows = copy.rows;							// ��������
		cols = copy.cols;							// ��������
		num = copy.num;								// ���Ʒ���Ԫ�ظ���
		for (int i = 0; i < num; i++) {
			twElems[i] = copy.twElems[i];
		}	// ���ƶ�Ԫ��
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
		for (int col = 0; col < cols; col++) {//�ڶ�Ԫ����б����У���ת�ú����
			for (int j = 0; j < num; j++) {
				if (twElems[j].col == col) {
					for (int i = rows - 1; i >= 0; i--) {//�Ҷ�Ӧ��Ԫ�����
						int t = 0;
						if (RowIndex[i] + t == NULL) continue;
						while (RowIndex[i] + t != twElems + j && RowIndex[i] + t != twElems + num) t++;
						if (RowIndex[i] + t == twElems + j) {
							r = i;
							break;
						}
					}
					b.twElems[k].col = r;//���ɶ�Ԫ��ת�Ƶ�ת�ö�Ԫ���У���������ָ�������и�ָ���ָ��
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
		for (int col = 0; col < cols; col++) cNum[col] = 0;	// ��ʼ��cNum
		for (int i = 0; i < num; i++) ++cNum[twElems[i].col];		// ͳ��ԭ������ÿһ�еķ���Ԫ����	
		cPos[0] = 0;						// ��һ�еĵ�һ������Ԫ��b�洢����ʼλ��
		for (int col = 1; col < cols; col++) cPos[col] = cPos[col - 1] + cNum[col - 1];	// ѭ����ÿһ�еĵ�һ������Ԫ��b�洢����ʼλ��
			
		for (int j = 0; j < num; j++) {//�ڶ�Ԫ����б����У���ת�ú����
				for (int i = rows - 1; i >= 0; i--) {//�Ҷ�Ӧ��Ԫ�����
					int t = 0;
					if (RowIndex[i] + t == NULL) continue;
					while (RowIndex[i] + t != twElems + j && RowIndex[i] + t != twElems + num) t++;
					if (RowIndex[i] + t == twElems + j) {
						r = i;
						break;
					}
				}
				k = cPos[twElems[j].col];
				b.twElems[k].col = r;//���ɶ�Ԫ��ת�Ƶ�ת�ö�Ԫ���У���������ָ�������и�ָ���ָ��
				b.twElems[k].value = twElems[j].value;
				if (b.RowIndex[twElems[j].col] == NULL) b.RowIndex[twElems[j].col] = b.twElems + k;
				++cPos[twElems[j].col];
		}
	}
}