# Sparse-Matrix-Binary
稀疏矩阵二元组
6．稀疏矩阵的二元组表示 
在稀疏矩阵的二元组表示中，用一个二元组表（TwElems）存放矩阵的非零元素，其中
每个二元组只记录非零元素的列号（col）和元素值（item），且各二元组按行号递增的顺序
排列。另外，设一个行指针数组（RowIndex），其第 i 个元素表示稀疏矩阵中第 i 行的第一
个非零元素在二元组表中的存放位置。
要求参照教材中三元组顺序表，完成稀疏矩阵的二元组表示类设计和实现。
