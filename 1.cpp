#include"iostream"
#include"string"
#include"algorithm"
#include"ctime"
#include"string.h"

using namespace std;


struct Internal_storage //�̶���������Ŀ��з�����Ľṹ�嶨�� 
{
	int num;			//������ 
	int size;			//������С��KB�� 
	int inchoate_aspect;//������ַ��K�� 
	string status;		//����״̬ 
	char name;			//�÷�������������������� 
};

struct Work                  // ���������ҵ�Ľṹ��ָ�붨�� 
{
	char name;						//������ 
	int id;							//����ռ�ķ����� 
	int size;						//�����С 
	int headposition;				//�������ڴ��е���ʼ��ַ 
	Work *next;						//ָ����һ��� 
	Work(Work *pr=NULL){next=pr;}	//��ʼ����һ�ڵ�ָ��� 
};

Work *whead=new Work;    // ��ҵָ���ͷ��� �����洢���� 
int allwork;			// �ѷ�����ҵ���� 

class IStorage_link		// ���з������Ľṹ��ڵ��� 
{
public:
	int num;			// ������ 
	int size;			//������С 
	int inchoate_aspect;	//������ʼ��ַ 
	string status;			//״̬ 
	
	IStorage_link *next;	//ָ����һ��� 
	IStorage_link *last;	//ָ��ǰһ��� 
};

class List				//	�״���Ӧ�㷨��ѭ����Ӧ�㷨���ඨ�� 
{
private:
	IStorage_link *pHead;	//	����ͷָ�루�����ʵ����ֵ�� 
	IStorage_link *pTail;	//	����βָ�루ָ��ǰһ��temp��㣬�Ա��ܹ����ɵ�ǰ������ʼ��ַ�� 
	IStorage_link *pNow;	//	ѭ���״���Ӧ�㷨����Ҫ�õ��ĵ�ǰ��� 
	int length;				//	�������� 
	int size;				//	�涨�����и��ʣ������Ĵ�С 
public:
	List(int length)		//���캯�� 
	{
		srand(time(0));		//��������� 
		size=20;
		this->length=length;
		pHead=new IStorage_link();	//����ͷ��� 
		pHead->last=NULL;			//��ʼǰһ���Ϊ�� 
		pTail=pHead;				 
		pNow=pHead;
		pHead->num=0;				//��ʼ��num size inchoate_aspectΪ0 �Ա���������һ������ʼ��ַ 
		pHead->size=0;
		pHead->inchoate_aspect=0;
		
		for(int i=0;i<length;i++)		//	���ɰ���ַ����˳�����е�����
		{
			IStorage_link *temp=new IStorage_link();
			temp->num=i+1;
			temp->size=rand()%200+1;
			temp->status="δ����";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			pTail->next=temp;
			pTail=temp;
		}
	}
	void Display()						//��ʾ���� 
	{
		IStorage_link *p=pHead->next;
		cout<<"������\t��С\t��ַ\t״̬"<<endl;
		while(p!=NULL)
		{
			cout<<p->num<<'\t'<<p->size<<'\t'<<p->inchoate_aspect<<'\t'<<p->status<<endl;
			p=p->next;
		}
	}
	void seek(int u_size,char m);		//�״���Ӧ�㷨���亯�� 
	void seekNF(int u_size,char m);		// ѭ���״���Ӧ�㷨���亯�� 
	void Free();						//	�����ڴ溯�� 
	void Insert(Work *wq);				//	�����·������� 
};

void List::seek(int u_size,char m)		//�״���Ӧ�㷨���亯�� 
{
		int k=0;						//��Կ�ס���ȷ���Ƿ����ɹ� 
		IStorage_link *p=pHead->next;
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&p->status=="δ����")
			{
				if(p->size-u_size>size)		//�����ʣ������С���ڹ涨����С������С 
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
				else					//���� 
				{
					w->size=p->size;
					p->size=0;
					p->status="�ѷ���";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				k=1;
				cout<<"-----------------����ɹ�!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------����ʧ��!,û���㹻�Ŀռ���з���!"<<endl;
		}
		Display();
}

void List::seekNF(int u_size,char m)
{
		int k=0;
		IStorage_link *p=pNow->next;	//��ǰλ��ָ�룬�ӵ�ǰλ�ÿ�ʼѭ���״��㷨 
		IStorage_link *pm=pNow->next; 	//��ʼѭ���״���Ӧ�㷨��λ��ָ�룬����ȷ����δ�ҵ������ѭ��һȦ���˳�ѭ�� 
		if(p==NULL)						//�� ��ǰλ��ָ��ָ��ĩβ����ת������ͷָ�� 
		{
			p=pHead->next;
		}
		Work *w=new Work;
		while(p!=NULL)
		{
			if(p->size>=u_size&&p->status=="δ����")
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
					p->status="�ѷ���";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}
				pNow=p;
				k=1;
				cout<<"-----------------����ɹ�!-----------"<<endl;
				break;
			}
			p=p->next;
			if(p==NULL)
			{
				p=pHead->next;
			}
			if(p->inchoate_aspect==pm->inchoate_aspect)		//ͨ����ʼ��ַ�Ƿ���ȣ��ж��Ƿ����һȦ 
			{
				break;
			}
		}
		if(k==0)
		{
			cout<<endl<<"----------����ʧ��!,û���㹻�Ŀռ���з���!"<<endl;
		}
		Display();
}

