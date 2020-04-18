
#include <stdio.h>
#include <stdlib.h>
#define numRow 5  /* power of 2 (2, 4, 8, 16, ...)�� �ƴ� ������ ���� */
#define numCol 10

int matrix[numRow][numCol] = {
{ 5,90,5,9,80,80,7,90,7,90 },
{ 100,30,30,51,160,160,160,51,160,59 },
{ 500,100,7000,100,900,600,100,100,650,100 },
{ 1000,300,41,300,41,41,41,900,900,950 },
{ 90,81,81,95,81,83,81,90,81,90 }
};

typedef struct Node* treePointer;
typedef struct Node {	// binary search tree ����
	int row;			// ����� ���� ����
	int key;			// ���� ��
	int count;			// ���� ������ ����
	treePointer left;
	treePointer right;
} Node;

treePointer* root[numRow];	// �� binary search tree�� root�� ����Ű�� ������
treePointer loserArray[numRow * 2];		// loser tree�� �迭�� ����
treePointer getNode = NULL;

int maxCount;
int theKey;

treePointer iterSearch(treePointer tree, int k) {

	treePointer parent = NULL;

	while (tree) {		// ���� ������ ������ count 1 ����
		if (k == tree->key) {
			tree->count++;
			return NULL;
		}

		parent = tree;
		if (k < tree->key)
			tree = tree->left;	// ���� ���� ������ ����
		else
			tree = tree->right;	// ���� ���� ũ�� ������
	}
	return parent;		// �θ��带 ��ȯ
}

void insert(treePointer *node, int k, int row) {	// ������� ���� ���� ���� ������ ��带 �����Ͽ� ����

	treePointer ptr;
	treePointer temp = iterSearch(*node, k);

	if (temp || !(*node)) {		// ��� ����
		ptr = (treePointer)(malloc(sizeof(Node)));
		ptr->row = row;
		ptr->key = k;
		ptr->count = 1;
		ptr->left = NULL;
		ptr->right = NULL;

		if (*node) {
			if (k < temp->key) temp->left = ptr;
			else temp->right = ptr;
		}
		else *node = ptr;
	}
}

void traverse(treePointer tree, int row) {	// loser tree�� �������� �� ��带 ã��
	
	if (!getNode) {
		maxCount = 0;
		theKey = 0;
	}

	if (tree == NULL) return;
	if ((tree->count > maxCount) || (tree->count == maxCount && tree->key > theKey)) { // tie breaking ��Ģ
		if (loserArray[row + numRow]) {
			if (tree->count < loserArray[row + numRow]->count || (tree->count == loserArray[row + numRow]->count && tree->key < loserArray[row + numRow]->key)) {
				getNode = tree;
				maxCount = getNode->count;
				theKey = getNode->key;
			}
		}
		else {
			getNode = tree;
			maxCount = getNode->count;
			theKey = getNode->key;
		}
	}
	traverse(tree->left, row);
	traverse(tree->right, row);
}

void loserTree(int x) {		// loser tree �˰���
	
	if (!loserArray[x] && x < numRow) {		// non-leaf node�� null�� ��
		if (!loserArray[(x/2)*2] && !loserArray[(x / 2) * 2 + 1]) loserTree(x / 2);
		else if (!loserArray[(x / 2) * 2]) loserTree((x / 2) * 2 + 1);
		else if (!loserArray[(x / 2) * 2 + 1]) loserTree((x / 2) * 2);
		return;
	}
	if (!loserArray[x] && x >= numRow) {	// leaf node�� null�� ��
		loserTree(x / 2);
		return;
	}

	treePointer p = loserArray[x];
	treePointer tmp = NULL;

	for (int i = x; i > 0; i /= 2) {	// ����ִ� �θ� ��尡 ���� ������ �ݺ�
		if (loserArray[i / 2]) {
			if (!compare(p, loserArray[i / 2])) {	// �θ� ��庸�� ������ �� ��ġ�� ���� �� �θ� ��尡 �ö�
				tmp = p;
				p = loserArray[i / 2];
				loserArray[i / 2] = tmp;
			}
		}
		else {
			if (i >= numRow && (!loserArray[(i / 2) * 2] || !loserArray[(i / 2) * 2 + 1]))
				continue;
			loserArray[i / 2] = p;
			break;
		}
	}
	if (loserArray[0]) {	// index 0�� ä������ ��ȯ
		if (x < numRow) loserArray[x] = NULL;
		return;
	}
	if (x > numRow) loserTree(x - 1);	// �ʱ� loser tree�� �����ϱ� ���Ͽ� �������� ��� ȣ��
}

int compare(treePointer a, treePointer b) {		// a ���� b��带 ���Ͽ� a ��尡 ũ�� 1, b ��尡 ũ�� 0

	if (a->count == b->count) {	// tie breaking ��Ģ ����
		if (a->key == b->key) return a->row > b->row;
		else return a->key > b->key;
	}
	else return a->count > b->count;
}

void main() {

	for (int i = 0; i < numRow; i++)	// ����� Binary tree�� ����
		for (int j = 0; j < numCol; j++)
			insert(root + i, matrix[i][j], i);

	for (int i = 0; i < numRow; i++) {	// ����Ʈ�� �ʱ� ������� ����
		traverse(root[i], i);
		loserArray[numRow + i] = getNode;
		getNode = NULL;
	};

	loserTree(numRow * 2 - 1);
	printf("(%d, %d, %d)\n", loserArray[0]->row, loserArray[0]->key, loserArray[0]->count); // ��ü ����

	while (1) {
		int d = loserArray[0]->row;
		if (loserArray[d + numRow])	// Binary tree�� ��µ������� ��� �� �󵵰� ���� ���� ���� ���� ������
			traverse(root[d], d);
		loserArray[d + numRow] = getNode;

		getNode = NULL;
		loserArray[0] = NULL;
		loserTree(d + numRow);

		if (!loserArray[1]) break; // ��������
		printf("(%d, %d, %d)\n", loserArray[0]->row, loserArray[0]->key, loserArray[0]->count);	// ���� ���� ���������� ������������ �����Ͽ� ���
	}
}