#include< iostream >

using namespace std;

struct Node {
	int XiShu ;
	int ZhiShu ;
	Node* next ;
    Node (int xishu,int ZhiShu,Node *next):XiShu(xishu),ZhiShu(zhishu),next(next){}
};

void printlist(Node* head) {
	Node *p = head;
	while (p) {
        // 先输出系数
		if (p->XiShu < 0) {
			cout << "(" << p->XiShu << ")";
		}
		else if (p->XiShu == 0) {
			p = p->next;
			continue;			
		}
		else  {
            cout << p->XiShu;
		}
        // 再输出指数
		if (p->ZhiShu != 0) {
			cout << "x^";
			if (p->ZhiShu < 0) {
				cout << "(" << p->ZhiShu << ")";
			}
			else cout << p->ZhiShu;
		}
		if (p->next!=NULL) cout << " + ";
		p = p->next;
	}
	cout << endl;
}
void Combine(Node* head1, Node* head2) {
	Node* head3 = new Node;
	Node* p = head3;
	while (head1 && head2) {
		if (head1->ZhiShu == head2->ZhiShu) {
			if (head1->XiShu + head2->XiShu!= 0) {
				head1->XiShu += head2->XiShu;
				p->next = head1;
				head1 = head1->next;
				head2 = head2->next;
				p = p->next;
			}
			else {
				head1 = head1->next;
				head2 = head2->next;
			}
		}
		else if (head1->ZhiShu < head2->ZhiShu) {
			p->next = head1;
			head1 = head1->next;
			p = p->next;
		}
		else {
			p->next = head2;
			head2 = head2->next;
			p = p->next;
		}
	}
	if (head2) {
		p->next = head2;
	}
	else p->next = head1;
	printlist(head3);
}
int main() {
	int t;
	cin >> t;
	while(t--) {
		int n;
		cin >> n;
		Node* head1 = new Node;
		Node* p = head1;
		for (int i = 1; i <= n; i++) {
            int a,b;
            cin>>a>>b;
        	Node *s=new Node(a,b);
            p->next=s;
            p=s;
		}
		printlist(head1);
		int m;
        cin >> m;
		Node *head2 = new Node;
		p = head2;
		for (int i = 1; i <= m; i++) {
            int a,b;
            cin>>a>>b;
        	Node *s=new Node(a,b);
            p->next=s;
            p=s;
		}
		printlist(head2);
		Combine(head1, head2);
	}

}