#include <map>
#include<string>
#include<fstream>
#include<queue>
#include<iostream>
#include<algorithm>
using namespace std;
map<string, int> mp; // 定义一个 string 到 int 的映射，用于存储每个字符的频率
string f[100010]; // 文本信息
map<string, string> mp2;  // 定义一个 string 到 string 的映射，用于存储每个字符的到哈夫曼编码的映射(解压时会用到)
const int N = 10010; // 定义树的最大节点数
int res, cnt, len, n = 1;        // res 用于存储哈夫曼编码的长度，cnt 用于计算树中节点的个数

struct node
{
    int id;                             // 节点的编号
    int w;                              // 节点的权重，即字符的频率
    int level;                          // 节点的层数
    int l, r;                           // 节点的左右儿子
    string str;                         // 节点存储的字符
    string encode;                      // 节点的编码
    bool operator<(const node& s) const // 重载小于运算符，用于优先级排序：
    //字符出现频率不同的话，出现频率小的字符优先级高
    //字符出现频率相同的话，层数大的字符优先级高
    {
        return w == s.w ? (level < s.level) : w > s.w;
        
    }
} tree[N]; // 定义节点的结构体
int fa[N]; // 定义父亲数组，用于记录每个节点的父节点

// 读入文件
void load()
{   
    ifstream fin("data.txt"); // 打开文件
    string s;
    len = 0;
    while (fin >> s)
        mp[s]++, f[len++] = s; // 读入每个字符，并将它的频率加 1
}

//将字符的二进制编码转换成十六进制编码
char num(string s) {//将 4 位的 2 进制转换为 1 位的 16 进制
    int r = 0, i, t = 1;
    for (i = s.size() - 1; i >= 0; i--) {
        r = r + (s[i] - '0') * t;
        t = t * 2;
    }

    char c;	// 存储1位的16进制字符
    if (r < 10) {
        c = r + '0';
    }
    else {
        c = r + 'A' - 10;
    }

    return c;
}
string transall(string s)
{
    string code = s;
    if (code.size() % 4 == 1) {
        code = "000" + code;
    }
    else if (code.size() % 4 == 2) {
        code = "00" + code;
    }
    else if (code.size() % 4 == 3) {
        code = "0" + code;
    }

    string newcode = "", tmp = "";
    for (int i = 0; i < code.size(); i += 4) {
        tmp = code.substr(i, 4);
        newcode += num(tmp);
    }
    return newcode;
}

// DFS 函数，用于遍历树(从下往上进行遍历)
void dfs(int x, string str, int s)
{
    if (fa[x] == 0) // 如果 x 是根节点，则结束遍历
    {
        reverse(str.begin(), str.end()); // 反转字符串
        string tmp;               
        tmp = transall(str);         //将二进制编码转换成十六进制编码，缩小存储所需要的空间
        mp2[tree[s].str] = tmp;         // 用于输出
        tree[s].encode = tmp;            // 将编码存储到节点中
        return;
    }
    if (x == tree[fa[x]].l)
        dfs(fa[x], str + '0', s); // 如果 x 是它的父节点的左儿子，则将 '0' 添加到编码字符串末尾，并继续向上遍历

    else
        dfs(fa[x], str + '1', s); // 如果 x 是它的父节点的右儿子，则将 '1' 添加到编码字符串末尾，并继续向上遍历
}

//将字符和其编码存到一个文本中
void save1()
{   
    string output_filename = "data_information.txt"; // 定义输出文件的文件名，输出字符和其编码
    ofstream out(output_filename);         // 打开输出文件
    if (out.is_open())                     // 如果文件打开成功
    {
        // 将要输出的内容写入文件中
        for (int i = 1; i <= n; i++) // 遍历所有字符
        {
            out << tree[i].str << " " << tree[i].encode << endl; // 输出字符和它的编码
        }
        out.close();                                         // 关闭文件
        //cout << "ok" << endl; // 输出提示信息
    }
    else // 如果文件打开失败
    {
        cout << "error:" << output_filename << "can't be opened." << endl; // 输出错误信息
    }
}

// 压缩，将压缩后的内容存到一个文本中
void save2()
{
    string output_filename = "data_compression.txt"; // 定义输出文件的文件名
    ofstream out(output_filename);         // 打开输出文件
    if (out.is_open())                     // 如果文件打开成功
    {
        // 将要输出的内容写入文件中
        for (int i = 0; i < len; i++) // 遍历所有字符
        {
            out << mp2[f[i]] << " " << endl; 
        }
        out.close();                                         // 关闭文件
        //cout << "ok" << endl; // 输出提示信息
    }
    else // 如果文件打开失败
    {
        cout << "error:" << output_filename << "can't be opened." << endl; // 输出错误信息
    }
}

int main()
{
    load();                    // 读入文件
    priority_queue<node> heap; // 定义一个优先队列，用于构造哈夫曼树
    // res 用于存储哈夫曼编码的长度，cnt 用于计算树中节点的个数,n=1
    for (auto x : mp)
    {                        // 遍历所有字符
        tree[n].id = n;        // 设置节点的编号
        tree[n].w = x.second;  // 设置节点的权重
        tree[n].str = x.first; // 设置节点存储的字符
        heap.push(tree[n++]);  // 将节点插入优先队列
    }
    cnt = n + 1;        // 设置节点计数器
    while (heap.size() > 1) // 利用优先队列构造哈夫曼树
    {
        auto a = heap.top();
        heap.pop(); // 出队
        auto b = heap.top();
        heap.pop();                                  // 出队
        res += a.w + b.w;                            // 计算 res
        tree[cnt].id = cnt;                          // 设置新节点的编号
        tree[cnt].w = a.w + b.w;                     // 设置新节点的权重
        tree[cnt].level = max(a.level, b.level) + 1; // 设置新节点的层数
        tree[cnt].l = a.id;                          // 设置新节点的左儿子
        tree[cnt].r = b.id;                          // 设置新节点的右儿子

        fa[a.id] = fa[b.id] = cnt; // 设置新节点为a、b父节点
        heap.push(tree[cnt++]);    // 将新节点插入优先队列
    }
    for (int i = 1; i <= n; i++) // 遍历所有字符
    {
        string s;     // 定义编码字符串
        dfs(i, s, i); // 调用 DFS 函数，遍历整棵树获得字符的编码
    }

    save1();
    save2();

    return 0;
}