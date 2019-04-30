��ϣ������
/*unsigned int BKDRHash(string str);
 * function: get a string's hash value
 * BKDR: HashValue(str) = sum(str[i]*seed^(length-i-1))
 * in: string str
 * out: unsigned int hash value
 * ע�⣺�ú������ص��ǹ�ϣֵ���������ڹ�ϣ���е�λ�ã�
 * Ҫ�õ���ϣ���е�λ�ã�Ҫ�øú�������ֵģ�Ϲ�ϣ���С��
 * ��ʱ��Ҫ�����ͻ��
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
    //��֤���λΪ0�������ڸ�ֵ��int�ͱ���ʱ�����ý��з���ת��
    int result = hash & 0x7fffffff;
    return result;
}

��ϣ��ͻ������������������ҵ�һ������λ�ã�ע��Ҫѭ���ң�
�����ҵ���ϣ������һ����û�ҵ���Ҫ�ص���һ����ʼ��
�ο�α���룺
while(Hashtable[i]��Ϊ��)
{
    i=(i+1)%Hashtable_size;
}

#define MultiFactor 1.7 //���ȱ��СΪʵ�ʿ�������Ǩ�򻡸�����1.7��

����������ӹ�ϣ��ĵ�һ����ʼ����������������Ǹ���Ԫ�ǿյģ�������������Ԫ�أ������
�����ʽ��
Total places��//����ʵ����
conflict times://��ϣ��ͻ����

id��             //�ÿ�����id����
index:           //�ÿ����ڹ�ϣ���е�����λ��
hashvalue:       //�ÿ������ݹ�ϣ����Ӧ���ڹ�ϣ���е�λ�ã����ǲ������ͻӦ�����ģ�����BKDRֵע�⣩
initialMarking:  //��ʼtoken��
producer:        //ǰ�̽ڵ������
consumer:        //��̽�������
//��һ��
id��
index:
hashvalue:
initialMarking:
producer:
consumer:

��Ǩ������ӹ�ϣ��ĵ�һ����ʼ����������������Ǹ���Ԫ�ǿյģ�������������Ԫ�أ������
�����ʽ��
Total transitions:
conflict times:

id:          //�ñ�Ǩ��id����
index:       //�ñ�Ǩ�ڹ�ϣ���е�����λ��
hashvalue:   //�ñ�Ǩ���ݹ�ϣ����Ӧ���ڹ�ϣ���е�λ�ã����ǲ������ͻӦ�����ģ�����BKDRֵע�⣩
producer:    //ǰ�̽ڵ������
consumer:    //��̽�������
//��һ��
id:
index:
hashvalue:
producer:
consumer: