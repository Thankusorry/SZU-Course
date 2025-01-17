#include<bits/stdc++.h> 
struct Node{
	int data;
	Node*next;
};
class List{
	Node*head;
	public:
	//按位置查找 
	Node*Search(int i){
		int j=1;
		Node*p=head;
		while(p&&j++<i){
			p=p->next;
		}
		if(p)
		return p;
		return NULL;
	}
	//按值查找	
	Node*SearchValue(int value){
		Node*p=head;
		while(p&&p->data!=value){
			p=p->next;
		}
		if(p&&p->data==value)
		return p;
		else return NULL;
	} 
	//访问 
	bool Traverse(){
		Node*p=head;
		while(p){
			//TODO
			p=p->next;
		}
		return 0;
	}
	//插入
	
	//删除
	
	// 递增有序合并
	static Node*MergeList(List&a,List&b){
		Node*head_c,*pc,*pa=a.head,*pb=b.head;
		
	} 
};
