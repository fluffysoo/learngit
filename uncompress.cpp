#include <map>
#include<string>
#include<fstream>
#include<queue>
#include<iostream>
using namespace std;

map<string, string> mp;

// 读入文件
void load()
{
    // 打开文件data_information.txt
    ifstream fin1("data_information.txt");
    string s1, s2;
    int cnt = 0;
    // 读取data_information.txt中的内容，并存入map容器中
    while (fin1 >> s1)
    {
        fin1 >> s2;
        mp[s2] = s1;//此时关键字是编码，值是字符
    }
}

// 解压缩
void loadAndsave()
{
    // 打开文件data_compression.txt
    ifstream fin2("data_compression.txt");
    string s;
    // 输出文件的文件名
    string output_filename = "data_uncompression.txt";
    // 以输出模式打开文件
    ofstream out(output_filename);
    // 读取data_compression.txt中的内容，并将map中对应的值输出到data_uncompression.txt中
    while (fin2 >> s)
    {
        // s是哈夫曼编码 mp是哈夫曼编码到字符串的映射
        out << mp[s] << " ";
    }
}
int main()
{
    load();
    loadAndsave();
    return 0;
}