void List::Insert(Work *wq)
{
	IStorage_link *p=new IStorage_link();		//�����µĽ�㣬������Ӧ����и�ֵ 
	p->num=wq->id;
	p->inchoate_aspect=wq->headposition;
	p->size=wq->size;
	p->status="δ����";
	p->next=NULL;
	IStorage_link *p1=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(p1!=NULL)							//���½����뵽����λ�ã�ʹ��������ַ�ӵ͵���
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
	cout<<endl;																				// �����ǰ�ѷ������ҵ��Ϣ 
	cout<<"��ҵ��\t����\t��С\t��ַ\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------Ҫ�����ͷŵ���ҵ��:";
				char name;
				cin>>name;
				Work *wq=new Work;
				Work *wa=new Work;
				wq=whead->next;
				wa=whead;
				IStorage_link *p=pHead->next;
				while(wq!=NULL)
				{
					if(name==wq->name)								//����ҵ������ƥ�� 
					{
						while(p!=NULL)								
						{
							if(p->num==wq->id)						//���ҵ���ҵ���ڷ����� 
							{
								if(p->status=="�ѷ���")				//���÷����ѷ����� 
								{
									p->status="δ����";
									p->size=wq->size;
									p->inchoate_aspect=wq->headposition;
									break;
								}
								if((p->inchoate_aspect+p->size)!=(wq->headposition)&&(wq->headposition+wq->size)!=(p->next->inchoate_aspect))		//�������Ȳ���ǰһ���������ڣ��ֲ����һ���������� 
								{
									Insert(wq);
									break;
								}
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)!=p->next->inchoate_aspect)			//��������ǰһ���������ڣ��������һ���������� 
								{
									p->size+=wq->size;
									break;
								}
								if((p->inchoate_aspect+p->size)!=wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)			//���������һ���������ڣ�������ǰһ���������� 
								{
									p->next->inchoate_aspect=wq->headposition;
									p->next->size+=wq->size;
									break;
								}		
								if((p->inchoate_aspect+p->size)==wq->headposition&&(wq->headposition+wq->size)==p->next->inchoate_aspect)			//��������ǰ������������� 
								{
									p->size=p->size+wq->size+p->next->size;
									p->next=p->next->next;
									p->next->next->last=p;
									break;
								}
								
							}
							p=p->next;
						}
						allwork--;				//������ҵ��-1����û��ʲô�ã� 
						Display();
						wa->next=wq->next;
						break;
					}
					wa=wa->next;
					wq=wq->next;
				}
}

bool cmp(Internal_storage a,Internal_storage b)			//sort�����ĵ������������ṹ����бȽ�ʱҪ�õ��ıȽϺ��� 
{
	if(a.size<b.size)
		return true;
	else return false;
}

void fixation()										//�̶��������� 
{
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"|                    �̶���������                       |"<<endl;
		cout<<"---------------------------------------------------------"<<endl;
		cout<<endl;
		Internal_storage a[10];
		int n;
		cout<<"|                   �������������:";
		cin>>n;
		cout<<endl;
		int i;
		a[0].num=0;
		a[0].size=0;
		a[0].inchoate_aspect=0;
		srand(time(0));
		for(i=1;i<=n;i++)				//��ʼ�����з����� 
		{
			
			a[i].num=i;
			a[i].size=rand()%100+1;
			a[i].status="δ����";
			a[i].inchoate_aspect=a[i-1].inchoate_aspect+a[i-1].size;
		}
		sort(a+1,a+n+1,cmp);
		cout<<"������\t��С\t��ַ\t״̬"<<endl;		//������з����� 
		for(i=1;i<=n;i++)
		{
			cout<<a[i].num<<'\t'<<a[i].size<<'\t'<<a[i].inchoate_aspect<<'\t'<<a[i].status<<endl;
		}
		char m='A';
		int size;
		
		while(1)					//��ҵ���� 
		{
			int k=0;
			cout<<"������ҵ "<<m<<" ����Ҫ�Ŀռ��С(KB)��";
			cin>>size;
			for(i=1;i<=n;i++)
			{
				if(size<=a[i].size&&a[i].status=="δ����")
				{
					k=1;
					a[i].name=m;
					a[i].status="�ѷ���";
					cout<<"|-------------Ϊ�ó�������ڴ�ɹ�!"<<endl;
					
					cout<<"������\t��С\t��ַ\t״̬"<<endl;		//������з����� 
					for(i=1;i<=n;i++)
					{
						cout<<a[i].num<<'\t'<<a[i].size<<'\t'<<a[i].inchoate_aspect<<'\t'<<a[i].status<<endl;
					}
					
					break;
				}
			}
			if(k==0)
			{
				cout<<"|-------------Ϊ�ó�������ڴ�ʧ�ܣ�δ�ҵ���С�㹻�ķ���!"<<endl;
			}
			while(1)
			{
				cout<<endl;
				cout<<"|-----------�Ƿ�����ĳ��ҵ�ͷ��ڴ�(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					
					cout<<"|---------Ҫ�����ͷŵ���ҵ��:";
					char name;
					cin>>name;
					for(i=1;i<=n;i++)
					{
						if(name==a[i].name)
						{
							a[i].name=' ';
							a[i].status="δ����";
							cout<<"|---------------�ͷųɹ���";
							break;
						}
					}
					if(i==n+1)
						cout<<"|---------------�ͷ�ʧ�ܣ�δ�ҵ�����ҵ����";
				}
				else break;
			}
			cout<<endl;
			cout<<"|-----------�Ƿ����������ҵ��y/n��:";
			char x;
			cin>>x;
			if(x=='y'||x=='Y')
			{
				cout<<"������\t��С\t��ַ\t״̬"<<endl;
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

void FF()			//�״���Ӧ�㷨���ú��� 
{	
	
	int n;
	cout<<"|                   �������������:";
	cin>>n;
	cout<<endl;
	List a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"������ҵ "<<m<<" ����Ҫ�Ŀռ��С(KB)��";
		cin>>u_size;
		a.seek(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------�Ƿ�����ĳ��ҵ�ͷ��ڴ�(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------�Ƿ����������ҵ��y/n��:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
		
}


void NF()			//ѭ���״���Ӧ�㷨���ú��� 
{
	int n;
	cout<<"|                   �������������:";
	cin>>n;
	cout<<endl;
	List a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"������ҵ "<<m<<" ����Ҫ�Ŀռ��С(KB)��";
		cin>>u_size;
		a.seekNF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------�Ƿ�����ĳ��ҵ�ͷ��ڴ�(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------�Ƿ����������ҵ��y/n��:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

class ListBF		//�����Ӧ�㷨 ���з����� 
{
private:
	IStorage_link *pHead;
	IStorage_link *pTail;
	IStorage_link *pNow;      //û�� 
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
			temp->status="δ����";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			int k=0;
			IStorage_link *p1=pHead->next;
			IStorage_link *p2=pHead;

			while(p1!=NULL)											//��������㰴��������С����С����˳����в��� 
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
		cout<<"������\t��С\t��ַ\t״̬"<<endl;
		while(p!=NULL)
		{
			cout<<p->num<<'\t'<<p->size<<'\t'<<p->inchoate_aspect<<'\t'<<p->status<<endl;
			p=p->next;
		}
	}
	void seekBF(int u_size,char m);
	void Free();
	void Insert(Work *wq);
	void Push(IStorage_link *p);			//���½�㰴��������С ��С����˳����뵽�����ʵ�λ�� 
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
			if(p->size>=u_size&&(p->status=="δ����"))
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
					p->status="�ѷ���";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}

				Push(p);

				k=1;
				cout<<"-----------------����ɹ�!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------����ʧ��!,û���㹻�Ŀռ���з���!"<<endl;
		}
		Display();
}

void ListBF::Free()
{
	cout<<endl;
	cout<<"��ҵ��\t����\t��С\t��ַ\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------Ҫ�����ͷŵ���ҵ��:";
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
								if(p->status=="�ѷ���")						//���÷���������Ϊ0
								{
									p->status="δ����";
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
										if((wq->headposition+wq->size)==l1->inchoate_aspect)	 //��������ǰ������������� 
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
									if(k==0)													//��������ǰһ���������ڣ��������һ���������� 
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
										if((wq->headposition+wq->size)==l2->inchoate_aspect)	//���������һ���������ڣ�������ǰһ���������� 
										{
											l2->inchoate_aspect=wq->headposition;
											l2->size+=wq->size;
											k=1;
											break;
										}
										l2=l2->next;
									}
									if(k==0)													//�������Ȳ���ǰһ���������ڣ��ֲ����һ����������
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

void ListBF::Insert(Work *wq)								//����������С����˳����뵽�����ʵ�λ�� 
{
	IStorage_link *p=new IStorage_link();					
	p->num=wq->id;
	p->inchoate_aspect=wq->headposition;
	p->size=wq->size;
	p->status="δ����";
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

void BF()					//�����Ӧ�㷨���ú��� 
{
	int n;
	cout<<"|                   �������������:";
	cin>>n;
	cout<<endl;
	ListBF a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"������ҵ "<<m<<" ����Ҫ�Ŀռ��С(KB)��";
		cin>>u_size;
		a.seekBF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------�Ƿ�����ĳ��ҵ�ͷ��ڴ�(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------�Ƿ����������ҵ��y/n��:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

class ListWF					//���Ӧ�㷨 ���з����� 
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
			temp->status="δ����";

			temp->last=pTail;
			
			temp->inchoate_aspect=temp->last->size+temp->last->inchoate_aspect+rand()%100+1;
			temp->next=NULL;
			
			int k=0;
			IStorage_link *p1=pHead->next;
			IStorage_link *p2=pHead;

			while(p1!=NULL)
			{
				if(temp->size>=p1->size)								//	�Ӵ�С 
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
		cout<<"������\t��С\t��ַ\t״̬"<<endl;
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
			if(p->size>=u_size&&(p->status=="δ����"))								 
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
					p->status="�ѷ���";
					w->id=p->num;
					w->name=m;
					w->headposition=p->inchoate_aspect;
					
					
					w->next=whead->next;
					whead->next=w;

					allwork++;
				}

				Push(p);

				k=1;
				cout<<"-----------------����ɹ�!-----------"<<endl;
				break;
			}
			p=p->next;
		}
		if(k==0)
		{
			cout<<endl<<"----------����ʧ��!,û���㹻�Ŀռ���з���!"<<endl;
		}
		Display();
}

void ListWF::Push(IStorage_link *p)						//	�Ӵ�С 
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
	cout<<"��ҵ��\t����\t��С\t��ַ\t"<<endl;

	Work *wc=new Work;
	wc=whead->next;
	while(wc!=NULL)
	{
		cout<<wc->name<<'\t'<<wc->id<<'\t'<<wc->size<<'\t'<<wc->headposition<<endl;
		wc=wc->next;
	}
	cout<<"|---------Ҫ�����ͷŵ���ҵ��:";
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
								if(p->status=="�ѷ���")
								{
									p->status="δ����";
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
	p->status="δ����";
	p->next=NULL;
	IStorage_link *p1=pHead->next;
	IStorage_link *p2=pHead;
	int k=0;
	while(p1!=NULL)
	{
		if(p1->size<=p->size)					//	�Ӵ�С 
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

void WF()										//���Ӧ�㷨���ú��� 
{
	int n;
	cout<<"|                   �������������:";
	cin>>n;
	cout<<endl;
	ListWF a(n);
	a.Display();
	char m='A';
	
	int u_size;
	while(1)
	{
		cout<<"������ҵ "<<m<<" ����Ҫ�Ŀռ��С(KB)��";
		cin>>u_size;
		a.seekWF(u_size,m);

		cout<<endl;

		while (1)
		{
			cout<<"|-----------�Ƿ�����ĳ��ҵ�ͷ��ڴ�(y/n):";
				char l;
				cin>>l;
				if(l=='y'||l=='Y')
				{
					a.Free();
				}
				else break;
		}

			cout<<"|-----------�Ƿ����������ҵ��y/n��:";
			char x;
			cin>>x;
		if(x=='y'||x=='Y')
			m++;
		else break;
	}
}

void dynamic_state()//�ɱ��������ѡ����� 
{
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"|                    �ɱ�ʽ��������                     |"<<endl;
	cout<<"---------------------------------------------------------"<<endl;
	cout<<"                   1.�״���Ӧ�㷨                        "<<endl;
	cout<<"                  2.ѭ���״���Ӧ�㷨                     "<<endl;
	cout<<"                   3.�����Ӧ�㷨                        "<<endl;
	cout<<"                   4.���Ӧ�㷨                        "<<endl;
	cout<<"                  ��ѡ��";
	int x;
		cin>>x;
	switch (x)
	{
	case 1:FF();break;
	case 2:NF();break;
	case 3:BF();break;
	case 4:WF();break;
	default:cout<<"ѡ�����������ѡ��!"<<endl;
		break;
	}
}

int main()//���˵����� 
{
	while(1)
	{
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"|          ѡ��̶���������or�ɱ�ʽ��������             |"<<endl;
		cout<<"---------------------------------------------------------"<<endl;
		cout<<"                   1.�̶���������                        "<<endl;
		cout<<"                  2.�ɱ�ʽ��������                       "<<endl;
		cout<<"                       3.�˳�                            "<<endl;
		cout<<"                  ��ѡ��";
		int x;
		cin>>x;
		switch (x)
		{
		case 1:fixation();break;
		case 2:dynamic_state();break;
		case 3:exit(0);break;
		default:cout<<"ѡ�����������ѡ��!"<<endl;
			break;
		}
		exit(0);
	}
}
