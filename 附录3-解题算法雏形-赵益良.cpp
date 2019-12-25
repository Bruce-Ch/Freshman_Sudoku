#include <iostream>
#include <stdlib.h>
#include <string.h>
using namespace std;
int shudu[10][10];//�������е�����
int row[10][10],column[10][10],block[4][4][10];//���ÿ��ÿ��ÿ���������Ƿ���ã���Ϊ1�򲻿���
int known[10][10];//��ⵥԪ���������Ƿ��Ѿ����룬�Ѿ�������Ϊ1
int flag=1;//���������������0Ϊ�޽⣬1ΪΨһ�⣬2Ϊ���
int cnt=0;//��������
int now[10][10];//�ݴ��һ������ΪΨһ�⣩�����е�����

bool Line[10][10];//Line[i][j]:�жϵ�i������j��û�г��ֹ�
bool Column[10][10];//Column[i][j]:�жϵ�i������j��û�г��ֹ�
bool Block[10][10];//Block[i][j]:�жϵ�i������j��û�г��ֹ�
bool check()//�ж������Ƿ����
{
    memset(Line,0,sizeof(Line));
    memset(Column,0,sizeof(Column));
    memset(Block,0,sizeof(Block));
    for(int i=1;i<=9;++i)
    {
        for(int j=1;j<=9;++j)
        {
            int Blocknum = (i-1)-((i-1)% 3) + ((j-1) / 3 +1);//������������ڵڼ����Ź���
            Line[i][shudu[i][j]] = 1;
            Column[j][shudu[i][j]] = 1;
            Block[Blocknum][shudu[i][j]] = 1;//�ڼ����Ź���ĵڼ������Ǵ��ڵ�
        }
    }
    for(int i=1;i<=9;++i)
    {
        for(int j=1;j<=9;++j)
        {
            if(!Line[i][j])return 0;
            if(!Column[i][j])return 0;
            if(!Block[i][j])return 0;
        }
    }
    return 1;
}

void print()//�������
{
    cout<<endl;//�������һ��
    //cout<<"OK"<<endl;//�н��־
    for (int i=1;i<=9;i++){
        for (int j=1;j<=9;j++){
            cout<<shudu[i][j]<<" ";
        }
        cout<<endl;    //����
    }
}

void search(int x,int y){//������һ��һ�н���
    //cout<<cnt<<endl;
    if (known[x][y])//�������Ƿ�����������
    {
        search((9*x+y-9)/9+1,y%9+1);
    }//���Ѿ����룬������һ����Ԫ
    else
    {
        if (x==10 && y==1) //�Ѿ�ѭ����9x9
        {
            if(cnt==2)return;//������2��������ˣ��������������
            if(check())cnt++;//���ͳ��
            if(cnt==1)
            {
                for (int i=1;i<=9;i++)
                {
                    for (int j=1;j<=9;j++)
                    {
                        now[i][j]=shudu[i][j];
                    }
                }
            }
            else            	//(cnt==2)
            {
                puts("Multiple_solutions");
                for (int i=1;i<=9;i++)
                {
                    for (int j=1;j<=9;j++)
                    {
                        cout<<now[i][j]<<" ";
                    }
                    cout<<endl;    //����
                }//�����һ��
                print();//������2����

            }

        }
        else
            for (int i=1;i<=9;i++)//ö�ٴ�1��9
            {
                if (row[x][i]==0 && column[y][i]==0 && block[(x-1)/3+1][(y-1)/3+1][i]==0){//�ж��Ƿ�Ϸ��������й���δ����i
                    shudu[x][y]=i;//����
                    row[x][i]=1;//��Ǵ����Ѿ��������
                    column[y][i]=1;//��Ǵ����Ѿ��������
                    block[(x-1)/3+1][(y-1)/3+1][i]=1;//��Ǵ˹��Ѿ��������
                    search((9*x+y-9)/9+1,y%9+1);//�����¸���Ԫ��
                    //����
                    shudu[x][y]=0;
                    row[x][i]=0;
                    column[y][i]=0;
                    block[(x-1)/3+1][(y-1)/3+1][i]=0;
                }
            }
    }
}
int main()
{
    string s[20];//�����ַ��������������
    for(int i=1;i<=9;++i)
    {
        //cin>>s[i];//��������
        getline(cin,s[i]);
    }
    for(int i=1;i<=9;++i)
    {
        int num = 1;
        for(int j=1;j<=s[i].size();++j)
        {
            if(s[i][j-1]=='-')
            {
                shudu[i][num]=0;//���� �� - ת��0����������
                num++;
            }
            else if(s[i][j-1]==' ')continue;//����ǿո�Ͳ���
            else
            {
                shudu[i][num]=s[i][j-1]-'0';//�����ַ����� תΪint��
                num++;
            }
        }
    }
    for (int i=1;i<=9;i++){
        for (int j=1;j<=9;j++)
        {
            if (shudu[i][j])
                known[i][j]=1;//������벻Ϊ0�������֪
        }
    }
    for (int i=1;i<=9;i++)
    {
        for (int j=1;j<=9;j++)
        {
            if (shudu[i][j])//���ÿ�У��У����Ѿ����ڵ�����
            {
                row[i][shudu[i][j]]=1; //��i���Ѿ�������shudu[i][j]
                column[j][shudu[i][j]]=1;//��j���Ѿ�������shudu[i][j]
                block[(i-1)/3+1][(j-1)/3+1][shudu[i][j]]=1;//�����Ѿ�������shudu[i][j]
            }
        }
    }
    search(1,1);//��ͷ��ʼ����
    if(cnt==0)puts("No_solution");//�޽����
    else if (cnt==1)//���Ψһ��
    {
        puts("OK");
        for (int i=1;i<=9;i++){
            for (int j=1;j<=9;j++){
                cout<<now[i][j]<<" ";
            }
            cout<<endl;    //����
        }
    }
    return 0;
}

