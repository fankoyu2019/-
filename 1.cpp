#include"iostream"
#include"string"
#include"algorithm"
#include"ctime"
#include"string.h"

using namespace std;


struct Internal_storage //固定分区分配的空闲分区表的结构体定义 
{
	int num;			//分区号 
	int size;			//分区大小（KB） 
	int inchoate_aspect;//分区起址（K） 
	string status;		//分区状态 
	char name;			//该分区的所分配给的任务名 
};

struct Work                  // 请求分配作业的结构体指针定义 
{
	char name;						//任务名 
	int id;							//分配空间的分区号 
	int size;						//任务大小 
	int headposition;				//任务在内存中的起始地址 
	Work *next;						//指向下一结点 
	Work(Work *pr=NULL){next=pr;}	//初始化下一节点指向空 
};

Work *whead=new Work;    // 作业指针的头结点 ，不存储内容 
int allwork;			// 已分配作业个数 

class IStorage_link		// 空闲分区链的结构体节点类 
{
public:
	int num;			// 分区号 
	int size;			//分区大小 
	int inchoate_aspect;	//分区起始地址 
	string status;			//状态 
	
	IStorage_link *next;	//指向下一结点 
	IStorage_link *last;	//指向前一结点 
};

class List				//	首次适应算法和循环适应算法的类定义 
{
private:
	IStorage_link *pHead;	//	链表头指针（不存放实际数值） 
	IStorage_link *pTail;	//	链表尾指针（指向前一个temp结点，以便能够生成当前结点的起始地址） 
	IStorage_link *pNow;	//	循环首次适应算法所需要用到的当前结点 
	int length;				//	分区总数 
	int size;				//	规定不再切割的剩余分区的大小 
public:
	List(int length)		//构造函数 
	{
		srand(time(0));		//随机数函数 
		size=20;
		this->length=length;
		pHead=new IStorage_link();	//定义头结点 
		pHead->last=NULL;			//初始前一结点为空 
		pTail=pHead;				 
		pNow=pHead;
		pHead->num=0;				//初始化num size inchoate_aspect为0 以便于生成下一结点的起始地址 
		pHead->size=0;
		pHead->inchoate_aspect=0;
		
		for(int i=0;i<length;i++)		//	生成按地址递增顺序排列的链表
		{
			IStorage_link *temp=new IStorage_link();
			temp->num=i+1;
			temp->size=rand()%200+1;
			temp->status="未分配";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			pTail->next=temp;
			pTail=temp;
		}
	}
	void Display()						//显示函数 
	{
		IStorage_link *p=pHead->next;
		cout<<"分区号\t大小\t起址\t状态"<<endl;
		while(p!=NULL)
		{
			cout<<p->num<<'\t'<<p->size<<'\t'<<p->inchoate_aspect<<'\t'<<p->status<<endl;
			p=p->next;
		}
	}
	void seek(int u_size,char m);		//首次适应算法分配函数 
	void seekNF(int u_size,char m);		// 循环首次适应算法分配函数 
	void Free();						//	回收内存函数 
	void Insert(Work *wq);				//	插入新分区函数 
};

void List::seek(int u_size,char m)		//首次适应算法分配函数 
{
		int k=0;						//“钥匙”，确定是否分配成功 
		IStorage_link *p=pHead->next;
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&p->status=="未分配")
			{
				if(p->size-u_size>size)		//如果所剩分区大小大于规定的最小分区大小 
				{
					w->headposition=p->inchoate_aspect+p->size-u_size;
					p->size-=u_size;
					w->id=p->num;
					w->name=m;
					
					w->size=u_size;
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				else					//否则 
				{
					w->size=p->size;
					p->size=0;
					p->status="已分配";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				k=1;
				cout<<"-----------------分配成功!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------分配失败!,没有足够的空间进行分配!"<<endl;
		}
		Display();
}

void List::seekNF(int u_size,char m)
{
		int k=0;
		IStorage_link *p=pNow->next;	//当前位置指针，从当前位置开始循环首次算法 
		IStorage_link *pm=pNow->next; 	//开始循环首次适应算法的位置指针，用来确定若未找到满足项，循环一圈后退出循环 
		if(p==NULL)						//当 当前位置指针指向末尾后，跳转到链表头指针 
		{
			p=pHead->next;
		}
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&p->status=="未分配")
			{
				if(p->size-u_size>size)
				{
					w->headposition=p->inchoate_aspect+p->size-u_size;
					p->size-=u_size;
					w->id=p->num;
					w->name=m;
					
					w->size=u_size;
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				else
				{
					w->size=p->size;
					p->size=0;
					p->status="已分配";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				pNow=p;
				k=1;
				cout<<"-----------------分配成功!-----------"<<endl;
				break;
			}
			p=p->next;
			if(p==NULL)
			{
				p=pHead->next;
			}
			if(p->inchoate_aspect==pm->inchoate_aspect)		//通过起始地址是否相等，判断是否完成一圈 
			{
				break;
			}
		}
		if(k==0)
		{
			cout<<endl<<"----------分配失败!,没有足够的空间进行分配!"<<endl;
		}
		Display();
}

void List::Insert(Work *wq)
{
	IStorage_link *p=new IStorage_link();		//定义新的结点，并对相应项进行赋值 
	p->num=wq->id;
	p->inchoate_aspect=wq->headposition;
	p->size=wq->size;
	p->status="未分配";
	p->next=NULL;
	IStorage_link *p1=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(p1!=NULL)							//将新结点插入到合适位置，使其链表按地址从低到高
	{
		if((p1->inchoate_aspect)>(p->inchoate_aspect+p->size))		 
		{
			p->next=p1;
			p1->last=p;
			p2->next=p;
			p->last=p2;
			k=1;
			break;
		}
		p2=p2->next;
		p1=p1->next;
	}
	
	if(k==1)
	{
		p2->next=p;
		p->last=p2;
	}
}

void List::Free()
{
	cout<<endl;																				// 输出当前已分配的作业信息 
	cout<<"作业名\t区号\t大小\t起址\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------要申请释放的作业名:";
				char name;
				cin>>name;
				Work *wq=new Work;
				Work *wa=new Work;
				wq=whead->next;
				wa=whead;
				IStorage_link *p=pHead->next;
				while(wq!=NULL)
				{
					if(name==wq->name)								//按作业名进行匹配 
					{
						while(p!=NULL)								
						{
							if(p->num==wq->id)						//查找到作业所在分区号 
							{
								if(p->status=="已分配")				//若该分区已分配完 
								{
									p->status="未分配";
									p->size=wq->size;
									p->inchoate_aspect=wq->headposition;
									break;
								}
								if((p->inchoate_aspect+p->size)!=(wq->headposition)&&(wq->headposition+wq->size)!=(p->next->inchoate_aspect))		//回收区既不与前一空闲区相邻，又不与后一空闲区相邻 
								{
									Insert(wq);
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)!=p->next->inchoate_aspect)			//回收区与前一空闲区相邻，但不与后一空闲区相邻 
								{
									p->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)!=wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)			//回收区与后一空闲区相邻，但不与前一空闲区相邻 
								{
									p->next->inchoate_aspect=wq->headposition;
									p->next->size+=wq->size;
									break;
								}		
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)			//回收区与前后空闲区都相邻 
								{
									p->size=p->size+wq->size+p->next->size;
									p->next=p->next->next;
									p->next->next->last=p;
									break;
								}
								
							}
							p=p->next;
						}
						allwork--;				//分配作业数-1（并没有什么用） 
						Display();
						wa->next=wq->next;
						break;
					}
					wa=wa->next;
					wq=wq->next;
				}
}

bool cmp(Internal_storage a,Internal_storage b)			//sort函数的第三个参数，结构体进行比较时要用到的比较函数 
{
	if(a.size<b.size)
		return true;
	else return false;
}

void fixation()										//固定分区界面 
{
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"|                    固定分区分配                       |"<<endl;
		cout<<"---------------------------------------------------------"<<endl;
		cout<<endl;
		Internal_storage a[10];
		int n;
		cout<<"|                   输入分区的数量:";
		cin>>n;
		cout<<endl;
		int i;
		a[0].num=0;
		a[0].size=0;
		a[0].inchoate_aspect=0;
		srand(time(0));
		for(i=1;i<=n;i++)				//初始化空闲分区表 
		{
			
			a[i].num=i;
			a[i].size=rand()%100+1;
			a[i].status="未分配";
			a[i].inchoate_aspect=a[i-1].inchoate_aspect+a[i-1].size;
		}
		sort(a+1,a+n+1,cmp);
		cout<<"分区号\t大小\t起址\t状态"<<endl;		//输出空闲分区表 
		for(i=1;i<=n;i++)
		{
			cout<<a[i].num<<'\t'<<a[i].size<<'\t'<<a[i].inchoate_aspect<<'\t'<<a[i].status<<endl;
		}
		char m='A';
		int size;
		
		while(1)					//作业分配 
		{
			int k=0;
			cout<<"输入作业 "<<m<<" 所需要的空间大小(KB)：";
			cin>>size;
			for(i=1;i<=n;i++)
			{
				if(size<=a[i].size&&a[i].status=="未分配")
				{
					k=1;
					a[i].name=m;
					a[i].status="已分配";
					cout<<"|-------------为该程序分配内存成功!"<<endl;
					
					cout<<"分区号\t大小\t起址\t状态"<<endl;		//输出空闲分区表 
					for(i=1;i<=n;i++)
					{
						cout<<a[i].num<<'\t'<<a[i].size<<'\t'<<a[i].inchoate_aspect<<'\t'<<a[i].status<<endl;
					}
					
					break;
				}
			}
			if(k==0)
			{
				cout<<"|-------------为该程序分配内存失败，未找到大小足够的分区!"<<endl;
			}
			while(1)
			{
				cout<<endl;
				cout<<"|-----------是否申请某作业释放内存(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					
					cout<<"|---------要申请释放的作业名:";
					char name;
					cin>>name;
					for(i=1;i<=n;i++)
					{
						if(name==a[i].name)
						{
							a[i].name=' ';
							a[i].status="未分配";
							cout<<"|---------------释放成功。";
							break;
						}
					}
					if(i==n+1)
						cout<<"|---------------释放失败！未找到该作业名。";
				}
				else break;
			}
			cout<<endl;
			cout<<"|-----------是否继续增加作业（y/n）:";
			char x;
			cin>>x;
			if(x=='y'||x=='Y')
			{
				cout<<"分区号\t大小\t起址\t状态"<<endl;
				for(i=1;i<=n;i++)
				{
					cout<<a[i].num<<'\t'<<a[i].size<<'\t'<<a[i].inchoate_aspect<<'\t'<<a[i].status<<endl;
				}
				m=m+1;
			}
			else
				break;
		}
		system("pause");
}

void FF()			//首次适应算法调用函数 
{	
	
	int n;
	cout<<"|                   输入分区的数量:";
	cin>>n;
	cout<<endl;
	List a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"输入作业 "<<m<<" 所需要的空间大小(KB)：";
		cin>>u_size;
		a.seek(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------是否申请某作业释放内存(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------是否继续增加作业（y/n）:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
		
}


void NF()			//循环首次适应算法调用函数 
{
	int n;
	cout<<"|                   输入分区的数量:";
	cin>>n;
	cout<<endl;
	List a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"输入作业 "<<m<<" 所需要的空间大小(KB)：";
		cin>>u_size;
		a.seekNF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------是否申请某作业释放内存(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------是否继续增加作业（y/n）:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

class ListBF		//最佳适应算法 空闲分区链 
{
private:
	IStorage_link *pHead;
	IStorage_link *pTail;
	IStorage_link *pNow;      //没用 
	int length;
	int size;
public:
	ListBF(int length)
	{
		srand(time(0));
		size=20;
		this->length=length;
		pHead=new IStorage_link();

		pHead->next=NULL;

		pHead->last=NULL;
		pTail=pHead;
		
		pHead->num=0;
		pHead->size=0;
		pHead->inchoate_aspect=0;
		for(int i=0;i<length;i++)
		{
			IStorage_link *temp=new IStorage_link();
			temp->num=i+1;
			temp->size=rand()%200+1;
			temp->status="未分配";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			int k=0;
			IStorage_link *p1=pHead->next;
			IStorage_link *p2=pHead;

			while(p1!=NULL)											//将分区结点按空闲区大小按从小到大顺序进行插入 
			{
				if(temp->size<=p1->size)
				{
					temp->next=p1;
					p2->next=temp;
					p1->last=temp;
					temp->last=p2;
					k=1;
					break;
				}
				p2=p2->next;
				p1=p1->next;
			}

			if(k==0)
			{
				p2->next=temp;
				temp->last=p2;
				
			}

			//pTail->next=temp;
			pTail=temp;
		}
	}
	void Display()
	{
		IStorage_link *p=pHead->next;
		cout<<"分区号\t大小\t起址\t状态"<<endl;
		while(p!=NULL)
		{
			cout<<p->num<<'\t'<<p->size<<'\t'<<p->inchoate_aspect<<'\t'<<p->status<<endl;
			p=p->next;
		}
	}
	void seekBF(int u_size,char m);
	void Free();
	void Insert(Work *wq);
	void Push(IStorage_link *p);			//将新结点按空闲区大小 从小到大顺序插入到链表适当位置 
};

void ListBF::Push(IStorage_link *p)
{
	p->last->next=p->next;
	if(p->next!=NULL)
		p->next->last=p->last;
	p->next=NULL;
	p->last=NULL;
	IStorage_link *pa=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(pa!=NULL)
	{
		if(p->size<=pa->size)
				{
					p->next=pa;
					p2->next=p;
					pa->last=p;
					p->last=p2;
					k=1;
					break;
				}
				p2=p2->next;
				pa=pa->next;
	}
	if(k==0)
			{
				p2->next=p;
				p->last=p2;
			}
}

void ListBF::seekBF(int u_size,char m)
{
		int k=0;
		IStorage_link *p=pHead->next;
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&(p->status=="未分配"))
			{
				if(p->size-u_size>size)
				{
					w->headposition=p->inchoate_aspect+p->size-u_size;
					p->size-=u_size;
					w->id=p->num;
					w->name=m;
					
					w->size=u_size;
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				else
				{
					w->size=p->size;
					p->size=0;
					p->status="已分配";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}

				Push(p);

				k=1;
				cout<<"-----------------分配成功!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------分配失败!,没有足够的空间进行分配!"<<endl;
		}
		Display();
}

void ListBF::Free()
{
	cout<<endl;
	cout<<"作业名\t区号\t大小\t起址\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------要申请释放的作业名:";
				char name;
				cin>>name;
				Work *wq=new Work;
				Work *wa=new Work;
				wq=whead->next;
				wa=whead;
				IStorage_link *p=pHead->next;
				while(wq!=NULL)
				{
					if(name==wq->name)
					{
						while(p!=NULL)
						{
							if(p->num==wq->id)
							{
								if(p->status=="已分配")						//若该分区空闲区为0
								{
									p->status="未分配";
									p->size=wq->size;
									p->inchoate_aspect=wq->headposition;
									break;
								}
								int k=0;
								if((p->inchoate_aspect+p->size)==wq->headposition)					
								{
									IStorage_link *l1=pHead->next;
									while(l1!=NULL)
									{
										if((wq->headposition+wq->size)==l1->inchoate_aspect)	 //回收区与前后空闲区都相邻 
										{
											p->size=p->size+wq->size+l1->size;
											if(l1->next!=NULL)
												l1->next->last=l1->last;
											l1->last->next=l1->next;
											k=1;
											break;
										}
										l1=l1->next;
									}
									if(k==0)													//回收区与前一空闲区相邻，但不与后一空闲区相邻 
									{
										p->size=p->size+wq->size;
									}
									break;
								}
								else
								{
									IStorage_link *l2=pHead->next;
									while(l2!=NULL)
									{
										if((wq->headposition+wq->size)==l2->inchoate_aspect)	//回收区与后一空闲区相邻，但不与前一空闲区相邻 
										{
											l2->inchoate_aspect=wq->headposition;
											l2->size+=wq->size;
											k=1;
											break;
										}
										l2=l2->next;
									}
									if(k==0)													//回收区既不与前一空闲区相邻，又不与后一空闲区相邻
									{
										Insert(wq);	
									}
									break;
								}
							/*	if((p->inchoate_aspect+p->size)!=(wq->headposition)&&(wq->headposition+wq->size)!=(p->next->inchoate_aspect))
								{
									Insert(wq);
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)!=p->next->inchoate_aspect)
								{
									p->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)!=wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)
								{
									p->next->inchoate_aspect=wq->headposition;
									p->next->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)
								{
									p->size=p->size+wq->size+p->next->size;
									p->next=p->next->next;
									p->next->next->last=p;
									break;
								}
								*/
							}
							p=p->next;
						}
						Push(p);
						allwork--;
						Display();
						wa->next=wq->next;
						break;
					}
					wa=wa->next;
					wq=wq->next;
				}
}

void ListBF::Insert(Work *wq)								//按空闲区从小到大顺序插入到链表适当位置 
{
	IStorage_link *p=new IStorage_link();					
	p->num=wq->id;
	p->inchoate_aspect=wq->headposition;
	p->size=wq->size;
	p->status="未分配";
	p->next=NULL;
	IStorage_link *p1=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(p1!=NULL)
	{
		if(p1->size>p->size)
		{
			p->next=p1;
			p1->last=p;
			p2->next=p;
			p->last=p2;
			k=1;
			break;
		}
		p2=p2->next;
		p1=p1->next;
	}
	if(k==1)
	{
		p2->next=p;
		p->last=p2;
	}
}

void BF()					//最佳适应算法调用函数 
{
	int n;
	cout<<"|                   输入分区的数量:";
	cin>>n;
	cout<<endl;
	ListBF a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"输入作业 "<<m<<" 所需要的空间大小(KB)：";
		cin>>u_size;
		a.seekBF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------是否申请某作业释放内存(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------是否继续增加作业（y/n）:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

class ListWF					//最坏适应算法 空闲分区链 
{
private:
	IStorage_link *pHead;
	IStorage_link *pTail;
	IStorage_link *pNow;
	int length;
	int size;
public:
	ListWF(int length)
	{
		srand(time(0));
		size=20;
		this->length=length;
		pHead=new IStorage_link();

		pHead->next=NULL;

		pHead->last=NULL;
		pTail=pHead;
		
		pHead->num=0;
		pHead->size=0;
		pHead->inchoate_aspect=0;
		for(int i=0;i<length;i++)
		{
			IStorage_link *temp=new IStorage_link();
			temp->num=i+1;
			temp->size=rand()%200+1;
			temp->status="未分配";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			int k=0;
			IStorage_link *p1=pHead->next;
			IStorage_link *p2=pHead;

			while(p1!=NULL)
			{
				if(temp->size>=p1->size)								//	从大到小 
				{
					temp->next=p1;
					p2->next=temp;
					p1->last=temp;
					temp->last=p2;
					k=1;
					break;
				}
				p2=p2->next;
				p1=p1->next;
			}

			if(k==0)
			{
				p2->next=temp;
				temp->last=p2;
			}

			//pTail->next=temp;
			pTail=temp;
		}
	}
	void Display()
	{
		IStorage_link *p=pHead->next;
		cout<<"分区号\t大小\t起址\t状态"<<endl;
		while(p!=NULL)
		{
			cout<<p->num<<'\t'<<p->size<<'\t'<<p->inchoate_aspect<<'\t'<<p->status<<endl;
			p=p->next;
		}
	}
	void seekWF(int u_size,char m);
	void Free();
	void Insert(Work *wq);
	void Push(IStorage_link *p);
};

void ListWF::seekWF(int u_size,char m)
{
	int k=0;
		IStorage_link *p=pHead->next;
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&(p->status=="未分配"))								 
			{
				if(p->size-u_size>size)													
				{
					w->headposition=p->inchoate_aspect+p->size-u_size;
					p->size-=u_size;
					w->id=p->num;
					w->name=m;
					
					w->size=u_size;
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				else
				{
					w->size=p->size;
					p->size=0;
					p->status="已分配";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}

				Push(p);

				k=1;
				cout<<"-----------------分配成功!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------分配失败!,没有足够的空间进行分配!"<<endl;
		}
		Display();
}

void ListWF::Push(IStorage_link *p)						//	从大到小 
{
	p->last->next=p->next;
	if(p->next!=NULL)
		p->next->last=p->last;
	p->next=NULL;
	p->last=NULL;
	IStorage_link *pa=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(pa!=NULL)
	{
		if(p->size>=pa->size)
				{
					p->next=pa;
					p2->next=p;
					pa->last=p;
					p->last=p2;
					k=1;
					break;
				}
				p2=p2->next;
				pa=pa->next;
	}
	if(k==0)
			{
				p2->next=p;
				p->last=p2;
			}
}

void ListWF::Free()
{
	cout<<endl;
	cout<<"作业名\t区号\t大小\t起址\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------要申请释放的作业名:";
				char name;
				cin>>name;
				Work *wq=new Work;
				Work *wa=new Work;
				wq=whead->next;
				wa=whead;
				IStorage_link *p=pHead->next;
				while(wq!=NULL)
				{
					if(name==wq->name)
					{
						while(p!=NULL)
						{
							if(p->num==wq->id)
							{
								if(p->status=="已分配")
								{
									p->status="未分配";
									p->size=wq->size;
									p->inchoate_aspect=wq->headposition;
									break;
								}
								int k=0;
								if((p->inchoate_aspect+p->size)==wq->headposition)
								{
									IStorage_link *l1=pHead->next;
									while(l1!=NULL)
									{
										if((wq->headposition+wq->size)==l1->inchoate_aspect)
										{
											p->size=p->size+wq->size+l1->size;
											if(l1->next!=NULL)
												l1->next->last=l1->last;
											l1->last->next=l1->next;
											k=1;
											break;
										}
										l1=l1->next;
									}
									if(k==0)
									{
										p->size=p->size+wq->size;
									}
									break;
								}
								else
								{
									IStorage_link *l2=pHead->next;
									while(l2!=NULL)
									{
										if((wq->headposition+wq->size)==l2->inchoate_aspect)
										{
											l2->inchoate_aspect=wq->headposition;
											l2->size+=wq->size;
											k=1;
											break;
										}
										l2=l2->next;
									}
									if(k==0)
									{
										Insert(wq);
									}
									break;
								}
							/*	if((p->inchoate_aspect+p->size)!=(wq->headposition)&&(wq->headposition+wq->size)!=(p->next->inchoate_aspect))
								{
									Insert(wq);
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)!=p->next->inchoate_aspect)
								{
									p->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)!=wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)
								{
									p->next->inchoate_aspect=wq->headposition;
									p->next->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)
								{
									p->size=p->size+wq->size+p->next->size;
									p->next=p->next->next;
									p->next->next->last=p;
									break;
								}
								*/
							}
							p=p->next;
						}
						Push(p);
						allwork--;
						Display();
						wa->next=wq->next;
						break;
					}
					wa=wa->next;
					wq=wq->next;
				}
}

void ListWF::Insert(Work *wq)								
{
	IStorage_link *p=new IStorage_link();
	p->num=wq->id;
	p->inchoate_aspect=wq->headposition;
	p->size=wq->size;
	p->status="未分配";
	p->next=NULL;
	IStorage_link *p1=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(p1!=NULL)
	{
		if(p1->size<=p->size)					//	从大到小 
		{
			p->next=p1;
			p1->last=p;
			p2->next=p;
			p->last=p2;
			k=1;
			break;
		}
		p2=p2->next;
		p1=p1->next;
	}
	if(k==1)
	{
		p2->next=p;
		p->last=p2;
	}
}

void WF()										//最坏适应算法调用函数 
{
	int n;
	cout<<"|                   输入分区的数量:";
	cin>>n;
	cout<<endl;
	ListWF a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"输入作业 "<<m<<" 所需要的空间大小(KB)：";
		cin>>u_size;
		a.seekWF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------是否申请某作业释放内存(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------是否继续增加作业（y/n）:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

void dynamic_state()//可变分区分配选择界面 
{
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"|                    可变式分区分配                     |"<<endl;
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"                   1.首次适应算法                        "<<endl;
	cout<<"                  2.循环首次适应算法                     "<<endl;
	cout<<"                   3.最佳适应算法                        "<<endl;
	cout<<"                   4.最坏适应算法                        "<<endl;
	cout<<"                  请选择：";
	int x;
		cin>>x;
	switch (x)
	{
	case 1:FF();break;
	case 2:NF();break;
	case 3:BF();break;
	case 4:WF();break;
	default:cout<<"选择出错请重新选择!"<<endl;
		break;
	}
}

int main()//主菜单界面 
{
	while(1)
	{
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"|          选择固定分区分配or可变式分区分配             |"<<endl;
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"                   1.固定分区分配                        "<<endl;
		cout<<"                  2.可变式分区分配                       "<<endl;
		cout<<"                       3.退出                            "<<endl;
		cout<<"                  请选择：";
		int x;
		cin>>x;
		switch (x)
		{
		case 1:fixation();break;
		case 2:dynamic_state();break;
		case 3:exit(0);break;
		default:cout<<"选择出错请重新选择!"<<endl;
			break;
		}
		exit(0);
	}
}
