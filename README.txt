哈希函数：
/*unsigned int BKDRHash(string str);
 * function: get a string's hash value
 * BKDR: HashValue(str) = sum(str[i]*seed^(length-i-1))
 * in: string str
 * out: unsigned int hash value
 * 注意：该函数返回的是哈希值，并不是在哈希表中的位置；
 * 要得到哈希表中的位置，要拿该函数返回值模上哈希表大小；
 * 有时还要解决冲突；
 * */
unsigned int BKDRHash(string str)
{
    unsigned int seed = 31;
    unsigned int hash = 0;
    int length = str.length();
    int i = 0;
    for(i; i<length; i++)
    {
        hash += hash*seed + str[i];
    }
    hash = hash * seed;
    //保证最高位为0，这样在赋值给int型变量时，不用进行符号转换
    int result = hash & 0x7fffffff;
    return result;
}

哈希冲突解决方法：依次往后找到一个空余位置，注意要循环找，
比如找到哈希表的最后一个还没找到，要回到第一个开始找
参考伪代码：
while(Hashtable[i]不为空)
{
    i=(i+1)%Hashtable_size;
}

#define MultiFactor 1.7 //哈比表大小为实际库所、变迁或弧个数的1.7倍

库所输出：从哈希表的第一个开始遍历到结束，如果那个单元是空的，则不输出，如果有元素，则输出
输出格式：
Total places：//库所实际数
conflict times://哈希冲突次数

id：             //该库所的id名字
index:           //该库所在哈希表中的索引位置
hashvalue:       //该库所根据哈希函数应该在哈希表中的位置（就是不解决冲突应该在哪，不是BKDR值注意）
initialMarking:  //初始token数
producer:        //前继节点的名称
consumer:        //后继结点的名称
//空一行
id：
index:
hashvalue:
initialMarking:
producer:
consumer:

变迁输出：从哈希表的第一个开始遍历到结束，如果那个单元是空的，则不输出，如果有元素，则输出
输出格式：
Total transitions:
conflict times:

id:          //该变迁的id名字
index:       //该变迁在哈希表中的索引位置
hashvalue:   //该变迁根据哈希函数应该在哈希表中的位置（就是不解决冲突应该在哪，不是BKDR值注意）
producer:    //前继节点的名称
consumer:    //后继结点的名称
//空一行
id:
index:
hashvalue:
producer:
consumer: