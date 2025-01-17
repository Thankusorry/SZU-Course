  #include <iostream>
using namespace std;

template <class T>
struct LNode{
	T data;
	LNode<T>* next;
};

template <class T>
class Linklist{
private:
	LNode<T>* head;
public:
	LNode<T>* getHead(){return head;}
	Linklist(){
		head=new LNode<T>;
		head->next=NULL;
	}
	Linklist(T* r,int n){
		head=new LNode<T>;		head->next=NULL;
		int	   j;
		LNode<T> *s;
		if (n > 0){
		 /*s = new LNode<T>; s->data = r[0]; s->next = NULL;      //������1�����
		 head->next= s;	*/			       // ������ָ��ָ���1�����
		 for (j=1; j<=n; j++)	insertItem(j, r[j-1]);
		}
	}
	~Linklist(){
		LNode<T> *p=head->next,*q;
		while(p) {q=p;p=p->next; delete q;}
		delete head;
	}
	T getItem(int i){
		int    j;
		LNode<T> *p;
		p = head;  j = 0;				// pָ���һ�����
		while (p && j < i) {p = p->next; j++;}	// ˳ָ�����
		if (!p || j>i) return(NULL);			// ��iԪ�ز�����
		return(p->data);	
	}
	LNode<T>* search(T key){
		LNode<T> *p=head->next;
		while  ( p!=NULL&& p->data!=key)    p=p->next;
		if  (p&&p->data==key)   return p;
		else  
		{    printf("��Ҫ���ҵĽ�㲻����!!\n"); 
		return(NULL);  
		}

	}
	bool Traverse()
	{    
		LNode<T> *p;
		p=head->next;      /*  ʹpָ���һ�����  */
		while  (p!=NULL)  {   
			cout<<p->data<<" ";
			p=p->next;   
		}
		cout<<endl;
		return  0;

	}
	bool insertItem(int i,T e){
		// �ڴ�ͷ���ĵ�����L�е�i��λ��֮ǰ����Ԫ��e
		LNode<T> *p = head,*s; 
	    int j = 0;
		while (p && j<i-1) {p = p->next; j++;}		// Ѱ��i-1���
		if (!p || j>i-1) return -1;
		s = new LNode<T>;		// �����½��
		s->data = e; s->next = p->next; p->next = s;	// ����L��
		return i;
	}
	bool deleteItem(T e){
		LNode<T> *p=head, *q;
		while  ( p!=NULL&& p->data!=e)    {q=p;p=p->next;}
			if (!p) return(false);
			q->next = p->next; 	// ɾ��i���
			delete p;				// ȡֵ���ͷŽ��
			return(true);

	}
};

template<class T>
void MergeList(Linklist<T>& La, Linklist<T>& Lb) {
	LNode<T> *pa=La.getHead(),*pb=Lb.getHead(),*pc=La.getHead();
  pa = pa->next;    pb = pb->next;        
  while (pa && pb) {
    if (pa->data <= pb->data) 
    {  pc->next = pa;   pc = pa;   pa = pa->next;  }
    else { pc->next = pb;   pc = pb;   pb = pb->next; }
  }

 
  pc->next = pa ? pa : pb;  // ����ʣ���
  Lb.getHead()->next=NULL;
 
} // MergeList_L


void main(){
	/*int a[]={1,2,3,4,5};
	Linklist<int> list(a,5);
	list.Traverse();*/

	
	Linklist<int> list1,list2;
	for(int i=1;i<=5;i++)
		list1.insertItem(i,i*10);
	for(int j=1;j<=5;j++)
		list2.insertItem(j,j*3);
	list1.Traverse();
	list2.Traverse();
	MergeList(list1,list2);
	list1.Traverse();
	/*cout<<list1.getItem(1)<<endl;
	cout<<list2.search(6)<<endl;
	list1.deleteItem(10);
	list2.deleteItem(15);
	list1.Traverse();
	list2.Traverse();
	LNode<int>* p=list1.search(20);cout<<p->data<<endl;*/

    }
