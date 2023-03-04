#pragma once

template<class T>
struct Twin
{
	int col;
	T value;
	Twin();
	Twin(int c, T v);
};

template<class T>
Twin<T>::Twin()
{

}

template<class T>
Twin<T>::Twin(int c, T v)
{
	col = c;
	value = v;
}
