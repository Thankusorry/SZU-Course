#include <iostream>
using namespace std;

struct BiNode {
	char data;
	BiNode  *lchild, *rchild;
};

class BiTree {
	BiNode  *root;
	//���ڵ�
public:
	BiTree() { root = NULL; }
	BiTree(char *c, int n) { root = CreatTree(c, 1, n); }
	BiNode* CreatTree(char *c, int i, int n) {
		if (i > n) return NULL;		// i��ʾ�����c[]�е����
		if (c[i - 1] == '0') return NULL;	// ��0����ʾ�ս��
		BiNode* p;
		p = new BiNode;			// �����½��
		p->data = c[i - 1];
		p->lchild = CreatTree(c, 2 * i, n);
		p->rchild = CreatTree(c, 2 * i + 1, n);
		return p;
	}
	void PreOrder() {
		PreOrderTraverse(root);
	}
	void PreOrderTraverse(BiNode* p) {
		if (p) {
			cout << p->data;
			PreOrderTraverse(p->lchild);
			PreOrderTraverse(p->rchild);
		}
	}
};

int main() {
	char c[100];
	cin >> c;
	BiTree t1(c, 10);
	t1.PreOrder();
	return 0;
}