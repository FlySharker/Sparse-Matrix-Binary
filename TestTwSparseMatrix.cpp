#include "Assistance.h"				// ���������
#include "TwSparseMatrix.h"		// ϡ��������Ԫ��˳���

int main()
{

	try								// ��try��װ���ܳ����쳣�Ĵ���
	{
		const int rs = 7, cs = 6;
		TwSparseMatrix<int> a(rs, cs);	// ����һ��n��n��ϡ�����
		TwSparseMatrix<int> c(rs, cs);	// ����һ��n��n��ϡ�����
		int m[rs][cs] = {
			15,	 0,	 0,	22,	 0, -5,
			 0,	11,	 3,	 0,	 0,  0,
			 0,	 0,	 0,	 6,	 0,  0,
			 0,	 0,	 0,	 0,	 0,  0,
			91,	 0,	 0,	 0,	 0,  0,
			 0,  7,  0,  0,  0,  0,
			 0,  0, 28,  0,  0,  0
		};
		int n[rs][cs] = {
			15,	 0,	 0,	 8,	 0,  5,
			 0,	-1,	-3,	 0,	 0,  0,
			 0,	 0,	 0,	14,	 0,  0,
			 0,	 0,	 0,	 0,	 0,  0,
			 9,	 0,	 0,	 0,	 0,  7,
			 0, 13,  0,  0,  0,  0,
			 0,  0, -8,  0,  0,  0
		};
		int i, j, v;					// ��ʱ����	
		// ����ϡ�����a��Ԫ��ֵ
		for (i = 0; i < rs; i++)
			for (j = 0; j < cs; j++)
				a.Init(i, j, m[i][j]);	// ����Ԫ��ֵ
		TwSparseMatrix<int> b(a);
		// ��ʾϡ�����a
		cout << "��ʾϡ�����a" << endl;
		for (i = 0; i < a.GetRows(); i++) {
			for (j = 0; j < a.GetCols(); j++) {
				a.GetElems(i, j, v);		// ȡԪ��ֵ
				cout << v << "\t";		// ��ʾԪ��ֵ
			}
			cout << endl;
		}
		cout << endl;
		cout << "��ת��" << endl;
		a.SimpleTranspose(b);
		for (i = 0; i < b.GetRows(); i++) {
			for (j = 0; j < b.GetCols(); j++) {
				b.GetElems(i, j, v);		// ��Ԫ��ֵ
				cout << v << "\t";		// ��ʾԪ��ֵ
			}
			cout << endl;
		}
		cout << endl;
		cout << "����ת��" << endl;
		a.FastTranspose(b);
		for (i = 0; i < b.GetRows(); i++) {
			for (j = 0; j < b.GetCols(); j++) {
				b.GetElems(i, j, v);		// ��Ԫ��ֵ
				cout << v << "\t";		// ��ʾԪ��ֵ
			}
			cout << endl;
		}
	}
	catch (Error err)		// ��׽�������쳣
	{
		err.Show();			// ��ʾ�쳣��Ϣ
	}
	system("PAUSE");        // ���ÿ⺯��system()
	return 0;               // ����ֵ0, ���ز���ϵͳ
}