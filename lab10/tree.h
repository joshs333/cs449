struct tree {
	int data;
        struct tree *left;
        struct tree *right;
};

void freetree(struct tree *root);
void randominsert(struct tree *head, struct tree *new);
void printtree(struct tree *head);
