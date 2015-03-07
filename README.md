# A-simple-Dictionary

A simple Dictionary
Provided search of example sentences with typing some words. Became familiar with
string match and word parsing and developed knowledge of Trie and Inverted Index.

此文件夹仅包含相关源码，具体运行需要GTK及其他运行依赖文件和数据文件。
注意事项：
1. SentenceIn.txt存放的是原始文章
2. SentenceOut.txt存放的经过分句处理后的句子文件
3. InvIndex.txt存放的是单词倒排索引
3. OffSet存放SentenceOut中每个句子的偏移量
使用时需要点击"生成词典"，然后对原始文章进行处理，点击“保存词典”后则会将处理结果保存，
下次使用时只需要点击"导入词典"，而不需要重复进行处理。查询时输入查询的单词，多个单词以空格分开，点击”Go”，
则会得到结果。
